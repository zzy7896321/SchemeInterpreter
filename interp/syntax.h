/** @file
 *  syntax.h declares all the syntax call functions.
 */

#ifndef SYNTAX_H_INCLUDED
#define SYNTAX_H_INCLUDED

#include <map>
#include <string>
#include <utility>
#include <functional>
#include <cassert>
#include <memory>
#include <stack>
#include <vector>

#include "../parser/parsetree.h"
#include "../object/objectdef.h"
#include "errorinfo.h"

namespace Interp{

typedef std::shared_ptr<ParserEnv::ParseTree> sptrParseTree;
typedef std::shared_ptr<ObjectDef::Object> sptrObject;
typedef std::shared_ptr<ObjectDef::Environment> sptrEnvironment;
typedef std::function<callResultType(sptrEnvironment&, sptrParseTree, sptrObject&, bool)> SyntaxCallFunctionType;
typedef std::map<std::string, SyntaxCallFunctionType> SyntaxMapType;

// Section 4.1 Primitive expression types

/**
 * Section 4.1.2 Literal expression
 * (quote <datum>)	syntax
 */
callResultType quote_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.1.4 Procedures
 * (lambda <formals> <body>)	syntax
 */
callResultType lambda_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);


/**
 * Section 4.1.5 Conditionals
 * (if <test> <consequent> [<alternate>])	syntax
 */
callResultType if_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.1.6 Assignments
 * (set! <variable> <expression>)	syntax
 */
callResultType set_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);


/**
 * Section 4.2.6 Quasiquotation
 * (quosiquote <qq template>)	syntax
 */
callResultType quasiquote_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);


/**
 * (unquote <datum>)	syntax
 * Should only appears within a quasiquotation.
 */
callResultType unquote_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * (unquote-splicing <datum>)	syntax
 * Should only apperas within a quosiquotation.
 */
callResultType unquote_splicing_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 5.2 Definitions
 * (define <variable> <expression>)	syntax
 * (define (<variable> [<formals>] [ . <formal>]) <body>)	syntax
 */
callResultType define_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

// Section 4.2 Derived expression types

/**
 * Section 4.2.1 Conditionals
 * (cond <clause 1> ...)	library syntax
 * Note: Slightly different from R5RS that there could be no clauses and returns an unspecified value.
 */
callResultType cond_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.1 Conditionals
 * (case <key> <clauses 1> ...)	library syntax
 * Note: Slightly different from R5RS that there could be no clauses and returns an unspecified value.
 */
callResultType case_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.1 Conditionals
 * (and <test 1> ...) library syntax
 */
callResultType and_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.1 Conditionals
 * (or <test 1> ...)	library syntax
 */
callResultType or_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);


/**
 * Section 4.2.2 Binding constructs
 * (let <bindings> <body>)	library syntax
 */
callResultType let_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.2 Binding constructs
 * (let* <bindings> <body>)	library syntax
 */
callResultType letstar_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.2 Binding constructs
 * (letrec <bindings> <body>) libarary syntax
 */
callResultType letrec_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.3 Sequencing
 * (begin <expression 1> ...) library syntax
 */
callResultType begin_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.4 Iteration
 * (do ((<variable 1> <init 1> <step 1>)
 *	    ...)
 *	   (<test> <expression> ...)
 *	   <command> ...)	library syntax      not implemented
 */
//callResultType do_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

/**
 * Section 4.2.5 Delayed evaluation
 * (delay <expression>)
 */
callResultType delay_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion = false);

inline SyntaxMapType& getSyntaxMap(){
	static SyntaxMapType syntaxMap({ {"quote", quote_func},
									 {"lambda", lambda_func},
									 {"if", if_func},
									 {"set!", set_func},
									 {"quasiquote", quasiquote_func},
									 {"unquote", unquote_func},
									 {"unquote_splicing_func", unquote_splicing_func},
									 {"define", define_func},

									 // Following are library syntaxes implemented as built-in
									 {"cond", cond_func},
									 {"case", case_func},
									 {"and", and_func},
									 {"or", or_func},
									 {"let", let_func},
									 {"let*", letstar_func},
									 {"letrec", letrec_func},
									 {"begin", begin_func},
									 //{"do", do_func},
									 {"delay", delay_func}
									});

	return syntaxMap;
}

}

#endif	//SYNTAX_H_INCLUDED

