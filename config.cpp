#include "config.h"

#include "builtin/builtinproc.h"
#include <cstdio>
#include <gmpxx.h>
#include <gmp.h>

mp_bitcnt_t _scm_mpf_default_precision = 128;

unsigned _scm_inexact_conversion_to_rational_bound = 10;

unsigned _scm_maximum_prefix_zero_of_fixed = 10;

bool _scm_protect_builtin = true;

void _scm_init_all(){
    mpf_set_default_prec(_scm_mpf_default_precision);

    #ifdef _SCMINTERP_DEBUG_PRINT_
        std::freopen("run.log", "w", stderr);
    #endif

    BuiltinProc::initBuiltinEnv();
    BuiltinProc::initIOStream(std::cin, std::cout);
}
