#include "syntax.h"
#include "evaluation.h"
#include "../config.h"
#include "../prompt/prompt_declaration.h"

#include <algorithm>

namespace Interp{

callResultType quote_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword = "quote";
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);
	if (! root->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "(quote) too few argument");
	}

	if (root->next->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "(quote) too many argument");
	}

	std::stack<sptrParseTree> father;
	std::vector<sptrObject> eval_res;
	auto cur = root->next;
	while (cur || !father.empty()){
		if (cur){
			switch (cur->obj->getType()){
				case ObjectDef::ObjectType::LIST:
				{
					father.push(cur);
					eval_res.push_back(cur->obj);
					cur = cur->child;
				}
				break;
				case ObjectDef::ObjectType::BOOLEAN:
				case ObjectDef::ObjectType::INT:
				case ObjectDef::ObjectType::BIGINT:
				case ObjectDef::ObjectType::RATIONAL:
				case ObjectDef::ObjectType::REAL:
				//case ObjectDef::ObjectType::COMPLEX:
				case ObjectDef::ObjectType::CHAR:
				case ObjectDef::ObjectType::STRING:
				case ObjectDef::ObjectType::SYMBOL:
				case ObjectDef::ObjectType::PAIRDOT:
				case ObjectDef::ObjectType::VOID:
				{
					eval_res.push_back(cur->obj);
					cur = cur->next;
				}
					break;

				default:
				{
                    assert(false);
					return std::make_pair(EVAL_UNEXPECTED_SITUATION, "");
				}
			}
		}
		else {
			cur = father.top();
			father.pop();
			auto begin_of_list = eval_res.end();
			for ( --begin_of_list; *begin_of_list != cur->obj; --begin_of_list);

			sptrObject tail;
			auto end_of_list = eval_res.end() - 1;
			if ( end_of_list - begin_of_list >= 3 &&
				 (*(end_of_list))->getType() != ObjectDef::ObjectType::PAIRDOT &&
				 (*(end_of_list - 2))->getType() != ObjectDef::ObjectType::PAIRDOT &&
				 (*(end_of_list - 1))->getType() == ObjectDef::ObjectType::PAIRDOT){

				tail = sptrObject(new ObjectDef::Pair(*(eval_res.end() - 3), *(eval_res.end() - 1)));
				end_of_list -= 3;
			} else {
				tail = ObjectDef::EmptyList::getEmptyList();
			}

			for (; end_of_list > begin_of_list; --end_of_list){
				if ( (*end_of_list) ->getType() == ObjectDef::ObjectType::PAIRDOT){
					return std::make_pair(EVAL_BAD_SYNTAX, "unexpected .");
				}
				tail = sptrObject(new ObjectDef::Pair(*end_of_list, tail));
			}

			eval_res.erase(begin_of_list, eval_res.end());
			eval_res.push_back(tail);

			cur = cur->next;
		}
	}

	obj = eval_res.front();
	return std::make_pair(EVAL_NO_ERROR, "");
}

