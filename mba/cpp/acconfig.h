/* $Id: acconfig.h,v 1.13 2001/07/25 22:33:31 bhudson Exp $ */

/*
 * Handling a bool type on systems that don't have one.
 * 'bool' should be defined to 'int'
 * 'true' should be defined to 1
 * 'false' should be defined to 0
 * This may give problems when including other code, if that code decides
 * to use typedef and const ints instead (I've seen that, unfortunately).
 */
#undef bool
#undef true
#undef false

/*
 * If the system doesn't allow placement new by default, 
 * we have to define it ourselves.
 */
#undef HAVE_PLACEMENT_NEW

/* Define 'IRIX' under that OS. */
#undef IRIX

/*
 * After this point, we only modify things that depend on the options
 * given to configure.
 *
 * Which search algorithms to include.
 */
#undef DISABLE_COVER
#undef DISABLE_CBFS

/* which progress methods to include */
#undef DISABLE_MIN_PROGRESS
#undef DISABLE_FULL_PROGRESS

/* whether the code for checkpoints is included */
#undef ENABLE_CHECKPOINT

/* whether to compile  system to perform recovery rather than diagnosis */
#undef ENABLE_RECOVERY

/* Error handling:
 *      - whether to throw exceptions
 *      - if we don't throw exceptions, whether to print a verbose description
 *        of the error.  We can save a lot of code by printing just the
 *        exception type (i.e. by not defining E_P_E).  E_P_E is ignored if
 *        E_E is defined.
 */
#undef ENABLE_EXCEPTIONS
#undef ENABLE_PRINT_ERROR

/* Control what we do in our special-purpose allocators:
 *      - disable them entirely ; useful for things like purify
 *      - print a report about the max number of elements allocated at any one
 *        time (or some approximation thereof)
 *      - disallow dynamically allocating anything
 *      - the level of memory checks (1 is cheap, 3 prohibitively expensive)
 */
#undef DISABLE_L2_ALLOCATORS
#undef ENABLE_COUNT_MAX_POOL_SIZE
#undef DISABLE_DYNAMIC_MEMORY
#undef ENABLE_MEMORY_CHECKS_1
#undef ENABLE_MEMORY_CHECKS_2
#undef ENABLE_MEMORY_CHECKS_3

/*
 * Model reader features:
 *      - whether to include the optimizer
 *      - whether to include debugging sections
 *      - whether to allow verbose output (which even if enabled is off by
 *              default)
 *      - whether to allow the XMPL or L2 file formats
 */
#undef DISABLE_OPTIMIZER
#undef ENABLE_L2_DEBUG_SECTIONS
#undef ENABLE_L2_VERBOSE
#undef ENABLE_L2_FORMAT
#undef ENABLE_L2BIN_FORMAT
#undef ENABLE_XMPL_FORMAT
