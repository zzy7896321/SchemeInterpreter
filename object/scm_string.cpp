#include "scm_string.h"
#include <cstddef>
#include <cassert>

namespace ObjectDef{

const std::string escapingChar("abfnrtv\\\"\'");
const std::string replacementChar("\a\b\f\n\r\t\v\\\"\'");

std::string String::_interp(const std::string& val){
    return std::move(_interp(std::string(val)));
}

std::string String::_interp(std::string&& val){
	std::string interp_val;
	unsigned i = 1, target = val.size() - 1;
	for (; i < target; ++i){
		if (val[i] == '\\'){
			if (++i == target) break;
			if (val[i] == '0'){
				interp_val.push_back('\0');
            }
            else{
                unsigned tmp = escapingChar.find(val[i]);
                if (tmp == std::string::npos){
                    interp_val.push_back(val[i]);
                } else {
                    interp_val.push_back(replacementChar[tmp]);
                }
            }
		}
		else
			interp_val.push_back(val[i]);
	}
	return std::move(interp_val);
}

std::string String::getExternalRep() const{
    std::string ret;
    ret.push_back('\"');
    for (unsigned i = 0; i < val.size(); ++i){
        if (val[i] == '\0'){
            ret.push_back('\\');
            ret.push_back('0');
        } else {
            unsigned tmp = replacementChar.find(val[i]);
            if (tmp == std::string::npos){
                ret.push_back(val[i]);
            }
            else {
                ret.push_back('\\');
                ret.push_back(escapingChar[tmp]);
            }
        }
    }
    ret.push_back('\"');
    return ret;
}

}
