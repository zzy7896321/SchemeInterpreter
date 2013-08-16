#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include "object.h"
#include "int.h"

#include <memory>
#include <cstring>

namespace ObjectDef{

using std::shared_ptr;

class Char:public Object{
public:

	Char() = delete;

	Char(const Char&) = default;
	Char(Char&&) = default;

	Char& operator=(const Char&) = default;
	Char& operator=(Char&&) = default;

	Char(char _val):Object(ObjectType::CHAR), val(_val){}

	~Char(){}

	virtual std::string getExternalRep() const{
        if (val == '\n') return "#\\newline";
        else if (val == ' ') return "#\\space";
        else return std::string({'#', '\\', val});
	}

    virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Char(*this));
    }

	char getVal() const{
		return val;
	}

private:
	char val;
};

inline bool isChar(shared_ptr<Object> obj){
	return obj->getType() == ObjectType::CHAR;
}

inline bool charEqual(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return static_cast<Char*>(obj1.get())->getVal()== static_cast<Char*>(obj2.get())->getVal();
}

inline bool charLess(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return static_cast<Char*>(obj1.get())->getVal() < static_cast<Char*>(obj2.get())->getVal();
}

inline bool charGreater(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return static_cast<Char*>(obj1.get())->getVal() > static_cast<Char*>(obj2.get())->getVal();
}

inline bool charLeq(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return static_cast<Char*>(obj1.get())->getVal() <= static_cast<Char*>(obj2.get())->getVal();
}

inline bool charGeq(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return static_cast<Char*>(obj1.get())->getVal() >= static_cast<Char*>(obj2.get())->getVal();
}

inline shared_ptr<Object> charToInteger(shared_ptr<Object> obj){
	return shared_ptr<Object>(new Int(true, static_cast<Char*>(obj.get())->getVal()));
}

inline shared_ptr<Object> IntegerToChar(shared_ptr<Object> obj){
	if (obj->getType() == ObjectType::INT)
		return shared_ptr<Object>(new Char(static_cast<char>(static_cast<Int*>(obj.get())->getVal())));
	else // ObjectType::BIGINT
		return shared_ptr<Object>(new Char(static_cast<char>(static_cast<BigInt*>(obj.get())->getVal().get_si())));
}

}

#endif	//CHAR_H_INCLUDED

