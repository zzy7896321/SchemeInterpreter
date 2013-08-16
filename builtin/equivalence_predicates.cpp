/** @file
 *  Section 6.1 Equivalence predicates.
 */
#include "builtinproc.h"

namespace BuiltinProc{

callResultType eqv_func(parameterType param, sptrObject& obj){
    using ObjectDef::ObjectType;
	// both number
	if (ObjectDef::isNumber(param[0]) && ObjectDef::isNumber(param[1])){
		// differenct exactness
		if (static_cast<ObjectDef::Number*>(param[0].get())->getExactness() ^ static_cast<ObjectDef::Number*>(param[1].get())->getExactness()){
			obj = ObjectDef::getBoolean(false);
		}

		// equal procedure returns false
		else if (!ObjectDef::equal(param[0], param[1])){
			obj = ObjectDef::getBoolean(false);
		}

		else {
            obj = ObjectDef::getBoolean(true);
        }
	}

	// any type matching process not being comparing the enum ObjectType should be above the following check

	// different type
	// with the special case where builtin and proc are both procedure but equivalent
	else if (param[0]->getType() != param[1] ->getType()){
		obj = ObjectDef::getBoolean(false);
	}

	// both boolean
	// note that all #t and all #f respectively share the same instance
	else if (param[0]->getType() == ObjectType::BOOLEAN){
		obj = ObjectDef::getBoolean(param[0] == param[1]);
	}

	// both symbol
	else if (param[0]->getType() == ObjectType::SYMBOL){
		obj = ObjectDef::getBoolean(static_cast<ObjectDef::Symbol*>(param[0].get())->getVal() == static_cast<ObjectDef::Symbol*>(param[1].get())->getVal());
	}

	// both char
	else if (param[0]->getType() == ObjectType::CHAR){
		obj = ObjectDef::getBoolean(static_cast<ObjectDef::Char*>(param[0].get())->getVal() == static_cast<ObjectDef::Char*>(param[1].get())->getVal());
	}

	// both empty list
	else if (param[0]->getType() == ObjectType::EMPTYLIST){
		obj = ObjectDef::getBoolean(true);
	}

	// both pair, string or vector*
	// vector has not been implemented
	else if (param[0]->getType() == ObjectType::PAIR ||
			 param[0]->getType() == ObjectType::STRING ||
			 param[0]->getType() == ObjectType::VECTOR){
		obj = ObjectDef::getBoolean(param[0] == param[1]);
	}

	// both proc or builtin
	else if (param[0]->getType() == ObjectType::BUILTIN ||
			 param[0]->getType() == ObjectType::PROC){
		obj = ObjectDef::getBoolean(param[0] == param[1]);
	}

	// other possible type
	else {
		obj = ObjectDef::getBoolean(param[0] == param[1]);
	}

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType eq_func(parameterType& param, sptrObject& obj){
	//currently implemented the same as eqv?
	return eqv_func(param, obj);
}

callResultType equal_func(parameterType& param, sptrObject& obj){
    using ObjectDef::ObjectType;
	// both number
	if (ObjectDef::isNumber(param[0]) && ObjectDef::isNumber(param[1])){
		// differenct exactness
		if (static_cast<ObjectDef::Number*>(param[0].get())->getExactness() ^ static_cast<ObjectDef::Number*>(param[1].get())->getExactness()){
			obj = ObjectDef::getBoolean(false);
		}

		// equal procedure returns false
		if (!ObjectDef::equal(param[0], param[1])){
			obj = ObjectDef::getBoolean(false);
		}

		obj = ObjectDef::getBoolean(true);
	}

	// any type matching process not being comparing the enum ObjectType should be above the following check

	// different type
	// with the special case where builtin and proc are both procedure but equivalent
	else if (param[0]->getType() != param[1] ->getType()){
		obj = ObjectDef::getBoolean(false);
	}

	// both boolean
	// note that all #t and all #f respectively share the same instance
	else if (param[0]->getType() == ObjectType::BOOLEAN){
		obj = ObjectDef::getBoolean(param[0] == param[1]);
	}

	// both symbol
	else if (param[0]->getType() == ObjectType::SYMBOL){
		obj = ObjectDef::getBoolean(static_cast<ObjectDef::Symbol*>(param[0].get())->getVal() == static_cast<ObjectDef::Symbol*>(param[1].get())->getVal());
	}

	// both char
	else if (param[0]->getType() == ObjectType::CHAR){
		obj = ObjectDef::getBoolean(static_cast<ObjectDef::Char*>(param[0].get())->getVal() == static_cast<ObjectDef::Char*>(param[1].get())->getVal());
	}

	// both empty list
	else if (param[0]->getType() == ObjectType::EMPTYLIST){
		obj = ObjectDef::getBoolean(true);
	}

	// both pair
	else if (param[0]->getType() == ObjectType::PAIR){
		obj = ObjectDef::getBoolean(ObjectDef::pairEqual(param[0], param[1]));
	}

	// both string
	else if (param[0]->getType() == ObjectType::STRING){
		obj = ObjectDef::getBoolean(static_cast<ObjectDef::String*>(param[0].get())->getVal() == static_cast<ObjectDef::String*>(param[1].get())->getVal());
	}

	// both vector (not implemented)
	// else if (param[0]->getType() == ObjectType::VECTOR);

	// both proc or builtin
	else if (param[0]->getType() == ObjectType::BUILTIN ||
			 param[0]->getType() == ObjectType::PROC){
		obj = ObjectDef::getBoolean(param[0] == param[1]);
	}

	// other possible type
	else {
		obj = ObjectDef::getBoolean(param[0] == param[1]);
	}

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}

