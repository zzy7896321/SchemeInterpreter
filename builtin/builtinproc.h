/** @file
 *  builtinproc.h contains the declaration of all the builtin procedures.
 */

#ifndef BUILTINPROC_H_INCLUDED
#define BUILTINPROC_H_INCLUDED

#include "../object/objectdef.h"
#include "../interp/errorinfo.h"

#include <memory>
#include <vector>
#include <iostream>

namespace BuiltinProc{

typedef Interp::callResultType callResultType;
typedef Interp::parameterType parameterType;
typedef std::shared_ptr<ObjectDef::Object> sptrObject;
typedef std::shared_ptr<ObjectDef::Environment> sptrEnvironment;
typedef	std::function<callResultType(parameterType, sptrObject&)> builtinFuncType;

struct IOStreamContainer{
    IOStreamContainer(std::istream& _input, std::ostream& _output):
        input(_input), output(_output){}

    std::istream& input;
    std::ostream& output;
};

extern std::shared_ptr<IOStreamContainer> iostreamContainer;

void initBuiltinEnv();
void initIOStream(std::istream& input, std::ostream& output);


// Section 6.1 Equivalence predicates

/**
 * (eqv? obj1 obj2)	procedure
 */
callResultType eqv_func(parameterType, sptrObject&);

/**
 * (eq? obj1 obj2) procedure
 */
callResultType eq_func(parameterType, sptrObject&);

/**
 * (equal? obj1 obj2) libarary procedure
 */
callResultType equal_func(parameterType, sptrObject&);



// Section 6.2.5 Numerical operations

/**
 * (number? obj)	procedure
 */
callResultType number_func(parameterType, sptrObject&);

/**
 * (complex? obj)	procedure
 */
callResultType complex_func(parameterType, sptrObject&);

/**
 * (real? obj)	procedure
 */
callResultType real_func(parameterType, sptrObject&);

/**
 * (rational? obj)	procedure
 */
callResultType rational_func(parameterType, sptrObject&);

/**
 * (integer? obj)	procedure
 */
callResultType integer_func(parameterType, sptrObject&);

/**
 * (exact? z)	procedure
 */
callResultType exact_func(parameterType, sptrObject&);

/**
 * (inexact? z)	procedure
 */
callResultType inexact_func(parameterType, sptrObject&);

/**
 * (= z1 z2 z3 ...)	procedure
 */
callResultType number_equal_func(parameterType, sptrObject&);

/**
 * (< n1 n2 n3 ...)	procedure
 */
callResultType number_lessThan_func(parameterType, sptrObject&);

/**
 * (> n1 n2 n3 ...)	procedure
 */
callResultType number_greaterThan_func(parameterType, sptrObject&);

/**
 * (<= n1 n2 n3 ...)	procedure
 */
callResultType number_leq_func(parameterType, sptrObject&);

/**
 * (>= n1 n2 n3 ...)	procedure
 */
callResultType number_geq_func(parameterType, sptrObject&);

/**
 * (zero? z)	library procedure
 */
callResultType zero_func(parameterType, sptrObject&);

/**
 * (positive? x)	library procedure
 */
callResultType positive_func(parameterType, sptrObject&);

/**
 * (negative? x)	library procedure
 */
callResultType negative_func(parameterType, sptrObject&);

/**
 * (odd? n)	library procedure
 */
callResultType odd_func(parameterType, sptrObject&);

/**
 * (even? n)	library procedure
 */
callResultType even_func(parameterType, sptrObject&);

/**
 * (max x1 x2 ...)	library procedure
 */
callResultType max_func(parameterType, sptrObject&);

/**
 * (min x1 x2 ...)	library procedure
 */
callResultType min_func(parameterType, sptrObject&);

/**
 * (+ z1 ...)	procedure
 */
callResultType plus_func(parameterType, sptrObject&);

/**
 * (* z1 ...)	procedure
 */
callResultType times_func(parameterType, sptrObject&);

/**
 * (- z1 z2)	procedure
 * (- z)	procedure
 * (- z1 z2 ...)	optional procedure
 */
callResultType minus_func(parameterType, sptrObject&);

/**
 * (/ z1 z2)	procedure
 * (/ z)	procedure
 * (/ z1 z2 ...)	optional procedure
 */
callResultType divide_func(parameterType, sptrObject&);

/**
 * (abs x)	library procedure
 */
callResultType abs_func(parameterType, sptrObject&);

/**
 * (quotient n1 n2)	procedure
 */
callResultType quotient_func(parameterType, sptrObject&);

/**
 * (remainder n1 n2)	procedure
 */
callResultType remainder_func(parameterType, sptrObject&);

/**
 * (modulo n1 n2)	procedure
 */
callResultType modulo_func(parameterType, sptrObject&);

/**
 * (gcd n1 ...) library procedure
 */
callResultType gcd_func(parameterType, sptrObject&);

/**
 * (lcm n1 ...) library procedure
 */
callResultType lcm_func(parameterType, sptrObject&);

/**
 * (numerator q)  procedure
 */
callResultType numerator_func(parameterType, sptrObject&);

/**
 * (denominator q)  procedure
 */
callResultType denominator_func(parameterType, sptrObject&);

/**
 * (floor x)  procedure
 */
callResultType floor_func(parameterType, sptrObject&);

/**
 * (ceiling x)  procedure
 */
callResultType ceiling_func(parameterType, sptrObject&);

/**
 * (truncate x)  procedure
 */
callResultType truncate_func(parameterType, sptrObject&);

/**
 * (round x)  procedure
 */
callResultType round_func(parameterType, sptrObject&);

/**
 * (rationalize x y) library procedure (not implemented)
 */
callResultType rationalize_func(parameterType, sptrObject&);

/**
 * (exp z)  procedure
 */
callResultType exp_func(parameterType, sptrObject&);

/**
 * (log z)  procedure
 */
callResultType log_func(parameterType, sptrObject&);

/**
 * (sin z)  procedure
 */
callResultType sin_func(parameterType, sptrObject&);

/**
 * (cos z)  procedure
 */
callResultType cos_func(parameterType, sptrObject&);

/**
 * (tan z)  procedure
 */
callResultType tan_func(parameterType, sptrObject&);

/**
 * (asin z)  procedure
 */
callResultType asin_func(parameterType, sptrObject&);

/**
 * (acos z)  procedure
 */
callResultType acos_func(parameterType, sptrObject&);

/**
 * (atan z)  procedure
 * (atan y x) procedure
 */
callResultType atan_func(parameterType, sptrObject&);

/**
 * (sqrt z)  procedure
 */
callResultType sqrt_func(parameterType, sptrObject&);

/**
 * (expt z1 z2)  procedure
 */
callResultType expt_func(parameterType, sptrObject&);

/**
 * (make-rectangular x1 x2)  procedure
 */
callResultType make_rectangular_func(parameterType, sptrObject&);

/**
 * (make-polar)  procedure
 */
callResultType make_polar_func(parameterType, sptrObject&);

/**
 * (real-part z)  procedure
 */
callResultType real_part_func(parameterType, sptrObject&);

/**
 * (imag-part z)  procedure
 */
callResultType imag_part_func(parameterType, sptrObject&);

/**
 * (magnitude z)  procedure
 */
callResultType magnitude_func(parameterType, sptrObject&);

/**
 * (angle z)  procedure
 */
callResultType angle_func(parameterType, sptrObject&);

/**
 * (exact->inexact z)  procedure
 */
callResultType exactToInexact_func(parameterType, sptrObject&);

/**
 * (inexact->exact z)  procedure
 */
callResultType inexactToExact_func(parameterType, sptrObject&);


// Section 6.2.6 Numerical input and output

/**
 * (number->string z)  procedure
 * (number->string z radix) procedure
 */
callResultType numberToString_func(parameterType, sptrObject&);

/**
 * (string->number string)  procedure
 * (string->number string radix)    procedure
 */
callResultType stringToNumber_func(parameterType, sptrObject&);


// Section 6.3.1 Booleans

/**
 * (not obj) library procedure
 */
callResultType not_func(parameterType, sptrObject&);

/**
 * (boolean? obj) library procedure
 */
callResultType boolean_func(parameterType, sptrObject&);

// Section 6.3.2 Pairs and lists

/**
 * (pair? obj) procedure
 */
callResultType pair_func(parameterType, sptrObject&);

/**
 * (cons obj1 obj2) procedure
 */
callResultType cons_func(parameterType, sptrObject&);

/**
 * (car pair) procedure
 */
callResultType car_func(parameterType, sptrObject&);

/**
 * (cdr pair) procedure
 */
callResultType cdr_func(parameterType, sptrObject&);

/**
 * (set-car! pair obj) procedure
 */

callResultType set_car_func(parameterType, sptrObject&);

/**
 * (set-cdr! pair obj) procedure
 */
callResultType set_cdr_func(parameterType, sptrObject&);


/**
 * (caar pair) library procedure
 */
callResultType caar_func(parameterType, sptrObject&);

/**
 * (cdar pair) library procedure
 */
callResultType cdar_func(parameterType, sptrObject&);

/**
 * (cadr pair) library procedure
 */
callResultType cadr_func(parameterType, sptrObject&);

/**
 * (cddr pair) library procedure
 */
callResultType cddr_func(parameterType, sptrObject&);

/**
 * (caaar pair) library procedure
 */
callResultType caaar_func(parameterType, sptrObject&);

/**
 * (cdaar pair) library procedure
 */
callResultType cdaar_func(parameterType, sptrObject&);

/**
 * (cadar pair) library procedure
 */
callResultType cadar_func(parameterType, sptrObject&);

/**
 * (cddar pair) library procedure
 */
callResultType cddar_func(parameterType, sptrObject&);

/**
 * (caadr pair) library procedure
 */
callResultType caadr_func(parameterType, sptrObject&);

/**
 * (cdadr pair) library procedure
 */
callResultType cdadr_func(parameterType, sptrObject&);

/**
 * (caddr pair) library procedure
 */
callResultType caddr_func(parameterType, sptrObject&);

/**
 * (cdddr pair) library procedure
 */
callResultType cdddr_func(parameterType, sptrObject&);

/**
 * (caaaar pair) library procedure
 */
callResultType caaaar_func(parameterType, sptrObject&);

/**
 * (cdaaar pair) library procedure
 */
callResultType cdaaar_func(parameterType, sptrObject&);

/**
 * (cadaar pair) library procedure
 */
callResultType cadaar_func(parameterType, sptrObject&);

/**
 * (cddaar pair) library procedure
 */
callResultType cddaar_func(parameterType, sptrObject&);

/**
 * (caadar pair) library procedure
 */
callResultType caadar_func(parameterType, sptrObject&);

/**
 * (cdadar pair) library procedure
 */
callResultType cdadar_func(parameterType, sptrObject&);

/**
 * (caddar pair) library procedure
 */
callResultType caddar_func(parameterType, sptrObject&);

/**
 * (cdddar pair) library procedure
 */
callResultType cdddar_func(parameterType, sptrObject&);

/**
 * (caaadr pair) library procedure
 */
callResultType caaadr_func(parameterType, sptrObject&);

/**
 * (cdaadr pair) library procedure
 */
callResultType cdaadr_func(parameterType, sptrObject&);

/**
 * (cadadr pair) library procedure
 */
callResultType cadadr_func(parameterType, sptrObject&);

/**
 * (cddadr pair) library procedure
 */
callResultType cddadr_func(parameterType, sptrObject&);

/**
 * (caaddr pair) library procedure
 */
callResultType caaddr_func(parameterType, sptrObject&);

/**
 * (cdaddr pair) library procedure
 */
callResultType cdaddr_func(parameterType, sptrObject&);

/**
 * (cadddr pair) library procedure
 */
callResultType cadddr_func(parameterType, sptrObject&);

/**
 * (cddddr pair) library procedure
 */
callResultType cddddr_func(parameterType, sptrObject&);

/**
 * (null? obj) library procedure
 */
callResultType null_func(parameterType, sptrObject&);

/**
 * (list? obj) library procedure
 */
callResultType listQuestion_func(parameterType, sptrObject&);

/**
 * (list obj ...) library procedure
 */
callResultType list_func(parameterType, sptrObject&);

/**
 * (length list) library procedure
 */
callResultType length_func(parameterType, sptrObject&);

/**
 * (append list ...) library procedure
 */
callResultType append_func(parameterType, sptrObject&);

/**
 * (reverse list) library procedure
 */
callResultType reverse_func(parameterType, sptrObject&);

/**
 * (list-tail list k) library procedure
 */
callResultType list_tail_func(parameterType, sptrObject&);

/**
 * (list-ref list k) library procedure
 */
callResultType list_ref_func(parameterType, sptrObject&);

/**
 * (memq obj list) library procedure
 */
callResultType memq_func(parameterType, sptrObject&);

/**
 * (memv obj list) library procedure
 */
callResultType memv_func(parameterType, sptrObject&);

/**
 * (member obj list) library procedure
 */
callResultType member_func(parameterType, sptrObject&);

/**
 * (assq obj alist) library procedure
 */
callResultType assq_func(parameterType, sptrObject&);

/**
 * (assv obj alist) library procedure
 */
callResultType assv_func(parameterType, sptrObject&);

/**
 * (assoc obj alist) library procedure
 */
callResultType assoc_func(parameterType, sptrObject&);



// Section 6.3.3 Symbols

/**
 * (symbol? obj)  procedure
 */
callResultType symbol_func(parameterType, sptrObject&);

/**
 * (symbol->string symbol)  procedure
 */
callResultType symbolToString_func(parameterType, sptrObject&);

/**
 * (string->symbol string)  procedure
 */
callResultType stringToSymbol_func(parameterType, sptrObject&);

// Section 6.3.4 Characters

/**
 * (char? obj)  procedure
 */
callResultType char_func(parameterType, sptrObject&);

/**
 * (char=? char1 char2)  procedure
 */
callResultType char_equal_func(parameterType, sptrObject&);

/**
 * (char<? char1 char2)  procedure
 */
callResultType char_lessThan_func(parameterType, sptrObject&);

/**
 * (char>? char1 char2)  procedure
 */
callResultType char_greaterThan_func(parameterType, sptrObject&);

/**
 * (char<=? char1 char2)  procedure
 */
callResultType char_leq_func(parameterType, sptrObject&);

/**
 * (char>=? char1 char2)  procedure
 */
callResultType char_geq_func(parameterType, sptrObject&);


/**
 * (char-ci=? char1 char2) library procedure
 */
callResultType char_ci_equal_func(parameterType, sptrObject&);

/**
 * (char-ci<? char1 char2) library procedure
 */
callResultType char_ci_lessThan_func(parameterType, sptrObject&);

/**
 * (char-ci>? char1 char2) library procedure
 */
callResultType char_ci_greaterThan_func(parameterType, sptrObject&);

/**
 * (char-ci<=? char1 char2) library procedure
 */
callResultType char_ci_leq_func(parameterType, sptrObject&);

/**
 * (char-ci>=? char1 char2) library procedure
 */
callResultType char_ci_geq_func(parameterType, sptrObject&);


/**
 * (char-alphabetic? char) library procedure
 */
callResultType char_alphabetic_func(parameterType, sptrObject&);

/**
 * (char-numeric? char) library procedure
 */
callResultType char_numeric_func(parameterType, sptrObject&);

/**
 * (char-whitespace? char) library procedure
 */
callResultType char_whitespace_func(parameterType, sptrObject&);

/**
 * (char-upper-case? char) library procedure
 */
callResultType char_upper_case_func(parameterType, sptrObject&);

/**
 * (char-lower-case? char) library procedure
 */
callResultType char_lower_case_func(parameterType, sptrObject&);

/**
 * (char->integer char)  procedure
 */
callResultType charToInteger_func(parameterType, sptrObject&);

/**
 * (integer->char n)  procedure
 */
callResultType integerToChar_func(parameterType, sptrObject&);

/**
 * (char-upcase char) library procedure
 */
callResultType char_upcase_func(parameterType, sptrObject&);

/**
 * (char-downcase char) library procedure
 */
callResultType char_downcase_func(parameterType, sptrObject&);


// Section 6.3.5 Strings

/**
 * (string? obj)  procedure
 */
callResultType string_func(parameterType, sptrObject&);

/**
 * (make-string k)  procedure
 * (make-string k char) procedure
 */
callResultType make_string_func(parameterType, sptrObject&);

/**
 * (string char ...) library procedure
 */
callResultType string_compose_func(parameterType, sptrObject&);

/**
 * (string-length string)  procedure
 */
callResultType string_length_func(parameterType, sptrObject&);

/**
 * (string-ref string k)  procedure
 */
callResultType string_ref_func(parameterType, sptrObject&);

/**
 * (string-set! string k char)  procedure
 */
callResultType string_set_func(parameterType, sptrObject&);

/**
 * (string=? string1 string2) library procedure
 */
callResultType string_equal_func(parameterType, sptrObject&);

/**
 * (string-ci=? string1 string2) library procedure
 */
callResultType string_ci_equal_func(parameterType, sptrObject&);

/**
 * (string<? string1 string2) library procedure
 */
callResultType string_lessThan_func(parameterType, sptrObject&);

/**
 * (string>? string1 string2) library procedure
 */
callResultType string_greaterThan_func(parameterType, sptrObject&);

/**
 * (string<=? string1 string2) library procedure
 */
callResultType string_leq_func(parameterType, sptrObject&);

/**
 * (string>=? string1 string2) library procedure
 */
callResultType string_geq_func(parameterType, sptrObject&);

/**
 * (string-ci<? string1 string2) library procedure
 */
callResultType string_ci_lessThan_func(parameterType, sptrObject&);

/**
 * (string-ci>? string1 string2) library procedure
 */
callResultType string_ci_greaterThan_func(parameterType, sptrObject&);

/**
 * (string-ci<=? string1 string2) library procedure
 */
callResultType string_ci_leq_func(parameterType, sptrObject&);

/**
 * (string-ci>=? string1 string2) library procedure
 */
callResultType string_ci_geq_func(parameterType, sptrObject&);

/**
 * (substring string start end) library procedure
 */
callResultType substring_func(parameterType, sptrObject&);

/**
 * (string-append string ...) library procedure
 */
callResultType string_append_func(parameterType, sptrObject&);

/**
 * (string->list string) library procedure
 */
callResultType stringToList_func(parameterType, sptrObject&);

/**
 * (list->string list) library procedure
 */
callResultType listToString_func(parameterType, sptrObject&);

/**
 * (string-copy string) library procedure
 */
callResultType string_copy_func(parameterType, sptrObject&);

/**
 * (string-fill! string char) library procedure
 */
callResultType string_fill_func(parameterType, sptrObject&);

// Section 6.4 Control features

/**
 * (procedure? obj)  procedure
 */
callResultType procedure_func(parameterType, sptrObject&);

/**
 * (apply proc arg1 ... args)  procedure
 */
callResultType apply_func(parameterType, sptrObject&);

/**
 * (map proc list1 list2 ...) library procedure
 */
callResultType map_func(parameterType, sptrObject&);

/**
 * (for-each proc list1 list2 ...) library procedure
 */
callResultType for_each_func(parameterType, sptrObject&);

/**
 * (force promise)	library procedure
 */
callResultType force_func(parameterType, sptrObject&);

// Section 6.6.3 Output

/**
 * (display obj)    library procedure
 */
callResultType display_func(parameterType, sptrObject&);

/**
 * (newline)    library procedure
 */
callResultType newline_func(parameterType, sptrObject&);

// Section 6.6.4 System interface

/**
 * (load filename)  procedure
 */
callResultType load_func(parameterType, sptrObject&);

}

#endif	//BUILTINPROC_H_INCLUEDED

