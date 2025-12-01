ifeq ($(OS),Windows_NT)
    CC_COMMON = x86_64-w64-mingw32-g++.exe
else
    CC_COMMON = g++
endif

CFLAGS_COMMON = -Wall -Wextra -Werror -std=gnu++23
CFLAGS_ALL_COMMON = -O3 -s -DNDEBUG
CFLAGS_DEBUG_COMMON = -O0 -g
