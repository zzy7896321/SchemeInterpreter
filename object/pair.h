#ifndef PAIR_H_INCLUDED
#define PAIR_H_INCLUDED

#include "object.h"

#include <memory>
#include <stack>
#include <sstream>

namespace ObjectDef{

using std::shared_ptr;

class EmptyList: public Object{
public:
	EmptyList(const EmptyList&) = delete;
	EmptyList(EmptyList&&) = delete;

	EmptyList& operator=(const EmptyList&) = delete;
	EmptyList& operator=(EmptyList&&) = delete;

	virtual std::string getExternalRep() const{
		return "()";
	}

private:
	EmptyList():Object(ObjectType::EMPTYLIST){}

public:
	static shared_ptr<Object> getEmptyList() {
		static shared_ptr<Object> emptyList = shared_ptr<Object>(new EmptyList());
		return emptyList;
	}
};

class Pair: public Object{
public:
	Pair() = delete;

	Pair(const Pair&) = default;
	Pair(Pair&&) = default;

	Pair& operator=(const Pair&) = default;
	Pair& operator=(Pair&&) = default;

	virtual ~Pair(){
		//to avoid circular reference, explicitly eliminate reference
		car = cdr = nullptr;
	}

	virtual std::string getExternalRep() const;

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Pair(*this));
	}

	Pair(shared_ptr<Object> _car, shared_ptr<Object> _cdr):
		Object(ObjectType::PAIR), car(_car), cdr(_cdr){}

	shared_ptr<Object> getCar() {
		return car;
	}

	shared_ptr<Object> getCdr() {
		return cdr;
	}

	void setCar(shared_ptr<Object> _car){
		car = _car;
	}

	void setCdr(shared_ptr<Object> _cdr){
		cdr = _cdr;
	}

	shared_ptr<Object> car;
	shared_ptr<Object> cdr;
};

inline bool isNull(shared_ptr<Object> obj){ return obj->getType() == ObjectType::EMPTYLIST;  }

inline bool isPair(shared_ptr<Object> obj){ return obj->getType() == ObjectType::PAIR; }

inline shared_ptr<Object> cons(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return shared_ptr<Object>(new Pair(obj1, obj2));
}

inline shared_ptr<Object> getCar(shared_ptr<Object> obj){ return static_cast<Pair*>(obj.get())-> getCar(); }

inline void setCar(shared_ptr<Object> obj1, shared_ptr<Object> obj2){ static_cast<Pair*>(obj1.get()) -> setCar(obj2); }

inline shared_ptr<Object> getCdr(shared_ptr<Object> obj){ return static_cast<Pair*>(obj.get())-> getCdr(); }

inline void setCdr(shared_ptr<Object> obj1, shared_ptr<Object> obj2){ static_cast<Pair*>(obj1.get()) -> setCdr(obj2); }

bool pairEqual(shared_ptr<Object> obj1, shared_ptr<Object> obj2);

}

#endif	//PAIR_H_INCLUDED
