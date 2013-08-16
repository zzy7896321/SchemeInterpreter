#ifndef BOOLEAN_H_INCLUDED
#define BOOLEAN_H_INCLUDED

#include "object.h"

#include <memory>
#include <iostream>

namespace ObjectDef{

class Boolean:public Object{
public:
	Boolean() = delete;
	Boolean(const Boolean&) = delete;
	Boolean(Boolean&&) = delete;

	Boolean& operator=(const Boolean&) = delete;
	Boolean& operator=(Boolean&&) = delete;

	~Boolean(){}

	bool getVal() const{
		return val;
	}

	virtual std::string getExternalRep() const{
		if (val) return std::string("#t");
		else return std::string("#f");
	}

	virtual std::shared_ptr<Object> clone(){
        return (val) ? True() : False();
	}
private:
	bool val;

	explicit Boolean(bool _val):Object(ObjectType::BOOLEAN), val(_val){}

public:

	static std::shared_ptr<Boolean> True(){
		static std::shared_ptr<Boolean> ptr(new Boolean(true));
		return ptr;
	}

	static std::shared_ptr<Boolean> False(){
		static std::shared_ptr<Boolean> ptr(new Boolean(false));
		return ptr;
	}
};

inline std::shared_ptr<Object> getBoolean(bool val){
	if (val)
		return Boolean::True();
	else
		return Boolean::False();
}

inline bool isBoolean(std::shared_ptr<Object> obj){
	return obj->getType() == ObjectType::BOOLEAN;
}

inline bool logicalNot(std::shared_ptr<Object> obj){
	return !static_cast<Boolean*>(obj.get())->getVal();
}

}

#endif	//BOOLEAN_H_INCLUDED
