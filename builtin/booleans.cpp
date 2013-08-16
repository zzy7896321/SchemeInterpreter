/** @file
 *  Section 6.3.1 Booleans
 */

#include "builtinproc.h"

namespace BuiltinProc{

callResultType not_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(param[0] == ObjectDef::getBoolean(false));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType boolean_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(param[0]->getType() == ObjectDef::ObjectType::BOOLEAN);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}
