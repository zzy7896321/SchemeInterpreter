#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

#include "object.h"

#include <string>
#include <memory>
#include <utility>

namespace ObjectDef{

using std::shared_ptr;

class Symbol: public Object{
public:
	Symbol() = delete;

	Symbol(const Symbol&) = default;
	Symbol(Symbol&&) = default;

	Symbol& operator=(const Symbol&) = default;
	Symbol& operator=(Symbol&&) = default;

	~Symbol(){}

	Symbol(const std::string& _val):Object(ObjectType::SYMBOL), val(_val){}

	virtual std::string getExternalRep() const{
		return val;
	}

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Symbol(*this));
	}

	const std::string& getVal() const{
		return val;
	}
private:
	std::string val;
};

inline bool isSymbol(shared_ptr<Object> obj){
	return obj->getType() == ObjectType::SYMBOL;
}

inline shared_ptr<Object> getPairDot(){
	static shared_ptr<Object> pairDot = shared_ptr<Object>(new Object(ObjectType::PAIRDOT));
	return pairDot;
}

}

#endif	//SYMBOL_H_INCLUDED

