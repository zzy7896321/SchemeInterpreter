/** @file
 *	prompt_declaration.h declares input and output utilities of the interpreter.
 */

#ifndef PROMPT_DECLARATOIN_H_INCLUDED
#define PROMPT_DECLARATION_H_INCLUDED

#include <iostream>
#include "../parser/parsetree.h"
#include "../object/object.h"
#include <memory>

int read_eval_loop(std::istream&, std::ostream&);

void printParseTree(std::shared_ptr<ParserEnv::ParseTree> root, std::ostream& output = std::cerr);
void printType(ObjectDef::ObjectType type, std::ostream& output = std::cerr);
void printObject(std::shared_ptr<ObjectDef::Object> obj, std::ostream& output = std::cerr);

#endif	//PROMPT_DECLARATION_H_INCLUDED

