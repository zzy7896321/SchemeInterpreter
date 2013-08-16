/** @file
 *  Section 6.6.4 System interface
 */

#include "builtinproc.h"
#include "../prompt/prompt_declaration.h"

#include <fstream>

namespace BuiltinProc{

callResultType load_func(parameterType param, sptrObject& obj){

    std::ifstream input(static_cast<ObjectDef::String*>(param[0].get())->getVal());
    if (!input.is_open()){
        return std::make_pair(Interp::EVAL_CANNOT_OPEN_FILE, "load.");
    }

    read_eval_loop(input, std::cout);

    obj = ObjectDef::Void::getVoid();
	return std::make_pair(Interp::EVAL_NO_ERROR, "");
}

}