callResultType lambda_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword = "lambda";
	assert( static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	//get formals
	root = root->next;

	std::vector<std::string> formal;
	bool enableList = false;

	// <variable>
	if (root->obj->getType() == ObjectDef::ObjectType::SYMBOL){
		enableList = true;
		formal.push_back( static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal());
	}

	// (<variable 1> ... <variable n> [ . <variable n+1>])
	else if (root->obj->getType() == ObjectDef::ObjectType::LIST){
		auto cur = root->child;
		if ( cur && cur->obj->getType() == ObjectDef::ObjectType::PAIRDOT){
			return std::make_pair(EVAL_BAD_SYNTAX, " expect at least one variable before \".\"");
		}

		while ( cur && cur->obj->getType() == ObjectDef::ObjectType::SYMBOL){
			if (std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal()) != formal.end()){
				return std::make_pair(EVAL_DUPLICATE_ARGUMENT_NAME, static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
			}
			formal.push_back( static_cast<ObjectDef::Symbol*>(cur->obj.get()) -> getVal());
			cur = cur->next;
		}

		if (cur){
			if (cur->obj->getType() == ObjectDef::ObjectType::PAIRDOT
				&& cur->next && cur->next->obj->getType() == ObjectDef::ObjectType::SYMBOL
				&& ! cur->next->next){
				cur = cur->next;
				if (std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal()) != formal.end()){
					return std::make_pair(EVAL_DUPLICATE_ARGUMENT_NAME, static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
				}
				enableList = true;
				formal.push_back( static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
			} else {
				return std::make_pair(EVAL_BAD_SYNTAX, "invalid form of formals");
			}
		}
	}

	//bad syntax
	else{
		return std::make_pair(EVAL_BAD_SYNTAX, " invalid form of formals");
	}

    //check for the existence of body
    if (!root->next){
        return std::make_pair(EVAL_BAD_SYNTAX, " no body");
    }

	obj = sptrObject(new ObjectDef::Procedure(env, std::move(formal), enableList, root->next));
	return std::make_pair(EVAL_NO_ERROR, "");
}

callResultType if_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword = "if";
	assert( static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	//syntax check
	if ( !root->next || !root->next->next || (root->next->next->next && root->next->next->next->next)){
		return std::make_pair(EVAL_BAD_SYNTAX, "invalid if syntax");
	}

	auto condition = sptrParseTree(new ParserEnv::ParseTree(root->next->child, nullptr, root->next->obj));
	//eval condition
	sptrObject ret;
	auto callRes = evaluateParseTree(env, condition, ret);
	if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

	//return raw expression for tail recursion
	if (ret == ObjectDef::getBoolean(false)){
		if (root->next->next->next){
            root = root->next->next->next;
			obj = sptrObject(new ObjectDef::ExpressionPack(sptrParseTree(new ParserEnv::ParseTree(root->child, nullptr, root->obj))));
			return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
		} else {
			obj = ObjectDef::Void::getVoid();
			return std::make_pair(EVAL_NO_ERROR, "");
		}
	} else {
        root = root->next->next;
		obj = sptrObject(new ObjectDef::ExpressionPack(sptrParseTree(new ParserEnv::ParseTree(root->child, nullptr, root->obj))));
		return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
	}
}

callResultType set_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword = "set!";
	assert( static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	static SyntaxMapType& syntaxMap = getSyntaxMap();
	static sptrEnvironment builtinEnv = ObjectDef::Environment::getBuiltinEnv();

	//check syntax
	if ( !root->next || root->next->obj->getType() != ObjectDef::ObjectType::SYMBOL){
		return std::make_pair(EVAL_BAD_SYNTAX, "set! expects a variable identifier");
	}

	if ( !root->next->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "set! expects one expression");
	}

	if ( root->next->next->next){
        return std::make_pair(EVAL_BAD_SYNTAX, "set! expects only one expression");
	}

	if (syntaxMap.find(static_cast<ObjectDef::Symbol*>(root->next->obj.get())->getVal()) != syntaxMap.end()){
        return std::make_pair(EVAL_BAD_SYNTAX, "syntatic keyword is not a valid variable identifier");
	}

	obj = ObjectDef::Void::getVoid();
	if (_scm_protect_builtin){  // protect objects in builtin environment from being modified
        // User can never evaluate expression directly in builtin environment
        // When lookupResult.second == builtinEnv.get(), lookup.first is definitely not end of map
        auto lookupResult = env->lookup_with_env(static_cast<ObjectDef::Symbol*>(root->next->obj.get())->getVal());
        if (lookupResult.second == builtinEnv.get()){
            return std::make_pair(EVAL_CONTRACTION_VIOLATION, "set!. Builtin symbols are protected. (Disable the feature by -np option)");
        }

        sptrObject ret;
        auto callRes = evaluateParseTree(env, root->next->next, ret);
        if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

        if (env->modify(lookupResult, ret)){
            return std::make_pair(EVAL_NO_ERROR, "");
        }
        else {
            return std::make_pair(EVAL_UNDEFINED_SYMBOL, "can not apply set! to undefined variable");
        }
	}
    else {
        sptrObject ret;
        auto callRes = evaluateParseTree(env, root->next->next, ret);
        if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

        if (env->modify(static_cast<ObjectDef::Symbol*>(root->next->obj.get())->getVal(), ret)){
            return std::make_pair(EVAL_NO_ERROR, "");
        } else {
            return std::make_pair(EVAL_UNDEFINED_SYMBOL, "can not apply set! to undefined variable");
        }
    }
}

