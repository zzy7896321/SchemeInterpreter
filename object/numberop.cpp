#include "numberop.h"
#include "common_constants.h"
#include <limits>
#include <gmpxx.h>
#include <cassert>
#include <type_traits>
#include <gmp.h>
#include <mpfr.h>
#include <mpc.h>
#include <vector>

namespace ObjectDef{

bool isNumber(shared_ptr<Object> obj){
	switch (obj->getType()){
	case ObjectType::COMPLEX :
	case ObjectType::REAL :
	case ObjectType::RATIONAL :
	case ObjectType::BIGINT :
	case ObjectType::INT :
		return true;
	default:
		return false;
	}
}

bool isComplex(shared_ptr<Object> obj){
	switch (obj->getType()){
	case ObjectType::COMPLEX :
	case ObjectType::REAL :
	case ObjectType::RATIONAL :
	case ObjectType::BIGINT :
	case ObjectType::INT :
		return true;
	default:
		return false;
	}
}

bool isReal(shared_ptr<Object> obj){
	switch (obj->getType()){
	case ObjectType::REAL :
	case ObjectType::RATIONAL :
	case ObjectType::BIGINT :
	case ObjectType::INT :
		return true;
	default:
		return false;
	}
}

bool isRational(shared_ptr<Object> obj){
	switch (obj->getType()){
	case ObjectType::RATIONAL :
	case ObjectType::BIGINT :
	case ObjectType::INT :
		return true;
	default:
		return false;
	}
}

bool isInteger(shared_ptr<Object> obj){
	switch (obj->getType()){
	case ObjectType::BIGINT :
	case ObjectType::INT :
		return true;
	default:
		return false;
	}

}

bool isExact(shared_ptr<Object> obj){
	return static_cast<Number*>(obj.get())->getExactness();
}

namespace ActualOps{

template<class T1, class T2>
struct _equal{
    static bool invoke(T1 t1, T2 t2){
        return t1->getVal() == t2->getVal();
    }
};

template<class T2>
struct _equal<Number*, T2>{
    static bool invoke(Number* t1, T2 t2){
    	switch (t1->getType()){
        case ObjectType::INT:
            return _equal<T2, Int*>::invoke(t2, static_cast<Int*>(t1));
        case ObjectType::BIGINT:
            return _equal<T2, BigInt*>::invoke(t2, static_cast<BigInt*>(t1));
        case ObjectType::RATIONAL:
            return _equal<T2, Rational*>::invoke(t2, static_cast<Rational*>(t1));
        case ObjectType::REAL:
            return _equal<T2, Real*>::invoke(t2, static_cast<Real*>(t1));
        case ObjectType::COMPLEX:
            return _equal<T2, Complex*>::invoke(t2, static_cast<Complex*>(t1));
        default:
            assert(false);
            return false;   //should never reach here
        }
    }
};

template<class T1>
struct _equal<T1, Complex*>{
    static bool invoke(T1 t1, Complex* t2){
        return _equal<Number*, Int*>::invoke(static_cast<Number*>(t2->getImag().get()), static_cast<Int*>(getZero().get())) &&
               _equal<Number*, T1>::invoke(static_cast<Number*>(t2->getReal().get()), t1);
    }
};

template<>
struct _equal<Number*, Complex*>{
    static bool invoke(Number* t1, Complex* t2){
        switch (t1->getType()){
        case ObjectType::INT:
        case ObjectType::BIGINT:
        case ObjectType::RATIONAL:
        case ObjectType::REAL:
            return _equal<Number*, Number*>::invoke(static_cast<Number*>(t2->getImag().get()), static_cast<Number*>(getZero().get())) &&
                   _equal<Number*, Number*>::invoke(static_cast<Number*>(t2->getReal().get()), t1);
        case ObjectType::COMPLEX:
            return _equal<Number*, Number*>::invoke(static_cast<Number*>(t2->getReal().get()), static_cast<Number*>(static_cast<Complex*>(t1)->getReal().get()))
                && _equal<Number*, Number*>::invoke(static_cast<Number*>(t2->getImag().get()), static_cast<Number*>(static_cast<Complex*>(t1)->getImag().get()));
        default:
            assert(false);
            return false;   //should never reach here
        }
    }
};

}

bool equal(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return ActualOps::_equal<Number*, Number*>::invoke( static_cast<Number*>(obj1.get()),
                                      static_cast<Number*>(obj2.get()));
}

namespace ActualOps{

template<class T1, class T2>
bool _lessThan(T1 t1, T2 t2){
	return t1->getVal() < t2->getVal();
}

template<class T2>
bool _lessThan(Number* t1, T2 t2){
	switch (t1->getType()){
	case ObjectType::INT:
		return _lessThan(t2, static_cast<Int*>(t1));
	case ObjectType::BIGINT:
		return _lessThan(t2, static_cast<BigInt*>(t1));
	case ObjectType::RATIONAL:
		return _lessThan(t2, static_cast<Rational*>(t1));
	case ObjectType::REAL:
		return _lessThan(t2, static_cast<Real*>(t1));
    default:
        assert(false);
        return false;   //should never reach here
	}
}

}

bool lessThan(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	return ActualOps::_lessThan( static_cast<Number*>(obj1.get()),
							  static_cast<Number*>(obj2.get()));
}

shared_ptr<Object> add(shared_ptr<Object>obj1, shared_ptr<Object> obj2){
	return static_cast<Number*>(obj1.get())->add(static_cast<Number*>(obj2.get()));
}

shared_ptr<Object> minus(shared_ptr<Object> obj1){
	return static_cast<Number*>(obj1.get())->minus();
}

shared_ptr<Object> substract(shared_ptr<Object>obj1, shared_ptr<Object> obj2){
	return static_cast<Number*>(obj1.get())->substract(static_cast<Number*>(obj2.get()));
}

shared_ptr<Object> times(shared_ptr<Object>obj1, shared_ptr<Object> obj2){
	return static_cast<Number*>(obj1.get())->times(static_cast<Number*>(obj2.get()));
}

shared_ptr<Object> divide(shared_ptr<Object>obj1, shared_ptr<Object> obj2){
	return static_cast<Number*>(obj1.get())->divide(static_cast<Number*>(obj2.get()));
}

shared_ptr<Object> mulInverse(shared_ptr<Object> obj1){
	return static_cast<Number*>(obj1.get())->mulInverse();
}

namespace ActualOps{
	shared_ptr<Object> _quotient( Int* obj1, Int* obj2){
		if (obj1->getVal() == std::numeric_limits<Int::integer>::min() && obj2->getVal() == -1){
			return shared_ptr<Object>(new BigInt(obj1->getExactness() && obj2->getExactness(), -mpz_class(std::numeric_limits<Int::integer>::min())));
		} else {
			return shared_ptr<Object>(new Int(obj1->getExactness() && obj2->getExactness(), obj1->getVal() / obj2->getVal()));
		}
	}

