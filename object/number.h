#ifndef NUMBER_H_INCLUDED
#define NUMBER_H_INCLUDED

#include "object.h"

#include <memory>
#include <string>
#include <iostream>

namespace ObjectDef{

using std::shared_ptr;

class Number:public Object{
public:
	Number() = delete;

public:

	Number(const Number&) = default;
	Number(Number&&) = default;

	Number& operator=(const Number&) = default;
	Number& operator=(Number&&) = default;

	virtual ~Number(){}

	explicit Number(ObjectType type, bool _en):Object(type), exactness(_en){}

	virtual std::string getExternalRep() const = 0;

    virtual std::shared_ptr<Object> clone() = 0;

	bool getExactness() const{
		return exactness;
	}
	void setExactness(bool _exactness){
		exactness = _exactness;
	}


	/* pass _seq = true to perform first - second
	 * pass _seq = false to perform second - first
	 * the rule applies to all the following functions
	*/
	virtual shared_ptr<Object> add(Number* obj) = 0;
	virtual shared_ptr<Object> minus() = 0;	// returns -val
	virtual shared_ptr<Object> substract(Number* obj, bool _seq = true) = 0;
	// pass _seq = true to perform first - second
	// pass _seq = false to perform second - first
	// the same rule applies to all the following functions
	virtual shared_ptr<Object> times(Number* obj) = 0;
	virtual shared_ptr<Object> divide(Number* obj, bool _seq = true) = 0;
	virtual shared_ptr<Object> mulInverse() = 0;

	virtual shared_ptr<Object> floor() = 0;
	virtual shared_ptr<Object> ceiling()= 0;
	virtual shared_ptr<Object> truncate() = 0;
	virtual shared_ptr<Object> round() = 0;
private:
	bool exactness;
};

}
#endif	//NUMBER_H_INCLUDED
