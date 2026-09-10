#pragma once
#include "scs.h"
#include "value.h"
#include "channel.h"
using scs_telemetry_register_for_event_t = void*;
using scs_telemetry_unregister_from_event_t = void*;
struct scs_telemetry_init_params_t { void method_indicating_this_is_not_a_c_struct(void); };
struct scs_telemetry_init_params_v100_t : public scs_telemetry_init_params_t {
    scs_sdk_init_params_v100_t common;
    scs_telemetry_register_for_event_t register_for_event;
    scs_telemetry_unregister_from_event_t unregister_from_event;
    scs_telemetry_register_for_channel_t register_for_channel;
    scs_telemetry_unregister_from_channel_t unregister_from_channel;
};
extern "C" {
SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params);
SCSAPI_VOID scs_telemetry_shutdown(void);
}
