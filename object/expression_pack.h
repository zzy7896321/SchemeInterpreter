#ifndef EXPRESSION_PACK_H_INCLUDED
#define EXPRESSION_PACK_H_INCLUDED

#include "object.h"
#include <memory>
#include "../parser/parsetree.h"

namespace ObjectDef{

using std::shared_ptr;

/**
 * Only for internal use. To pass raw expression from syntax function call to caller for futher implemetation of tail recursion.
 */
struct ExpressionPack: public Object{
	ExpressionPack(shared_ptr<ParserEnv::ParseTree> _proc):Object(ObjectType::EXPRESSIONPACK), proc(_proc){}

	shared_ptr<ParserEnv::ParseTree> proc;

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new ExpressionPack(*this));
	}

};

}

#endif	//EXPRESSION_PACK_H_INCLUDED
