/** @file
 *  Section 6.2.6 Numerical input and output
 */

#include "builtinproc.h"
#include "../parser/parser.h"

#include <iomanip>
#include <sstream>
#include <cstdio>
#include <gmpxx.h>
#include <gmp.h>

namespace BuiltinProc{

static inline void numberToString_impl_conv(sptrObject obj, int radix, std::ostringstream& os){
    switch (obj->getType()){
        case ObjectDef::ObjectType::INT:{
            mpz_class x(static_cast<ObjectDef::Int*>(obj.get())->getVal());
            char* ret = mpz_get_str(0, radix, x.get_mpz_t());
            os << ret;
            free(ret);
        }
            break;
        case ObjectDef::ObjectType::BIGINT:{
            char* ret = mpz_get_str(0, radix, static_cast<ObjectDef::BigInt*>(obj.get())->getVal().get_mpz_t());
            os << ret;
            free(ret);
        }
            break;
        case ObjectDef::ObjectType::RATIONAL:{
            char* ret = mpq_get_str(0, radix, static_cast<ObjectDef::Rational*>(obj.get())->getVal().get_mpq_t());
            os << ret;
            free(ret);
        }
            break;
        case ObjectDef::ObjectType::REAL:{
            assert(radix == 10);
            mp_exp_t expt;
            char* ret = mpf_get_str(0, &expt, 10, 0, static_cast<ObjectDef::Real*>(obj.get())->getVal().get_mpf_t());
            if (ret[0] == '\0'){
                os << "0e0";
            }
            else {
                unsigned i = 0;
                if (ret[0] == '-' || ret[0] == '+') {
                    os << ret[0];
                    ++i;
                }
                os << ret[i];
                ++i;
                os << '.' << ret + i << 'e' << expt-1;
            }
            free(ret);
        }
            break;
        default:
            assert(false);  //should never be here
    }
}

static inline std::string numberToString_impl(sptrObject obj, int radix){
    std::ostringstream os;

    if (radix == 16) os << "#x";
    else if (radix == 8) os << "#o";
    else if (radix == 2) os << "#b";

    if (obj->getType() == ObjectDef::ObjectType::COMPLEX){
        if (static_cast<ObjectDef::Number*>(obj.get())->getExactness() &&
            (!ObjectDef::isRational(static_cast<ObjectDef::Complex*>(obj.get())->getReal()) ||
             !ObjectDef::isRational(static_cast<ObjectDef::Complex*>(obj.get())->getImag()))){
                os << "#e";
        }
        else
        if (!static_cast<ObjectDef::Number*>(obj.get())->getExactness() &&
            (ObjectDef::isRational(static_cast<ObjectDef::Complex*>(obj.get())->getReal()) ||
             ObjectDef::isRational(static_cast<ObjectDef::Complex*>(obj.get())->getImag()))){
                os << "#i";
        }
        numberToString_impl_conv(static_cast<ObjectDef::Complex*>(obj.get())->getReal(), radix, os);
        std::ostringstream os2;
        numberToString_impl_conv(static_cast<ObjectDef::Complex*>(obj.get())->getImag(), radix, os2);
        if (os2.str().front() != '+' && os2.str().front() != '-') os << '+';
        os << os2.str() << 'i';
    }
    else {
        if (static_cast<ObjectDef::Number*>(obj.get())->getExactness() && !ObjectDef::isRational(obj)){
            os << "#e";
        }
        else if (!static_cast<ObjectDef::Number*>(obj.get())->getExactness() && ObjectDef::isRational(obj)){
            os << "#i";
        }
        numberToString_impl_conv(obj, radix, os);
    }

    return os.str();
}

callResultType numberToString_func(parameterType param, sptrObject& obj){
    if (param.size() > 2){
        std::ostringstream os;
        os << "number->string. need 1 or 2, given " << param.size();
        return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, os.str());
    }

    int default_radix = 10;
    if (param.size() == 2){
        if (! static_cast<ObjectDef::Number*>(param[1].get())->getExactness()){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "number->string. Position 1, need exact integer, given inexact integer");
        }

        if (ObjectDef::equal(param[1], getTen())){
            default_radix = 10;
        }
        else if (ObjectDef::equal(param[1], getSixteen())){
            default_radix = 16;
        }
        else if (ObjectDef::equal(param[1], getEight())){
            default_radix = 8;
        }
        else if (ObjectDef::equal(param[1], getTwo())){
            default_radix = 2;
        }
        else {
            std::ostringstream os;
            os << "number->string. Position, need 2, 8, 10 or 16, given" << param[1]->getExternalRep();
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
    }

    if (default_radix != 10){
        if (param[0]->getType() == ObjectDef::ObjectType::COMPLEX){
            if (static_cast<ObjectDef::Complex*>(param[0].get())->getReal()->getType() == ObjectDef::ObjectType::REAL ||
                static_cast<ObjectDef::Complex*>(param[0].get())->getImag()->getType() == ObjectDef::ObjectType::REAL){
                return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "number->string. base has to be 10 to convert a complex containing real part");
            }
        }
        if (param[0]->getType() == ObjectDef::ObjectType::REAL){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "number->string. base has to be 10 to convert a real");
        }
    }

    obj = sptrObject(new ObjectDef::String(numberToString_impl(param[0], default_radix), true, false));

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType stringToNumber_func(parameterType param, sptrObject& obj){
    if (param.size() > 2){
        std::ostringstream os;
        os << "string->number. need 1 or 2, given " << param.size();
        return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, os.str());
    }

    int default_radix = 10;
    if (param.size() == 2){
        if (! static_cast<ObjectDef::Number*>(param[1].get())->getExactness()){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "string->number. Position 1, need exact integer, given inexact integer");
        }

        if (ObjectDef::equal(param[1], getTen())){
            default_radix = 10;
        }
        else if (ObjectDef::equal(param[1], getSixteen())){
            default_radix = 16;
        }
        else if (ObjectDef::equal(param[1], getEight())){
            default_radix = 8;
        }
        else if (ObjectDef::equal(param[1], getTwo())){
            default_radix = 2;
        }
        else {
            std::ostringstream os;
            os << "string->number. Position, need 2, 8, 10 or 16, given" << param[1]->getExternalRep();
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, os.str());
        }
    }

    int ret = ParserEnv::Parser::parseNumber(obj, static_cast<ObjectDef::String*>(param[0].get())->getInterpretedString(), default_radix);
	if (ret != ParserEnv::PARSE_NORMAL_EXIT){
        return std::make_pair(Interp::EVAL_INVALID_NUMBER, static_cast<ObjectDef::String*>(param[0].get())->getInterpretedString());
	}

	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


}