callResultType quasiquote_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword = "quasiquote";
	assert( static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (! root->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "(quasiquote) too few argument");
	}

	if (root->next->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "(quasiquote) too many argument");
	}

	std::stack<sptrParseTree> father;
	std::vector<sptrObject> eval_res;
	auto cur = root->next;
	int nest_level = 0;
	while (cur || !father.empty()){
		if (cur){
			switch (cur->obj->getType()){
				case ObjectDef::ObjectType::LIST:
				{
					father.push(cur);
					eval_res.push_back(cur->obj);
					cur = cur->child;
				}
					break;

                case ObjectDef::ObjectType::BOOLEAN:
				case ObjectDef::ObjectType::INT:
				case ObjectDef::ObjectType::BIGINT:
				case ObjectDef::ObjectType::RATIONAL:
				case ObjectDef::ObjectType::REAL:
				//case ObjectDef::ObjectType::COMPLEX:
				case ObjectDef::ObjectType::CHAR:
				case ObjectDef::ObjectType::STRING:
				case ObjectDef::ObjectType::PAIRDOT:
				case ObjectDef::ObjectType::VOID:
				{
					eval_res.push_back(cur->obj);
					cur = cur->next;
				}
					break;

				case ObjectDef::ObjectType::SYMBOL:
				{
					if (!father.empty() && eval_res.back() == father.top()->obj){
						if (static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal() == "unquote"){
                            if (nest_level--){
                                eval_res.push_back(cur->obj);
                                cur = cur->next;
                            } else {
                                if (!cur->next){
                                    return std::make_pair(EVAL_BAD_SYNTAX, "(unquote) too few argument");
                                }
                                if (cur->next->next){
                                    return std::make_pair(EVAL_BAD_SYNTAX, "(unquote) too many arguments");
                                }

                                if (father.empty() || eval_res.back() != father.top()->obj){
                                    return std::make_pair(EVAL_BAD_SYNTAX, "unexpected syntatic keyword");
                                }

                                sptrObject ret;
                                auto callRes = evaluateParseTree(env, cur->next, ret);
                                if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

                                eval_res.back() = ret;
                                cur = father.top()->next;
                                father.pop();

                                ++nest_level;
                            }
						}
						else if (static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal() == "unquote-splicing"){
                            if (nest_level--){
                                eval_res.push_back(cur->obj);
                                cur = cur->next;
                            } else {
                                if (!cur->next){
                                    return std::make_pair(EVAL_BAD_SYNTAX, "(unquote-splicing) too few argument");
                                }
                                if (cur->next->next){
                                    return std::make_pair(EVAL_BAD_SYNTAX, "(unquote-splicing) too many arguments");
                                }

                                if (father.empty() || eval_res.back() != father.top()->obj){
                                    return std::make_pair(EVAL_BAD_SYNTAX, "unexpected syntatic keyword");
                                }

                                sptrObject ret;
                                auto callRes = evaluateParseTree(env, cur->next, ret);
                                if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

                                eval_res.pop_back();

                                while (ret->getType() == ObjectDef::ObjectType::PAIR){
                                    eval_res.push_back(static_cast<ObjectDef::Pair*>(ret.get())->getCar());
                                    ret = static_cast<ObjectDef::Pair*>(ret.get())->getCdr();
                                }

                                if (ret->getType() != ObjectDef::ObjectType::EMPTYLIST){
                                    return std::make_pair(EVAL_PARAMETER_TYPE_MISMATCH, "unquote-splicing expect a list");
                                }

                                cur = father.top()->next;
                                father.pop();

                                ++nest_level;
                            }
						}
						else if (static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal() == "quasiquote"){
                            ++nest_level;
                            eval_res.push_back(cur->obj);
                            cur = cur->next;
						}
						else {
							eval_res.push_back(cur->obj);
							cur = cur->next;
						}
					}
					else {
						eval_res.push_back(cur->obj);
						cur = cur->next;
					}
				}
					break;

				default:
				{
                    assert(false);
					return std::make_pair(EVAL_UNEXPECTED_SITUATION, "");
				}
			}
		}
		else {
			cur = father.top();
			father.pop();
			auto begin_of_list = eval_res.end();
			for ( --begin_of_list; *begin_of_list != cur->obj; --begin_of_list);

            if (begin_of_list + 1 != eval_res.end() && (*(begin_of_list+1))->getType() == ObjectDef::ObjectType::SYMBOL){
                if (static_cast<ObjectDef::Symbol*>((begin_of_list+1)->get())->getVal() == "unquote"){
                    ++nest_level;
                }
                else if (static_cast<ObjectDef::Symbol*>((begin_of_list+1)->get())->getVal() == "unquote-splicing"){
                    ++nest_level;
                }
                else if (static_cast<ObjectDef::Symbol*>((begin_of_list+1)->get())->getVal() == "quasiquote"){
                    --nest_level;
                }
            }

			sptrObject tail;
			auto end_of_list = eval_res.end() - 1;
			if ( end_of_list - begin_of_list >= 3 &&
				 (*(end_of_list))->getType() != ObjectDef::ObjectType::PAIRDOT &&
				 (*(end_of_list - 2))->getType() != ObjectDef::ObjectType::PAIRDOT &&
				 (*(end_of_list - 1))->getType() == ObjectDef::ObjectType::PAIRDOT){

				tail = sptrObject(new ObjectDef::Pair(*(eval_res.end() - 3), *(eval_res.end() - 1)));
				end_of_list -= 3;
			} else {
				tail = ObjectDef::EmptyList::getEmptyList();
			}

			for (; end_of_list > begin_of_list; --end_of_list){
				if ( (*end_of_list) ->getType() == ObjectDef::ObjectType::PAIRDOT){
					return std::make_pair(EVAL_BAD_SYNTAX, "unexpected .");
				}
				tail = sptrObject(new ObjectDef::Pair(*end_of_list, tail));
			}

			eval_res.erase(begin_of_list, eval_res.end());
			eval_res.push_back(tail);

			cur = cur->next;
		}
	}

	obj = eval_res.front();
	return std::make_pair(EVAL_NO_ERROR, "");

}

