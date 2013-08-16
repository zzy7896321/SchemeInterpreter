#ifndef RATIONAL_H_INCLUDED
#define RATIONAL_H_INCLUDED

#include "number.h"

#include <sstream>
#include <gmpxx.h>

namespace ObjectDef{

class Rational: public Number{
public:
	Rational() = delete;

	Rational(const Rational&) = default;
	Rational(Rational&&) = default;

	Rational& operator=(const Rational&) = default;
	Rational& operator=(Rational&&) = default;

	~Rational(){}

	template<typename ... Args>
	Rational(bool _exactness, Args... args):Number(ObjectType::RATIONAL, _exactness), val(args...){
		val.canonicalize();
	}

	const mpq_class& getVal() const{
		return val;
	}

	virtual std::string getExternalRep() const{
		std::ostringstream os;
		if (!getExactness()) os << "#i";
		os << val;
		return os.str();
	}

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Rational(*this));
	}

	virtual shared_ptr<Object> add(Number* obj);
	virtual shared_ptr<Object> minus();
	virtual shared_ptr<Object> substract(Number* obj, bool _seq = true);
	virtual shared_ptr<Object> times(Number* obj);
	virtual shared_ptr<Object> divide(Number* obj, bool _seq = true);
	virtual shared_ptr<Object> mulInverse();

	virtual shared_ptr<Object> floor();
	virtual shared_ptr<Object> ceiling();
	virtual shared_ptr<Object> truncate();
	virtual shared_ptr<Object> round();

private:
	template<class T>
	shared_ptr<Object> _add(bool _exactness, const T& obj);
	template<class T>
	shared_ptr<Object> _substract(bool _seq, bool _exactness, const T& obj);
	template<class T>
	shared_ptr<Object> _times(bool _exactness, const T& obj);
	template<class T>
	shared_ptr<Object> _divide(bool _seq, bool _exactness, const T& obj);
private:
	mpq_class val;
};

}

#endif	//RATIONAL_H_INCLUDED

