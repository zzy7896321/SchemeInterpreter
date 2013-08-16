#ifndef NUMBEROP_H_INCLUDED
#define NUMBEROP_H_INCLUDED

#include "object.h"
#include "number.h"
#include "int.h"
#include "real.h"
#include "boolean.h"
#include "rational.h"
#include "complex.h"

#include <memory>
#include <string>

namespace ObjectDef{

using std::shared_ptr;

bool isNumber(shared_ptr<Object> obj);
bool isComplex(shared_ptr<Object> obj);
bool isReal(shared_ptr<Object> obj);
bool isRational(shared_ptr<Object> obj);
bool isInteger(shared_ptr<Object> obj);


//The following functions need explicit type cheching.
//All need at least Number type.

bool isExact(shared_ptr<Object> obj);

bool equal(shared_ptr<Object> obj1, shared_ptr<Object> obj2);
bool lessThan(shared_ptr<Object> obj1, shared_ptr<Object> obj2);

inline bool geq(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return !lessThan(obj1, obj2);
}	//greater than or equal to

inline bool leq(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return equal(obj1, obj2) || lessThan(obj1, obj2);
}	//less than or equal to

inline bool greaterThan(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return !leq(obj1, obj2);
}

shared_ptr<Object> add(shared_ptr<Object>obj1, shared_ptr<Object> obj2);
shared_ptr<Object> minus(shared_ptr<Object>obj);
shared_ptr<Object> substract(shared_ptr<Object> obj1, shared_ptr<Object> obj2);
shared_ptr<Object> times(shared_ptr<Object> obj1, shared_ptr<Object> obj2);
shared_ptr<Object> divide(shared_ptr<Object> obj1, shared_ptr<Object> obj2);
shared_ptr<Object> mulInverse(shared_ptr<Object> obj1);

shared_ptr<Object> quotient(shared_ptr<Object> obj1, shared_ptr<Object> obj2);
shared_ptr<Object> remainder(shared_ptr<Object> obj1, shared_ptr<Object> obj2);
shared_ptr<Object> modulo(shared_ptr<Object> obj1, shared_ptr<Object> obj2);

shared_ptr<Object> numerator(shared_ptr<Object> obj);
shared_ptr<Object> denominator(shared_ptr<Object> obj);

shared_ptr<Object> floor(shared_ptr<Object> obj);
shared_ptr<Object> ceiling(shared_ptr<Object> obj);
shared_ptr<Object> truncate(shared_ptr<Object> obj);
shared_ptr<Object> round(shared_ptr<Object> obj);

shared_ptr<Object> gcd(shared_ptr<Object> obj1, shared_ptr<Object> obj2);
shared_ptr<Object> lcm(shared_ptr<Object> obj1, shared_ptr<Object> obj2);

shared_ptr<Object> rationalize(shared_ptr<Object> obj1, shared_ptr<Object> obj2);

shared_ptr<Object> exp(shared_ptr<Object> obj);
shared_ptr<Object> log(shared_ptr<Object> obj);
shared_ptr<Object> sin(shared_ptr<Object> obj);
shared_ptr<Object> cos(shared_ptr<Object> obj);
shared_ptr<Object> tan(shared_ptr<Object> obj);
shared_ptr<Object> asin(shared_ptr<Object> obj);
shared_ptr<Object> acos(shared_ptr<Object> obj);
shared_ptr<Object> atan(shared_ptr<Object> obj);
shared_ptr<Object> atan(shared_ptr<Object> obj1, shared_ptr<Object> obj2);

shared_ptr<Object> sqrt(shared_ptr<Object> obj);
shared_ptr<Object> expt(shared_ptr<Object> obj1, shared_ptr<Object> obj2);

}

#endif	//NUMBEROP_H_INCLUDED