callResultType unquote_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	return std::make_pair(EVAL_BAD_SYNTAX, "unquote: not in quasiquote");
}

callResultType unquote_splicing_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	return std::make_pair(EVAL_BAD_SYNTAX, "unquote-splicing: not in quasiquote");
}

callResultType define_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword("define");
	assert( static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

    static SyntaxMapType& syntaxMap = getSyntaxMap();
    static sptrEnvironment topLevelEnv = ObjectDef::Environment::getTopLevelEnv();
    static sptrEnvironment builtinEnv = ObjectDef::Environment::getBuiltinEnv();

	// get variable
	root = root->next;
	if (!root){
		return std::make_pair(EVAL_BAD_SYNTAX, "define expects <variable> identifier");
	}

	// variable definition
	if (root->obj->getType() == ObjectDef::ObjectType::SYMBOL){
		if (!root->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "define expects one expression");
		}

		if (root->next->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "define expects only one expression");
		}

		if (syntaxMap.find(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal()) != syntaxMap.end()){
            return std::make_pair(EVAL_BAD_SYNTAX, "define. syntatic keyword is not a valid variable identifier");
		}

		if (_scm_protect_builtin){
            if (env == topLevelEnv &&
                builtinEnv->lookup(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal())){
                return std::make_pair(EVAL_CONTRACTION_VIOLATION, "define.  Builtin symbols are protected. (Disable the feature by -np option)");
            }
		}

		sptrObject ret;
		auto callRes = evaluateParseTree(env, root->next, ret);
		if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

		env->insert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal(), ret);

		obj = ObjectDef::Void::getVoid();
		return std::make_pair(EVAL_NO_ERROR, "");
	}

	// named lambda definition
	else if (root->obj->getType() == ObjectDef::ObjectType::LIST){
		auto cur = root->child;
		if (cur->obj->getType() != ObjectDef::ObjectType::SYMBOL){
			return std::make_pair(EVAL_BAD_SYNTAX, "define requires <variable>");
		}

		if (syntaxMap.find(static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal())!= syntaxMap.end()){
            return std::make_pair(EVAL_BAD_SYNTAX, "define. syntatic keyword is not a valid variable identifier");
		}

		auto variable_identifier = cur->obj;

		cur = cur->next;
		std::vector<std::string> formal;
		bool enableList = false;
		while ( cur && cur->obj->getType() == ObjectDef::ObjectType::SYMBOL){
			if (std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal()) != formal.end()){
				return std::make_pair(EVAL_DUPLICATE_ARGUMENT_NAME, static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
			}
			formal.push_back( static_cast<ObjectDef::Symbol*>(cur->obj.get()) -> getVal());
			cur = cur->next;
		}

		if (cur){
			if (cur->obj->getType() == ObjectDef::ObjectType::PAIRDOT
				&& cur->next && cur->next->obj->getType() == ObjectDef::ObjectType::SYMBOL
				&& ! cur->next->next){
				cur = cur->next;
				if (std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal()) != formal.end()){
					return std::make_pair(EVAL_DUPLICATE_ARGUMENT_NAME, static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
				}
				enableList = true;
				formal.push_back( static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal());
			} else {
				return std::make_pair(EVAL_BAD_SYNTAX, "invalid form of formals");
			}
		}

		if (!root->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "define of a lambda requires a body");
		}

		if (_scm_protect_builtin){
            if (env == topLevelEnv &&
                builtinEnv->lookup(static_cast<ObjectDef::Symbol*>(variable_identifier.get())->getVal())){
                return std::make_pair(EVAL_CONTRACTION_VIOLATION, "define.  Builtin symbols are protected. (Disable the feature by -np option)");
            }
		}

		env->insert(static_cast<ObjectDef::Symbol*>(variable_identifier.get()) -> getVal(),
                    sptrObject(new ObjectDef::Procedure(env, std::move(formal), enableList, root->next)));
        obj = ObjectDef::Void::getVoid();
		return std::make_pair(EVAL_NO_ERROR, "");
	}

	// bad syntax
	else {
		return std::make_pair(EVAL_BAD_SYNTAX, "invalid form of <variable>");
	}
}

