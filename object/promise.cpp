#include "promise.h"
#include "../interp/evaluation.h"


namespace ObjectDef{
    Interp::callResultType Promise::force(std::shared_ptr<Object>& receiver){
		if (obj->getType() == ObjectType::EXPRESSIONPACK){
			auto callResult =  Interp::evaluateParseTree(env, static_cast<ExpressionPack*>(obj.get())->proc, receiver, false);
			if (callResult.first == Interp::EVAL_NO_ERROR)
				obj = receiver;
			return std::move(callResult);
		}
		else {
			receiver = obj;
			return std::make_pair(Interp::EVAL_NO_ERROR, "");
		}
	}
}
