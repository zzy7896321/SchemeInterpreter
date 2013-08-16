/** @file
 *  Section 6.3.4 Characters
 */

#include "builtinproc.h"

#include <cctype>

namespace BuiltinProc{

callResultType char_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(param[0]->getType() == ObjectDef::ObjectType::CHAR);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_equal_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::Char*>(param[0].get())->getVal() ==
                                static_cast<ObjectDef::Char*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_lessThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::Char*>(param[0].get())->getVal() <
                                static_cast<ObjectDef::Char*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_greaterThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::Char*>(param[0].get())->getVal() >
                                static_cast<ObjectDef::Char*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_leq_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::Char*>(param[0].get())->getVal() <=
                                static_cast<ObjectDef::Char*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_geq_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::Char*>(param[0].get())->getVal() >=
                                static_cast<ObjectDef::Char*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


callResultType char_ci_equal_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::tolower(static_cast<ObjectDef::Char*>(param[0].get())->getVal()) ==
                                std::tolower(static_cast<ObjectDef::Char*>(param[1].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_ci_lessThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::tolower(static_cast<ObjectDef::Char*>(param[0].get())->getVal()) <
                                std::tolower(static_cast<ObjectDef::Char*>(param[1].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_ci_greaterThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::tolower(static_cast<ObjectDef::Char*>(param[0].get())->getVal()) >
                                std::tolower(static_cast<ObjectDef::Char*>(param[1].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_ci_leq_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::tolower(static_cast<ObjectDef::Char*>(param[0].get())->getVal()) <=
                                std::tolower(static_cast<ObjectDef::Char*>(param[1].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_ci_geq_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::tolower(static_cast<ObjectDef::Char*>(param[0].get())->getVal()) >=
                                std::tolower(static_cast<ObjectDef::Char*>(param[1].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


callResultType char_alphabetic_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::isalpha(static_cast<ObjectDef::Char*>(param[0].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_numeric_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::isdigit(static_cast<ObjectDef::Char*>(param[0].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_whitespace_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::isspace(static_cast<ObjectDef::Char*>(param[0].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_upper_case_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::isupper(static_cast<ObjectDef::Char*>(param[0].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_lower_case_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::islower(static_cast<ObjectDef::Char*>(param[0].get())->getVal()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType charToInteger_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::Int(true, (int)(static_cast<ObjectDef::Char*>(param[0].get())->getVal())));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType integerToChar_func(parameterType param, sptrObject& obj){
    if (param[0]->getType() == ObjectDef::ObjectType::INT){
        if (static_cast<ObjectDef::Int*>(param[0].get())->getVal() < 0 ||
            static_cast<ObjectDef::Int*>(param[0].get())->getVal() >= 0xff){

            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "interger->char. Need an integer in range [0,0xff]");
        }
        obj = sptrObject(new ObjectDef::Char((char)static_cast<ObjectDef::Int*>(param[0].get())->getVal()));
    }
    else {  //BIGINT
        if (static_cast<ObjectDef::BigInt*>(param[0].get())->getVal() < 0 ||
            static_cast<ObjectDef::BigInt*>(param[0].get())->getVal() >= 0xff){

            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "interger->char. Need an integer in range [0,0xff]");
        }
        obj = sptrObject(new ObjectDef::Char((char)(static_cast<ObjectDef::BigInt*>(param[0].get())->getVal().get_si())));
    }
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_upcase_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::Char(std::toupper(static_cast<ObjectDef::Char*>(param[0].get())->getVal())));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType char_downcase_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::Char(std::tolower(static_cast<ObjectDef::Char*>(param[0].get())->getVal())));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


}
