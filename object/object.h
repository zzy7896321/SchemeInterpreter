/** @file
 *  object.h defines the public base class of all objects.
 */
#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <memory>

namespace ObjectDef{

enum class ObjectType{
	OBJECT,		// object of any type
	BOOLEAN,	// boolean
	INT, BIGINT, RATIONAL, REAL, COMPLEX,	//numbers
	PROC,	// user-defined and library procedures
	BUILTIN,	//built-in procedures
	PAIR,	//pair
	LIST,	// list
	SYMBOL,	//symbol
	CHAR,	//character
	STRING,	//string
	VECTOR,	//vector
	EMPTYLIST,	//empty list
	ENVIRONMENT,	//environment
	PAIRDOT,	// . to denote the dot
	VOID,    // void object, used as unspecified results
	PROMISE,	//promise made by delay syntax
	EXPRESSIONPACK	//internal expression pack
};

class Object{
public:
	Object():type(ObjectType::OBJECT){}
	Object(ObjectType _type):type(_type){}

	virtual ~Object(){}

	void setType(ObjectType _type){
		type = _type;
	}

	ObjectType getType() const {
		return type;
	}

	virtual std::string getExternalRep() const{
		return "<Object>";
	}

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Object(*this));
	}
protected:
	ObjectType type;
};

}

#endif	//OBJECT_H_INCLUDED
