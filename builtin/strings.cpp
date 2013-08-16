/** @file
 *  Section 6.3.5 Strings
 */

#include "builtinproc.h"

#include <algorithm>
#include <cctype>

namespace BuiltinProc{

callResultType string_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(param[0]->getType() == ObjectDef::ObjectType::STRING);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType make_string_func(parameterType param, sptrObject& obj){
    static std::string::size_type string_max_size = std::string().max_size();
    if (param.size() > 2){
        std::ostringstream os;
        os << "make-string. need at most 2, given " << param.size();
        return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, os.str());
    }

    if (!ObjectDef::isExact(param[0])){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "make-string. Position 0, need exact non-negative integer, given inexact integer");
    }

    std::string::size_type k;
    if (param[0]->getType() == ObjectDef::ObjectType::INT){
        int tmp = static_cast<ObjectDef::Int*>(param[0].get())->getVal();
        if (tmp < 0){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "make-string. Position 0, need exact non-negative integer, given exact negative integer");
        }
        if ((k = (std::string::size_type)(tmp)) > string_max_size){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "make-string. Position 0, cannot create such large a string");
        }
    }
    else{   //bigint
        mpz_class tmp = static_cast<ObjectDef::BigInt*>(param[0].get())->getVal();\
        if (tmp < 0){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "make-string. Position 0, need exact non-negative integer, given exact negative integer");
        }
        if (!tmp.fits_sint_p() || (k = (std::string::size_type)(tmp.get_si())) > string_max_size){
             return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "make-string. Position 0, cannot create such large a string");
        }
    }

    if (param.size() == 2){
        obj = sptrObject(new ObjectDef::String(k, static_cast<ObjectDef::Char*>(param[1].get())->getVal(), true));
    } else {
        obj = sptrObject(new ObjectDef::String(k, '\0', true));
    }

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_compose_func(parameterType param, sptrObject& obj){
    std::string res;
    res.reserve(param.size());
    for (auto i : param){
        res.push_back(static_cast<ObjectDef::Char*>(i.get())->getVal());
    }
    obj = sptrObject(new ObjectDef::String(std::move(res), true, false));

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_length_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::Int(true, static_cast<ObjectDef::String*>(param[0].get())->getVal().size()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_ref_func(parameterType param, sptrObject& obj){
    if (!ObjectDef::isExact(param[1])){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "string-ref. Position 1, need exact non-negative integer, given inexact integer");
    }

    if (param[1]->getType() == ObjectDef::ObjectType::INT){
        int k = static_cast<ObjectDef::Int*>(param[1].get())->getVal();
        if (k < 0 || (unsigned)(k) >= static_cast<ObjectDef::String*>(param[0].get())->getVal().size()){
            std::ostringstream os;
            os << "string-ref. reference out of range [0, "
               <<  static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "), given " << k;
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
        obj = sptrObject(new ObjectDef::Char(static_cast<ObjectDef::String*>(param[0].get())->getVal().at(k)));
    }
    else {  //bigint
        int k;
        if (!static_cast<ObjectDef::BigInt*>(param[1].get())->getVal().fits_sint_p() ||
            (k = static_cast<ObjectDef::BigInt*>(param[1].get())->getVal().get_si(),
             k < 0 || (unsigned)(k) >= static_cast<ObjectDef::String*>(param[0].get())->getVal().size())){
            std::ostringstream os;
            os << "string-ref. reference out of range [0, "
               <<  static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "), given "
               << static_cast<ObjectDef::BigInt*>(param[1].get())->getVal();
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
        obj = sptrObject(new ObjectDef::Char(static_cast<ObjectDef::String*>(param[0].get())->getVal().at(k)));
    }

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_set_func(parameterType param, sptrObject& obj){
    if (!static_cast<ObjectDef::String*>(param[0].get())->getMutability()){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "string-set!. Position 0, require mutable string.");
    }

    if (!ObjectDef::isExact(param[1])){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "string-set!. Position 1, need exact non-negative integer, given inexact integer");
    }

    if (param[1]->getType() == ObjectDef::ObjectType::INT){
        int k = static_cast<ObjectDef::Int*>(param[1].get())->getVal();
        if (k < 0 || (unsigned)(k) >= static_cast<ObjectDef::String*>(param[0].get())->getVal().size()){
            std::ostringstream os;
            os << "string-set!. reference out of range [0, "
               <<  static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "), given " << k;
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
        static_cast<ObjectDef::String*>(param[0].get())->setChar(k, static_cast<ObjectDef::Char*>(param[2].get())->getVal());
    }
    else {  //bigint
        int k;
        if (!static_cast<ObjectDef::BigInt*>(param[1].get())->getVal().fits_sint_p() ||
            (k = static_cast<ObjectDef::BigInt*>(param[1].get())->getVal().get_si(),
             k < 0 || (unsigned)(k) >= static_cast<ObjectDef::String*>(param[0].get())->getVal().size())){
            std::ostringstream os;
            os << "string-set!. reference out of range [0, "
               <<  static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "), given "
               << static_cast<ObjectDef::BigInt*>(param[1].get())->getVal();
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
        static_cast<ObjectDef::String*>(param[0].get())->setChar(k, static_cast<ObjectDef::Char*>(param[2].get())->getVal());
    }

    obj = ObjectDef::Void::getVoid();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_equal_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::String*>(param[0].get())->getVal() ==
                                static_cast<ObjectDef::String*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_ci_equal_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::String*>(param[0].get())->getVal().size() ==
                                static_cast<ObjectDef::String*>(param[1].get())->getVal().size() &&
                                std::equal(
                                static_cast<ObjectDef::String*>(param[0].get())->getVal().begin(),
                                static_cast<ObjectDef::String*>(param[0].get())->getVal().end(),
                                static_cast<ObjectDef::String*>(param[1].get())->getVal().begin(),
                                [](char x, char y){ return std::tolower(x) == std::tolower(y); }));

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_lessThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::String*>(param[0].get())->getVal() <
                                static_cast<ObjectDef::String*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_greaterThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::String*>(param[0].get())->getVal() >
                                static_cast<ObjectDef::String*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_leq_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::String*>(param[0].get())->getVal() <=
                                static_cast<ObjectDef::String*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_geq_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(static_cast<ObjectDef::String*>(param[0].get())->getVal() >=
                                static_cast<ObjectDef::String*>(param[1].get())->getVal());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_ci_lessThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::lexicographical_compare(
                                static_cast<ObjectDef::String*>(param[0].get())->getVal().begin(),
                                static_cast<ObjectDef::String*>(param[0].get())->getVal().end(),
                                static_cast<ObjectDef::String*>(param[1].get())->getVal().begin(),
                                static_cast<ObjectDef::String*>(param[1].get())->getVal().end(),
                                [](char x, char y){ return (std::isalpha(x) && std::isalpha(y))
                                                           ? (std::tolower(x) < std::tolower(y))
                                                           : (x < y); }
                                ));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_ci_greaterThan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::getBoolean(std::lexicographical_compare(
                                static_cast<ObjectDef::String*>(param[0].get())->getVal().begin(),
                                static_cast<ObjectDef::String*>(param[0].get())->getVal().end(),
                                static_cast<ObjectDef::String*>(param[1].get())->getVal().begin(),
                                static_cast<ObjectDef::String*>(param[1].get())->getVal().end(),
                                [](char x, char y){ return (std::isalpha(x) && std::isalpha(y))
                                                         ? (std::tolower(x) < std::tolower(y))
                                                         : (x > y); }
                                ));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_ci_leq_func(parameterType param, sptrObject& obj){
    string_ci_equal_func(param, obj);
    if (obj == ObjectDef::getBoolean(false))
        string_ci_lessThan_func(param, obj);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_ci_geq_func(parameterType param, sptrObject& obj){
    string_ci_equal_func(param, obj);
    if (obj == ObjectDef::getBoolean(false))
        string_ci_greaterThan_func(param, obj);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType substring_func(parameterType param, sptrObject& obj){
    if (!isExact(param[1])){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "substring. Position 1, need exact integer, given inexact");
    }
    if (!isExact(param[2])){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "substring. Position 2, need exact integer, given inexact");
    }

    int start;
    if (param[1]->getType() == ObjectDef::ObjectType::INT){
        start = static_cast<ObjectDef::Int*>(param[1].get())->getVal();
        if (start < 0 || (unsigned)(start) > static_cast<ObjectDef::String*>(param[0].get())->getVal().size()){
            std::ostringstream os;
            os << "substring. Position 1 should be in range [0, "
               << static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "], given " << start;
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
    } else {    //bigint
        if (!static_cast<ObjectDef::BigInt*>(param[1].get())->getVal().fits_sint_p() ||
            (start = static_cast<ObjectDef::BigInt*>(param[1].get())->getVal().get_si(),
            start < 0 || (unsigned)(start) > static_cast<ObjectDef::String*>(param[0].get())->getVal().size())){
            std::ostringstream os;
            os << "substring. Position 1 should be in range [0, "
               << static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "], given "
               << static_cast<ObjectDef::BigInt*>(param[1].get())->getVal();
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
    }

    int end;
        if (param[1]->getType() == ObjectDef::ObjectType::INT){
        end = static_cast<ObjectDef::Int*>(param[2].get())->getVal();
        if (end < start || (unsigned)(end) > static_cast<ObjectDef::String*>(param[0].get())->getVal().size()){
            std::ostringstream os;
            os << "substring. Position 2 should be in range [" << start << ", "
               << static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "], given " << end;
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
    } else {    //bigint
        if (!static_cast<ObjectDef::BigInt*>(param[2].get())->getVal().fits_sint_p() ||
            (end = static_cast<ObjectDef::BigInt*>(param[2].get())->getVal().get_si(),
            end < start || (unsigned)(end) > static_cast<ObjectDef::String*>(param[0].get())->getVal().size())){
            std::ostringstream os;
            os << "substring. Position 2 should be in range [" << start << ", "
               << static_cast<ObjectDef::String*>(param[0].get())->getVal().size() << "], given "
               << static_cast<ObjectDef::BigInt*>(param[2].get())->getVal();
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
    }

    obj = sptrObject(new ObjectDef::String(
                     static_cast<ObjectDef::String*>(param[0].get())->getVal().substr(start, end-start), true, false));

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_append_func(parameterType param, sptrObject& obj){
    std::string tmp;
    for (auto iter : param){
        tmp.append(static_cast<ObjectDef::String*>(iter.get())->getVal());
    }
    obj = sptrObject(new ObjectDef::String(std::move(tmp), true, false));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType stringToList_func(parameterType param, sptrObject& obj){
    const std::string& str = static_cast<ObjectDef::String*>(param[0].get())->getVal();
    obj = ObjectDef::EmptyList::getEmptyList();
    for (auto iter = str.rbegin(); iter != str.rend(); ++iter){
        obj = sptrObject(new ObjectDef::Pair(sptrObject(new ObjectDef::Char(*iter)), obj));
    }

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType listToString_func(parameterType param, sptrObject& obj){
    std::string str;
    obj = param[0];
    sptrObject tmp;
    while (obj->getType() == ObjectDef::ObjectType::PAIR){
        if ((tmp = ObjectDef::getCar(obj))->getType() != ObjectDef::ObjectType::CHAR){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "list->string. Need a list of characters.");
        }
        str.push_back(static_cast<ObjectDef::Char*>(tmp.get())->getVal());
        obj = ObjectDef::getCdr(obj);
    }

    obj = sptrObject(new ObjectDef::String(std::move(str), true, false));

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_copy_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::String(static_cast<ObjectDef::String*>(param[0].get())->getVal(), true, false));

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType string_fill_func(parameterType param, sptrObject& obj){
    if (!static_cast<ObjectDef::String*>(param[0].get())->getMutability()){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "string-fill!. Need a mutable string, given an imutable string.");
    }
    auto size = static_cast<ObjectDef::String*>(param[0].get())->getVal().size();
    for (decltype(size) i = 0; i < size; ++i){
        static_cast<ObjectDef::String*>(param[0].get())->setChar(i, static_cast<ObjectDef::Char*>(param[1].get())->getVal());
    }
    obj = ObjectDef::Void::getVoid();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}