callResultType cond_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword("cond");
	assert( static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	root = root->next;
	while (root){
		if (root->obj->getType() != ObjectDef::ObjectType::LIST){
			return std::make_pair(EVAL_BAD_SYNTAX, "cond expects clauses");
		}

		auto cur = root->child;
		if (!cur){
			return std::make_pair(EVAL_BAD_SYNTAX, "Ooops, nothing in a clause of cond");
		}

		// (else <expression 1> <expression 2> ...)
		if (cur->obj->getType() == ObjectDef::ObjectType::SYMBOL && static_cast<ObjectDef::Symbol*>(cur->obj.get())->getVal() == "else"){
			cur = cur->next;
			if (!cur){
				return std::make_pair(EVAL_BAD_SYNTAX, "else clause expects at least one expression");
			}

			obj = sptrObject(new ObjectDef::ExpressionPack(cur));
			return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");

		}

		//(<test> => <expressoin>)
		else if (cur->next && cur->next->obj->getType() == ObjectDef::ObjectType::SYMBOL &&
			 static_cast<ObjectDef::Symbol*>(cur->next->obj.get())->getVal() == "=>"){
				if (!cur->next->next){
					return std::make_pair(EVAL_BAD_SYNTAX, "(<test> => <expression>) clause missing <expression>");
				}
				if (cur->next->next->next){
					return std::make_pair(EVAL_BAD_SYNTAX, "(<test> => <expression>) clause contains multiple expressions");
				}

				//evaluate condition
				auto condition = sptrParseTree(new ParserEnv::ParseTree(cur->child, nullptr, cur->obj));
				sptrObject ret;
				auto callRes = evaluateParseTree(env, condition, ret);
				if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);


				if (ret != ObjectDef::getBoolean(false)){
					//evaluate expression
					auto expression = cur->next->next;
					sptrObject ret2;
					auto callRes2 = evaluateParseTree(env, expression, ret2);
					if (callRes2.first != EVAL_NO_ERROR) return std::move(callRes);

					//repack expression
					obj = sptrObject(new ObjectDef::ExpressionPack(
                          sptrParseTree(new ParserEnv::ParseTree(
                                        sptrParseTree(new ParserEnv::ParseTree(
                                                      nullptr,
                                                      sptrParseTree(new ParserEnv::ParseTree(nullptr, nullptr, ret)),
                                                      ret2))
                                        , nullptr, ObjectDef::getDummyList()))));
                    return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
				}
		}

		// (<test> <expression 1> ...)
		else{
			//evaluate condition
			auto condition = sptrParseTree(new ParserEnv::ParseTree(cur->child, nullptr, cur->obj));
			sptrObject ret;
			auto callRes = evaluateParseTree(env, condition, ret);
			if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

			if (ret != ObjectDef::getBoolean(false)){
				if (! cur->next){
					obj = ret;
					return std::make_pair(EVAL_NO_ERROR, "");
				}
				else {
					obj = sptrObject(new ObjectDef::ExpressionPack(cur->next));
					return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
				}
			}
		}

		root = root->next;
	}

	obj = ObjectDef::Void::getVoid();
	return std::make_pair(EVAL_NO_ERROR, "");
}

