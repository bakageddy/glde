#ifndef __GLDE_UTILS_H
#define __GLDE_UTILS_H

#include <stdio.h>

#define LOG(MESSAGE, ...) fprintf(stdout, "LOG: %s:%d: " MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define ERR(MESSAGE, ...) fprintf(stderr, "ERR: %s:%d: " MESSAGE "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#endif // !__GLDE_UTILS_H
