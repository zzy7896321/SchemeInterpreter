/** @file
 *	Section 6.2.5 Numerical operations.
 */
#include "builtinproc.h"
#include "../prompt/prompt_declaration.h"

namespace BuiltinProc{

callResultType number_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::isNumber(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType complex_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::isComplex(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType real_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::isReal(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType rational_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::isRational(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType integer_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::isInteger(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


callResultType exact_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::isExact(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


callResultType inexact_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(!ObjectDef::isExact(param[0]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType number_equal_func(parameterType param, sptrObject& obj){
	unsigned i = 1;
	for (; i < param.size() && ObjectDef::equal(param[0], param[i]); ++i);
	obj = ObjectDef::getBoolean(i == param.size());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType number_lessThan_func(parameterType param, sptrObject& obj){
	unsigned i = 1;
	for (; i < param.size() && ObjectDef::lessThan(param[i-1], param[i]); ++i);
	obj = ObjectDef::getBoolean(i == param.size());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


callResultType number_greaterThan_func(parameterType param, sptrObject& obj){
	unsigned i = 1;
	for (; i < param.size() && ObjectDef::greaterThan(param[i-1], param[i]); ++i);
	obj = ObjectDef::getBoolean(i == param.size());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType number_leq_func(parameterType param, sptrObject& obj){
	unsigned i = 1;
	for (; i < param.size() && ObjectDef::leq(param[i-1], param[i]); ++i);
	obj = ObjectDef::getBoolean(i == param.size());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


callResultType number_geq_func(parameterType param, sptrObject& obj){
	unsigned i = 1;
	for (; i < param.size() && ObjectDef::geq(param[i-1], param[i]); ++i);
	obj = ObjectDef::getBoolean(i == param.size());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType zero_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::equal(param[0], getZero()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType positive_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::greaterThan(param[0], getZero()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType negative_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::lessThan(param[0], getZero()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType odd_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(!ObjectDef::equal(ObjectDef::remainder(param[0], getTwo()), getZero()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType even_func(parameterType param, sptrObject& obj){
	obj = ObjectDef::getBoolean(ObjectDef::equal(ObjectDef::remainder(param[0], getTwo()), getZero()));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType max_func(parameterType param, sptrObject& obj){
	obj = param[0];
	bool exactness = ObjectDef::isExact(obj);
	for (unsigned i = 1; i < param.size(); ++i){
		if (ObjectDef::greaterThan(param[i], obj))
			obj = param[i];
		exactness = exactness && ObjectDef::isExact(param[i]);
	}
	if (static_cast<ObjectDef::Number*>(obj.get())->getExactness() ^ exactness){
        obj = obj->clone();
        assert(obj.unique());
        static_cast<ObjectDef::Number*>(obj.get())->setExactness(exactness);
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType min_func(parameterType param, sptrObject& obj){
	obj = param[0];
	bool exactness = ObjectDef::isExact(obj);
	for (unsigned i = 1; i < param.size(); ++i){
		if (ObjectDef::lessThan(param[i], obj))
			obj = param[i];
		exactness = exactness && ObjectDef::isExact(param[i]);
	}
	if (static_cast<ObjectDef::Number*>(obj.get())->getExactness() ^ exactness){
        obj = obj->clone();
        assert(obj.unique());
        static_cast<ObjectDef::Number*>(obj.get())->setExactness(exactness);
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType plus_func(parameterType param, sptrObject& obj){
	obj = getZero();
	for (auto i : param){
		obj = ObjectDef::add(obj, i);
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType times_func(parameterType param, sptrObject& obj){
	obj = getOne();
	for (auto i : param){
		obj = ObjectDef::times(obj, i);
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType minus_func(parameterType param, sptrObject& obj){
	if (param.size() == 1){
		obj = ObjectDef::minus(param[0]);
	}
	else {
		obj = param[0];
		for (unsigned i = 1; i < param.size(); ++i){
			obj = ObjectDef::substract(obj, param[i]);
		}
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType divide_func(parameterType param, sptrObject& obj){
	if (param.size() == 1){
		if ( ObjectDef::equal(param[0], getZero())){
			return std::make_pair(Interp::EVAL_DIVISION_BY_ZERO, "multiplicative inverse");
		}
		obj = ObjectDef::mulInverse(param[0]);
	}
	else {
		obj = param[0];
		for (unsigned i = 1; i < param.size(); ++i){
			if ( ObjectDef::equal(param[i], getZero())){
				return std::make_pair(Interp::EVAL_DIVISION_BY_ZERO, "divide");
			}
			obj = ObjectDef::divide(obj, param[i]);
		}
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType abs_func(parameterType param, sptrObject& obj){
    if (ObjectDef::lessThan(param[0], getZero())){
        obj = ObjectDef::minus(param[0]);
    }
    else{
        obj = param[0];
    }
    return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType quotient_func(parameterType param, sptrObject& obj){
    if (ObjectDef::equal(param[1], getZero())){
        return std::make_pair(Interp::EVAL_DIVISION_BY_ZERO, "quotient");
    }
    obj = ObjectDef::quotient(param[0], param[1]);
    return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType remainder_func(parameterType param, sptrObject& obj){
    if (ObjectDef::equal(param[1], getZero())){
        return std::make_pair(Interp::EVAL_DIVISION_BY_ZERO, "remainder");
    }
    obj = ObjectDef::remainder(param[0], param[1]);
    return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType modulo_func(parameterType param, sptrObject& obj){
    if (ObjectDef::equal(param[1], getZero())){
        return std::make_pair(Interp::EVAL_DIVISION_BY_ZERO, "modulo");
    }
    obj = ObjectDef::modulo(param[0], param[1]);
    return std::make_pair(Interp::EVAL_NO_ERROR, "");
}


callResultType gcd_func(parameterType param, sptrObject& obj){
    if (param.size() == 0){
        obj = getZero();
    }
    else if (param.size() == 1){
        obj = param[0];
    }
    else {
        obj = param[0];
        for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
            obj = ObjectDef::gcd(obj, *iter);
        }
    }
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType lcm_func(parameterType param, sptrObject& obj){
    if (param.size() == 0){
        obj = getOne();
    }
    else if (param.size() == 1){
        obj = param[0];
    }
    else {
        obj = param[0];
        for (auto iter = param.begin() + 1; iter != param.end(); ++iter){
            obj = ObjectDef::lcm(obj, *iter);
        }
    }
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType numerator_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::numerator(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType denominator_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::denominator(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType floor_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::floor(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType ceiling_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::ceiling(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType truncate_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::truncate(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType round_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::round(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType rationalize_func(parameterType param, sptrObject& obj){
    assert(false);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType exp_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::exp(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType log_func(parameterType param, sptrObject& obj){
    if (ObjectDef::equal(param[0], getZero())){
        return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "log. log 0 is not well defined.");
    }
    obj = ObjectDef::log(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType sin_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::sin(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType cos_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::cos(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType tan_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::tan(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType asin_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::asin(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType acos_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::acos(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType atan_func(parameterType param, sptrObject& obj){
    if (param.size() == 1){
        obj = ObjectDef::atan(param[0]);
    }
    else if (param.size() == 2){
        if (!ObjectDef::isReal(param[0])){
            std::ostringstream os;
            os << "atan y x. Position 0, need real, given "; printType(param[0]->getType(), os);
            return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, os.str());
        }
        if (!ObjectDef::isReal(param[1])){
            std::ostringstream os;
            os << "atan y x. Position 1, need real, given "; printType(param[1]->getType(), os);
            return std::make_pair(Interp::EVAL_PARAMETER_TYPE_MISMATCH, os.str());
        }

        if (ObjectDef::equal(param[0], getZero()) && ObjectDef::equal(param[1], getZero())){
            return std::make_pair(Interp::EVAL_CONTRACTION_VIOLATION, "atan y x. atan 0 0 is not well defined.");
        }

        obj = ObjectDef::atan(param[1], param[0]);
    }
    else {
        std::ostringstream os;
        os << "atan, need 1 or 2, given " << param.size();
        return std::make_pair(Interp::EVAL_PARAMETER_NUMBER_MISMATCH, os.str());
    }
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType sqrt_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::sqrt(param[0]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType expt_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::expt(param[0], param[1]);
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType make_rectangular_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::Complex(true, param[0], param[1]));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType make_polar_func(parameterType param, sptrObject& obj){
    obj = sptrObject(new ObjectDef::Complex(true,
            ObjectDef::times(param[0], ObjectDef::cos(param[1])),
            ObjectDef::times(param[0], ObjectDef::sin(param[1]))));
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType real_part_func(parameterType param, sptrObject& obj){
    if (param[0]->getType() == ObjectDef::ObjectType::COMPLEX){
        obj = static_cast<ObjectDef::Complex*>(param[0].get())->getReal();
	}
	else {
        obj = param[0];
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType imag_part_func(parameterType param, sptrObject& obj){
    if (param[0]->getType() == ObjectDef::ObjectType::COMPLEX){
        obj = static_cast<ObjectDef::Complex*>(param[0].get())->getImag();
	}
	else {
        obj = getZero();
	}
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType magnitude_func(parameterType param, sptrObject& obj){
    if (param[0]->getType() != ObjectDef::ObjectType::COMPLEX){
        return abs_func(param, obj);
    }
    obj = ObjectDef::sqrt(ObjectDef::add(
                            ObjectDef::times(static_cast<ObjectDef::Complex*>(param[0].get())->getReal(),
                                             static_cast<ObjectDef::Complex*>(param[0].get())->getReal()),
                            ObjectDef::times(static_cast<ObjectDef::Complex*>(param[0].get())->getImag(),
                                             static_cast<ObjectDef::Complex*>(param[0].get())->getImag())));
    return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType angle_func(parameterType param, sptrObject& obj){
    obj = ObjectDef::atan(static_cast<ObjectDef::Complex*>(param[0].get())->getReal(),
                          static_cast<ObjectDef::Complex*>(param[0].get())->getImag());
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType exactToInexact_func(parameterType param, sptrObject& obj){
    if (param[0]->getType() != ObjectDef::ObjectType::COMPLEX){
        if (static_cast<ObjectDef::Number*>(param[0].get())->getExactness()){
            obj = param[0]->clone();
            assert(obj.unique());
            static_cast<ObjectDef::Number*>(obj.get())->setExactness(false);
        }
        else {
            obj = param[0];
        }
    }
    else {
        obj = param[0];
        sptrObject real, imag;
        param[0] = static_cast<ObjectDef::Complex*>(obj.get())->getReal();
        exactToInexact_func(param, real);
        param[0] = static_cast<ObjectDef::Complex*>(obj.get())->getImag();
        exactToInexact_func(param, imag);
        obj = sptrObject(new ObjectDef::Complex(false, real, imag));
    }
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType inexactToExact_func(parameterType param, sptrObject& obj){
    switch (param[0]->getType()){
        case ObjectDef::ObjectType::INT:
        case ObjectDef::ObjectType::BIGINT:
        case ObjectDef::ObjectType::RATIONAL:{
            if (!static_cast<ObjectDef::Number*>(param[0].get())->getExactness()){
                obj = param[0]->clone();
                assert(obj.unique());
                static_cast<ObjectDef::Number*>(obj.get())->setExactness(true);
            }
            else {
                obj = param[0];
            }
        }
            break;
        case ObjectDef::ObjectType::REAL:{
            mpq_class ret;
            mpq_set_f(ret.get_mpq_t(), static_cast<ObjectDef::Real*>(param[0].get())->getVal().get_mpf_t());
            ret.canonicalize();
            if (ret.get_den() == 1){
                obj = sptrObject(new ObjectDef::BigInt(true, ret.get_num()));
            }
            else {
                obj = sptrObject(new ObjectDef::Rational(true, ret));
            }
        }
            break;
        case ObjectDef::ObjectType::COMPLEX:
        {
            obj = param[0];
            sptrObject real, imag;
            param[0] = static_cast<ObjectDef::Complex*>(obj.get())->getReal();
            inexactToExact_func(param, real);
            param[0] = static_cast<ObjectDef::Complex*>(obj.get())->getImag();
            inexactToExact_func(param, imag);
            obj = sptrObject(new ObjectDef::Complex(true, real, imag));
        }
            break;
        default:
            assert(false);  //should never reach here
    }
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}