callResultType case_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword("case");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (! root->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "case syntax expect a <key>");
	}

	root = root->next;
	//evaluate key
	std::vector<sptrObject> param;
	param.push_back(nullptr);
	auto keyParseTree = sptrParseTree(new ParserEnv::ParseTree(root->child, nullptr, root->obj));
	auto callRes = evaluateParseTree(env, keyParseTree, param[0]);
	if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

	param.push_back(nullptr);
	auto eqvProc = static_cast<ObjectDef::Builtin*>(ObjectDef::Environment::getBuiltinEnv()->lookup("eqv?").get());
	auto trueValue = ObjectDef::getBoolean(true);
	sptrObject ret;
	while ( (root = root->next) ){
		if (!root->child){
			return std::make_pair(EVAL_BAD_SYNTAX, "case clause expects the first element to be a list of data or an else");
		}

		if (!root->child->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "case clause expects expressions after a list of data");
		}

		//((<data>) <expressions>)
		if (root->child->obj->getType() == ObjectDef::ObjectType::LIST){
            auto dataList = root->child->child;
            while (dataList){
                param[1] = dataList->obj;
                auto callRes = eqvProc->invoke(param, ret);
                if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);
                if (ret == trueValue){
                    obj = sptrObject(new ObjectDef::ExpressionPack(root->child->next));
                    return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
                }
                dataList = dataList->next;
            }
        }

        //(else <expressions>)
        else if (root->child->obj->getType() == ObjectDef::ObjectType::SYMBOL &&
                 static_cast<ObjectDef::Symbol*>(root->child->obj.get())->getVal() == "else"){

            obj = sptrObject(new ObjectDef::ExpressionPack(root->child->next));
            return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
        }

        //bad syntax
        else{
            return std::make_pair(EVAL_BAD_SYNTAX, "case clause expects the first element to be a list of data or an else");
        }
    }

	obj = ObjectDef::Void::getVoid();
	return std::make_pair(EVAL_NO_ERROR, "");
}

callResultType and_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword("and");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (! root->next){
		obj = ObjectDef::getBoolean(true);
		return std::make_pair(EVAL_NO_ERROR, "");
	}

	while ((root = root->next)->next){
		auto expression = sptrParseTree(new ParserEnv::ParseTree(root->child, nullptr, root->obj));
		sptrObject ret;
		auto callRes = evaluateParseTree(env, expression, ret);
		if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

		if (ret == ObjectDef::getBoolean(false)){
			obj = ret;
			return std::make_pair(EVAL_NO_ERROR, "");
		}
	}

	assert(root && !root->next);
	obj = sptrObject(new ObjectDef::ExpressionPack(root));
	return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
}


callResultType or_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword("or");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (! root->next){
		obj = ObjectDef::getBoolean(false);
		return std::make_pair(EVAL_NO_ERROR, "");
	}

	while ((root = root->next)->next){
		auto expression = sptrParseTree(new ParserEnv::ParseTree(root->child, nullptr, root->obj));
		sptrObject ret;
		auto callRes = evaluateParseTree(env, expression, ret);
		if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

		if (ret != ObjectDef::getBoolean(false)){
			obj = ret;
			return std::make_pair(EVAL_NO_ERROR, "");
		}
	}

	assert(root && !root->next);
	obj = sptrObject(new ObjectDef::ExpressionPack(root));
	return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
}