	template<class T1, class T2>
	shared_ptr<Object> _quotient(T1 obj1, T2 obj2){
		return shared_ptr<Object>(new BigInt(obj1->getExactness() && obj2->getExactness(), obj1->getVal() / obj2->getVal()));
	}
}

shared_ptr<Object> quotient(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	switch (obj1->getType()){
	case ObjectType::INT:
		switch (obj2->getType()){
		case ObjectType::INT:
			return ActualOps::_quotient(static_cast<Int*>(obj1.get()), static_cast<Int*>(obj2.get()));
		case ObjectType::BIGINT:
			return ActualOps::_quotient(static_cast<Int*>(obj1.get()), static_cast<BigInt*>(obj2.get()));
		default:;
		}
	case ObjectType::BIGINT:
		switch(obj2->getType()){
		case ObjectType::INT:
			return ActualOps::_quotient(static_cast<BigInt*>(obj1.get()), static_cast<Int*>(obj2.get()));
		case ObjectType::BIGINT:
			return ActualOps::_quotient(static_cast<BigInt*>(obj1.get()), static_cast<BigInt*>(obj2.get()));
		default:;
		}
    default:;
	}

    assert(false);
    return nullptr; //should never be reach here
}

namespace ActualOps{
	shared_ptr<Object> _remainder( Int* obj1, Int* obj2){
		return shared_ptr<Object>(new Int(obj1->getExactness() && obj2->getExactness(), obj1->getVal() % obj2->getVal()));
	}

