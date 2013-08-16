#ifndef PROMISE_H_INCLUDED
#define PROMISE_H_INCLUDED

#include "object.h"
#include "expression_pack.h"
#include "environment.h"
#include "../interp/errorinfo.h"
#include "../parser/parsetree.h"

#include <memory>
#include <utility>

namespace ObjectDef{

class Promise: public Object{
public:
	Promise() = delete;

	Promise(const Promise&) = default;
	Promise(Promise&&) = default;

	Promise& operator=(const Promise&) = default;
	Promise& operator=(Promise&&) = default;

	Promise(std::shared_ptr<ParserEnv::ParseTree> _parseTree,
			std::shared_ptr<Environment> _env):
		Object(ObjectType::PROMISE), obj(new ExpressionPack(_parseTree)), env(_env){}

	Interp::callResultType force(std::shared_ptr<Object>& receiver);

	virtual std::string getExternalRep() const{
		return "<promise>";
	}

    virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Promise(*this));
	}
private:
	std::shared_ptr<Object> obj;
	std::shared_ptr<Environment> env;
};

}

#endif	//PROMISE_H_INCLUDED

