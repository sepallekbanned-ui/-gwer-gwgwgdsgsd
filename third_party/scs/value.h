#pragma once
#include "scs.h"
static constexpr scs_u32_t SCS_VALUE_TYPE_INVALID=0;
static constexpr scs_u32_t SCS_VALUE_TYPE_bool=1;
static constexpr scs_u32_t SCS_VALUE_TYPE_s32=2;
static constexpr scs_u32_t SCS_VALUE_TYPE_u32=3;
static constexpr scs_u32_t SCS_VALUE_TYPE_u64=4;
static constexpr scs_u32_t SCS_VALUE_TYPE_float=5;
struct scs_value_bool_t { scs_u8_t value; };
struct scs_value_s32_t { scs_s32_t value; };
struct scs_value_u32_t { scs_u32_t value; };
struct scs_value_u64_t { scs_u64_t value; };
struct scs_value_float_t { float value; };
struct scs_value_t {
    scs_u32_t type;
#ifdef _WIN64
    scs_u32_t _padding;
#endif
    union {
        scs_value_bool_t value_bool;
        scs_value_s32_t value_s32;
        scs_value_u32_t value_u32;
        scs_value_u64_t value_u64;
        scs_value_float_t value_float;
    };
};
