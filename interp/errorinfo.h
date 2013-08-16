/** @file
 *  errorinfo.h defines evaluation return values, procedure parameter type and procedure return value type.
 */
#ifndef ERRORINFO_H_INCLUDED
#define ERRORINFO_H_INCLUDED

#include <string>
#include <utility>
#include <vector>

#include "../parser/parsetree.h"

namespace Interp{

typedef std::pair<int, std::string> callResultType;
typedef std::vector<std::shared_ptr<ObjectDef::Object>>& parameterType;

extern const std::string errorInfo[];

constexpr int EVAL_NO_ERROR = 0;
constexpr int EVAL_PARAMETER_NUMBER_MISMATCH = 1;
constexpr int EVAL_PARAMETER_TYPE_MISMATCH = 2;
constexpr int EVAL_UNDEFINED_SYMBOL = 3;
constexpr int EVAL_MISSING_PROCEDURE = 4;
constexpr int EVAL_NON_PROCEDURE = 5;
constexpr int EVAL_UNKNOWN_TYPE = 6;
constexpr int EVAL_BAD_SYNTAX = 7;
constexpr int EVAL_UNEXPECTED_SITUATION = 8;
constexpr int EVAL_DUPLICATE_ARGUMENT_NAME = 9;
constexpr int EVAL_RAW_EXPR_RETURNED = 10;	// for tail recursion
constexpr int EVAL_DIVISION_BY_ZERO = 11;
constexpr int EVAL_CONTRACTION_VIOLATION = 12;
constexpr int EVAL_CANNOT_OPEN_FILE = 13;
constexpr int EVAL_INVALID_NUMBER = 14;

}

#endif	//ERRORINFO_H_INCLUDED

