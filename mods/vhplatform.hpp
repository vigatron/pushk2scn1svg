#pragma once

#include <stdint.h>

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>

#define PACKED_STRUCT __attribute__((packed))

#define u8          uint8_t
#define u16         uint16_t
#define verr        uint8_t
#define vok         0
#define verror(x)   x

verr verrmsg(int x, std::string strerr);
