#include "../object/objectdef.h"
#include "errorinfo.h"
#include "../prompt/prompt_declaration.h"

#include <utility>
#include <string>
#include <sstream>

namespace ObjectDef{

	auto Builtin::checkType(parameterType param) -> callResultType{
		if (enableList) {
			if (param.size() + 1 < formal.size()){
				std::string err(identifier);
				err += ". Need at least ";
				err += std::to_string(formal.size() - 1);
				err += ", given ";
				err += std::to_string(param.size());
				return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, err);
			}

			unsigned pos;
			for (pos = 0; pos + 1< formal.size() && isMatch(param[pos], formal[pos]); ++pos);

			if (pos + 1 == formal.size()){
				for (; pos < param.size() && isMatch(param[pos], formal.back()); ++pos);
			}

			if (pos == param.size()){
				return std::make_pair(Interp::EVAL_NO_ERROR, std::string());
			}
			else {
                std::ostringstream os;
                os << identifier << ". Position: " << pos << ", need ";
                if (pos + 1 < formal.size())
                    printType(formal[pos], os);
                else
                    printType(formal.back(), os);
                os << ", given ";
                printType(param[pos]->getType(), os);
				return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, os.str());
			}
		}
		else{
			if (param.size() != formal.size()){
                std::string err(identifier);
				err += ". Need ";
				err += std::to_string(formal.size());
				err += ", given ";
				err += std::to_string(param.size());
				return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, err);
			}
			unsigned pos;
			for (pos = 0; pos < formal.size() && isMatch(param[pos], formal[pos]); ++pos);
			if (pos == param.size())
				return std::make_pair(Interp::EVAL_NO_ERROR, std::string());
			else {
                std::ostringstream os;
                os << identifier << ". Position: " << pos << ", need ";
                printType(formal[pos], os);
                os << ", given ";
                printType(param[pos]->getType(), os);
				return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, os.str());
			}

		}
	}

	auto Builtin::invoke(parameterType param, shared_ptr<Object>& obj) -> callResultType{
		auto retCheck = checkType(param);
		if (retCheck.first != Interp::EVAL_NO_ERROR)
			return std::move(retCheck);
		return func(param, obj);
	}

	bool Builtin::isMatch(shared_ptr<Object> obj, ObjectType type){
        switch (type){
            case ObjectType::OBJECT:
                return true;
            case ObjectType::COMPLEX:
                return isComplex(obj);
            case ObjectType::REAL:
                return isReal(obj);
            case ObjectType::INT: case ObjectType::BIGINT:
                return isInteger(obj);
            case ObjectType::RATIONAL:
                return isRational(obj);
            case ObjectType::LIST:
                return isList(obj);
            case ObjectType::PROC: case ObjectType::BUILTIN:
                return obj->getType() == ObjectType::PROC || obj->getType() == ObjectType::BUILTIN;
            default:
                return obj->getType() == type;
        }
	}
}

