#ifndef INT_H_INCLUDED
#define INT_H_INCLUDED

#include "number.h"

#include <memory>
#include <sstream>
#include <gmpxx.h>

namespace ObjectDef{

class Int:public Number{
public:
	typedef long integer;

	Int() = delete;
	Int(const Int&) = default;
	Int(Int&&) = default;

	Int& operator=(const Int&) = default;
	Int& operator=(Int&&) = default;

	~Int(){}

	Int(bool _exactness, integer _val):Number(ObjectType::INT, _exactness), val(_val){}

	integer getVal() const{
		return val;
	}

	virtual std::string getExternalRep() const{
		std::ostringstream os;
		if (!getExactness()) os << "#i";
		os << val;
		return os.str();
	}

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Int(*this));
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
	shared_ptr<Object> _add(bool _exactness, integer obj);
	shared_ptr<Object> _substract(bool _seq, bool _exactness, integer obj);
	shared_ptr<Object> _times(bool _exactness, integer obj);
	shared_ptr<Object> _divide(bool _seq, bool _exactness, integer obj);

private:
	integer val;
};



class BigInt:public Number{
public:
	BigInt() = delete;

	BigInt(const BigInt&) = default;
	BigInt(BigInt&&) = default;

	BigInt& operator=(const BigInt&) = default;
	BigInt& operator=(BigInt&&) = default;

	~BigInt(){}

	template<typename ... Args>
	BigInt(bool _exactness, Args&&... args):Number(ObjectType::BIGINT, _exactness), val(args...){}

	//conversion from Int*
	explicit BigInt(Int* rhs):Number(ObjectType::BIGINT, rhs->getExactness()), val(rhs->getVal()){}

	const mpz_class& getVal() const{
		return val;
	}

	virtual std::string getExternalRep() const{
		std::ostringstream os;
		if (!getExactness()) os << "#i";
        os << val;
		return os.str();
	}

    virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new BigInt(*this));
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
	mpz_class val;
};

}

#endif // INT_H_INCLUDED
