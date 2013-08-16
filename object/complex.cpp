#include "number.h"
#include "int.h"
#include "rational.h"
#include "real.h"
#include "numberop.h"
#include "common_constants.h"

#include <memory>
#include <cassert>

namespace ObjectDef{

shared_ptr<Object> Complex::add(Number* obj){
	switch(obj->getType()){
	case ObjectType::INT:
	case ObjectType::BIGINT:
	case ObjectType::RATIONAL:
	case ObjectType::REAL:{
		return shared_ptr<Object>(new Complex(
					true,
                    obj->add(static_cast<Number*>(real.get())), imag));
    }
	case ObjectType::COMPLEX:{
		shared_ptr<Object> _imag = ObjectDef::add(imag, static_cast<Complex*>(obj)->imag);
		if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
            return ObjectDef::add(real, static_cast<Complex*>(obj)->real);
		}
		else {
            return shared_ptr<Object>(new Complex(
					true,
					ObjectDef::add(real, static_cast<Complex*>(obj)->real),
					_imag));
        }
	}
	default:
        assert(false);
		return nullptr;	//should never be reached
	}
}

shared_ptr<Object> Complex::minus(){
	return shared_ptr<Object>(new Complex(true, ObjectDef::minus(real), ObjectDef::minus(imag)));
}

shared_ptr<Object> Complex::substract(Number* obj, bool _seq){
	switch(obj->getType()){
	case ObjectType::INT:
	case ObjectType::BIGINT:
	case ObjectType::RATIONAL:
	case ObjectType::REAL:
		return shared_ptr<Object>(new Complex(
					true,
				    obj->substract(static_cast<Number*>(real.get()), !_seq), (_seq) ? imag : ObjectDef::minus(imag)));
	case ObjectType::COMPLEX:
		if (_seq){
            shared_ptr<Object> _imag = ObjectDef::substract(imag, static_cast<Complex*>(obj)->imag);
            if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
                return ObjectDef::substract(real, static_cast<Complex*>(obj)->real);
            }
			else  {
                return shared_ptr<Object>(new Complex(true,
                            ObjectDef::substract(real, static_cast<Complex*>(obj)->real),
                            _imag));
            }
		}
		else{
            shared_ptr<Object> _imag = ObjectDef::substract(static_cast<Complex*>(obj)->imag, imag);
            if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
                return ObjectDef::substract(static_cast<Complex*>(obj)->real, real);
            }
			else  {
                return shared_ptr<Object>(new Complex(true,
                            ObjectDef::substract(static_cast<Complex*>(obj)->real, real),
                            _imag));
            }
        }
	default:
        assert(false);
		return nullptr;	//should never be reached
	}
}

shared_ptr<Object> Complex::times(Number* obj){
	switch(obj->getType()){
	case ObjectType::INT:
	case ObjectType::BIGINT:
	case ObjectType::RATIONAL:
	case ObjectType::REAL:{
        shared_ptr<Object> _imag = obj->times(static_cast<Number*>(imag.get()));
        if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
            return obj->times(static_cast<Number*>(real.get()));
        }
        else {
            return shared_ptr<Object>(new Complex(
                        true,
                        obj->times(static_cast<Number*>(real.get())),
                        _imag));
        }
    }
	case ObjectType::COMPLEX:{
        shared_ptr<Object> _imag = ObjectDef::add(ObjectDef::times(real, static_cast<Complex*>(obj)->imag),
                                                  ObjectDef::times(imag, static_cast<Complex*>(obj)->real));
        if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
            return ObjectDef::substract(ObjectDef::times(real, static_cast<Complex*>(obj)->real),
                                        ObjectDef::times(imag, static_cast<Complex*>(obj)->imag));
        }
		else {
            return shared_ptr<Object>(new Complex(
                        true,
                        ObjectDef::substract(ObjectDef::times(real, static_cast<Complex*>(obj)->real),
                                             ObjectDef::times(imag, static_cast<Complex*>(obj)->imag)),
                        _imag));
        }
	}
	default:
        assert(false);
		return nullptr;	//should never be reached
	}
}

