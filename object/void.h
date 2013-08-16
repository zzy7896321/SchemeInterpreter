#ifndef VOID_H_INCLUDED
#define VOID_H_INLCUDED

#include "object.h"
#include <memory>

namespace ObjectDef{

class Void: public Object{
public:
	virtual std::string getExternalRep() const{
		return "<Void>";
	}

private:
	Void():Object(ObjectType::VOID){}

	Void(const Void&) = delete;
	Void(Void&&) = delete;

	Void& operator=(const Void&) = delete;
	Void& operator=(Void&&) = delete;

    virtual std::shared_ptr<Object> clone(){
        return getVoid();
	}

public:
	static std::shared_ptr<Object> getVoid(){
		std::shared_ptr<Object> obj = std::shared_ptr<Object>(new Void());
		return obj;
	}
};

}

#endif	//VOID_H_INCLUDED

