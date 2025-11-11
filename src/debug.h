/** @file debug.h
 *  @brief Header containing debugging utilities
 *
 *  This contains the macros useful for debugging
 *
 *  @author Youngsung Kim
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <cstdio>

#define FILELINE() \
    std::fprintf(stderr, "[DEBUG] %s:%d (%s)\n", __FILE__, __LINE__, __func__);

#define DPRINT(fmt, ...) \
    do { \
        std::fprintf(stderr, "[DPRINT] " fmt "\n", ##__VA_ARGS__); \
    } while (0)

#endif
