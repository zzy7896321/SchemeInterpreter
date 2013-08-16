#include "prompt_declaration.h"

#include <string>
#include <cstdio>
#include <utility>

#include "../parser/parser.h"
#include "../parser/parsetree.h"
#include "../object/objectdef.h"
#include "../interp/evaluation.h"

const std::string blankAndComment(" \n\r\t\f\v;");

int read_eval_loop(std::istream& input, std::ostream& output){
	auto top_level_env = ObjectDef::Environment::getTopLevelEnv();

	ParserEnv::StreamReader reader(input);

	while (input){
        auto parseResult = ParserEnv::Parser::parseOneFromStreamReader(reader);

        #ifdef _SCMINTERP_DEBUG_PRINT_
            std::cerr << __FILE__ << ':' << __LINE__ << ' ' << __func__ << std::endl;
            printParseTree(parseResult);
        #endif

        int returnCode = ParserEnv::Parser::getLastReturnCode();
        if ( returnCode != ParserEnv::PARSE_NORMAL_EXIT ){
            output << " ==> Parsing error: " << ParserEnv::parsingError[returnCode]
                    << " (at input:" << ParserEnv::Parser::getLastErrorPos() << ')' << std::endl;
            continue;
        }

        if (!parseResult){
            if (input) output << " ==> Error: empty input" << std::endl;
            continue;
        }

        std::shared_ptr<ObjectDef::Object> ret;
        auto callRes = Interp::evaluateParseTree(top_level_env, parseResult, ret);

        if (callRes.first != Interp::EVAL_NO_ERROR){
            output << " ==> Evaluation error: " << Interp::errorInfo[callRes.first] << std::endl;
            output << " ==> " << callRes.second << std::endl;
            continue;
        }

        if (ret->getType() != ObjectDef::ObjectType::VOID)
            output << " ==> " << ret->getExternalRep() << std::endl;
        else
            output << " ==> <VOID>" << std::endl;
        #ifdef _SCMINTERP_DEBUG_PRINT_
            output << "[debug_print] "; printType(ret->getType(), output);
            output << std::endl;
        #endif
	}

    return 0;
}

