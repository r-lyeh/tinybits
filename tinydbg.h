// Tiny debug macros
// - rlyeh, public domain
//
// Build cube of 3 dimensions, 5 levels each:
//
//     . PUBLIC AXIS: 0 STUDIO/INTERNAL, 1 QA, 2 USER-TESTING, 3 SHOWCASE, 4 SHIPPING/MASTER
//    /
//   /
//  +--------> DEBUG AXIS: 0 FATAL, 1 ERROR, 2 WARN, 3 INFO, 4 VERBOSE
//  |
//  |
//  |
//  V OPTIMIZATION AXIS: 0 DEBUG, 1 DEBUGOPT, 2 OPTSYM, 3 OPTMAX, 4 STRIPPED
//
// Example: a RETAIL build might be PUB>=3 DBG<=1 OPT>=3

#pragma once

#if defined(DEBUG) && (defined(NDEBUG) || defined(_NDEBUG))
#undef DEBUG      // NDEBUG has precedence
#endif

#ifndef DBGLVL
#define DBGLVL (0)
#endif

#ifndef OPTLVL
#define OPTLVL (0)
#endif

#ifndef PUBLVL
#define PUBLVL (0)
#endif

#if DBGLVL
#define REL  if(0)
#define DBG  if(1)
#define DBG0 if(DBGLVL >= 0)
#define DBG1 if(DBGLVL >= 1)
#define DBG2 if(DBGLVL >= 2)
#define DBG3 if(DBGLVL >= 3)
#define DBG4 if(DBGLVL >= 4)
#else
#define REL  if(1)
#define DBG  if(0)
#define DBG0 if(0)
#define DBG1 if(0)
#define DBG2 if(0)
#define DBG3 if(0)
#define DBG4 if(0)
#endif

#if OPTLVL
#define OPT  if(1)
#define OPT0 if(OPTLVL == 0)
#define OPT1 if(OPTLVL >= 1)
#define OPT2 if(OPTLVL >= 2)
#define OPT3 if(OPTLVL >= 3)
#define OPT4 if(OPTLVL >= 4)
#else
#define OPT  if(0)
#define OPT0 if(1)
#define OPT1 if(0)
#define OPT2 if(0)
#define OPT3 if(0)
#define OPT4 if(0)
#endif

#if PUBLVL
#define DEV  if(0)
#define PUB  if(1)
#define PUB0 if(PUBLVL >= 0)
#define PUB1 if(PUBLVL >= 1)
#define PUB2 if(PUBLVL >= 2)
#define PUB3 if(PUBLVL >= 3)
#define PUB4 if(PUBLVL >= 4)
#else
#define DEV  if(1)
#define PUB  if(0)
#define PUB0 if(0)
#define PUB1 if(0)
#define PUB2 if(0)
#define PUB3 if(0)
#define PUB4 if(0)
#endif

// aliases
#define DEBUGSYM DEV DBG OPT0
#define DEBUGOPT DEV DBG OPT2
#define DEVELSYM DEV REL OPT0
#define DEVELOPT DEV REL OPT2
#define SHIPPING PUB REL OPT3

/*
#include <stdio.h>
#include <string.h>
int main() {
    DBG      puts("shown in debug builds");
    REL      puts("shown in release builds");
    DEV      puts("shown in internal development builds");
    DEV OPT0 puts("shown in internal development builds, with no optimization level");
    PUB OPT3 puts("shown in public builds with optimization level >= 3");
    SHIPPING puts("shown in final builds");

    char collected_flags[128] = {0};
    char *buf = collected_flags;

    DBG  strcat(buf, "DEBUG,");
    REL  strcat(buf, "RELEASE,");

    DBG0 strcat(buf, "DEBUG >= 0,");
    DBG1 strcat(buf, "DEBUG >= 1,");
    DBG2 strcat(buf, "DEBUG >= 2,");
    DBG3 strcat(buf, "DEBUG >= 3,");
    DBG4 strcat(buf, "DEBUG >= 4,");

    OPT0 strcat(buf, "OPTIM == 0,");
    OPT1 strcat(buf, "OPTIM == 1,");
    OPT2 strcat(buf, "OPTIM == 2,");
    OPT3 strcat(buf, "OPTIM == 3,");
    OPT4 strcat(buf, "OPTIM == 4,");

    OPT0 DBG strcat(buf, "DEVELDBG (OPT0 && DBG && DEV),");
    OPT2 DBG strcat(buf, "DEVELOPT (OPT2 && DBG && DEV),");
    OPT2 REL strcat(buf, "OPTIMSYM (OPT2 && REL && DEV),");
    SHIPPING strcat(buf, "SHIPPING (OPT3 && REL && PUB),");

    puts( collected_flags );
}
*/