	template<class T1, class T2>
	shared_ptr<Object> _remainder(T1 obj1, T2 obj2){
		return shared_ptr<Object>(new BigInt(obj1->getExactness() && obj2->getExactness(), obj1->getVal() % obj2->getVal()));
	}
}

shared_ptr<Object> remainder(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	switch (obj1->getType()){
	case ObjectType::INT:
		switch (obj2->getType()){
		case ObjectType::INT:
			return ActualOps::_remainder(static_cast<Int*>(obj1.get()), static_cast<Int*>(obj2.get()));
		case ObjectType::BIGINT:
			return ActualOps::_remainder(static_cast<Int*>(obj1.get()), static_cast<BigInt*>(obj2.get()));
        default:;
		}
	case ObjectType::BIGINT:
		switch(obj2->getType()){
		case ObjectType::INT:
			return ActualOps::_remainder(static_cast<BigInt*>(obj1.get()), static_cast<Int*>(obj2.get()));
		case ObjectType::BIGINT:
			return ActualOps::_remainder(static_cast<BigInt*>(obj1.get()), static_cast<BigInt*>(obj2.get()));
        default:;
		}
    default:;
	}

    assert(false);
    return nullptr; //which should never be reached
}

namespace ActualOps{
	shared_ptr<Object> _modulo( Int* obj1, Int* obj2){
		if (obj1->getVal() % obj2->getVal() == 0){
			return shared_ptr<Object>(new Int(obj1->getExactness() && obj2->getExactness(), 0));
		}
		else if ((obj1->getVal() > 0 && obj2->getVal() > 0) || (obj1->getVal() < 0 && obj2->getVal() < 0)){
			return shared_ptr<Object>(new Int(obj1->getExactness() && obj2->getExactness(), obj1->getVal() % obj2->getVal()));
		}
		else {
			return shared_ptr<Object>(new Int(obj1->getExactness() && obj2->getExactness(), obj1->getVal() % obj2->getVal() + obj2->getVal()));
		}
	}

