#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED

#include "object.h"

#include <memory>
#include <map>
#include <iostream>

namespace ObjectDef{

using std::shared_ptr;

class Environment: public Object{
public:
	typedef std::map<std::string, shared_ptr<Object>> IdentifierTableType;
	typedef std::pair<typename IdentifierTableType::iterator, Environment*> lookupWithEnvType;

	Environment():Object(ObjectType::ENVIRONMENT), prevEnv(), table(){}

	Environment(shared_ptr<Environment> _prevEnv):Object(ObjectType::ENVIRONMENT), prevEnv(_prevEnv), table(){}

	Environment(const Environment&) = default;
	Environment(Environment&&) = default;

	Environment& operator=(const Environment&) = default;
	Environment& operator=(Environment&&) = default;

	virtual std::string getExternalRep() const{
		return "<Environment>";
	}

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new Environment(*this));
	}

	shared_ptr<Object> lookup(const std::string& identifier) {
        auto cur_env = this;
		while (cur_env){
			auto iter_item = cur_env->table.find(identifier);
			if (iter_item != cur_env->table.end()){
                return iter_item->second;
			}
			cur_env = cur_env->prevEnv.get();
		}
		return nullptr;
	}

	lookupWithEnvType lookup_with_env(const std::string& identifier){
        auto cur_env = this;
		while (cur_env){
			auto iter_item = cur_env->table.find(identifier);
			if (iter_item != cur_env->table.end()){
                return std::make_pair(iter_item, cur_env);
			}
			cur_env = cur_env->prevEnv.get();
		}
		return make_pair(table.end(), this);
	}

	void insert(const std::string& identifier, shared_ptr<Object> obj){
		table[identifier] = obj;
	}

	bool modify(const std::string& identifier, shared_ptr<Object> obj){
        auto cur_env = this;
		while (cur_env){
			auto iter_item = cur_env->table.find(identifier);
			if (iter_item != cur_env->table.end()){
                iter_item->second = obj;
                return true;
			}
			cur_env = cur_env->prevEnv.get();
		}
		return false;
	}

	bool modify(lookupWithEnvType lookupResult, shared_ptr<Object> obj){
        if (lookupResult.first != lookupResult.second->table.end()){
            lookupResult.first->second = obj;
            return true;
        }
        return false;
	}

	void clear(){
		table.clear();
	}

	shared_ptr<Environment> getPrevEnv(){
		return prevEnv;
	}

	void setPrevEnv(shared_ptr<Environment> _prevEnv){
		prevEnv = _prevEnv;
	}

private:
	shared_ptr<Environment> prevEnv;
	IdentifierTableType table;

public:
	static shared_ptr<Environment> getBuiltinEnv(){
		static shared_ptr<Environment> builtinEnv(new Environment());
		return builtinEnv;
	}

	static shared_ptr<Environment> getTopLevelEnv(){
		static shared_ptr<Environment> topLevelEnv(new Environment(getBuiltinEnv()));
		return topLevelEnv;
	}
};

}

#endif	//ENVIRONMENT_H_INCLUDED
