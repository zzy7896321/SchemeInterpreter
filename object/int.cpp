#include "int.h"
#include "rational.h"

#include <limits>
#include <memory>

namespace ObjectDef{

shared_ptr<Object> Int::add(Number* obj) {
	switch (obj->getType()){
	case ObjectType::INT:
		return _add(getExactness() && obj->getExactness(),
					static_cast<Int*>(obj)->getVal());
	default:
		return obj->add(this);
	}
}

shared_ptr<Object> Int::_add(bool _exactness, integer obj){
	if (obj >= 0){
		if ( val <= std::numeric_limits<integer>::max() - obj)
			return shared_ptr<Object>(new Int(_exactness, val + obj));
	}
	else {
		if (val >= std::numeric_limits<integer>::min() - obj)
			return shared_ptr<Object>(new Int(_exactness, val + obj));
	}

	// Cannot fit in a Int, convert to BigInt
	mpz_class tmp(val);
	tmp += obj;
	return shared_ptr<Object>( new BigInt(_exactness, tmp) );
}

shared_ptr<Object> Int::substract(Number* obj, bool _seq) {
	switch (obj->getType()){
	case ObjectType::INT:
		return _substract(_seq, getExactness() && obj->getExactness(),
					static_cast<Int*>(obj)->getVal());
	default:
		return obj->substract(this, !_seq);
	}
}

shared_ptr<Object> Int::_substract(bool _seq, bool _exactness, integer obj){
if (_seq){
	if (obj <= 0){
		if ( val <= std::numeric_limits<integer>::max() + obj)
			return shared_ptr<Object>(new Int(_exactness, val - obj));
	}
	else {
		if (val >= std::numeric_limits<integer>::min() + obj)
			return shared_ptr<Object>(new Int(_exactness, val - obj));
	}

	// Cannot fit in a Int, convert to BigInt
	mpz_class tmp(val);
	tmp -= obj;
	return shared_ptr<Object>( new BigInt(_exactness, tmp) );
} else {
	if (val <= 0){
		if ( obj <= std::numeric_limits<integer>::max() + val)
			return shared_ptr<Object>(new Int(_exactness, obj - val));
	}
	else {
		if ( obj >= std::numeric_limits<integer>::min() + val)
			return shared_ptr<Object>(new Int(_exactness, obj - val));
	}

	// Cannot fit in a Int, convert to BigInt
	mpz_class tmp(obj);
	tmp -= val;
	return shared_ptr<Object>( new BigInt(_exactness, tmp) );
}
}

shared_ptr<Object> Int::times(Number* obj) {
	switch (obj->getType()){
	case ObjectType::INT:
		return _times(getExactness() && obj->getExactness(),
					static_cast<Int*>(obj)->getVal());
	default:
		return obj->times(this);
	}
}

shared_ptr<Object> Int::_times(bool _exactness, integer obj){
	if (obj == 0){
		return shared_ptr<Object>(new Int(_exactness, 0));
	}
	else if (obj > 0){
		if ( (val > 0 && val <= std::numeric_limits<integer>::max() /  obj) || (val <= 0 && val >= std::numeric_limits<integer>::min() / obj))
			return shared_ptr<Object>(new Int(_exactness, val * obj));
	}
	else {
		if ((val > 0 && obj <= std::numeric_limits<integer>::min() /  obj) || (val <= 0 && val >= std::numeric_limits<integer>::max() / obj))
			return shared_ptr<Object>(new Int(_exactness, val * obj));
	}

	// Cannot fit in a Int, convert to BigInt
	mpz_class tmp(val);
	tmp *= obj;
	return shared_ptr<Object>( new BigInt(_exactness, tmp) );
}

shared_ptr<Object> Int::divide(Number* obj, bool _seq) {
	switch (obj->getType()){
	case ObjectType::INT:
		return _divide(_seq, getExactness() && obj->getExactness(),
					static_cast<Int*>(obj)->getVal());
	default:
		return obj->divide(this, !_seq);
	}
}

/* require non-zero denominator
 */
shared_ptr<Object> Int::_divide(bool _seq, bool _exactness, integer obj){
	if (_seq){
		if (val == std::numeric_limits<integer>::min() && obj == -1){
			return shared_ptr<Object>(new BigInt(_exactness, - mpz_class(std::numeric_limits<integer>::min())));
		} else {
			if ( val % obj == 0 ){
				return shared_ptr<Object>(new Int(_exactness, val / obj));
			} else {
				return shared_ptr<Object>(new Rational(_exactness, val, obj));
			}
		}
	} else {
		if (obj == std::numeric_limits<integer>::min() && val == -1){
			return shared_ptr<Object>(new BigInt(_exactness, - mpz_class(std::numeric_limits<integer>::min())));
		} else {
			if ( obj % val == 0 ){
				return shared_ptr<Object>(new Int(_exactness, obj / val));
			} else {
				return shared_ptr<Object>(new Rational(_exactness, obj, val));
			}
		}
	}
}

shared_ptr<Object> Int::minus(){
	return shared_ptr<Object>(new Int(getExactness(), -val));
}

shared_ptr<Object> Int::mulInverse(){
	return shared_ptr<Object>(new Rational(getExactness(), 1, val));
}

shared_ptr<Object> Int::floor(){
	return shared_ptr<Object>(new Int(*this));
}

shared_ptr<Object> Int::ceiling(){
	return shared_ptr<Object>(new Int(*this));
}

shared_ptr<Object> Int::truncate(){
	return shared_ptr<Object>(new Int(*this));
}

shared_ptr<Object> Int::round(){
	return shared_ptr<Object>(new Int(*this));
}

/*
 *----------------------------------------------------
 *Following are functions of BigInt.
 *----------------------------------------------------
 *
 */


template<class T>
shared_ptr<Object> BigInt::_add(bool _exactness, const T& obj){
	BigInt* ret = new BigInt(_exactness, val);
	ret->val += obj;
	return shared_ptr<Object>(ret);
}

shared_ptr<Object> BigInt::add(Number* obj){
	switch (obj->getType()){
	case ObjectType::INT:
		return _add( getExactness() && obj->getExactness(),
					 static_cast<Int*>(obj)->getVal());
	case ObjectType::BIGINT:
		return _add( getExactness() && obj->getExactness(),
					 static_cast<BigInt*>(obj)->getVal()) ;
	default:
		return obj->add(this);
	}
}

template<class T>
shared_ptr<Object> BigInt::_times(bool _exactness, const T& obj){
	BigInt* ret = new BigInt(_exactness, val);
	ret->val *= obj;
	return shared_ptr<Object>(ret);
}

shared_ptr<Object> BigInt::times(Number* obj){
	switch (obj->getType()){
	case ObjectType::INT:
		return _times( getExactness() && obj->getExactness(),
					 static_cast<Int*>(obj)->getVal());
	case ObjectType::BIGINT:
		return _times( getExactness() && obj->getExactness(),
					 static_cast<BigInt*>(obj)->getVal()) ;
	default:
		return obj->times(this);
	}
}

template<class T>
shared_ptr<Object> BigInt::_substract(bool _seq, bool _exactness, const T& obj){
	if (_seq){
		BigInt* ret = new BigInt(_exactness, val);
		ret->val -= obj;
		return shared_ptr<Object>(ret);
	}
	else{
		BigInt* ret = new BigInt(_exactness, obj);
		ret->val -= val;
		return shared_ptr<Object>(ret);
	}
}

shared_ptr<Object> BigInt::substract(Number* obj, bool _seq){
	switch (obj->getType()){
	case ObjectType::INT:
		return _substract(_seq, getExactness() && obj->getExactness(),
						  static_cast<Int*>(obj) -> getVal());
	case ObjectType::BIGINT:
		return _substract(_seq, getExactness() && obj->getExactness(),
						  static_cast<BigInt*>(obj) -> getVal());
	default:
		return obj->substract(this, !_seq);
	}
}

template<class T>
shared_ptr<Object> BigInt::_divide(bool _seq, bool _exactness, const T& obj){
	if (_seq){
		if ( val % obj == 0){
			return shared_ptr<Object>(new BigInt(_exactness, val / obj));
		} else {
			return shared_ptr<Object>(new Rational(_exactness, val, obj));
		}
	}
	else {
		if ( obj % val == 0){
			return shared_ptr<Object>(new BigInt(_exactness, obj / val));
		} else {
			return shared_ptr<Object>(new Rational(_exactness, obj, val));
		}
	}
}

shared_ptr<Object> BigInt::divide(Number* obj, bool _seq){
	switch (obj->getType()){
	case ObjectType::INT:
		return _divide(_seq, getExactness() && obj->getExactness(),
					   static_cast<Int*>(obj) -> getVal());
	case ObjectType::BIGINT:
		return _divide(_seq, getExactness() && obj->getExactness(),
						static_cast<BigInt*>(obj) -> getVal());
	default:
		return obj->divide(this, !_seq);
	}
}

shared_ptr<Object> BigInt::minus(){
	return shared_ptr<Object>(new BigInt(getExactness(), -val));
}

shared_ptr<Object> BigInt::mulInverse(){
	return shared_ptr<Object>(new Rational(getExactness(), 1, val));
}

shared_ptr<Object> BigInt::floor(){
	return shared_ptr<Object>(new BigInt(*this));
}

shared_ptr<Object> BigInt::ceiling(){
	return shared_ptr<Object>(new BigInt(*this));
}

shared_ptr<Object> BigInt::truncate(){
	return shared_ptr<Object>(new BigInt(*this));
}

shared_ptr<Object> BigInt::round(){
	return shared_ptr<Object>(new BigInt(*this));
}

}