callResultType let_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion){
	static const std::string syntatic_keyword("let");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

    SyntaxMapType& syntaxMap = getSyntaxMap();

	if (!root->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "missing <bindings> in let or <variable> in named-let");
	}

	root = root->next;
	sptrObject symb;
	//named-let
	if (root->obj->getType() == ObjectDef::ObjectType::SYMBOL){
		symb = root->obj;
		if (syntaxMap.find(static_cast<ObjectDef::Symbol*>(symb.get())->getVal()) != syntaxMap.end()){
            return std::make_pair(EVAL_BAD_SYNTAX, "let. syntatic keyword is not a valid variable identfier");
		}
		root = root->next;
	}

	if (!root || root->obj->getType() != ObjectDef::ObjectType::LIST){
		return std::make_pair(EVAL_BAD_SYNTAX, "missing <bindings> in let or named-let");
	}

	if (!root->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "missing <body> in let or named-let");
	}

	//evaluate bindings
	std::vector<std::string> formal;
	std::vector<sptrObject> eval_res;
	auto bindingList = root->child;
	while (bindingList){
		if (bindingList->obj->getType() != ObjectDef::ObjectType::LIST){
			return std::make_pair(EVAL_BAD_SYNTAX, "Ooops, the binding is not a pair");
		}

		auto item = bindingList->child;
		if (!item || item->obj->getType() != ObjectDef::ObjectType::SYMBOL){
			return std::make_pair(EVAL_BAD_SYNTAX, "binding should begin with a variable");
		}
		if (!item->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "missing init in a binding");
		}
		if (item->next->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "too many expressions in a binding");
		}

		if (std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(item->obj.get())->getVal()) != formal.end()){
			return std::make_pair(EVAL_BAD_SYNTAX, "duplicate bindings of variable");
		}
		formal.push_back(static_cast<ObjectDef::Symbol*>(item->obj.get())->getVal());

		sptrObject ret;
		auto callRes = evaluateParseTree(env, item->next, ret);
		if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);

		eval_res.push_back(ret);

		bindingList = bindingList->next;
	}

	if (symb && std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(symb.get())->getVal()) != formal.end()){
		return std::make_pair(EVAL_BAD_SYNTAX, "identifier of variable and loop procedure conflicts");
	}

	assert(formal.size() == eval_res.size());
	sptrEnvironment curEnv(new ObjectDef::Environment(env));

    for (unsigned i = 0; i<formal.size(); ++i)
        curEnv->insert(formal[i], eval_res[i]);

	//named-let
	if (symb){
		sptrObject proc_to_call(new ObjectDef::Procedure(curEnv, std::move(formal), false, root->next));
		curEnv->insert(static_cast<ObjectDef::Symbol*>(symb.get())->getVal(), proc_to_call);
	}

    if (enableTailRecursion){
        obj = sptrObject(new ObjectDef::ExpressionPack(root->next));
        env = curEnv;
        return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
    }
    else {
        return evaluateParseTree(curEnv, root->next, obj, true);
    }

    assert(false);
    return std::make_pair(EVAL_UNEXPECTED_SITUATION, "should never reach here!");
}

callResultType letstar_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion){
	static const std::string syntatic_keyword("let*");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (!root->next || root->next->obj->getType() != ObjectDef::ObjectType::LIST){
		return std::make_pair(EVAL_BAD_SYNTAX, "missing <bindings> in let*");
	}

	if (!root->next->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "missing <body> in let*");
	}

	root = root->next;
	//binding variables
	sptrEnvironment curEnv(new ObjectDef::Environment(env));
	std::vector<std::string> formal;
	auto bindingList = root->child;
	while (bindingList){
		if (bindingList->obj->getType() != ObjectDef::ObjectType::LIST){
			return std::make_pair(EVAL_BAD_SYNTAX, "Ooops, the binding is not a variable - init pair.");
		}

		if (!bindingList->child || bindingList->child->obj->getType() != ObjectDef::ObjectType::SYMBOL){
			return std::make_pair(EVAL_BAD_SYNTAX, "binding should begin with a variable");
		}

		if (!bindingList->child->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "binding should contain one init");
		}

		if (bindingList->child->next->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "too many expressions in a binding pair");
		}

		if (std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(bindingList->child->obj.get())->getVal()) != formal.end()){
			return std::make_pair(EVAL_BAD_SYNTAX, "duplicate variable names in binding");
		}

		formal.push_back(static_cast<ObjectDef::Symbol*>(bindingList->child->obj.get())->getVal());
		sptrObject ret;
		auto callRes = evaluateParseTree(curEnv, bindingList->child->next, ret);
		if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);
		curEnv->insert(formal.back(), ret);

		bindingList = bindingList->next;
	}

	root = root->next;
	if (enableTailRecursion){
        obj = sptrObject(new ObjectDef::ExpressionPack(root));
        env = curEnv;
        return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
	}
	else {
        return evaluateParseTree(curEnv, root, obj, true);
	}

    assert(false);
	return std::make_pair(EVAL_UNEXPECTED_SITUATION, "should never reach here");
}

