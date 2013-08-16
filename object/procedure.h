#ifndef PROCEDURE_H_INCLUDED
#define PROCEDURE_H_INCLUDED

#include "object.h"
#include "environment.h"
#include "pair.h"
#include "builtin.h"
#include "../parser/parsetree.h"
#include "../interp/errorinfo.h"

#include <vector>
#include <memory>
#include <utility>

namespace ObjectDef{

using std::shared_ptr;

class Procedure: public Object{
public:
	typedef	Interp::callResultType callResultType;
	typedef Interp::parameterType parameterType;

	Procedure() = delete;

	Procedure(const Procedure&) = default;
	Procedure(Procedure&&) = default;

	Procedure& operator=(const Procedure&) = default;
	Procedure& operator=(Procedure&&) = default;

	~Procedure(){}

	virtual std::string getExternalRep() const{
		return std::string("<Procedure>");
	}

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Procedure(*this));
	}

	Procedure(shared_ptr<Environment> _env,
			  const std::vector<std::string>& _formal,
			  bool _enableList,
			  shared_ptr<ParserEnv::ParseTree> _proc):
		Object(ObjectType::PROC), env(_env), formal(_formal), enableList(_enableList), proc(_proc){}

	Procedure(shared_ptr<Environment> _env,
			  std::vector<std::string>&& _formal,
			  bool _enableList,
			  shared_ptr<ParserEnv::ParseTree> _proc):
		Object(ObjectType::PROC), env(_env), formal(std::move(_formal)), enableList(_enableList), proc(_proc){}


	shared_ptr<Environment> env;	// upper-level environment
	std::vector<std::string> formal;
	bool enableList;
	shared_ptr<ParserEnv::ParseTree> proc;

	callResultType applyFormal(shared_ptr<Environment>&, parameterType);
	callResultType invoke(parameterType, shared_ptr<Object>&);

};


}


#endif	//PROCEDURE_H_INCLUDED
