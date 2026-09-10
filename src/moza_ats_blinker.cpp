#include <windows.h>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "telemetry.h"
#include "mozaAPI.h"
#include <cstring>

static_assert(sizeof(scs_telemetry_init_params_v100_t) == 64, "Unexpected SCS telemetry init ABI");

namespace {
    std::atomic<bool> g_running{false};
    std::atomic<bool> g_left_enabled{false};
    std::atomic<bool> g_right_enabled{false};
    std::atomic<bool> g_hazard_enabled{false};
    std::atomic<bool> g_left_on{false};
    std::atomic<bool> g_right_on{false};
    std::thread g_worker;
    scs_log_t g_log = nullptr;

    int g_original_mode = 0;
    int g_original_switch = 2;
    std::vector<std::string> g_original_colors;
    std::vector<int> g_original_rpm;

    constexpr int LEDS = 10;
    const std::string AMBER = "#FFFFA000";
    const std::string OFF   = "#00000000";

    void log(scs_s32_t type, const std::string& text) {
        if (g_log) g_log(type, text.c_str());
    }

    bool read_bool(const scs_value_t* value) {
        return value && value->type == SCS_VALUE_TYPE_bool && value->value_bool.value != 0;
    }

    void set_leds(bool leftOn, bool rightOn, bool hazard) {
        std::vector<std::string> c(LEDS, OFF);

        if (hazard) {
            if (leftOn || rightOn) {
                std::fill(c.begin(), c.end(), AMBER);
            }
        } else {
            if (leftOn) {
                for (int i = 0; i < 5; ++i) c[i] = AMBER;
            }
            if (rightOn) {
                for (int i = 5; i < 10; ++i) c[i] = AMBER;
            }
        }

        auto err = moza::setSteeringWheelShiftIndicatorSwitch(2);
        if (err != NORMAL) return;
        err = moza::setSteeringWheelShiftIndicatorMode(1);
        if (err != NORMAL) return;
        moza::setSteeringWheelShiftIndicatorColor(c);
    }

    void restore_moza() {
        if (!g_original_colors.empty())
            moza::setSteeringWheelShiftIndicatorColor(g_original_colors);
        if (!g_original_rpm.empty())
            moza::setSteeringWheelShiftIndicatorLightRpm(g_original_rpm);
        moza::setSteeringWheelShiftIndicatorMode(g_original_mode);
        moza::setSteeringWheelShiftIndicatorSwitch(g_original_switch);
    }

    void worker_main() {
        bool last_active = false;
        bool last_left = false;
        bool last_right = false;
        bool last_hazard = false;

        while (g_running.load()) {
            bool le = g_left_enabled.load();
            bool re = g_right_enabled.load();
            bool hz = g_hazard_enabled.load();
            bool lo = g_left_on.load();
            bool ro = g_right_on.load();

            bool active = le || re || hz;
            if (!active) {
                if (last_active) restore_moza();
            } else {
                if (!last_active || lo != last_left || ro != last_right || hz != last_hazard) {
                    set_leds(lo && (le || hz), ro && (re || hz), hz);
                }
            }

            last_active = active;
            last_left = lo;
            last_right = ro;
            last_hazard = hz;
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
        restore_moza();
    }

    void SCSAPIFUNC channel_callback(const scs_string_t name,
                                      const scs_u32_t,
                                      const scs_value_t* const value,
                                      const scs_context_t) {
        if (!name) return;
        if (strcmp(name, "truck.lblinker") == 0)
            g_left_enabled.store(read_bool(value));
        else if (strcmp(name, "truck.rblinker") == 0)
            g_right_enabled.store(read_bool(value));
        else if (strcmp(name, "truck.hazard.warning") == 0)
            g_hazard_enabled.store(read_bool(value));
        else if (strcmp(name, "truck.light.lblinker") == 0)
            g_left_on.store(read_bool(value));
        else if (strcmp(name, "truck.light.rblinker") == 0)
            g_right_on.store(read_bool(value));
    }

    bool register_channel(scs_telemetry_register_for_channel_t fn, const char* name) {
        if (!fn) return false;
        auto r = fn(name, SCS_U32_NIL, SCS_VALUE_TYPE_bool,
                    SCS_TELEMETRY_CHANNEL_FLAG_each_frame,
                    channel_callback, nullptr);
        if (r != SCS_RESULT_ok) {
            log(SCS_LOG_TYPE_warning, std::string("Failed to register telemetry channel: ") + name);
            return false;
        }
        return true;
    }
}

extern "C" SCSAPI_RESULT scs_telemetry_init(
    const scs_u32_t version,
    const scs_telemetry_init_params_t* const params)
{
    const scs_u32_t major = (version >> 16) & 0xffffu;
    const scs_u32_t minor = version & 0xffffu;
    if (major != 1 || minor < 0 || minor > 99)
        return SCS_RESULT_unsupported;

    const auto* p = reinterpret_cast<const scs_telemetry_init_params_v100_t*>(params);
    g_log = p->common.log;

    // Snapshot the user's current MOZA indicator configuration.
    scs_s32_t mozaErr = NORMAL;
    if (auto v = moza::getSteeringWheelShiftIndicatorMode(reinterpret_cast<ERRORCODE&>(mozaErr)))
        g_original_mode = v;
    if (auto v = moza::getSteeringWheelShiftIndicatorSwitch(reinterpret_cast<ERRORCODE&>(mozaErr)))
        g_original_switch = v;
    if (auto v = moza::getSteeringWheelShiftIndicatorColor(reinterpret_cast<ERRORCODE&>(mozaErr)))
        g_original_colors = *v;
    if (auto v = moza::getSteeringWheelShiftIndicatorLightRpm(reinterpret_cast<ERRORCODE&>(mozaErr)))
        g_original_rpm = *v;

    bool ok = true;
    ok &= register_channel(p->register_for_channel, "truck.lblinker");
    ok &= register_channel(p->register_for_channel, "truck.rblinker");
    ok &= register_channel(p->register_for_channel, "truck.light.lblinker");
    ok &= register_channel(p->register_for_channel, "truck.light.rblinker");
    register_channel(p->register_for_channel, "truck.hazard.warning");

    if (!ok) {
        log(SCS_LOG_TYPE_error, "MOZA ATS Blinker: telemetry registration failed.");
        return SCS_RESULT_generic_error;
    }

    g_running.store(true);
    g_worker = std::thread(worker_main);

    std::ostringstream msg;
    msg << "MOZA ATS Blinker loaded for " << (p->common.game_id ? p->common.game_id : "unknown game");
    log(SCS_LOG_TYPE_message, msg.str());
    return SCS_RESULT_ok;
}

extern "C" SCSAPI_VOID scs_telemetry_shutdown(void)
{
    g_running.store(false);
    if (g_worker.joinable())
        g_worker.join();

    g_left_enabled.store(false);
    g_right_enabled.store(false);
    g_hazard_enabled.store(false);
    g_left_on.store(false);
    g_right_on.store(false);
    moza::removeMozaSDK();
    g_log = nullptr;
}
