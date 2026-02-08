#ifndef framework_hpp
#define framework_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <intrin.h>
#include <stdarg.h>
#include <cstdint>
#include <stdlib.h>

#include <types/ketypes.hpp>
#include <types/ex.hpp>

extern "C" PLIST_ENTRY PsLoadedModuleList;

#endif // !framework_hpp