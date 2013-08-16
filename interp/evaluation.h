/** @file
 * evaluation.h declares evalutaion function of parse tree.
 */

#ifndef EVALUATION_H_INCLUDED
#define EVALUATION_H_INCLUDED

#include "errorinfo.h"
#include "syntax.h"

namespace Interp{

/**
 * evaluateParseTree evaluates a parse tree in a given environment, report possible error and returns the parsing result of the last expression.
 */
callResultType evaluateParseTree(sptrEnvironment env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

}

#endif	//EVALUATION_H_INCLUDED

