#ifndef REAL_H_INCLUDED
#define REAL_H_INCLUDED

#include "number.h"

#include <iomanip>
#include <sstream>
#include <gmpxx.h>

//Need to call mpf_set_default_prec() in main()

namespace ObjectDef{

class Real: public Number{
public:
	Real() = delete;

	Real(const Real&) = default;
	Real(Real&&) = default;

	Real& operator=(const Real&) = default;
	Real& operator=(Real&&) = default;

    ~Real(){}

	template<typename ... Args>
	Real(bool, Args... args):Number(ObjectType::REAL, false), val(args...){}

	const mpf_class& getVal() const{
		return val;
	}

	virtual std::string getExternalRep() const;

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Real(*this));
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
	mpf_class val;
};

}

#endif	//REAL_H_INCLUDED