	template<class T1, class T2>
	shared_ptr<Object> _modulo(T1 obj1, T2 obj2){
		if (obj1->getVal() % obj2->getVal() == 0){
			return shared_ptr<Object>(new BigInt(obj1->getExactness() && obj2->getExactness(), 0));
		}
		else if ( (obj1->getVal() > 0 && obj2->getVal() > 0) || (obj1->getVal() < 0 && obj2->getVal() < 0)){
			return shared_ptr<Object>(new BigInt(obj1->getExactness() && obj2->getExactness(), obj1->getVal() % obj2->getVal()));
		} else{
			return shared_ptr<Object>(new BigInt(obj1->getExactness() && obj2->getExactness(), obj1->getVal() % obj2->getVal() + obj2->getVal()));
		}
	}
}

shared_ptr<Object> modulo(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
	switch (obj1->getType()){
	case ObjectType::INT:
		switch (obj2->getType()){
		case ObjectType::INT:
			return ActualOps::_modulo(static_cast<Int*>(obj1.get()), static_cast<Int*>(obj2.get()));
		case ObjectType::BIGINT:
			return ActualOps::_modulo(static_cast<Int*>(obj1.get()), static_cast<BigInt*>(obj2.get()));
        default:;
		}
	case ObjectType::BIGINT:
		switch(obj2->getType()){
		case ObjectType::INT:
			return ActualOps::_modulo(static_cast<BigInt*>(obj1.get()), static_cast<Int*>(obj2.get()));
		case ObjectType::BIGINT:
			return ActualOps::_modulo(static_cast<BigInt*>(obj1.get()), static_cast<BigInt*>(obj2.get()));
        default:;
		}
    default:;
	}

    assert(false);
	return nullptr; //which should never be reached
}

shared_ptr<Object> numerator(shared_ptr<Object> obj){
    switch (obj->getType()){
        case ObjectType::INT:
        case ObjectType::BIGINT:
            return obj;
        case ObjectType::RATIONAL:
            return shared_ptr<Object>(new BigInt(static_cast<Rational*>(obj.get())->getExactness(),
                                                 static_cast<Rational*>(obj.get())->getVal().get_num()));
        default:;
    }

    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> denominator(shared_ptr<Object> obj){
	    switch (obj->getType()){
        case ObjectType::INT:
        case ObjectType::BIGINT:
            return shared_ptr<Object>(new Int(static_cast<Number*>(obj.get())->getExactness(), 1));
        case ObjectType::RATIONAL:
            return shared_ptr<Object>(new BigInt(static_cast<Rational*>(obj.get())->getExactness(),
                                                 static_cast<Rational*>(obj.get())->getVal().get_den()));
        default:;
    }

    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> floor(shared_ptr<Object> obj){
	return static_cast<Number*>(obj.get())->floor();
}

shared_ptr<Object> ceiling(shared_ptr<Object> obj){
	return static_cast<Number*>(obj.get())->ceiling();
}

shared_ptr<Object> truncate(shared_ptr<Object> obj){
	return static_cast<Number*>(obj.get())->truncate();
}

shared_ptr<Object> round(shared_ptr<Object> obj){
	return static_cast<Number*>(obj.get())->round();
}

static inline void set_mpz_class_from_object(mpz_class& rop, shared_ptr<Object> obj){
    switch (obj->getType()){
        case ObjectType::INT:
            rop = static_cast<Int*>(obj.get())->getVal();
            break;
        case ObjectType::BIGINT:
            rop = static_cast<BigInt*>(obj.get())->getVal();
            break;
        default:
            assert(false);  // should never reach here
    }
}

static inline void set_mpfr_t_from_object(mpfr_t& rop, shared_ptr<Object> obj){
    switch (obj->getType()){
        case ObjectType::INT:
            mpfr_set_si(rop, static_cast<Int*>(obj.get())->getVal(), MPFR_RNDN);
            break;
        case ObjectType::BIGINT:
            mpfr_set_z(rop, static_cast<BigInt*>(obj.get())->getVal().get_mpz_t(), MPFR_RNDN);
            break;
        case ObjectType::RATIONAL:
            mpfr_set_q(rop, static_cast<Rational*>(obj.get())->getVal().get_mpq_t(), MPFR_RNDN);
            break;
        case ObjectType::REAL:
            mpfr_set_f(rop, static_cast<Real*>(obj.get())->getVal().get_mpf_t(), MPFR_RNDN);
            break;
        default:
            assert(false);  // should never reach here
    }
}

static inline void set_mpc_t_from_object(mpc_t& rop, shared_ptr<Object> obj){
    switch (obj->getType()){
        case ObjectType::INT:{
            mpc_set_si(rop, static_cast<Int*>(obj.get())->getVal(), MPC_RNDNN);
        }
            break;
        case ObjectType::BIGINT:{
            mpc_set_z(rop, static_cast<BigInt*>(obj.get())->getVal().get_mpz_t(), MPC_RNDNN);
        }
            break;
        case ObjectType::RATIONAL:{
            mpc_set_q(rop, static_cast<Rational*>(obj.get())->getVal().get_mpq_t(), MPC_RNDNN);
        }
            break;
        case ObjectType::REAL:{
            mpc_set_f(rop, static_cast<Real*>(obj.get())->getVal().get_mpf_t(), MPC_RNDNN);
        }
            break;
        case ObjectType::COMPLEX:{
            mpfr_t oprand;
            mpfr_init2(oprand, mpf_get_default_prec());
            set_mpfr_t_from_object(oprand, static_cast<Complex*>(obj.get())->getReal());
            mpfr_swap(mpc_realref(rop), oprand);
            set_mpfr_t_from_object(oprand, static_cast<Complex*>(obj.get())->getImag());
            mpfr_swap(mpc_imagref(rop), oprand);
            mpfr_clear(oprand);
        }
            break;
        default:
            assert(false);  //should never reach here
    }
}

static inline bool getExactness(shared_ptr<Object> obj){
    return static_cast<Number*>(obj.get())->getExactness();
}

shared_ptr<Object> gcd(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
    mpz_class oprand1, oprand2, res;
    set_mpz_class_from_object(oprand1, obj1);
    set_mpz_class_from_object(oprand2, obj2);
    mpz_gcd(res.get_mpz_t(), oprand1.get_mpz_t(), oprand2.get_mpz_t());
    return shared_ptr<Object>(new BigInt(getExactness(obj1) && getExactness(obj2), res));
}

shared_ptr<Object> lcm(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
    mpz_class oprand1, oprand2, res;
    set_mpz_class_from_object(oprand1, obj1);
    set_mpz_class_from_object(oprand2, obj2);
    mpz_lcm(res.get_mpz_t(), oprand1.get_mpz_t(), oprand2.get_mpz_t());
    return shared_ptr<Object>(new BigInt(getExactness(obj1) && getExactness(obj2), res));
}

/*shared_ptr<Object> rationalize(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
    auto lowerBound = substract(obj1, obj2);
    auto upperBound = add(obj1, obj2);
    if ( leq(times(lowerBound, upperBound), getZero()) ){
        return shared_ptr<Object>(new Rational(getExactness(obj1) && getExactness(obj2), 0, 1));
    }
    else if (lessThan(upperBound, getZero())){

    }
    else{
        mpz_class num, den;

    }
}*/

shared_ptr<Object> sqrt(shared_ptr<Object> obj){
    switch (obj->getType()){
        case ObjectType::INT:
        case ObjectType::BIGINT:
        {
            bool positive = true;
            mpz_class op;
            set_mpz_class_from_object(op, obj);
            if (op < 0){
                positive = false;
                op = -op;
            }
            if (mpz_perfect_square_p(op.get_mpz_t())){
                mpz_sqrt(op.get_mpz_t(), op.get_mpz_t());
                if (positive){
                    return shared_ptr<Object>(new BigInt(getExactness(obj), op));
                }
                else{
                    return shared_ptr<Object>(new Complex(true, getZero(),
                        shared_ptr<Object>(new BigInt(getExactness(obj), op))));
                }
            }
            else {
                mpfr_t op;
                mpfr_init2(op, mpf_get_default_prec());
                set_mpfr_t_from_object(op, obj);
                if (!positive) mpfr_neg(op, op, MPFR_RNDN);
                mpfr_sqrt(op, op, MPFR_RNDN);
                mpf_class ret;
                mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
                mpfr_clear(op);
                if (positive){
                    return shared_ptr<Object>(new Real(false, ret));
                }
                else {
                    return shared_ptr<Object>(new Complex(false, getZero(),
                            shared_ptr<Object>(new Real(false, ret))));
                }
            }
        }
            break;
        case ObjectType::RATIONAL:{
            mpz_class num(static_cast<Rational*>(obj.get())->getVal().get_num() *
                          static_cast<Rational*>(obj.get())->getVal().get_den());
            bool positive = true;
            if (num < 0){
                positive = false;
                num = -num;
            }
            if (mpz_perfect_square_p(num.get_mpz_t())){
                mpz_sqrt(num.get_mpz_t(), num.get_mpz_t());
                if (positive){
                    return shared_ptr<Object>(new Rational(getExactness(obj),
                        num, static_cast<Rational*>(obj.get())->getVal().get_den()));
                }
                else{
                    return shared_ptr<Object>(new Complex(true,
                        getZero(), shared_ptr<Object>(new Rational(getExactness(obj), num,
                            static_cast<Rational*>(obj.get())->getVal().get_den()))));
                }
            }
            else {
                mpfr_t op;
                mpfr_init2(op, mpf_get_default_prec());
                set_mpfr_t_from_object(op, obj);
                if (!positive) mpfr_neg(op, op, MPFR_RNDN);
                mpfr_sqrt(op, op, MPFR_RNDN);
                mpf_class ret;
                mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
                mpfr_clear(op);
                if (positive){
                    return shared_ptr<Object>(new Real(false, ret));
                }
                else {
                    return shared_ptr<Object>(new Complex(false, getZero(),
                        shared_ptr<Object>(new Real(false, ret))));
                }
            }
        }
            break;
        case ObjectType::REAL:{
            mpfr_t op;
            mpfr_init2(op, mpf_get_default_prec());
            set_mpfr_t_from_object(op, obj);
            bool positive = true;
            if (mpfr_sgn(op) < 0) {
                positive = false;
                mpfr_neg(op, op, MPFR_RNDN);
            }
            mpfr_sqrt(op, op, MPFR_RNDN);
            mpf_class ret;
            mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
            mpfr_clear(op);
            if (positive){
                return shared_ptr<Object>(new Real(false, ret));
            }
            else {
                return shared_ptr<Object>(new Complex(false, getZero(),
                    shared_ptr<Object>(new Real(false, ret))));
            }
        }
            break;

        case ObjectType::COMPLEX:{
            mpc_t op;
            mpc_init2(op, mpf_get_default_prec());
            set_mpc_t_from_object(op, obj);
            mpc_sqrt(op, op, MPC_RNDNN);
            mpf_class ret1;
            mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
            mpf_class ret2;
            mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
            mpc_clear(op);
            return shared_ptr<Object>(new Complex(false,
                                                  shared_ptr<Object>(new Real(false, ret1)),
                                                  shared_ptr<Object>(new Real(false, ret2))));
        }
            break;
        default:
            assert(false);
            return nullptr;     //should never reach here
    }
}

shared_ptr<Object> expt(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
    if (ObjectDef::equal(obj1, getZero())){
        if (ObjectDef::equal(obj2, getZero())){
            return shared_ptr<Object>(new Int(getExactness(obj1) && getExactness(obj2), 1));
        }
        else {
            return shared_ptr<Object>(new Int(getExactness(obj1) && getExactness(obj2), 0));
        }
    }
    else if (isInteger(obj1) && isInteger(obj2)){
        mpz_class op1, op2;
        set_mpz_class_from_object(op1, obj1);
        set_mpz_class_from_object(op2, obj2);
        bool fraction = false;
        if (op2 < 0){
            fraction = true;
            op2 = - op2;
        }
        mp_bitcnt_t ibit = 0;
        for (mp_bitcnt_t nxt = mpz_scan1(op2.get_mpz_t(), ibit + 1);
             nxt != std::numeric_limits<mp_bitcnt_t>::max();
             nxt = mpz_scan1(op2.get_mpz_t(), ibit + 1)){
             ibit = nxt;
        }

        mpz_class ret(1);
        for (; ibit > 0; --ibit){
            ret *= ret;
            if (mpz_tstbit(op2.get_mpz_t(), ibit))
                ret *= op1;
        }
        ret *= ret;
        if (mpz_tstbit(op2.get_mpz_t(), ibit)) ret *= op1;

        if (!fraction){
            return shared_ptr<Object>(new BigInt(getExactness(obj1) && getExactness(obj2), ret));
        }
        else {
            return shared_ptr<Object>(new Rational(getExactness(obj1) && getExactness(obj2), 1, ret));
        }
    }

    else if (isRational(obj1) && isInteger(obj2)){
        mpz_class op1, op2;
        set_mpz_class_from_object(op2, obj2);
        bool fraction = false;
        if (op2 < 0){
            fraction = true;
            op2 = - op2;
        }

        op1 = static_cast<Rational*>(obj1.get())->getVal().get_num();
        mp_bitcnt_t ibit = 0;
        for (mp_bitcnt_t nxt = mpz_scan1(op2.get_mpz_t(), ibit + 1);
             nxt != std::numeric_limits<mp_bitcnt_t>::max();
             nxt = mpz_scan1(op2.get_mpz_t(), ibit + 1)){
             ibit = nxt;
        }
        mpz_class ret1(1);
        for (; ibit > 0; --ibit){
            ret1 *= ret1;
            if (mpz_tstbit(op2.get_mpz_t(), ibit))
                ret1 *= op1;
        }
        ret1 *= ret1;
        if (mpz_tstbit(op2.get_mpz_t(), ibit)) ret1 *= op1;

        op1 = static_cast<Rational*>(obj1.get())->getVal().get_den();
        ibit = 0;
        for (mp_bitcnt_t nxt = mpz_scan1(op2.get_mpz_t(), ibit + 1);
             nxt != std::numeric_limits<mp_bitcnt_t>::max();
             nxt = mpz_scan1(op2.get_mpz_t(), ibit + 1)){
             ibit = nxt;
        }
        mpz_class ret2(1);
        for (; ibit > 0; --ibit){
            ret2 *= ret2;
            if (mpz_tstbit(op2.get_mpz_t(), ibit))
                ret2 *= op1;
        }
        ret2 *= ret2;
        if (mpz_tstbit(op2.get_mpz_t(), ibit)) ret2 *= op1;

        if (!fraction){
            if (ret2 == 1){
                return shared_ptr<Object>(new BigInt(getExactness(obj1) && getExactness(obj2), ret1));
            } else {
                return shared_ptr<Object>(new Rational(getExactness(obj1) && getExactness(obj2), ret1, ret2));
            }
        }
        else {
            if (ret1 < 0){
                ret1 = -ret1;
                ret2 = -ret2;
            }
            if (ret1 == 1){
                return shared_ptr<Object>(new BigInt(getExactness(obj1) && getExactness(obj2), ret2));
            } else {
                return shared_ptr<Object>(new Rational(getExactness(obj1) && getExactness(obj2), ret2, ret1));
            }
        }
    }

    else if (isReal(obj1) && isReal(obj2)){
        mpfr_t op1, op2;
        mpfr_init2(op1, mpf_get_default_prec());
        mpfr_init2(op2, mpf_get_default_prec());
        set_mpfr_t_from_object(op1, obj1);
        set_mpfr_t_from_object(op2, obj2);
        mpfr_pow(op1, op1, op2, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op1, MPFR_RNDN);
        mpfr_clear(op1);
        mpfr_clear(op2);
        return shared_ptr<Object>(new Real(false, ret));
    }

    else{
        mpc_t op1, op2;
        mpc_init2(op1, mpf_get_default_prec());
        mpc_init2(op2, mpf_get_default_prec());
        set_mpc_t_from_object(op1, obj1);
        set_mpc_t_from_object(op2, obj2);
        mpc_pow(op1, op1, op2, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op1), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op1), MPFR_RNDN);
        mpc_clear(op1);
        mpc_clear(op2);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }

    assert(false);
    return nullptr;
}

shared_ptr<Object> exp(shared_ptr<Object> obj){
    if (isReal(obj)){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_exp(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_exp(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> log(shared_ptr<Object> obj){
    if (ObjectDef::equal(obj, getZero())){
        assert(false);
        return nullptr; // should never reach here
    }
    else if (isReal(obj) && (lessThan(getZero(), obj))){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_log(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_log(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> sin(shared_ptr<Object> obj){
    if (isReal(obj)){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_sin(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_sin(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> cos(shared_ptr<Object> obj){
    if (isReal(obj)){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_cos(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_cos(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> tan(shared_ptr<Object> obj){
    if (isReal(obj)){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_tan(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_tan(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> asin(shared_ptr<Object> obj){
    if (isReal(obj)){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_asin(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_asin(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> acos(shared_ptr<Object> obj){
    if (isReal(obj)){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_acos(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_acos(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> atan(shared_ptr<Object> obj){
    if (isReal(obj)){
        mpfr_t op;
        mpfr_init2(op, mpf_get_default_prec());
        set_mpfr_t_from_object(op, obj);
        mpfr_atan(op, op, MPFR_RNDN);
        mpf_class ret;
        mpfr_get_f(ret.get_mpf_t(), op, MPFR_RNDN);
        mpfr_clear(op);
        return shared_ptr<Object>(new Real(false, ret));
    }
    else {  //complex
        mpc_t op;
        mpc_init2(op, mpf_get_default_prec());
        set_mpc_t_from_object(op, obj);
        mpc_atan(op, op, MPC_RNDNN);
        mpf_class ret1, ret2;
        mpfr_get_f(ret1.get_mpf_t(), mpc_realref(op), MPFR_RNDN);
        mpfr_get_f(ret2.get_mpf_t(), mpc_imagref(op), MPFR_RNDN);
        mpc_clear(op);
        return shared_ptr<Object>(new Complex(false,
                    shared_ptr<Object>(new Real(false, ret1)),
                    shared_ptr<Object>(new Real(false, ret2))));
    }
    assert(false);
    return nullptr; //should never reach here
}

shared_ptr<Object> atan(shared_ptr<Object> obj1, shared_ptr<Object> obj2){
    static shared_ptr<Object> pi = acos(minus(getOne()));
    if (ObjectDef::equal(obj1, getZero())){
        if (lessThan(obj2, getZero())){
            return shared_ptr<Object>(new Real(false, -1.0));
        }
        else {
            return shared_ptr<Object>(new Real(false, 1.0));
        }
    }
    else if (ObjectDef::lessThan(obj1, getZero())){
        if (lessThan(obj2, getZero())){
            return substract(atan(divide(obj2, obj1)), pi);
        }
        else {
            return add(atan(divide(obj2, obj1)), pi);
        }
    }
    else {
        return atan(divide(obj2, obj1));
    }
}

}
