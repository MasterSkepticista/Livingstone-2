/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: def_conf.h,v 1.7 2001/11/16 19:32:08 lbrown Exp $ */
/**
 * Based on gnu_conf.h created on Solaris 2.8 with:
 *     ./configure
 * I manually got rid of NEW_THROWS_EXCEPTION, which may cause warnings on your
 * platform.  Depending on your platform, you may need to define the bool type
 * (just uncomment the bool, true, and false lines).
 */

/* whether new throws a bad_alloc or returns 0.   */
/* #define NEW_THROWS_EXCEPTION 1 */

/* handling a bool type on systems that don't have one */
/* #define bool */
/* #define true  1 */
/* #define false 0 */

/* which search algorithms to include */
/* #define DISABLE_COVER */
/* #define DISABLE_CBFS */

/* which progress methods to include */
/* #define DISABLE_MIN_PROGRESS */
/* #define DISABLE_FULL_PROGRESS */

/* whether the code for checkpoints is included */
#define ENABLE_CHECKPOINT 1

/* whether to compile  system to perform recovery rather than diagnosis */
/* #define ENABLE_RECOVERY */

/* Error handling:
 *      - whether to throw exceptions
 *      - if we don't throw exceptions, whether to print a verbose description
 *        of the error.  We can save a lot of code by printing just the
 *        exception type.  E_P_E is ignored if E_E is defined.
 */
#define ENABLE_EXCEPTIONS 1
/* #define ENABLE_PRINT_ERROR */

/* Control what we do in our special-purpose allocators:
 *      - print a report about the max number of elements allocated at any one
 *        time (or some approximation thereof)
 *      - disallow dynamically allocating anything
 *      - the level of memory checks (1 is cheap, 3 prohibitively expensive)
 */
/* #define ENABLE_COUNT_MAX_POOL_SIZE */
/* #define DISABLE_DYNAMIC_MEMORY */
#define ENABLE_MEMORY_CHECKS_1 1
/* #define ENABLE_MEMORY_CHECKS_2 */
/* #define ENABLE_MEMORY_CHECKS_3 */

/*
 * Model reader features:
 *      - whether to include the optimizer
 *      - whether to include debugging sections
 *      - whether to allow verbose output (which even if enabled is off by
 *              default)
 *      - whether to allow the XMPL or L2 file formats
 */
/* #define DISABLE_OPTIMIZER */
#define ENABLE_L2_DEBUG_SECTIONS 1
#define ENABLE_L2_VERBOSE 1
#define ENABLE_L2_FORMAT 1
#define ENABLE_L2BIN_FORMAT 1
#define ENABLE_XMPL_FORMAT 1

/* Define 'IRIX' under that OS. */
/* #define IRIX */
