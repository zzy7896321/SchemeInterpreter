#include "real.h"
#include "int.h"
#include "rational.h"

#include "../config.h"

#include <gmp.h>
#include <cstdlib>
#include <cstring>

namespace ObjectDef{

std::string Real::getExternalRep() const{
    std::ostringstream os;
    if (getExactness()) os << "#e";

    mp_exp_t expt;
    char* str = mpf_get_str(0, &expt, 10, 0, val.get_mpf_t());
    if (str[0] == '\0'){
        os << "0.0";
    }
    else if (expt > 0){
        char* number_from = str;
        if (str[0] == '-') {
            os << '-';
            number_from += 1;
        }
        if (expt > (mp_exp_t)strlen(number_from)){
            os << number_from[0];
            if (number_from[1] != '\0') os << '.' << number_from+1;
            if (expt != 1) os << 'e' << expt -1 ;
        }
        else {
            for (int i = 0; i <expt; ++i) os << number_from[i];
            os << '.';
            if (number_from[expt] == '\0') os << '0'; os << number_from + expt;
         }
    }
    else {
        char* number_from = str;
        if (number_from[0] == '-') {
            os << '-';
            number_from += 1;
        }
        if ((unsigned)(-expt) <= _scm_maximum_prefix_zero_of_fixed){
            os << "0.";
            while (expt++) os << '0';
            os << number_from;
        }
        else {
            os << number_from[0];
            if (number_from[1] != '\0') os << '.' << number_from + 1;
            os << 'e' << expt-1;
        }
    }

    free(str);

    return os.str();
}

shared_ptr<Object> Real::add(Number* obj){
	switch(obj->getType()){
	case ObjectType::INT:
		return _add(getExactness() && obj->getExactness(),
                    static_cast<Int*>(obj)->getVal());
	case ObjectType::BIGINT:
		return _add(getExactness() && obj->getExactness(),
                    static_cast<BigInt*>(obj)->getVal());
	case ObjectType::RATIONAL:
		return _add(getExactness() && obj->getExactness(),
                    static_cast<Rational*>(obj)->getVal());
	case ObjectType::REAL:
		return _add(getExactness() && obj->getExactness(),
                    static_cast<Real*>(obj)->getVal());
	default:
		return obj->add(this);
	}
}

template<class T>
shared_ptr<Object> Real::_add(bool _exactness, const T& obj){
	return shared_ptr<Object>(new Real(_exactness, val + obj));
}

shared_ptr<Object> Real::times(Number* obj){
	switch(obj->getType()){
	case ObjectType::INT:
		return _times(getExactness() && obj->getExactness(),
                      static_cast<Int*>(obj)->getVal());
	case ObjectType::BIGINT:
		return _times(getExactness() && obj->getExactness(),
                      static_cast<BigInt*>(obj)->getVal());
	case ObjectType::RATIONAL:
		return _times(getExactness() && obj->getExactness(),
                      static_cast<Rational*>(obj)->getVal());
	case ObjectType::REAL:
		return _times(getExactness() && obj->getExactness(),
                      static_cast<Real*>(obj)->getVal());
	default:
		return obj->times(this);
	}
}

template<class T>
shared_ptr<Object> Real::_times(bool _exactness, const T& obj){
	return shared_ptr<Object>(new Real(_exactness, val * obj));
}

shared_ptr<Object> Real::substract(Number* obj, bool _seq){
	switch(obj->getType()){
	case ObjectType::INT:
		return _substract( getExactness() && obj->getExactness(),
                           _seq, static_cast<Int*>(obj)->getVal());
	case ObjectType::BIGINT:
		return _substract( getExactness() && obj->getExactness(),
                           _seq, static_cast<BigInt*>(obj)->getVal());
	case ObjectType::RATIONAL:
		return _substract( getExactness() && obj->getExactness(),
                           _seq, static_cast<Rational*>(obj)->getVal());
	case ObjectType::REAL:
		return _substract( getExactness() && obj->getExactness(),
                           _seq, static_cast<Real*>(obj)->getVal());
	default:
		return obj->substract(this, !_seq);
	}
}

template<class T>
shared_ptr<Object> Real::_substract(bool _exactness, bool _seq, const T& obj){
	if (_seq){
		return shared_ptr<Object>(new Real(_exactness, val - obj));
	} else {
		return shared_ptr<Object>(new Real(_exactness, obj - val));
	}
}

shared_ptr<Object> Real::divide(Number* obj, bool _seq){
	switch(obj->getType()){
	case ObjectType::INT:
		return _divide( getExactness() && obj->getExactness(),
                        _seq, static_cast<Int*>(obj)->getVal());
	case ObjectType::BIGINT:
		return _divide( getExactness() && obj->getExactness(),
                        _seq, static_cast<BigInt*>(obj)->getVal());
	case ObjectType::RATIONAL:
		return _divide( getExactness() && obj->getExactness(),
                        _seq, static_cast<Rational*>(obj)->getVal());
	case ObjectType::REAL:
		return _divide( getExactness() && obj->getExactness(),
                        _seq, static_cast<Real*>(obj)->getVal());
	default:
		return obj->divide(this, !_seq);
	}
}

template<class T>
shared_ptr<Object> Real::_divide(bool _exactness, bool _seq, const T& obj){
	if (_seq){
		return shared_ptr<Object>(new Real(_exactness, val / obj));
	} else {
        return shared_ptr<Object>(new Real(false, obj / val));
	}
}

shared_ptr<Object> Real::minus(){
	return shared_ptr<Object>(new Real(getExactness(), -val));
}

shared_ptr<Object> Real::mulInverse(){
	return shared_ptr<Object>(new Real(getExactness(), 1 / val));
}

shared_ptr<Object> Real::floor(){
	return shared_ptr<Object>(new BigInt(getExactness(), mpz_class(::floor(val))));
}

shared_ptr<Object> Real::ceiling(){
	return shared_ptr<Object>(new BigInt(getExactness(), mpz_class(::ceil(val))));
}

shared_ptr<Object> Real::truncate(){
	return shared_ptr<Object>(new BigInt(getExactness(), mpz_class(::trunc(val))));
}

shared_ptr<Object> Real::round(){
	mpf_class q(::floor(val));
	if (val - q > mpf_class(0.5)){
		return shared_ptr<Object>(new BigInt(getExactness(), mpz_class(q) + 1));
	}
	else if (val - q < mpf_class(0.5)){
		return shared_ptr<Object>(new BigInt(getExactness(), mpz_class(q)));
	}
	else {
		mpz_class tmp(q);
		if (mpz_odd_p(tmp.get_mpz_t())){
			return shared_ptr<Object>(new BigInt(getExactness(), tmp + 1));
		} else {
			return shared_ptr<Object>(new BigInt(getExactness(), tmp));
		}
	}
}

}
