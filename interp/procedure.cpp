#include "../object/objectdef.h"
#include "errorinfo.h"
#include "evaluation.h"

#include <utility>
#include <stack>
#include <iostream>

namespace ObjectDef{

auto Procedure::applyFormal(shared_ptr<Environment>& curEnv, parameterType param) ->callResultType {
	curEnv = shared_ptr<Environment>(new Environment(env));
	if (enableList)	{
		if (param.size() + 1 < formal.size()){
			std::string err("Need at least ");
			err += std::to_string(formal.size() - 1);
			err += ", given ";
			err += std::to_string(param.size());
			return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, std::move(err));
		}
		for (unsigned pos = 0; pos < formal.size() - 1; ++pos){
			curEnv->insert( formal[pos], param[pos]);
		}

		shared_ptr<Object> l = EmptyList::getEmptyList();
		for (unsigned pos = param.size(); pos >= formal.size(); --pos){
				l = shared_ptr<Object>(new Pair(param[pos-1], l));
		}
		curEnv->insert( formal.back(), l);
	}
	else {
		if (param.size() != formal.size()){
			std::string err("Need ");
			err += std::to_string(formal.size());
			err += ", given ";
			err += std::to_string(param.size());
			return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, std::move(err));
		}
		for (unsigned pos = 0; pos < formal.size(); ++pos){
			curEnv->insert( formal[pos], param[pos]);
		}
	}

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

auto Procedure::invoke(parameterType param, shared_ptr<Object>& obj) -> callResultType{
	shared_ptr<Environment> curEnv;

	auto callRes = applyFormal(curEnv, param);
	if (callRes.first != Interp::EVAL_NO_ERROR)
		return std::move(callRes);

	return Interp::evaluateParseTree(curEnv, proc, obj, true);
}

}

