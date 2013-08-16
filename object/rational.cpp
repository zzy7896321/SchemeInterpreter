#include "rational.h"
#include "int.h"
#include <gmp.h>

#include <memory>

namespace ObjectDef{

shared_ptr<Object> Rational::add(Number* obj){
	switch (obj->getType()){
	case ObjectType::INT:
		return _add( getExactness() && obj->getExactness() ,
					 static_cast<Int*>(obj) -> getVal() );
	case ObjectType::BIGINT:
		return _add( getExactness() && obj->getExactness() ,
				     static_cast<BigInt*>(obj) -> getVal() );
	case ObjectType::RATIONAL:
		return _add( getExactness() && obj->getExactness() ,
					 static_cast<Rational*>(obj) -> getVal() );
	default:
		return obj->add(this);
	}
}

template<class T>
shared_ptr<Object> Rational::_add(bool _exactness, const T& obj){
	mpq_class tmp(val);
	tmp += obj;
	if (tmp.get_den() == 1){
		return shared_ptr<Object>(new BigInt(_exactness, tmp.get_num()));
	}
	else{
		return shared_ptr<Object>(new Rational(_exactness, tmp));
	}
}

shared_ptr<Object> Rational::times(Number* obj){
	switch (obj->getType()){
	case ObjectType::INT:
		return _times( getExactness() && obj->getExactness(),
					   static_cast<Int*>(obj)->getVal());
	case ObjectType::BIGINT:
		return _times( getExactness() && obj->getExactness(),
					   static_cast<BigInt*>(obj) -> getVal());
	case ObjectType::RATIONAL:
		return _times( getExactness() && obj->getExactness(),
					   static_cast<Rational*>(obj) -> getVal());
	default:
		return obj->times(this);
	}
}

template<class T>
shared_ptr<Object> Rational::_times(bool _exactness, const T& obj){
	mpq_class tmp(val);
	tmp *= obj;
	if (tmp.get_den() == 1){
		return shared_ptr<Object>(new BigInt(_exactness, tmp.get_num()));
	}
	else{
		return shared_ptr<Object>(new Rational(_exactness, tmp));
	}
}

shared_ptr<Object> Rational::substract(Number* obj, bool _seq){
	switch (obj->getType()){
	case ObjectType::INT:
		return _substract(_seq, getExactness() && obj->getExactness(),
						  static_cast<Int*>(obj) -> getVal());
	case ObjectType::BIGINT:
		return _substract(_seq, getExactness() && obj->getExactness(),
						  static_cast<BigInt*>(obj) -> getVal());
	case ObjectType::RATIONAL:
		return _substract(_seq, getExactness() && obj->getExactness(),
						  static_cast<Rational*>(obj) -> getVal());
	default:
		return obj->substract(this, !_seq);
	}
}

template<class T>
shared_ptr<Object> Rational::_substract(bool _seq, bool _exactness, const T& obj){
	if (_seq){
		mpq_class tmp(val);
		tmp -= obj;
		if (tmp.get_den() == 1){
			return shared_ptr<Object>(new BigInt(_exactness, tmp.get_num()));
		} else {
			return shared_ptr<Object>(new Rational(_exactness, tmp));
		}
	}
	else {
		mpq_class tmp(obj);
		tmp -= val;
		if (tmp.get_den() == 1){
			return shared_ptr<Object>(new BigInt(_exactness, tmp.get_num()));
		} else {
			return shared_ptr<Object>(new Rational(_exactness, tmp));
		}
	}
}

shared_ptr<Object> Rational::divide(Number* obj, bool _seq){
	switch (obj->getType()){
	case ObjectType::INT:
		return _divide(_seq, getExactness() && obj->getExactness(),
					   static_cast<Int*>(obj) -> getVal());
	case ObjectType::BIGINT:
		return _divide(_seq, getExactness() && obj->getExactness(),
					   static_cast<BigInt*>(obj) -> getVal());
	case ObjectType::RATIONAL:
		return _divide(_seq, getExactness() && obj->getExactness(),
					   static_cast<Rational*>(obj) -> getVal());
	default:
		return obj->divide(this, !_seq);
	}
}

template<class T>
shared_ptr<Object> Rational::_divide(bool _seq, bool _exactness, const T& obj){
	if (_seq){
		mpq_class tmp(val);
		tmp /= obj;
		if (tmp.get_den() == 1){
			return shared_ptr<Object>(new BigInt(_exactness, tmp.get_num()));
		} else {
			return shared_ptr<Object>(new Rational(_exactness, tmp));
		}
	}
	else {
		mpq_class tmp(obj);
		tmp /= val;
		if (tmp.get_den() == 1){
			return shared_ptr<Object>(new BigInt(_exactness, tmp.get_num()));
		} else {
			return shared_ptr<Object>(new Rational(_exactness, tmp));
		}
	}
}

shared_ptr<Object> Rational::minus(){
	return shared_ptr<Object>(new Rational(getExactness(), -val));
}

shared_ptr<Object> Rational::mulInverse(){
	return shared_ptr<Object>(new Rational(getExactness(), val.get_den(), val.get_num()));
}

shared_ptr<Object> Rational::floor(){
	mpz_class tmp;
	mpz_fdiv_q( tmp.get_mpz_t(), val.get_num().get_mpz_t(), val.get_den().get_mpz_t());
	return shared_ptr<Object>(new BigInt(getExactness(), tmp));
}

shared_ptr<Object> Rational::ceiling(){
	mpz_class tmp;
	mpz_cdiv_q( tmp.get_mpz_t(), val.get_num().get_mpz_t(), val.get_den().get_mpz_t());
	return shared_ptr<Object>(new BigInt(getExactness(), tmp));
}

shared_ptr<Object> Rational::truncate(){
	return shared_ptr<Object>(new BigInt(getExactness(), val.get_num() / val.get_den()));
}

shared_ptr<Object> Rational::round(){
	mpz_class q, r;
	mpz_fdiv_qr(q.get_mpz_t(), r.get_mpz_t(), val.get_num().get_mpz_t(), val.get_den().get_mpz_t());
	r *= 2;
	int tmp = ::cmp(r, val.get_den());
	if (tmp < 0){
		return shared_ptr<Object>(new BigInt(getExactness(), q));
	}
	else if (tmp == 0){
		if (mpz_odd_p(q.get_mpz_t())){
			return shared_ptr<Object>(new BigInt(getExactness(), q+1));
		} else {
			return shared_ptr<Object>(new BigInt(getExactness(), q));
		}
	}
	else {
		return shared_ptr<Object> (new BigInt(getExactness(), q+1));
	}
}

}

