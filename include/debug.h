#pragma once

#ifdef DEBUG
    #define D_PRINT(FORMAT, args...) fprintf(stderr, "[DEBUG] %s() in %s, line %i: " FORMAT "\n", __func__, __FILE__, __LINE__, ## args)
#else
    #define D_PRINT(fmt, args...)    /* Do not print */
#endif