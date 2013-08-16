#include "evaluation.h"

    #include <iostream>
    #include "../prompt/prompt_declaration.h"

namespace Interp{

callResultType evaluateParseTree(sptrEnvironment env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion){
	assert(root);

	#ifdef _SCMINTERP_DEBUG_PRINT_
        std::cerr << std::endl;
        std::cerr << __FILE__ << ':' << __LINE__ << ' ' << __func__ << std::endl;
        printParseTree(root);
	#endif

	static SyntaxMapType& syntaxMap = getSyntaxMap();

	std::stack<sptrParseTree> father;
	std::vector<sptrObject> eval_res;
	auto cur = root;
	while (cur || !father.empty()){
		if (cur){
			switch (cur->obj->getType()){
				case ObjectDef::ObjectType::LIST:
				{
                    if (father.empty() && !eval_res.empty()){
                        eval_res.back() = cur->obj;
                    } else {
                        eval_res.push_back(cur->obj);
                    }
					father.push(cur);
					cur = cur->child;
				}
					break;

				case ObjectDef::ObjectType::INT:
				case ObjectDef::ObjectType::BIGINT:
				case ObjectDef::ObjectType::RATIONAL:
				case ObjectDef::ObjectType::REAL:
				case ObjectDef::ObjectType::COMPLEX:
				case ObjectDef::ObjectType::BOOLEAN:
				case ObjectDef::ObjectType::PAIR:
				case ObjectDef::ObjectType::CHAR:
				case ObjectDef::ObjectType::STRING:
				case ObjectDef::ObjectType::PROC:
				case ObjectDef::ObjectType::BUILTIN:
				case ObjectDef::ObjectType::PROMISE:
				//case ObjectDef::ObjectType::VECTOR:
				{
                    if (father.empty() && !eval_res.empty()){
                        eval_res.back() = cur->obj;
                    }
					else {
                        eval_res.push_back(cur->obj);
					}
					cur = cur->next;
				}
					break;

				case ObjectDef::ObjectType::SYMBOL:
				{
					//syntax checking is prior to identifier cheching
					auto iter_syntax = syntaxMap.find(static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
					if (iter_syntax != syntaxMap.end()){
						if (father.empty() || eval_res.back() != father.top()->obj){
							return std::make_pair(EVAL_BAD_SYNTAX, "unexpected syntatic_keyword");
						}

						sptrObject ret;
						callResultType callRes;
						if (enableTailRecursion && father.size() == 1 && !father.top()->next){
                            //allow syntax call to dump bindings into current environment
                            callRes = (iter_syntax->second)(env, cur, ret, true);
                        }
                        else {
                            callRes = (iter_syntax->second)(env, cur, ret, false);
                        }

						if (callRes.first == EVAL_NO_ERROR){
							eval_res.back() = ret;
							cur = father.top()->next;
							father.pop();
						}
						else if (callRes.first == EVAL_RAW_EXPR_RETURNED){
							//replace the list with the expression returned
							cur = static_cast<ObjectDef::ExpressionPack*>(ret.get())->proc;

                        #ifdef _SCMINTERP_DEBUG_PRINT_
							printParseTree(cur);
                        #endif

							auto pack_end = cur;
							while (pack_end->next){
                                pack_end = pack_end->next;
							}
							pack_end->next = father.top() -> next;
							father.pop();
							eval_res.pop_back();
						}
						else {
							return std::move(callRes);
						}
					}

					//identifier lookup
					else{
						auto ret = env->lookup(static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
						if (ret){
                            if (father.empty() && !eval_res.empty()){
                                assert(eval_res.size() == 1);
                                eval_res.back() = ret;
                            }
                            else {
                                eval_res.push_back(ret);
                            }
							cur = cur->next;
						}

						else {
							return std::make_pair(EVAL_UNDEFINED_SYMBOL, static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
						}
					}

				}
					break;
                case ObjectDef::ObjectType::PAIRDOT:
                    return std::make_pair(EVAL_BAD_SYNTAX, "Invalid use of '.'");
                    break;
				default:
				{
                    assert(false);
					return std::make_pair(EVAL_UNEXPECTED_SITUATION, cur->obj->getExternalRep());
				}
			}
		}

		else {
			cur = father.top();
			father.pop();

			auto begin_of_list = eval_res.end();
			for (; *(--begin_of_list) != cur->obj;);

			if (eval_res.end() - begin_of_list <= 1){
				return std::make_pair(EVAL_MISSING_PROCEDURE, "");
			}

			//tail recursion
			if (enableTailRecursion && father.empty() && !cur->next && (*(begin_of_list+1))->getType() == ObjectDef::ObjectType::PROC){
				assert(begin_of_list == eval_res.begin());
				assert(env != ObjectDef::Environment::getTopLevelEnv());

				auto proc = eval_res[1];
				eval_res.erase(eval_res.begin(), eval_res.begin() + 2);

				auto callRes = static_cast<ObjectDef::Procedure*>(proc.get())->applyFormal(env, eval_res);
				if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

				eval_res.clear();
				cur = static_cast<ObjectDef::Procedure*>(proc.get())->proc;
			}

			else {
				if ((*(begin_of_list+1))->getType() == ObjectDef::ObjectType::PROC){
					std::vector<sptrObject> param(begin_of_list+2, eval_res.end());
					auto proc = *(begin_of_list+1);
					if (father.empty()){
						assert(begin_of_list == eval_res.begin());
						eval_res.clear();
					}
					else{
						eval_res.erase(begin_of_list, eval_res.end());
					}

					sptrObject ret;
					auto callRes = static_cast<ObjectDef::Procedure*>(proc.get())->invoke(param, ret);
					if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

					eval_res.push_back(ret);
					cur = cur->next;
				}
				else if ((*(begin_of_list+1))->getType() == ObjectDef::ObjectType::BUILTIN){
					std::vector<sptrObject> param(begin_of_list+2, eval_res.end());
					auto proc = *(begin_of_list+1);
					if (father.empty()){
						assert(begin_of_list == eval_res.begin());
						eval_res.clear();
					}
					else {
						eval_res.erase(begin_of_list, eval_res.end());
					}

					sptrObject ret;
					auto callRes = static_cast<ObjectDef::Builtin*>(proc.get())->invoke(param, ret);
					if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

					eval_res.push_back(ret);
					cur = cur->next;
				}
				else{
					return std::make_pair(EVAL_NON_PROCEDURE, "");
				}
			}

		}
	}

	assert(eval_res.size() <= 1);
	if (eval_res.size())
        obj = eval_res[0];
	else
        obj = ObjectDef::Void::getVoid();

	return std::make_pair(EVAL_NO_ERROR, "");
}

}

