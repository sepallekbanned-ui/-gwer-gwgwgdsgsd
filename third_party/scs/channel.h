#pragma once
#include "scs.h"
#include "value.h"
static constexpr scs_u32_t SCS_TELEMETRY_CHANNEL_FLAG_none=0;
static constexpr scs_u32_t SCS_TELEMETRY_CHANNEL_FLAG_each_frame=1;
static constexpr scs_u32_t SCS_TELEMETRY_CHANNEL_FLAG_no_value=2;
using scs_telemetry_channel_callback_t =
    void (SCSAPIFUNC *)(const scs_string_t name, const scs_u32_t index,
                        const scs_value_t* const value, const scs_context_t context);
using scs_telemetry_register_for_channel_t =
    scs_result_t (SCSAPIFUNC *)(const scs_string_t name, const scs_u32_t index,
        const scs_u32_t type, const scs_u32_t flags,
        const scs_telemetry_channel_callback_t callback, const scs_context_t context);
using scs_telemetry_unregister_from_channel_t =
    scs_result_t (SCSAPIFUNC *)(const scs_string_t name, const scs_u32_t index, const scs_u32_t type);
