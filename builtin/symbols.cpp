/** @file
 *  Section 6.3.3 Symbols
 */

#include "builtinproc.h"

namespace BuiltinProc{

callResultType symbol_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(param[0]->getType() == ObjectDef::ObjectType::SYMBOL);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType symbolToString_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::String(
        static_cast<ObjectDef::Symbol*>(param[0].get())->getVal(), false, false));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType stringToSymbol_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::Symbol(static_cast<ObjectDef::String*>(param[0].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}
