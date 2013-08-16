#ifndef BUILTIN_H_INCLUDED
#define BUILTIN_H_INCLUDED

#include "numberop.h"
#include "scm_list.h"
#include "object.h"
#include "../interp/errorinfo.h"

#include <vector>
#include <memory>
#include <utility>
#include <functional>
#include <initializer_list>
#include <tuple>

namespace ObjectDef{

using std::shared_ptr;

class Builtin: public Object{
public:
	typedef typename Interp::callResultType callResultType;
	typedef typename Interp::parameterType parameterType;

	Builtin() = delete;

	Builtin(const Builtin&) = default;
	Builtin(Builtin&&) = default;

	Builtin& operator=(const Builtin&) = default;
	Builtin& operator=(Builtin&&) = default;

	~Builtin(){}

	virtual std::string getExternalRep() const{
		return std::string("<Builtin>: ") + identifier;
	}

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Builtin(*this));
	}

	Builtin(std::string _identifier,
			std::initializer_list<ObjectType> _formal,
			bool _enableList,
			std::function<callResultType(parameterType, shared_ptr<Object>&)> _func):
		Object(ObjectType::BUILTIN), identifier(_identifier), formal(_formal), enableList(_enableList),func(_func){}

	callResultType invoke(parameterType, shared_ptr<Object>&);

	std::string identifier;
	std::vector<ObjectType> formal;
	bool enableList;
	std::function<callResultType(parameterType, shared_ptr<Object>&)> func;

	callResultType checkType(parameterType);

	bool isMatch(shared_ptr<Object> obj, ObjectType type);
};

}

#endif	//BUILTIN_H_INCLUDED
