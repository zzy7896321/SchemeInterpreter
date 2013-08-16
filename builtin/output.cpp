/** @file
 *  Section 6.3.3 Output
 */
#include "builtinproc.h"
#include "../object/numberop.h"

namespace BuiltinProc{

callResultType display_func(parameterType param, sptrObject& obj){
    if (param[0]->getType() == ObjectDef::ObjectType::STRING){
        iostreamContainer->output << static_cast<ObjectDef::String*>(param[0].get())->getVal();
    }

    else if (param[0]->getType() == ObjectDef::ObjectType::CHAR){
        iostreamContainer->output << static_cast<ObjectDef::Char*>(param[0].get())->getVal();
    }

    else if (ObjectDef::isNumber(param[0])){
        std::string&& tmp = param[0]->getExternalRep();
        if (tmp[0] == '#')
            iostreamContainer->output << tmp.substr(2, std::string::npos);
        else
            iostreamContainer->output << tmp;
    }
    else{
        iostreamContainer->output << param[0]->getExternalRep();
    }

    obj = ObjectDef::Void::getVoid();
    return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

callResultType newline_func(parameterType param, sptrObject& obj){
    iostreamContainer->output << std::endl;

    obj = ObjectDef::Void::getVoid();
    return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}
