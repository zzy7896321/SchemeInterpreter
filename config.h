/** @file
 *  config.h declares configuration function and constants.
 */

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <cstdio>
#include <gmp.h>

/** Default floating-point number precision */
extern mp_bitcnt_t _scm_mpf_default_precision;

/** The maximum absolute number of the length of an inexact number to be converted into a rational number when parsed. */
extern unsigned _scm_inexact_conversion_to_rational_bound;

/** The maximum number of prefix zero to be added to a real number display. */
extern unsigned _scm_maximum_prefix_zero_of_fixed;

extern bool _scm_protect_builtin;

void _scm_init_all();

#endif // CONFIG_H_INCLUDED
