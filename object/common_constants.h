#ifndef COMMON_CONSTANTS_H_INCLUDED
#define COMMON_CONSTANTS_H_INCLUDED

#include "int.h"
#include <memory>

inline std::shared_ptr<ObjectDef::Object> getZero(){
	static std::shared_ptr<ObjectDef::Object> zero(new ObjectDef::Int(true, 0));
	return zero;
}

inline std::shared_ptr<ObjectDef::Object> getOne(){
	static std::shared_ptr<ObjectDef::Object> one(new ObjectDef::Int(true, 1));
	return one;
}

inline std::shared_ptr<ObjectDef::Object> getTwo(){
	static std::shared_ptr<ObjectDef::Object> two(new ObjectDef::Int(true, 2));
	return two;
}

inline std::shared_ptr<ObjectDef::Object> getEight(){
	static std::shared_ptr<ObjectDef::Object> eight(new ObjectDef::Int(true, 8));
	return eight;
}

inline std::shared_ptr<ObjectDef::Object> getTen(){
	static std::shared_ptr<ObjectDef::Object> ten(new ObjectDef::Int(true, 10));
	return ten;
}

inline std::shared_ptr<ObjectDef::Object> getSixteen(){
	static std::shared_ptr<ObjectDef::Object> sixteen(new ObjectDef::Int(true, 16));
	return sixteen;
}

#endif	//COMMON_CONSTANTS_H_INCLUDED

