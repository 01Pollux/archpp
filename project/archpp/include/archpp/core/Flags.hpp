#pragma once

#include <type_traits>

#define ARCHPP_ENUM_FLAGS_OR(type)                                                \
    inline constexpr type operator|(type lhs, type rhs)                           \
    {                                                                             \
        return static_cast<type>(static_cast<std::underlying_type_t<type>>(lhs) | \
                                 static_cast<std::underlying_type_t<type>>(rhs)); \
    }                                                                             \
    inline constexpr type& operator|=(type& lhs, type rhs)                        \
    {                                                                             \
        return lhs = lhs | rhs;                                                   \
    }

#define ARCHPP_ENUM_FLAGS_AND(type)                                                \
    inline constexpr type operator&(type lhs, type rhs)                            \
    {                                                                              \
        return static_cast<type>(static_cast<std::underlying_type_t<type>>(lhs) && \
                                 static_cast<std::underlying_type_t<type>>(rhs));  \
    }                                                                              \
    inline constexpr type& operator&=(type& lhs, type rhs)                         \
    {                                                                              \
        return lhs = lhs & rhs;                                                    \
    }

#define ARCHPP_ENUM_FLAGS_XOR(type)                                               \
    inline constexpr type operator^(type lhs, type rhs)                           \
    {                                                                             \
        return static_cast<type>(static_cast<std::underlying_type_t<type>>(lhs) ^ \
                                 static_cast<std::underlying_type_t<type>>(rhs)); \
    }                                                                             \
    inline constexpr type& operator^=(type& lhs, type rhs)                        \
    {                                                                             \
        return lhs = lhs ^ rhs;                                                   \
    }

#define ARCHPP_ENUM_FLAGS_NOT(type)                                                \
    inline constexpr type operator~(type lhs)                                      \
    {                                                                              \
        return static_cast<type>(~static_cast<std::underlying_type_t<type>>(lhs)); \
    }

#define ARCHPP_ENUM_FLAGS(type) \
    ARCHPP_ENUM_FLAGS_OR(type)  \
    ARCHPP_ENUM_FLAGS_AND(type) \
    ARCHPP_ENUM_FLAGS_XOR(type) \
    ARCHPP_ENUM_FLAGS_NOT(type)\
