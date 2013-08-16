/** @file
 *  Section 6.3.2 Pairs and lists.
 */
#include "builtinproc.h"
#include "../prompt/prompt_declaration.h"

#include <queue>
#include <stack>
#include <gmpxx.h>

namespace BuiltinProc{

callResultType pair_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(param[0]->getType() == ObjectDef::ObjectType::PAIR);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cons_func(parameterType param, sptrObject& obj){
	obj = sptrObject(new ObjectDef::Pair(param[0], param[1]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType car_func(parameterType param, sptrObject& obj){
	obj = static_cast<ObjectDef::Pair*>(param[0].get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdr_func(parameterType param, sptrObject& obj){
	obj = static_cast<ObjectDef::Pair*>(param[0].get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType set_car_func(parameterType param, sptrObject& obj){
	static_cast<ObjectDef::Pair*>(param[0].get())->setCar(param[1]);
	obj = ObjectDef::Void::getVoid();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType set_cdr_func(parameterType param, sptrObject& obj){
	static_cast<ObjectDef::Pair*>(param[0].get())->setCdr(param[1]);
	obj = ObjectDef::Void::getVoid();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}



callResultType caar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cadr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cddr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caaar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdaar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cadar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cddar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caadr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdadr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caddr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdddr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caaaar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdaaar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cadaar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cddaar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddaar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caadar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdadar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdadar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caddar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdddar_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdddar, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caaadr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdaadr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cadadr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cddadr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddadr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType caaddr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "caaddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cdaddr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cdaddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cadddr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cadddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCar();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cddddr_func(parameterType param, sptrObject& obj){
    obj = param[0];
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        std::ostringstream os;
        printType(obj->getType(), os);
        return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, "cddddr, need: pair, given: " + os.str());
    }
    obj = static_cast<ObjectDef::Pair*>(obj.get())->getCdr();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType null_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(param[0]->getType() == ObjectDef::ObjectType::EMPTYLIST);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType listQuestion_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(ObjectDef::isList(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType list_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::EmptyList::getEmptyList();
    for (auto item = param.rbegin(); item != param.rend(); ++item){
        obj = sptrObject(new ObjectDef::Pair(*item, obj));
    }

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType length_func(parameterType param, sptrObject& obj){
    ObjectDef::Object* cur = param[0].get();
    int len = 0;
    while (cur->getType() == ObjectDef::ObjectType::PAIR){
        cur = static_cast<ObjectDef::Pair*>(cur)->getCdr().get();
        ++len;
    }
    obj = sptrObject(new ObjectDef::Int(true, len));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType append_func(parameterType param, sptrObject& obj){
    if (param.size() == 0){
        obj = ObjectDef::EmptyList::getEmptyList();
    }
    else{
        for (unsigned i = 0; i < param.size() - 1; ++i){
            if (!ObjectDef::isList(param[i])){
                std::ostringstream os;
                os << "append. Position " << i << ", need list, given "; printType(param[i]->getType(), os);
                return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, os.str());
            }
        }

        obj = param.back();
        for (auto iter = param.rbegin() + 1; iter != param.rend();  ++iter){
            sptrObject cur = *iter;
            if (cur->getType() == ObjectDef::ObjectType::PAIR){
                sptrObject last = cur->clone();
                sptrObject head = last;
                while (ObjectDef::getCdr(cur)->getType() == ObjectDef::ObjectType::PAIR){
                    ObjectDef::setCdr(last, ObjectDef::getCdr(cur)->clone());
                    last = ObjectDef::getCdr(last);
                    cur = ObjectDef::getCdr(cur);
                }
                ObjectDef::setCdr(last, obj);
                obj = head;
            }
            //otherwise it's an empty list
        }
    }
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType reverse_func(parameterType param, sptrObject& obj){
    obj = param[0];
    sptrObject last = ObjectDef::EmptyList::getEmptyList();
    while (obj != ObjectDef::EmptyList::getEmptyList()){
        last = sptrObject(new ObjectDef::Pair(ObjectDef::getCar(obj), last));
        obj = ObjectDef::getCdr(obj);
    }
    obj = last;
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType list_tail_func(parameterType param, sptrObject& obj){
    if (!isExact(param[1])){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "Position: 1, need exact non-negative integer, given inexact integer");
    }
    mpz_class k;
    if (param[1]->getType() == ObjectDef::ObjectType::INT){
        k = static_cast<ObjectDef::Int*>(param[1].get())->getVal();
    } else {
        k = static_cast<ObjectDef::BigInt*>(param[1].get())->getVal();
    }

    if (k < 0){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "position 1, need exact non-negative integer, given exact negative integer");
    }

    obj = param[0];
    while ((k--) != 0){
        if (obj->getType() != ObjectDef::ObjectType::PAIR){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION,
                "list-tail. list has less than " + param[1]->getExternalRep() + " elements");
        }
        obj = ObjectDef::getCdr(obj);
    }

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType list_ref_func(parameterType param, sptrObject& obj){
    if (!isExact(param[1])){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "Position: 1, need exact non-negative integer, given inexact integer");
    }

    mpz_class k;
    if (param[1]->getType() == ObjectDef::ObjectType::INT){
        k = static_cast<ObjectDef::Int*>(param[1].get())->getVal();
    } else {
        k = static_cast<ObjectDef::BigInt*>(param[1].get())->getVal();
    }

    if (k < 0){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "position 1, need exact non-negative integer, given exact negative integer");
    }

    obj = param[0];
    while ((k--) != 0){
        if (obj->getType() != ObjectDef::ObjectType::PAIR){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION,
                "list-ref. list has <=" + param[1]->getExternalRep() + " elements");
        }
        obj = ObjectDef::getCdr(obj);
    }

    if (obj->getType() != ObjectDef::ObjectType::PAIR){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "list-ref. list has <=" + param[1]->getExternalRep() + " elements");
    }
    obj = ObjectDef::getCar(obj);

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType memq_func(parameterType param, sptrObject& obj){
    obj = param[1];
    std::vector<sptrObject> param2;
    param2.push_back(param[0]);
    param2.push_back(nullptr);
    while (obj->getType() == ObjectDef::ObjectType::PAIR){
        param2[1] = ObjectDef::getCar(obj);
        sptrObject ret;
        eq_func(param2, ret);
        if (ret != ObjectDef::getBoolean(false)){
            return std::make_pair(Interp::EVAL_NO_ERROR, "");
        }
        obj = ObjectDef::getCdr(obj);
    }
    if (obj->getType() == ObjectDef::ObjectType::EMPTYLIST)
        obj = ObjectDef::getBoolean(false);

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType memv_func(parameterType param, sptrObject& obj){
    obj = param[1];
    std::vector<sptrObject> param2;
    param2.push_back(param[0]);
    param2.push_back(nullptr);
    while (obj->getType() == ObjectDef::ObjectType::PAIR){
        param2[1] = ObjectDef::getCar(obj);
        sptrObject ret;
        eqv_func(param2, ret);
        if (ret != ObjectDef::getBoolean(false)){
            return std::make_pair(Interp::EVAL_NO_ERROR, "");
        }
        obj = ObjectDef::getCdr(obj);
    }
    if (obj->getType() == ObjectDef::ObjectType::EMPTYLIST)
        obj = ObjectDef::getBoolean(false);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType member_func(parameterType param, sptrObject& obj){
    obj = param[1];
    std::vector<sptrObject> param2;
    param2.push_back(param[0]);
    param2.push_back(nullptr);
    while (obj->getType() == ObjectDef::ObjectType::PAIR){
        param2[1] = ObjectDef::getCar(obj);
        sptrObject ret;
        equal_func(param2, ret);
        if (ret != ObjectDef::getBoolean(false)){
            return std::make_pair(Interp::EVAL_NO_ERROR, "");
        }
        obj = ObjectDef::getCdr(obj);
    }

    obj = ObjectDef::getBoolean(false);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType assq_func(parameterType param, sptrObject& obj){
    obj = param[1];
    std::vector<sptrObject> param2;
    param2.push_back(param[0]);
    param2.push_back(nullptr);
    while (obj->getType() == ObjectDef::ObjectType::PAIR){
        sptrObject p(ObjectDef::getCar(obj));
        if (p->getType() != ObjectDef::ObjectType::PAIR){
            std::ostringstream os;
            os << "assq. Need pair, given "; printType(p->getType(), os);
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }

        param2[1] = ObjectDef::getCar(p);
        sptrObject ret;
        eq_func(param2, ret);
        if (ret != ObjectDef::getBoolean(false)){
            obj = p;
            return std::make_pair(Interp::EVAL_NO_ERROR, "");
        }

        obj = ObjectDef::getCdr(obj);
    }

    obj = ObjectDef::getBoolean(false);

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType assv_func(parameterType param, sptrObject& obj){
    obj = param[1];
    std::vector<sptrObject> param2;
    param2.push_back(param[0]);
    param2.push_back(nullptr);
    while (obj->getType() == ObjectDef::ObjectType::PAIR){
        sptrObject p(ObjectDef::getCar(obj));
        if (p->getType() != ObjectDef::ObjectType::PAIR){
            std::ostringstream os;
            os << "assq. Need pair, given "; printType(p->getType(), os);
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }

        param2[1] = ObjectDef::getCar(p);
        sptrObject ret;
        eqv_func(param2, ret);
        if (ret != ObjectDef::getBoolean(false)){
            obj = p;
            return std::make_pair(Interp::EVAL_NO_ERROR, "");
        }

        obj = ObjectDef::getCdr(obj);
    }

    obj = ObjectDef::getBoolean(false);

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType assoc_func(parameterType param, sptrObject& obj){
    obj = param[1];
    std::vector<sptrObject> param2;
    param2.push_back(param[0]);
    param2.push_back(nullptr);
    while (obj->getType() == ObjectDef::ObjectType::PAIR){
        sptrObject p(ObjectDef::getCar(obj));
        if (p->getType() != ObjectDef::ObjectType::PAIR){
            std::ostringstream os;
            os << "assq. Need pair, given "; printType(p->getType(), os);
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }

        param2[1] = ObjectDef::getCar(p);
        sptrObject ret;
        equal_func(param2, ret);
        if (ret != ObjectDef::getBoolean(false)){
            obj = p;
            return std::make_pair(Interp::EVAL_NO_ERROR, "");
        }

        obj = ObjectDef::getCdr(obj);
    }

    obj = ObjectDef::getBoolean(false);

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}

