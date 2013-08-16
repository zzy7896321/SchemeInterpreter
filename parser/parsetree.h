/** @file
 * parsetree.h declares the class ParseTree.
 */
#ifndef PARSETREE_H_INCLUDED
#define PARSETREE_H_INCLUDED

#include "../object/object.h"

#include <memory>
#include <tuple>
#include <utility>
#include <string>
#include <iostream>

/**
 * ParseTree is a struct that stores the parsing result.
 */

namespace ParserEnv{

struct ParseTree{

	/**
	 * Default constructor.
	 */
    ParseTree():child(), next(), obj(){}

	/**
	 * Constructor. Token is passed by const lvalue reference.
	 * Initialize the members with respective parameters.
	 */
	ParseTree(std::shared_ptr<ParseTree> _child,
			  std::shared_ptr<ParseTree> _next,
			  std::shared_ptr<ObjectDef::Object> _obj):
		child(_child), next(_next), obj(_obj){}

    ~ParseTree(){}

	/**
	 * Assignment operator is default.
	 */
    ParseTree& operator=(const ParseTree&) = default;

	/**
	 * Copy constructor is default.
	 */
	ParseTree(const ParseTree&) = default;

	/**
	 * First child of the root.
	 */
	std::shared_ptr<ParseTree> child;

	/**
	 * Next sibling of the root.
	 */
	std::shared_ptr<ParseTree> next;

	/**
	 * Object at the root.
	 */
	std::shared_ptr<ObjectDef::Object> obj;

};

}

#endif	//PARSETREE_H_INCLUDED
