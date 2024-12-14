#ifndef _ERROR_H
#define _ERROR_H
#include <stdint.h>

void kernel_panic(const char* str);
void error_kernel(const uint32_t pid, const char* tipo, const char* str);

#endif