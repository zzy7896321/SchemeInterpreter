#include "builtinproc.h"

#include <iostream>

namespace BuiltinProc{

void initBuiltinEnv(){
	using ObjectDef::ObjectType;
	auto builtinEnv = ObjectDef::Environment::getBuiltinEnv();

	//Secion 6.1 Equivalence predicates

	builtinEnv->insert("eqv?", sptrObject(new ObjectDef::Builtin(
                       "eqv?",
					   {ObjectType::OBJECT,
						ObjectType::OBJECT},
					   false,
					   eqv_func)));

	builtinEnv->insert("eq?", sptrObject(new ObjectDef::Builtin(
					   "eq?",
					   {ObjectType::OBJECT,
					    ObjectType::OBJECT},
					   false,
					   eq_func)));

	builtinEnv->insert("equal?", sptrObject(new ObjectDef::Builtin(
					   "equal?",
					   {ObjectType::OBJECT,
					    ObjectType::OBJECT},
					   false,
					   equal_func)));

	// Section 6.2.5 Numerical operations

	builtinEnv->insert("number?", sptrObject(new ObjectDef::Builtin(
					   "number?",
					   {ObjectType::OBJECT},
					   false,
					   number_func)));

	builtinEnv->insert("complex?", sptrObject(new ObjectDef::Builtin(
					   "complex?",
					   {ObjectType::OBJECT},
					   false,
					   complex_func)));

	builtinEnv->insert("real?", sptrObject(new ObjectDef::Builtin(
					   "real?",
					   {ObjectType::OBJECT},
					   false,
					   real_func)));

	builtinEnv->insert("rational?", sptrObject(new ObjectDef::Builtin(
					   "rational?",
					   {ObjectType::OBJECT},
					   false,
					   rational_func)));

	builtinEnv->insert("integer?", sptrObject(new ObjectDef::Builtin(
					   "integer?",
					   {ObjectType::OBJECT},
					   false,
					   integer_func)));

	builtinEnv->insert("exact?", sptrObject(new ObjectDef::Builtin(
					   "exact?",
					   {ObjectType::COMPLEX},
					   false,
					   exact_func)));

	builtinEnv->insert("inexact?", sptrObject(new ObjectDef::Builtin(
					   "inexact?",
					   {ObjectType::COMPLEX},
					   false,
					   inexact_func)));

	builtinEnv->insert("=", sptrObject(new ObjectDef::Builtin(
					   "=",
					   {ObjectType::COMPLEX,
					    ObjectType::COMPLEX,
						ObjectType::COMPLEX},
					   true,
					   number_equal_func)));

	builtinEnv->insert("<", sptrObject(new ObjectDef::Builtin(
					   "<",
					   {ObjectType::REAL,
					    ObjectType::REAL,
						ObjectType::REAL},
					   true,
					   number_lessThan_func)));

	builtinEnv->insert(">", sptrObject(new ObjectDef::Builtin(
					   ">",
					   {ObjectType::REAL,
					    ObjectType::REAL,
						ObjectType::REAL},
					   true,
					   number_greaterThan_func)));

	builtinEnv->insert("<=", sptrObject(new ObjectDef::Builtin(
					   "<=",
					   {ObjectType::REAL,
					    ObjectType::REAL,
						ObjectType::REAL},
					   true,
					   number_leq_func)));

	builtinEnv->insert(">=", sptrObject(new ObjectDef::Builtin(
					   ">=",
					   {ObjectType::REAL,
					    ObjectType::REAL,
						ObjectType::REAL},
					   true,
					   number_geq_func)));

	builtinEnv->insert("zero?", sptrObject(new ObjectDef::Builtin(
					   "zero?",
					   {ObjectType::COMPLEX},
					   false,
					   zero_func)));

	builtinEnv->insert("positive?", sptrObject(new ObjectDef::Builtin(
					   "positive?",
					   {ObjectType::REAL},
					   false,
					   positive_func)));

	builtinEnv->insert("negative?", sptrObject(new ObjectDef::Builtin(
					   "negative?",
					   {ObjectType::REAL},
					   false,
					   negative_func)));

	builtinEnv->insert("odd?", sptrObject(new ObjectDef::Builtin(
					   "odd?",
					   {ObjectType::INT},
					   false,
					   odd_func)));

	builtinEnv->insert("even?", sptrObject(new ObjectDef::Builtin(
					   "even?",
					   {ObjectType::INT},
					   false,
					   even_func)));

	builtinEnv->insert("max", sptrObject(new ObjectDef::Builtin(
					   "max",
					   {ObjectType::REAL,
					    ObjectType::REAL},
					   true,
					   max_func)));

	builtinEnv->insert("min", sptrObject(new ObjectDef::Builtin(
					   "min",
					   {ObjectType::REAL,
					    ObjectType::REAL},
					   true,
					   min_func)));

	builtinEnv->insert("+", sptrObject(new ObjectDef::Builtin(
					   "+",
					   {ObjectType::COMPLEX},
					   true,
					   plus_func)));

	builtinEnv->insert("*", sptrObject(new ObjectDef::Builtin(
					   "*",
					   {ObjectType::COMPLEX},
					   true,
					   times_func)));

	builtinEnv->insert("-", sptrObject(new ObjectDef::Builtin(
					   "-",
					   {ObjectType::COMPLEX,
					    ObjectType::COMPLEX},
					   true,
					   minus_func)));

	builtinEnv->insert("/", sptrObject(new ObjectDef::Builtin(
					   "/",
					   {ObjectType::COMPLEX,
					    ObjectType::COMPLEX},
					   true,
					   divide_func)));

    builtinEnv->insert("abs", sptrObject(new ObjectDef::Builtin(
					   "abs",
					   {ObjectType::REAL},
					   false,
					   abs_func)));

    builtinEnv->insert("quotient", sptrObject(new ObjectDef::Builtin(
					   "quotient",
					   {ObjectType::INT,
					    ObjectType::INT},
					   false,
					   quotient_func)));

    builtinEnv->insert("remainder", sptrObject(new ObjectDef::Builtin(
					   "remainder",
					   {ObjectType::INT,
					    ObjectType::INT},
					   false,
					   remainder_func)));

    builtinEnv->insert("modulo", sptrObject(new ObjectDef::Builtin(
					   "modulo",
					   {ObjectType::INT,
					    ObjectType::INT},
					   false,
					   modulo_func)));

	builtinEnv->insert("gcd", sptrObject(new ObjectDef::Builtin(
                       "gcd",
                       {ObjectType::INT},
                       true,
                       gcd_func)));

	builtinEnv->insert("lcm", sptrObject(new ObjectDef::Builtin(
                       "lcm",
                       {ObjectType::INT},
                       true,
                       lcm_func)));

	builtinEnv->insert("numerator", sptrObject(new ObjectDef::Builtin(
                       "numerator",
                       {ObjectType::RATIONAL},
                       false,
                       numerator_func)));

	builtinEnv->insert("denominator", sptrObject(new ObjectDef::Builtin(
                       "denominator",
                       {ObjectType::RATIONAL},
                       false,
                       denominator_func)));

	builtinEnv->insert("floor", sptrObject(new ObjectDef::Builtin(
                       "floor",
                       {ObjectType::REAL},
                       false,
                       floor_func)));

	builtinEnv->insert("ceiling", sptrObject(new ObjectDef::Builtin(
                       "ceiling",
                       {ObjectType::REAL},
                       false,
                       ceiling_func)));

	builtinEnv->insert("truncate", sptrObject(new ObjectDef::Builtin(
                       "truncate",
                       {ObjectType::REAL},
                       false,
                       truncate_func)));

	builtinEnv->insert("round", sptrObject(new ObjectDef::Builtin(
                       "round",
                       {ObjectType::REAL},
                       false,
                       round_func)));

	/*builtinEnv->insert("rationalize", sptrObject(new ObjectDef::Builtin(
                       "rationalize",
                       {ObjectType::REAL,
                        ObjectType::REAL},
                       false,
                       rationalize_func)));*/

	builtinEnv->insert("exp", sptrObject(new ObjectDef::Builtin(
                       "exp",
                       {ObjectType::COMPLEX},
                       false,
                       exp_func)));

	builtinEnv->insert("log", sptrObject(new ObjectDef::Builtin(
                       "log",
                       {ObjectType::COMPLEX},
                       false,
                       log_func)));

	builtinEnv->insert("sin", sptrObject(new ObjectDef::Builtin(
                       "sin",
                       {ObjectType::COMPLEX},
                       false,
                       sin_func)));

	builtinEnv->insert("cos", sptrObject(new ObjectDef::Builtin(
                       "cos",
                       {ObjectType::COMPLEX},
                       false,
                       cos_func)));

	builtinEnv->insert("tan", sptrObject(new ObjectDef::Builtin(
                       "tan",
                       {ObjectType::COMPLEX},
                       false,
                       tan_func)));

	builtinEnv->insert("asin", sptrObject(new ObjectDef::Builtin(
                       "asin",
                       {ObjectType::COMPLEX},
                       false,
                       asin_func)));

	builtinEnv->insert("acos", sptrObject(new ObjectDef::Builtin(
                       "acos",
                       {ObjectType::COMPLEX},
                       false,
                       acos_func)));

	builtinEnv->insert("atan", sptrObject(new ObjectDef::Builtin(
                       "atan",
                       {ObjectType::COMPLEX},
                       true,
                       atan_func)));

	builtinEnv->insert("sqrt", sptrObject(new ObjectDef::Builtin(
                       "sqrt",
                       {ObjectType::COMPLEX},
                       false,
                       sqrt_func)));

	builtinEnv->insert("expt", sptrObject(new ObjectDef::Builtin(
                       "expt",
                       {ObjectType::COMPLEX,
                        ObjectType::COMPLEX},
                       false,
                       expt_func)));

	builtinEnv->insert("make-rectangular", sptrObject(new ObjectDef::Builtin(
                       "make-rectangular",
                       {ObjectType::REAL,
                        ObjectType::REAL},
                       false,
                       make_rectangular_func)));

	builtinEnv->insert("make-polar", sptrObject(new ObjectDef::Builtin(
                       "make-polar",
                       {ObjectType::REAL,
                        ObjectType::REAL},
                       false,
                       make_polar_func)));

	builtinEnv->insert("real-part", sptrObject(new ObjectDef::Builtin(
                       "real-part",
                       {ObjectType::COMPLEX},
                       false,
                       real_part_func)));

	builtinEnv->insert("imag-part", sptrObject(new ObjectDef::Builtin(
                       "imag-part",
                       {ObjectType::COMPLEX},
                       false,
                       imag_part_func)));

	builtinEnv->insert("magnitude", sptrObject(new ObjectDef::Builtin(
                       "magnitude",
                       {ObjectType::COMPLEX},
                       false,
                       magnitude_func)));

	builtinEnv->insert("angle", sptrObject(new ObjectDef::Builtin(
                       "angle",
                       {ObjectType::COMPLEX},
                       false,
                       angle_func)));

	builtinEnv->insert("exact->inexact", sptrObject(new ObjectDef::Builtin(
                       "exact->inexact",
                       {ObjectType::COMPLEX},
                       false,
                       exactToInexact_func)));

	builtinEnv->insert("inexact->exact", sptrObject(new ObjectDef::Builtin(
                       "inexact->exact",
                       {ObjectType::COMPLEX},
                       false,
                       inexactToExact_func)));

    // Section 6.2.6 Numerical input and output

	builtinEnv->insert("number->string", sptrObject(new ObjectDef::Builtin(
                       "number->string",
                       {ObjectType::COMPLEX,
                        ObjectType::INT},
                       true,
                       numberToString_func)));

	builtinEnv->insert("string->number", sptrObject(new ObjectDef::Builtin(
                       "string->number",
                       {ObjectType::STRING,
                        ObjectType::INT},
                       true,
                       stringToNumber_func)));

    // Section 6.3.1 Booleans

	builtinEnv->insert("not", sptrObject(new ObjectDef::Builtin(
                       "not",
                       {ObjectType::OBJECT},
                       false,
                       not_func)));

	builtinEnv->insert("boolean?", sptrObject(new ObjectDef::Builtin(
                       "boolean?",
                       {ObjectType::OBJECT},
                       false,
                       boolean_func)));

	// Section 6.3.2 Pairs and lists

	builtinEnv->insert("pair?", sptrObject(new ObjectDef::Builtin(
					   "pair?",
					   {ObjectType::OBJECT},
					   false,
					   pair_func)));

	builtinEnv->insert("cons", sptrObject(new ObjectDef::Builtin(
					   "cons",
					   {ObjectType::OBJECT,
					    ObjectType::OBJECT},
					   false,
					   cons_func)));

	builtinEnv->insert("car", sptrObject(new ObjectDef::Builtin(
					   "car",
					   {ObjectType::PAIR},
					   false,
					   car_func)));

	builtinEnv->insert("cdr", sptrObject(new ObjectDef::Builtin(
					   "cdr",
					   {ObjectType::PAIR},
					   false,
					   cdr_func)));

	builtinEnv->insert("set-car!", sptrObject(new ObjectDef::Builtin(
					   "set-car!",
					   {ObjectType::PAIR,
					    ObjectType::OBJECT},
					   false,
					   set_car_func)));

	builtinEnv->insert("set-cdr!", sptrObject(new ObjectDef::Builtin(
					   "set-cdr!",
					   {ObjectType::PAIR,
					    ObjectType::OBJECT},
					   false,
					   set_cdr_func)));


	builtinEnv->insert("caar", sptrObject(new ObjectDef::Builtin(
                       "caar",
                       {ObjectType::PAIR},
                       false,
                       caar_func)));

	builtinEnv->insert("cdar", sptrObject(new ObjectDef::Builtin(
                       "cdar",
                       {ObjectType::PAIR},
                       false,
                       cdar_func)));

	builtinEnv->insert("cadr", sptrObject(new ObjectDef::Builtin(
                       "cadr",
                       {ObjectType::PAIR},
                       false,
                       cadr_func)));

	builtinEnv->insert("cddr", sptrObject(new ObjectDef::Builtin(
                       "cddr",
                       {ObjectType::PAIR},
                       false,
                       cddr_func)));

	builtinEnv->insert("caaar", sptrObject(new ObjectDef::Builtin(
                       "caaar",
                       {ObjectType::PAIR},
                       false,
                       caaar_func)));

	builtinEnv->insert("cdaar", sptrObject(new ObjectDef::Builtin(
                       "cdaar",
                       {ObjectType::PAIR},
                       false,
                       cdaar_func)));

	builtinEnv->insert("cadar", sptrObject(new ObjectDef::Builtin(
                       "cadar",
                       {ObjectType::PAIR},
                       false,
                       cadar_func)));

	builtinEnv->insert("cddar", sptrObject(new ObjectDef::Builtin(
                       "cddar",
                       {ObjectType::PAIR},
                       false,
                       cddar_func)));

	builtinEnv->insert("caadr", sptrObject(new ObjectDef::Builtin(
                       "caadr",
                       {ObjectType::PAIR},
                       false,
                       caadr_func)));

	builtinEnv->insert("cdadr", sptrObject(new ObjectDef::Builtin(
                       "cdadr",
                       {ObjectType::PAIR},
                       false,
                       cdadr_func)));

	builtinEnv->insert("caddr", sptrObject(new ObjectDef::Builtin(
                       "caddr",
                       {ObjectType::PAIR},
                       false,
                       caddr_func)));

	builtinEnv->insert("cdddr", sptrObject(new ObjectDef::Builtin(
                       "cdddr",
                       {ObjectType::PAIR},
                       false,
                       cdddr_func)));

	builtinEnv->insert("caaaar", sptrObject(new ObjectDef::Builtin(
                       "caaaar",
                       {ObjectType::PAIR},
                       false,
                       caaaar_func)));

	builtinEnv->insert("cdaaar", sptrObject(new ObjectDef::Builtin(
                       "cdaaar",
                       {ObjectType::PAIR},
                       false,
                       cdaaar_func)));

	builtinEnv->insert("cadaar", sptrObject(new ObjectDef::Builtin(
                       "cadaar",
                       {ObjectType::PAIR},
                       false,
                       cadaar_func)));

	builtinEnv->insert("cddaar", sptrObject(new ObjectDef::Builtin(
                       "cddaar",
                       {ObjectType::PAIR},
                       false,
                       cddaar_func)));

	builtinEnv->insert("caadar", sptrObject(new ObjectDef::Builtin(
                       "caadar",
                       {ObjectType::PAIR},
                       false,
                       caadar_func)));

	builtinEnv->insert("cdadar", sptrObject(new ObjectDef::Builtin(
                       "cdadar",
                       {ObjectType::PAIR},
                       false,
                       cdadar_func)));

	builtinEnv->insert("caddar", sptrObject(new ObjectDef::Builtin(
                       "caddar",
                       {ObjectType::PAIR},
                       false,
                       caddar_func)));

	builtinEnv->insert("cdddar", sptrObject(new ObjectDef::Builtin(
                       "cdddar",
                       {ObjectType::PAIR},
                       false,
                       cdddar_func)));

	builtinEnv->insert("caaadr", sptrObject(new ObjectDef::Builtin(
                       "caaadr",
                       {ObjectType::PAIR},
                       false,
                       caaadr_func)));

	builtinEnv->insert("cdaadr", sptrObject(new ObjectDef::Builtin(
                       "cdaadr",
                       {ObjectType::PAIR},
                       false,
                       cdaadr_func)));

	builtinEnv->insert("cadadr", sptrObject(new ObjectDef::Builtin(
                       "cadadr",
                       {ObjectType::PAIR},
                       false,
                       cadadr_func)));

	builtinEnv->insert("cddadr", sptrObject(new ObjectDef::Builtin(
                       "cddadr",
                       {ObjectType::PAIR},
                       false,
                       cddadr_func)));

	builtinEnv->insert("caaddr", sptrObject(new ObjectDef::Builtin(
                       "caaddr",
                       {ObjectType::PAIR},
                       false,
                       caaddr_func)));

	builtinEnv->insert("cdaddr", sptrObject(new ObjectDef::Builtin(
                       "cdaddr",
                       {ObjectType::PAIR},
                       false,
                       cdaddr_func)));

	builtinEnv->insert("cadddr", sptrObject(new ObjectDef::Builtin(
                       "cadddr",
                       {ObjectType::PAIR},
                       false,
                       cadddr_func)));

	builtinEnv->insert("cddddr", sptrObject(new ObjectDef::Builtin(
                       "cddddr",
                       {ObjectType::PAIR},
                       false,
                       cddddr_func)));
	builtinEnv->insert("null?", sptrObject(new ObjectDef::Builtin(
                       "null?",
                       {ObjectType::OBJECT},
                       false,
                       null_func)));

	builtinEnv->insert("list?", sptrObject(new ObjectDef::Builtin(
                       "list?",
                       {ObjectType::OBJECT},
                       false,
                       listQuestion_func)));

	builtinEnv->insert("list", sptrObject(new ObjectDef::Builtin(
                       "list",
                       {ObjectType::OBJECT},
                       true,
                       list_func)));

	builtinEnv->insert("length", sptrObject(new ObjectDef::Builtin(
                       "length",
                       {ObjectType::LIST},
                       false,
                       length_func)));

	builtinEnv->insert("append", sptrObject(new ObjectDef::Builtin(
                       "append",
                       {ObjectType::OBJECT},
                       true,
                       append_func)));

	builtinEnv->insert("reverse", sptrObject(new ObjectDef::Builtin(
                       "reverse",
                       {ObjectType::LIST},
                       false,
                       reverse_func)));

	builtinEnv->insert("list-tail", sptrObject(new ObjectDef::Builtin(
                       "list-tail",
                       {ObjectType::LIST,
                        ObjectType::INT},
                       false,
                       list_tail_func)));

	builtinEnv->insert("list-ref", sptrObject(new ObjectDef::Builtin(
                       "list-ref",
                       {ObjectType::LIST,
                        ObjectType::INT},
                       false,
                       list_ref_func)));

	builtinEnv->insert("memq", sptrObject(new ObjectDef::Builtin(
                       "memq",
                       {ObjectType::OBJECT,
                        ObjectType::LIST},
                       false,
                       memq_func)));

	builtinEnv->insert("memv", sptrObject(new ObjectDef::Builtin(
                       "memv",
                       {ObjectType::OBJECT,
                        ObjectType::LIST},
                       false,
                       memv_func)));

	builtinEnv->insert("member", sptrObject(new ObjectDef::Builtin(
                       "member",
                       {ObjectType::OBJECT,
                        ObjectType::LIST},
                       false,
                       member_func)));

	builtinEnv->insert("assq", sptrObject(new ObjectDef::Builtin(
                       "assq",
                       {ObjectType::OBJECT,
                        ObjectType::LIST},
                       false,
                       assq_func)));

	builtinEnv->insert("assv", sptrObject(new ObjectDef::Builtin(
                       "assv",
                       {ObjectType::OBJECT,
                        ObjectType::LIST},
                       false,
                       assv_func)));

	builtinEnv->insert("assoc", sptrObject(new ObjectDef::Builtin(
                       "assoc",
                       {ObjectType::OBJECT,
                        ObjectType::LIST},
                       false,
                       assoc_func)));

    // Section 6.3.3 Symbols

	builtinEnv->insert("symbol?", sptrObject(new ObjectDef::Builtin(
                       "symbol?",
                       {ObjectType::OBJECT},
                       false,
                       symbol_func)));

	builtinEnv->insert("symbol->string", sptrObject(new ObjectDef::Builtin(
                       "symbol->string",
                       {ObjectType::SYMBOL},
                       false,
                       symbolToString_func)));

	builtinEnv->insert("string->symbol", sptrObject(new ObjectDef::Builtin(
                       "string->symbol",
                       {ObjectType::STRING},
                       false,
                       stringToSymbol_func)));

    // Section 6.3.4 Characters

	builtinEnv->insert("char?", sptrObject(new ObjectDef::Builtin(
                       "char?",
                       {ObjectType::OBJECT},
                       false,
                       char_func)));

	builtinEnv->insert("char=?", sptrObject(new ObjectDef::Builtin(
                       "char=?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_equal_func)));

	builtinEnv->insert("char<?", sptrObject(new ObjectDef::Builtin(
                       "char<?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_lessThan_func)));

	builtinEnv->insert("char>?", sptrObject(new ObjectDef::Builtin(
                       "char>?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_greaterThan_func)));

	builtinEnv->insert("char<=?", sptrObject(new ObjectDef::Builtin(
                       "char<=?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_leq_func)));

	builtinEnv->insert("char>=?", sptrObject(new ObjectDef::Builtin(
                       "char>=?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_geq_func)));


	builtinEnv->insert("char-ci=?", sptrObject(new ObjectDef::Builtin(
                       "char-ci=?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_ci_equal_func)));

	builtinEnv->insert("char-ci<?", sptrObject(new ObjectDef::Builtin(
                       "char-ci<?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_ci_lessThan_func)));

	builtinEnv->insert("char-ci>?", sptrObject(new ObjectDef::Builtin(
                       "char-ci>?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_ci_greaterThan_func)));

	builtinEnv->insert("char-ci<=?", sptrObject(new ObjectDef::Builtin(
                       "char-ci<=?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_ci_leq_func)));

	builtinEnv->insert("char-ci>=?", sptrObject(new ObjectDef::Builtin(
                       "char-ci>=?",
                       {ObjectType::CHAR,
                        ObjectType::CHAR},
                       false,
                       char_ci_geq_func)));


	builtinEnv->insert("char-alphabetic?", sptrObject(new ObjectDef::Builtin(
                       "char-alphabetic?",
                       {ObjectType::CHAR},
                       false,
                       char_alphabetic_func)));

	builtinEnv->insert("char-numeric?", sptrObject(new ObjectDef::Builtin(
                       "char-numeric?",
                       {ObjectType::CHAR},
                       false,
                       char_numeric_func)));

	builtinEnv->insert("char-whitespace?", sptrObject(new ObjectDef::Builtin(
                       "char-whitespace?",
                       {ObjectType::CHAR},
                       false,
                       char_whitespace_func)));

	builtinEnv->insert("char-upper-case?", sptrObject(new ObjectDef::Builtin(
                       "char-upper-case?",
                       {ObjectType::CHAR},
                       false,
                       char_upper_case_func)));

	builtinEnv->insert("char-lower-case?", sptrObject(new ObjectDef::Builtin(
                       "char-lower-case?",
                       {ObjectType::CHAR},
                       false,
                       char_lower_case_func)));

	builtinEnv->insert("char->integer", sptrObject(new ObjectDef::Builtin(
                       "char->integer",
                       {ObjectType::CHAR},
                       false,
                       charToInteger_func)));

	builtinEnv->insert("integer->char", sptrObject(new ObjectDef::Builtin(
                       "integer->char",
                       {ObjectType::INT},
                       false,
                       integerToChar_func)));

	builtinEnv->insert("char-upcase", sptrObject(new ObjectDef::Builtin(
                       "char-upcase",
                       {ObjectType::CHAR},
                       false,
                       char_upcase_func)));

	builtinEnv->insert("char-downcase", sptrObject(new ObjectDef::Builtin(
                       "char-downcase",
                       {ObjectType::CHAR},
                       false,
                       char_downcase_func)));

    // Section 6.3.5 Strings

	builtinEnv->insert("string?", sptrObject(new ObjectDef::Builtin(
                       "string?",
                       {ObjectType::OBJECT},
                       false,
                       string_func)));

	builtinEnv->insert("make-string", sptrObject(new ObjectDef::Builtin(
                       "make-string",
                       {ObjectType::INT,
                        ObjectType::CHAR},
                       true,
                       make_string_func)));

	builtinEnv->insert("string", sptrObject(new ObjectDef::Builtin(
                       "string",
                       {ObjectType::CHAR},
                       true,
                       string_compose_func)));

	builtinEnv->insert("string-length", sptrObject(new ObjectDef::Builtin(
                       "string-length",
                       {ObjectType::STRING},
                       false,
                       string_length_func)));

	builtinEnv->insert("string-ref", sptrObject(new ObjectDef::Builtin(
                       "string-ref",
                       {ObjectType::STRING,
                        ObjectType::INT},
                       false,
                       string_ref_func)));

	builtinEnv->insert("string-set!", sptrObject(new ObjectDef::Builtin(
                       "string-set!",
                       {ObjectType::STRING,
                        ObjectType::INT,
                        ObjectType::CHAR},
                       false,
                       string_set_func)));

	builtinEnv->insert("string=?", sptrObject(new ObjectDef::Builtin(
                       "string=?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_equal_func)));

	builtinEnv->insert("string-ci=?", sptrObject(new ObjectDef::Builtin(
                       "string-ci=?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_ci_equal_func)));

	builtinEnv->insert("string<?", sptrObject(new ObjectDef::Builtin(
                       "string<?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_lessThan_func)));

	builtinEnv->insert("string>?", sptrObject(new ObjectDef::Builtin(
                       "string>?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_greaterThan_func)));

	builtinEnv->insert("string<=?", sptrObject(new ObjectDef::Builtin(
                       "string<=?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_leq_func)));

	builtinEnv->insert("string>=?", sptrObject(new ObjectDef::Builtin(
                       "string>=?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_geq_func)));

	builtinEnv->insert("string-ci<?", sptrObject(new ObjectDef::Builtin(
                       "string-ci<?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_ci_lessThan_func)));

	builtinEnv->insert("string-ci>?", sptrObject(new ObjectDef::Builtin(
                       "string-ci>?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_ci_greaterThan_func)));

	builtinEnv->insert("string-ci<=?", sptrObject(new ObjectDef::Builtin(
                       "string-ci<=?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_ci_leq_func)));

	builtinEnv->insert("string-ci>=?", sptrObject(new ObjectDef::Builtin(
                       "string-ci>=?",
                       {ObjectType::STRING,
                        ObjectType::STRING},
                       false,
                       string_ci_geq_func)));

	builtinEnv->insert("substring", sptrObject(new ObjectDef::Builtin(
                       "substring",
                       {ObjectType::STRING,
                        ObjectType::INT,
                        ObjectType::INT},
                       false,
                       substring_func)));

	builtinEnv->insert("string-append", sptrObject(new ObjectDef::Builtin(
                       "string-append",
                       {ObjectType::STRING},
                       true,
                       string_append_func)));

	builtinEnv->insert("string->list", sptrObject(new ObjectDef::Builtin(
                       "string->list",
                       {ObjectType::STRING},
                       false,
                       stringToList_func)));

	builtinEnv->insert("list->string", sptrObject(new ObjectDef::Builtin(
                       "list->string",
                       {ObjectType::LIST},
                       false,
                       listToString_func)));

	builtinEnv->insert("string-copy", sptrObject(new ObjectDef::Builtin(
                       "string-copy",
                       {ObjectType::STRING},
                       false,
                       string_copy_func)));

	builtinEnv->insert("string-fill!", sptrObject(new ObjectDef::Builtin(
                       "string-fill!",
                       {ObjectType::STRING,
                        ObjectType::CHAR},
                       false,
                       string_fill_func)));

	// Section 6.4 Control features

	builtinEnv->insert("procedure?", sptrObject(new ObjectDef::Builtin(
                       "procedure?",
                       {ObjectType::OBJECT},
                       false,
                       procedure_func)));

	builtinEnv->insert("apply", sptrObject(new ObjectDef::Builtin(
                       "apply",
                       {ObjectType::PROC,
                        ObjectType::OBJECT,
                        ObjectType::OBJECT},
                       true,
                       apply_func)));

	builtinEnv->insert("map", sptrObject(new ObjectDef::Builtin(
                       "map",
                       {ObjectType::PROC,
                        ObjectType::LIST,
                        ObjectType::LIST},
                       true,
                       map_func)));

	builtinEnv->insert("for-each", sptrObject(new ObjectDef::Builtin(
                       "for-each",
                       {ObjectType::PROC,
                        ObjectType::LIST,
                        ObjectType::LIST},
                       true,
                       for_each_func)));

	builtinEnv->insert("force", sptrObject(new ObjectDef::Builtin(
					   "force",
					   {ObjectType::PROMISE},
					   false,
					   force_func)));

    // Section 6.6.3 Output

	builtinEnv->insert("display", sptrObject(new ObjectDef::Builtin(
					   "display",
					   {ObjectType::OBJECT},
					   false,
					   display_func)));

	builtinEnv->insert("newline", sptrObject(new ObjectDef::Builtin(
					   "newline",
					   {},
					   false,
					   newline_func)));

    // Section 6.6.4 System interface

	builtinEnv->insert("load", sptrObject(new ObjectDef::Builtin(
                       "load",
                       {ObjectType::STRING},
                       false,
                       load_func)));

}

std::shared_ptr<IOStreamContainer> iostreamContainer;

void initIOStream(std::istream& input, std::ostream& output){
    iostreamContainer = std::shared_ptr<IOStreamContainer>(new IOStreamContainer(input, output));
}

}