callResultType letrec_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool enableTailRecursion){
	static const std::string syntatic_keyword("letrec");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (!root->next || root->next->obj->getType() != ObjectDef::ObjectType::LIST){
		return std::make_pair(EVAL_BAD_SYNTAX, "missing <bindings> in letrec");
	}

	if (!root->next->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "missing <body> in letrec");
	}

	root = root->next;
	//predefine variables
	sptrEnvironment curEnv(new ObjectDef::Environment(env));
	std::vector<std::string> formal;
	auto bindingList = root->child;
	while (bindingList){
		if (bindingList->obj->getType() != ObjectDef::ObjectType::LIST){
			return std::make_pair(EVAL_BAD_SYNTAX, "Ooops, the binding is not a variable - init pair.");
		}

		if (!bindingList->child || bindingList->child->obj->getType() != ObjectDef::ObjectType::SYMBOL){
			return std::make_pair(EVAL_BAD_SYNTAX, "binding should begin with a variable");
		}

		if (!bindingList->child->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "binding should contain one init");
		}

		if (bindingList->child->next->next){
			return std::make_pair(EVAL_BAD_SYNTAX, "too many expressions in a binding pair");
		}

		if (std::find(formal.begin(), formal.end(), static_cast<ObjectDef::Symbol*>(bindingList->child->obj.get())->getVal()) != formal.end()){
			return std::make_pair(EVAL_BAD_SYNTAX, "duplicate variable names in binding");
		}

		formal.push_back(static_cast<ObjectDef::Symbol*>(bindingList->child->obj.get())->getVal());
		curEnv->insert(formal.back(), ObjectDef::Void::getVoid());

		bindingList = bindingList->next;
	}

	//binding variables
	bindingList = root->child;
	unsigned formaliter = 0;
	while (bindingList){
		sptrObject ret;
		auto callRes = evaluateParseTree(curEnv, bindingList->child->next, ret);
		if (callRes.first != EVAL_NO_ERROR) return std::move(callRes);
		curEnv->insert(formal[formaliter++], ret);
		bindingList = bindingList->next;
	}

	root = root->next;
	if (enableTailRecursion){
        obj = sptrObject(new ObjectDef::ExpressionPack(root));
        env = curEnv;
        return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
	}
	else {
        return evaluateParseTree(curEnv, root, obj, true);
	}

    assert(false);
	return std::make_pair(EVAL_UNEXPECTED_SITUATION, "should never reach here");
}

callResultType begin_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword("begin");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (!root->next){
		obj = ObjectDef::Void::getVoid();
		return std::make_pair(EVAL_NO_ERROR, "");
	}
	else {
		obj = sptrObject(new ObjectDef::ExpressionPack(root->next));
		return std::make_pair(EVAL_RAW_EXPR_RETURNED, "");
	}

    assert(false);
	return std::make_pair(EVAL_UNEXPECTED_SITUATION, "should never reach here");
}

callResultType delay_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
	static const std::string syntatic_keyword("delay");
	assert(static_cast<ObjectDef::Symbol*>(root->obj.get())->getVal() == syntatic_keyword);

	if (!root->next){
		return std::make_pair(EVAL_BAD_SYNTAX, "delay expects expression");
	}

	obj = sptrObject(new ObjectDef::Promise(root->next, env));
	return std::make_pair(EVAL_NO_ERROR, "");
}

callResultType rightArrow_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
    return std::make_pair(EVAL_BAD_SYNTAX, "=>: not in cond");
}

callResultType else_func(sptrEnvironment& env, sptrParseTree root, sptrObject& obj, bool){
    return std::make_pair(EVAL_BAD_SYNTAX, "else: not in conditional expression");
}

}
