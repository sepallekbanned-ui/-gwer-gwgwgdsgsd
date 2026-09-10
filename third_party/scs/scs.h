#pragma once
#ifdef _WIN32
#define SCSAPIFUNC __stdcall
#define SCSAPI_RESULT scs_result_t SCSAPIFUNC
#define SCSAPI_VOID void SCSAPIFUNC
#else
#define SCSAPIFUNC
#define SCSAPI_RESULT scs_result_t
#define SCSAPI_VOID void
#endif
using scs_u8_t = unsigned char;
using scs_u32_t = unsigned int;
using scs_s32_t = int;
using scs_u64_t = unsigned long long;
using scs_string_t = const char*;
using scs_timestamp_t = scs_u64_t;
using scs_context_t = void*;
using scs_result_t = scs_s32_t;
static constexpr scs_result_t SCS_RESULT_ok=0;
static constexpr scs_result_t SCS_RESULT_unsupported=-1;
static constexpr scs_result_t SCS_RESULT_invalid_parameter=-2;
static constexpr scs_result_t SCS_RESULT_already_registered=-3;
static constexpr scs_result_t SCS_RESULT_not_found=-4;
static constexpr scs_result_t SCS_RESULT_unsupported_type=-5;
static constexpr scs_result_t SCS_RESULT_not_now=-6;
static constexpr scs_result_t SCS_RESULT_generic_error=-7;
static constexpr scs_s32_t SCS_LOG_TYPE_message = 0;
static constexpr scs_s32_t SCS_LOG_TYPE_warning = 1;
static constexpr scs_s32_t SCS_LOG_TYPE_error = 2;
static constexpr scs_u32_t SCS_U32_NIL=0xFFFFFFFFu;
#define SCS_MAKE_VERSION(major,minor) (((major)<<16)|(minor))
#define SCS_TELEMETRY_VERSION_1_00 SCS_MAKE_VERSION(1,0)
#define SCS_TELEMETRY_VERSION_1_01 SCS_MAKE_VERSION(1,1)
#define SCS_TELEMETRY_VERSION_CURRENT SCS_TELEMETRY_VERSION_1_01
using scs_log_t = void (SCSAPIFUNC *)(const scs_s32_t, const scs_string_t);
struct scs_sdk_init_params_v100_t {
    scs_string_t game_name;
    scs_string_t game_id;
    scs_u32_t game_version;
#ifdef _WIN64
    scs_u32_t _padding;
#endif
    scs_log_t log;
};
