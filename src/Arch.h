#ifndef _GRAV_ARCH_H_
#define _GRAV_ARCH_H_

#if defined(__i386) || defined(__i386__) || defined(_M_IX86)
#define GRAV_TARGET_X86 1
#elif defined(__x86_64) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
#define GRAV_TARGET_X64 1
#elif defined(__arm) || defined(__arm__) || defined(__ARM) || defined(__ARM__)
#define GRAV_TARGET_ARM 1
#else
#error "No support for this architecture (yet)"
#endif

#endif
