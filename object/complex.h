#ifndef COMPLEX_H_INCLUDED
#define COMPLEX_H_INCLUDED

#include "numberop.h"
#include "number.h"

#include <gmpxx.h>
#include <memory>
#include <cassert>

namespace ObjectDef{

using std::shared_ptr;

class Complex: public Number{
public:
	Complex() = delete;

	Complex(const Complex&) = default;
	Complex(Complex&&) = default;

	Complex& operator=(const Complex&) = default;
	Complex& operator=(Complex&&) = default;

	~Complex(){}

	Complex(bool, shared_ptr<Object> _real, shared_ptr<Object> _imag):  // the first argument is ignored
		Number(ObjectType::COMPLEX,
               static_cast<Number*>(_real.get())->getExactness() && static_cast<Number*>(_imag.get())->getExactness())
               , real(_real), imag(_imag){}

	shared_ptr<Object> getReal(){
		return real;
	}

	shared_ptr<Object> getImag(){
		return imag;
	}

	virtual std::string getExternalRep() const;

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Complex(*this));
	}

	virtual shared_ptr<Object> add(Number* obj);
	virtual shared_ptr<Object> minus();
	virtual shared_ptr<Object> substract(Number* obj, bool _seq = true);
	virtual shared_ptr<Object> times(Number* obj);
	virtual shared_ptr<Object> divide(Number* obj, bool _seq = true);
	virtual shared_ptr<Object> mulInverse();

	virtual shared_ptr<Object> floor(){ assert(false); return nullptr; }	//should never called
	virtual shared_ptr<Object> ceiling(){ assert(false); return nullptr; }
	virtual shared_ptr<Object> truncate(){ assert(false); return nullptr; }
	virtual shared_ptr<Object> round(){ assert(false); return nullptr; }

private:
	shared_ptr<Object> real;
	shared_ptr<Object> imag;
};

}

#endif	//COMPLEX_H_INCLUDED
