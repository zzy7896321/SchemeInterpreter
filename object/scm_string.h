#ifndef SCM_STRING_H_INCLUDED
#define SCM_STRING_H_INCLUDED

#include "object.h"
#include "int.h"
#include "char.h"

#include <memory>
#include <string>
#include <utility>

namespace ObjectDef{

using std::shared_ptr;

class String:public Object{
public:
	String() = default;

	String(const String&) = default;
	String(String&&) = default;

	String& operator=(const String&) = default;
	String& operator=(String&&) = default;

	String(const std::string& _val, bool _isMutable = false, bool _hasDoubleQuote = true)
        :Object(ObjectType::STRING), val((_hasDoubleQuote) ? std::move(_interp(_val)) : _val), isMutable(_isMutable){}

    String(std::string&& _val, bool _isMutable = false, bool _hasDoubleQuote = true)
        :Object(ObjectType::STRING), val((_hasDoubleQuote) ? std::move(_interp(std::move(_val))) : std::move(_val)), isMutable(_isMutable){}

    String(std::string::size_type _cnt, char ch = '\0', bool _isMutable = false)
        :Object(ObjectType::STRING), val(_cnt, ch), isMutable(_isMutable){}

	virtual std::string getExternalRep() const;

	virtual std::shared_ptr<Object> clone(){
        return std::shared_ptr<Object>(new String(*this));
	}

	const std::string& getVal(){
        return val;
	}

	std::string getInterpretedString(){
		return val;
	}

    bool getMutability(){
        return isMutable;
    }

    void setMutability(bool _isMutable){
        isMutable = _isMutable;
    }

    void setChar(int pos, char ch){
        val[pos] = ch;
    }

	int getLength(){
		return val.size();
	}

private:
	std::string val;
	bool isMutable;

	static std::string _interp(const std::string&);

	static std::string _interp(std::string&&);
};

inline bool isString(shared_ptr<Object> obj){
	return obj->getType() == ObjectType::STRING;
}

}

#endif	//SCM_STRING_H_INCLUDED

