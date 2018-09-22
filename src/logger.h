#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <bitset>

enum class LOG_LEVELS {
    DISABLED,
    LOG0,
    LOG1,
    LOG2,
    LOG3,
    LOG4,
    LOG5,
    LOG6,
    LOG7,
    TOTAL_COUNT
};

const std::string LOG_LEVEL_NAMES[] = {
    "LOG0\t",
    "LOG1\t",
    "LOG2\t",
    "LOG3\t",
    "LOG4\t",
    "LOG5\t",
    "LOG7\t",
    "LOG6\t"
};

constexpr LOG_LEVELS operator <<(int lhs, LOG_LEVELS rhs) {
    return(static_cast<LOG_LEVELS>(static_cast<uint16_t>(lhs) << static_cast<unsigned>(rhs)));
}

constexpr int operator >>(LOG_LEVELS lhs, LOG_LEVELS rhs) {
    return(static_cast<uint16_t>(lhs) >> static_cast<unsigned>(rhs));
}

constexpr LOG_LEVELS operator |(LOG_LEVELS lhs, LOG_LEVELS rhs) {
    return(static_cast<LOG_LEVELS>(static_cast<uint16_t>(lhs) | static_cast<unsigned>(rhs)));
}

template<typename T>
constexpr T set_level(T first) {
    return (1 << first);
}

template<typename T, typename ... Args>
constexpr T set_level(T first, Args ... args) {
    return (1 << first) | set_level(args...);
}

template<typename T>
const char *get_binary(const T input) {
    return std::bitset<16>(input).to_string().c_str();
}

template<LOG_LEVELS LEVEL>
struct logger {
    template<LOG_LEVELS internal_n>
    struct logger_internal {
        logger_internal(const char *format, ...) {
            if constexpr ((LEVEL >> internal_n) & 1) {
                // in enum index 0 = DISABLED, so
                // index = internal_n - 1
                int index = static_cast<int>(internal_n) - 1;
                printf("%s ", LOG_LEVEL_NAMES[index].c_str());
                va_list args;
                va_start(args, format);
                vprintf(format, args);
                va_end(args);
            }
        };
    };

    typedef logger_internal<LOG_LEVELS::LOG0> LOG0;
    typedef logger_internal<LOG_LEVELS::LOG1> LOG1;
    typedef logger_internal<LOG_LEVELS::LOG2> LOG2;
    typedef logger_internal<LOG_LEVELS::LOG3> LOG3;
    typedef logger_internal<LOG_LEVELS::LOG4> LOG4;
    typedef logger_internal<LOG_LEVELS::LOG5> LOG5;
    typedef logger_internal<LOG_LEVELS::LOG6> LOG6;
    typedef logger_internal<LOG_LEVELS::LOG7> LOG7;
};

#endif /* LOG_H_ */
