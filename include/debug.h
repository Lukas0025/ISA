/**
 * Project: ICMP encrypted file transfer - ISA BUT FIT 2021
 * file with debug functions
 * @author Lukáš Plevač <xpleva07> (BUT FIT)
 * @date 15.10.2021
 */

#pragma once

#ifdef DEBUG
    #define D_PRINT(FORMAT, args...) fprintf(stderr, "[DEBUG] %s() in %s, line %i: " FORMAT "\n", __func__, __FILE__, __LINE__, ## args)
#else
    #define D_PRINT(fmt, args...)    /* Do not print */
#endif