shared_ptr<Object> Complex::divide(Number* obj, bool _seq){
	switch(obj->getType()){
	case ObjectType::INT:
	case ObjectType::BIGINT:
	case ObjectType::RATIONAL:
	case ObjectType::REAL:
		if (_seq)   // complex with non-zero imag / real is impossible to be a real
			return shared_ptr<Object>(new Complex(
						true,
					    obj->divide(static_cast<Number*>(real.get()), false),
					    obj->divide(static_cast<Number*>(imag.get()), false)));
		else{
			shared_ptr<Object> tmp = ObjectDef::add(ObjectDef::times(real, real), ObjectDef::times(imag, imag));
			shared_ptr<Object> _imag = ObjectDef::divide(ObjectDef::minus(obj->times(static_cast<Number*>(imag.get()))), tmp);
			if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
                return ObjectDef::divide(obj->times(static_cast<Number*>(real.get())), tmp);
			}
			else {
                return shared_ptr<Object>(new Complex(
                            true,
                            ObjectDef::divide(obj->times(static_cast<Number*>(real.get())), tmp),
                            _imag));
            }
		}
	case ObjectType::COMPLEX:
		if (_seq){
			shared_ptr<Object> real2 = static_cast<Complex*>(obj)->real;
			shared_ptr<Object> imag2 = static_cast<Complex*>(obj)->imag;
			shared_ptr<Object> tmp = ObjectDef::add(ObjectDef::times(real2, real2), ObjectDef::times(imag2, imag2));
			shared_ptr<Object> _imag = ObjectDef::divide(ObjectDef::substract(ObjectDef::times(imag, real2),
                                                                              ObjectDef::times(real, imag2)), tmp);
            if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
                return ObjectDef::divide(ObjectDef::add(ObjectDef::times(real, real2),
                                                        ObjectDef::times(imag, imag2)), tmp);
			}
			else {
                return shared_ptr<Object>(new Complex(
                            true,
                            ObjectDef::divide(ObjectDef::add(ObjectDef::times(real, real2),
                                                             ObjectDef::times(imag, imag2)), tmp),
                            _imag));
            }
		}
		else{
			shared_ptr<Object> real2 = static_cast<Complex*>(obj)->real;
			shared_ptr<Object> imag2 = static_cast<Complex*>(obj)->imag;
			shared_ptr<Object> tmp = ObjectDef::add(ObjectDef::times(real, real), ObjectDef::times(imag, imag));
			shared_ptr<Object> _imag = ObjectDef::divide(ObjectDef::substract(ObjectDef::times(imag2, real),
                                                                              ObjectDef::times(real2, imag)), tmp);
            if (static_cast<Number*>(_imag.get())->getExactness() && ObjectDef::equal(_imag, getZero())){
                return ObjectDef::divide(ObjectDef::add(ObjectDef::times(real2, real),
                                                        ObjectDef::times(imag2, imag)), tmp);
            }
			else {
                return shared_ptr<Object>(new Complex(
                            true,
                            ObjectDef::divide(ObjectDef::add(ObjectDef::times(real2, real),
                                                             ObjectDef::times(imag2, imag)), tmp),
                            _imag));
            }
		}
	default:
        assert(false);
		return nullptr;	//should never be reached
	}
}

shared_ptr<Object> Complex::mulInverse(){
	shared_ptr<Object> tmp = ObjectDef::add(ObjectDef::times(real, real), ObjectDef::times(imag, imag));
	return shared_ptr<Object>(new Complex(
				true,
				ObjectDef::divide(real, tmp), ObjectDef::minus(ObjectDef::divide(imag, tmp))));
}

std::string Complex::getExternalRep() const{
    std::ostringstream os;
	if (!getExactness() && (ObjectDef::isRational(real) || ObjectDef::isRational(imag))) os << "#i";
	else if (getExactness() && (!ObjectDef::isRational(real) || !ObjectDef::isRational(imag))) os << "#e";

    std::string tmp = real->getExternalRep();
	if (tmp[0] == '#') os << tmp.substr(2, std::string::npos); else os << tmp;
	tmp = imag->getExternalRep();
	if (tmp[0] == '#') {
           if (tmp[2] != '+' && tmp[2] != '-') os << '+';
           os << tmp.substr(2, std::string::npos);
	}
	else {
           if (tmp[0] != '+' && tmp[2] != '-') os << '+';
           os << tmp;
	}
	os << 'i';
	return os.str();
}

}
