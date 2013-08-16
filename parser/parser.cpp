#include <cctype>
#include <cstring>
#include <utility>
#include <sstream>
#include <fstream>
#include <cassert>
#include <memory>
#include <limits>
#include <stdexcept>
#include <gmpxx.h>
#include <algorithm>

#include "../object/objectdef.h"
#include "parser.h"
#include "../config.h"

namespace ParserEnv{

    const std::string delimiter(" ()\n\r\t\";\f\v");

    const std::string reservedChar("[]{}|");

	const std::string specialInitial("!$%&*/:<=>?^_~");

	const std::string specialSubsequent("+-.@");

    int Parser::lastReturnCode = 0;
    int Parser::lastErrorPos = 0;
    std::string Parser::token;

	std::shared_ptr<ParseTree> Parser::parseFromString(const std::string& _expr){
        return parseFromString(std::string(_expr));
    }

	std::shared_ptr<ParseTree> Parser::parseFromString(std::string&& _expr){
        lastReturnCode = 0;
        std::istringstream _sstream(_expr);
        StreamReader reader(_sstream);
		std::shared_ptr<ParseTree> root;
        parse(root, reader);
        return root;
    }

	std::shared_ptr<ParseTree> Parser::parseFromFile(const std::string& _filename){
        lastReturnCode = 0;
        std::ifstream _fstream(_filename);
        StreamReader reader(_fstream);
		std::shared_ptr<ParseTree> root;
        parse(root, reader);
        return root;
    }

    std::shared_ptr<ParseTree> Parser::parseOneFromStreamReader(StreamReader& _reader){
        lastReturnCode = 0;
        std::shared_ptr<ParseTree> root;

        switch (getNextExpression(root, _reader)){
        case -1:
            root = nullptr;
            break;
        case 1:
            lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
            lastErrorPos = _reader.getCounter();
            break;
        }

        return root;
    }


	/*
	 *	Return value:
	 *	-1: Error encountered.
	 *	0: Normal exit.
	 *	1: ')' encountered. No '(' to match leads to returning -1.
	 */
	int Parser::getNextExpression(std::shared_ptr<ParseTree>& root, StreamReader& reader){
		lastReturnCode = reader.getNextToken(token);

        //Unexpected token
        if (lastReturnCode != PARSE_NORMAL_EXIT){
            lastErrorPos = reader.getCounter();
            root = std::shared_ptr<ParseTree>();
            return -1;
        }

        //empty token
        else if (token.empty()){
            root = std::shared_ptr<ParseTree>();
            return 0;
        }

		//right bracket, end of an expression
        else if (token[0] == ')'){
            assert(token.size() == 1);
            root = std::shared_ptr<ParseTree>();
            return 1;
        }

		//left bracket, beginning of an expression
        else if (token[0] == '('){
            assert(token.size() == 1);

			root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, ObjectDef::getDummyList())) ;

			int retCode = getNextExpression(root->child, reader);
			auto cur = root->child;

			while (cur && retCode == 0){
				retCode = getNextExpression(cur->next, reader);
				cur = cur->next;
			}

			switch (retCode){
			case -1:
				root = std::shared_ptr<ParseTree>();
				return -1;
			case 0:
				lastReturnCode = PARSE_UNEXPECTED_EOF;
				lastErrorPos = reader.getCounter();
				root = std::shared_ptr<ParseTree>();
				return -1;
			case 1:
				return 0;
			}
		}

		// string
        else if (token[0] == '\"'){
			root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::String(std::move(token), false, true))));
		}

		// #, could be boolean, character, number
		else if (token[0] == '#'){
			//missing input after #
			if (token.size() == 1){
				lastReturnCode = PARSE_UNEXPECTED_EOF;
				lastErrorPos = reader.getCounter();
				root = std::shared_ptr<ParseTree>();
				return -1;
			}

			// boolean: true
			else if (token[1] == 't'){
				//Unrecogized literal
				if (token.size() != 2){
					lastReturnCode = PARSE_UNRECOGNIZED_LITERAL;
					lastErrorPos = reader.getCounter();
					root = std::shared_ptr<ParseTree>();
					return -1;
				}

				root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, ObjectDef::Boolean::True()));
			}

			// boolean: false
			else if (token[1] == 'f'){
				//Unrecognized literal
				if (token.size() != 2){
					lastReturnCode = PARSE_UNRECOGNIZED_LITERAL;
					lastErrorPos = reader.getCounter();
					root = std::shared_ptr<ParseTree>();
					return -1;
				}
				root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, ObjectDef::Boolean::False()));
			}

			// character
			else if (token[1] == '\\'){
				// single character
				if (token.size() == 3){
					root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Char(token[2]))));
				}

				// character name
				else{
					const char* cName = token.c_str() + 2;
					if (strcmp(cName, "space") == 0){
						root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Char(' '))));
					}

					else if (strcmp(cName, "newline") == 0){
						root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Char('\n'))));
					}

					else{
						lastReturnCode =  PARSE_UNRECOGNIZED_LITERAL;
						lastErrorPos = reader.getCounter();
						root = std::shared_ptr<ParseTree>();
						return -1;
					}
				}
			}

			// number
			else{
				std::shared_ptr<ObjectDef::Object> obj;
				if ((lastReturnCode = parseNumber(obj, token)) != PARSE_NORMAL_EXIT){
					lastErrorPos = reader.getCounter();
					root = std::shared_ptr<ParseTree>();
					return -1;
				}
				root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, obj));
			}
		}

        //peculiar identifier
        else if ((token.size() == 1 && (token[0] == '+' || token[0] == '-')) ||
            (token.size() == 3 && token[0] == '.' && token[1] == '.' && token[2] == '.')){
            root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Symbol(token))));
        }

		//number
		else if ( std::isdigit(token[0]) || (token.size() >= 2 && (token[0] == '+' || token[0] == '-' || token[0] == '.')) ){
			std::shared_ptr<ObjectDef::Object> obj;
			if ( (lastReturnCode = parseNumber(obj, token)) != PARSE_NORMAL_EXIT){
				lastErrorPos = reader.getCounter();
				root = std::shared_ptr<ParseTree>();
				return -1;
			}
			root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, obj));
		}

		//' quote
		else if (token[0] == '\''){
			root = std::shared_ptr<ParseTree>(
                new ParseTree(
                std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Symbol("quote"))))
                , nullptr, ObjectDef::getDummyList())
                );

			switch (getNextExpression(root->child->next, reader)){
			case -1:
				root = std::shared_ptr<ParseTree>();
				return -1;
			case 1:
				lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
				lastErrorPos = reader.getCounter();
				root = std::shared_ptr<ParseTree>();
				return -1;
			}
		}

		//` quasiquote
		else if (token[0] == '`'){
            root = std::shared_ptr<ParseTree>(
                new ParseTree(
                std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Symbol("quasiquote"))))
                , nullptr, ObjectDef::getDummyList())
                );
            switch (getNextExpression(root->child->next, reader)){
			case -1:
				root = std::shared_ptr<ParseTree>();
				return -1;
			case 1:
				lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
				lastErrorPos = reader.getCounter();
				root = std::shared_ptr<ParseTree>();
				return -1;
			}
		}

		//, unquote
		else if (token.size() == 1 && token[0] == ','){
            root = std::shared_ptr<ParseTree>(
                new ParseTree(
                std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Symbol("unquote"))))
                , nullptr, ObjectDef::getDummyList())
                );
            switch (getNextExpression(root->child->next, reader)){
			case -1:
				root = std::shared_ptr<ParseTree>();
				return -1;
			case 1:
				lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
				lastErrorPos = reader.getCounter();
				root = std::shared_ptr<ParseTree>();
				return -1;
			}
		}

		//,@ unquote-splicing
		else if (token.size() == 2 && token[0] == ',' && token[1] == '@'){
            root = std::shared_ptr<ParseTree>(
                new ParseTree(
                std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Symbol("unquote-splicing"))))
                , nullptr, ObjectDef::getDummyList())
                );
            switch (getNextExpression(root->child->next, reader)){
			case -1:
				root = std::shared_ptr<ParseTree>();
				return -1;
			case 1:
				lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
				lastErrorPos = reader.getCounter();
				root = std::shared_ptr<ParseTree>();
				return -1;
			}
		}

		//.
		else if (token.size() == 1 && token[0] == '.'){
			root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, ObjectDef::getPairDot()));
		}

		//identifier
		else {
			//syntatic keyword and variable
            unsigned i = 0;
			if ( std::isalpha(token[i]) || specialInitial.find(token[i]) != std::string::npos) ++i;
			if ( i != 0){
				for (; i < token.size() && (std::isalpha(token[i]) || std::isdigit(token[i]) || specialInitial.find(token[i]) != std::string::npos || specialSubsequent.find(token[i]) != std::string::npos ); ++i);
            }
            if ( i == token.size()){
				root = std::shared_ptr<ParseTree>(new ParseTree(nullptr, nullptr, std::shared_ptr<ObjectDef::Object>(new ObjectDef::Symbol(token))));
			}
			else {
                lastReturnCode = PARSE_INVALID_IDENTIFIER;
				root = std::shared_ptr<ParseTree>();
				return -1;
			}
		}
		return 0;
	}

    static constexpr int DEFAULT_EXACTNESS = 0;
    static constexpr int EXPLICIT_EXACT = 1;
    static constexpr int EXPLICIT_INEXACT = 2;

    static const std::string intLimit = std::to_string(std::numeric_limits<ObjectDef::Int::integer>::max());
    static const std::string expMarker("esfdl");
    static const std::string splitMarker("./");
    static const std::string signMarker("+-");

    int Parser::parseNumber(std::shared_ptr<ObjectDef::Object>& obj, const std::string& str, int default_radix){
        //empty string
		if (str.empty()){
			obj = nullptr;
			return PARSE_INVALID_NUMBER;
		}

		unsigned cur = 0;
		int exactness = DEFAULT_EXACTNESS;
		int radix = 0;
		while (str[cur] == '#' && cur <= 2){	// at most two prefix denoting radix and exactness
			if (str.size() <= ++cur){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}
			switch (str[cur]){
				case 'e':
                    if (exactness == DEFAULT_EXACTNESS)
                        exactness = EXPLICIT_EXACT;
                    else{
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    break;
                case 'i':
                    if (exactness == DEFAULT_EXACTNESS)
                        exactness = EXPLICIT_INEXACT;
                    else{
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    break;
				case 'b':
                    if (radix == 0)
                        radix = 2;
                    else {
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    break;
				case 'o':
                    if (radix == 0)
                        radix = 8;
                    else {
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    break;
				case 'd':
                    if (radix == 0)
                        radix = 10;
                    else {
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    break;
				case 'x':
                    if (radix == 0)
                        radix = 16;
                    else {
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    break;
				default:{
					obj = nullptr;
					return PARSE_INVALID_NUMBER;
				}
			}
			if (str.size() <= ++cur){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}
		}

		if (radix == 0) radix = default_radix;

		// complex that has imaginary part in the Cartesian Coordinate
        if (str.back() == 'i'){
            unsigned iSign = str.find_last_of(signMarker);

            if (iSign < cur){
                obj = nullptr;
                return PARSE_INVALID_NUMBER;
            }

            // complex that only has imaginary part
            else if (iSign == cur){

                // + <ureal R> i
                // - <ureal R> i
                if (iSign + 2 != str.size()){
                    std::shared_ptr<ObjectDef::Object> imag;
                    int ret = parseRealNumber(imag, str.substr(iSign, str.size() - 1 - iSign), exactness, radix);
                    if (ret != PARSE_NORMAL_EXIT) return ret;

                    obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Complex(true,
                            std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness != EXPLICIT_INEXACT, 0))
                            , imag));
                    return PARSE_NORMAL_EXIT;
                }

                // + i
                // - i
                else{
                    if (str[iSign] == '+'){
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Complex(true,
                                std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness != EXPLICIT_INEXACT, 0)),
                                std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness != EXPLICIT_INEXACT, 1))));
                        return PARSE_NORMAL_EXIT;
                    }
                    else{
                        assert(str[iSign] == '-');
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Complex(true,
                                std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness != EXPLICIT_INEXACT, 0)),
                                std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness != EXPLICIT_INEXACT, -1))));
                        return PARSE_NORMAL_EXIT;
                    }
                }
            }

            // complex has both real and imaginary parts
            else {
                std::shared_ptr<ObjectDef::Object> real;
                int ret = parseRealNumber(real, str.substr(cur, iSign - cur), exactness, radix);
                if (ret != PARSE_NORMAL_EXIT) return ret;

                std::shared_ptr<ObjectDef::Object> imag;
                if (iSign + 2 != str.size()){
                    ret = parseRealNumber(imag, str.substr(iSign, str.size() - 1 - iSign), exactness, radix);
                    if (ret != PARSE_NORMAL_EXIT) return ret;
                }

                else {
                    if (str[iSign] == '+'){
                        imag = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness != EXPLICIT_INEXACT, 1));
                    }
                    else {
                        assert(str[iSign] == '-');
                        imag = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness != EXPLICIT_INEXACT, -1));
                    }
                }

                if (static_cast<ObjectDef::Number*>(imag.get())->getExactness() && ObjectDef::equal(imag, getZero())){
                    assert(real.unique());
                    static_cast<ObjectDef::Number*>(real.get())->setExactness(
                                                    static_cast<ObjectDef::Number*>(real.get())->getExactness() &&
                                                    static_cast<ObjectDef::Number*>(imag.get())->getExactness());
                    obj = real;
                    return PARSE_NORMAL_EXIT;
                }
                else {
                    obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Complex(true, real, imag));
                    return PARSE_NORMAL_EXIT;
                }
            }
        }

        // complex in polar coordinate
        // real
        else {
            unsigned atMarkPos = str.find('@');

            // <real R> @ <real R>
            if (atMarkPos != std::string::npos){
                if (atMarkPos + 1 >= str.size() ||
                    ((str[atMarkPos+1] == '+' || str[atMarkPos+1] == '-') && atMarkPos + 2 == str.size())){
                    return PARSE_INVALID_NUMBER;
                }

                if (atMarkPos <= cur ||
                    ((str[cur] == '+' || str[cur] == '-') && atMarkPos == cur+1)){
                    return PARSE_INVALID_NUMBER;
                }

                std::shared_ptr<ObjectDef::Object> magn;
                int ret = parseRealNumber(magn, str.substr(cur, atMarkPos - cur), exactness, radix);
                if (ret != PARSE_NORMAL_EXIT) return ret;

                std::shared_ptr<ObjectDef::Object> angl;
                ret = parseRealNumber(angl, str.substr(atMarkPos + 1, std::string::npos), exactness, radix);
                if (ret != PARSE_NORMAL_EXIT) return ret;

                obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Complex(true,
                            ObjectDef::times(magn, ObjectDef::cos(angl)),
                            ObjectDef::times(magn, ObjectDef::sin(angl))));
                return PARSE_NORMAL_EXIT;
            }

            else{
                return parseRealNumber(obj, str.substr(cur), exactness, radix);
            }
        }

        return PARSE_NORMAL_EXIT;
    }

	int Parser::parseRealNumber(std::shared_ptr<ObjectDef::Object>& obj, std::string&& str, int exactness, int radix){
		//empty string
		if (str.empty()){
			obj = nullptr;
			return PARSE_INVALID_NUMBER;
		}

        unsigned cur = 0;

		bool hasSign = false;
		if (str[cur] == '+' || str[cur] == '-'){
			hasSign = true;
			if (str.size() <= ++cur){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}
		}

		unsigned begin_of_number = cur;
		unsigned split = str.find_first_of(splitMarker, begin_of_number);
		unsigned expMarkerPos = (radix == 10) ? str.find_first_of(expMarker, begin_of_number) : std::string::npos;

		// <decimal 10> <suffix>
		if (expMarkerPos != std::string::npos){
			if (radix != 10){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}

			if (expMarkerPos + 1 == str.size()){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}

			// EXPLICIT_EXACT
			if (exactness == EXPLICIT_EXACT){
				// #e <uinteger 10> <suffix>
				if (split == std::string::npos){
					if (hasSign && str[begin_of_number - 1] == '-') --begin_of_number;
					std::string num_str = str.substr(begin_of_number, expMarkerPos - begin_of_number);

					unsigned i = 0;
					if (num_str[i] == '-') ++i;
					begin_of_number = i;
					if ( !std::isdigit(num_str[i])){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}
					for (++i; i < num_str.size(); ++i){
						if (num_str[i] == '#'){
							num_str[i] = '0';
							for (++i ; i < num_str.size(); ++i){
								if (num_str[i] == '#')
									num_str[i] = '0';
								else{
									obj = nullptr;
									return PARSE_INVALID_NUMBER;
								}
							}
						}
						else if ( !std::isdigit(num_str[i])){
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}

					//erase the prefix 0s
					i = begin_of_number;
					while (i < num_str.size() && num_str[i] == '0') ++i;
					if (i == num_str.size()) --i;
					num_str.erase(begin_of_number, i - begin_of_number);

					if ((num_str.size() == 1 && num_str.back() == '0')
                        || ( num_str.size() == 2 && num_str.front() == '-' && num_str.back() == '0') ){
						obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(true, 0));
						return PARSE_NORMAL_EXIT;
					}

				    unsigned errpos = 0;
					std::string exp_str = str.substr(expMarkerPos + 1, std::string::npos);
					int exp_val;
					try{
						exp_val = std::stoi(exp_str, &errpos, 10);
					}
					catch (std::invalid_argument){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}
					catch (std::out_of_range){
						obj = nullptr;
						return PARSE_NUMBER_OVERFLOW;
					}
					if (errpos != exp_str.size()){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}

					if (exp_val > 0){
						num_str.append(exp_val, '0');
					} else
					if (exp_val < 0){
						while (exp_val < 0 && num_str.back() == '0'){
							num_str.pop_back();
							++exp_val;
						}
						if (exp_val < 0){
							mpz_class num(num_str);
							num_str.clear();
							num_str.push_back('1');
							num_str.append(-exp_val, '0');
							mpz_class den(num_str);
							obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Rational(true, num, den));
							return PARSE_NORMAL_EXIT;
						}
					}

                    //try if it could fit in a Int
                    int val;
                    try{
                        val = std::stoi(num_str, 0, 10);
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(true, val));
                    }
                    catch (std::out_of_range){
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::BigInt(true, num_str));
                    }
					return PARSE_NORMAL_EXIT;
				}

				// #e  . <digit 10>+ #* <suffix>
				// #e <digit 10>+ . <digit10>* #* <suffix>
				// #e <digit 10>+ #+ . #* <suffix>
				else {
                    if (str[begin_of_number] == '.' && !std::isdigit(str[begin_of_number+1])){
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }

					std::string num_str = (hasSign && str[begin_of_number-1] == '-') ?
										 ((split == begin_of_number) ? (std::string("-0")) : (str.substr(begin_of_number-1, split - begin_of_number + 1))):
										 ((split == begin_of_number) ? (std::string("0")) : (str.substr(begin_of_number, split - begin_of_number)) );

					unsigned i = 0;
					if ( num_str[i] == '-') ++i;
					begin_of_number = i;
					if ( !std::isdigit(num_str[i])){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}
					for (++i; i < num_str.size(); ++i){
						if (num_str[i] == '#'){
							num_str[i] = '0';
							for ( ++i; i < num_str.size(); ++i){
								if (num_str[i] == '#'){
									num_str[i] = '0';
								} else {
									obj = nullptr;
									return PARSE_INVALID_NUMBER;
								}
							}
							i = 0;
							break;
						}
						else if ( !std::isdigit(num_str[i])){
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}

					int exp_val = 0;
					if (i == num_str.size()){ // No # has ever been encountered
						i = split + 1;
						for (; i < expMarkerPos; ++i){
							if (str[i] == '#'){
								for (++i; i < expMarkerPos; ++i){
									if (str[i] != '#'){
										obj = nullptr;
										return PARSE_INVALID_NUMBER;
									}
								}
							}
							else if ( std::isdigit(str[i]) ) {
								if (exp_val == std::numeric_limits<int>::min()){
									obj = nullptr;
									return PARSE_NUMBER_OVERFLOW;
								}
								exp_val -= 1;
								num_str.push_back(str[i]);
							} else {
								obj = nullptr;
								return PARSE_INVALID_NUMBER;
							}
						}
					} else {
						i = split + 1;
						for (; i < expMarkerPos; ++i){
							if (str[i] != '#'){
								obj = nullptr;
								return PARSE_INVALID_NUMBER;
							}
						}
					}

                    //erase prefix 0
					i = begin_of_number;
					for (; i < num_str.size() && num_str[i] == '0' ; ++i);
					if (i == num_str.size()) --i;
					num_str.erase(begin_of_number, i - begin_of_number);

					if ((num_str.size() == 1 && num_str.back() == '0')
                        || (num_str.size() == 2 && num_str.front() == '-' && num_str.back() == '0') ){
						obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(true, 0));
						return PARSE_NORMAL_EXIT;
					}

					unsigned errpos = 0;
					std::string exp_str = str.substr(expMarkerPos + 1, std::string::npos);
					int exp_val2;
					try{
						exp_val2 = std::stoi(exp_str, &errpos, 10);
					}
					catch (std::invalid_argument){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}
					catch (std::out_of_range){
						obj = nullptr;
						return PARSE_NUMBER_OVERFLOW;
					}
					if (errpos != exp_str.size()){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}

                    assert(exp_val <= 0);
					if (exp_val2 < std::numeric_limits<int>::min() - exp_val){
						obj = nullptr;
						return PARSE_NUMBER_OVERFLOW;
					}

					exp_val += exp_val2;

					if (exp_val > 0){
						num_str.append(exp_val, '0');
					} else
					if (exp_val < 0){
						while (exp_val < 0 && num_str.back() == '0'){
							num_str.pop_back();
							++exp_val;
						}
						if (exp_val < 0){
							mpz_class num(num_str);
							num_str.clear();
							num_str.push_back('1');
							num_str.append(-exp_val, '0');
							mpz_class den(num_str);
							obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Rational(true, num, den));
							return PARSE_NORMAL_EXIT;
						}
					}

                    //try if it could fit in a Int
                    int val;
                    try{
                        val = std::stoi(num_str, 0, 10);
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(true, val));
                    }
                    catch (std::out_of_range){
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::BigInt(true, num_str));
                    }
					return PARSE_NORMAL_EXIT;

				}
			}

			// INEXACT
			else {

                // <uinteger 10> <suffix>
                if (split == std::string::npos){
                    if (hasSign && str[begin_of_number - 1] == '-') --begin_of_number;
					std::string num_str = str.substr(begin_of_number, std::string::npos);

					unsigned i = 0;
					expMarkerPos = expMarkerPos - begin_of_number;
					if (num_str[i] == '-') ++i;
					begin_of_number = i;
					if ( !std::isdigit(num_str[i])){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}
					for (++i; i < expMarkerPos; ++i){
						if (num_str[i] == '#'){
							num_str[i] = '0';
							for (++i ; i < expMarkerPos; ++i){
								if (num_str[i] == '#')
									num_str[i] = '0';
								else{
									obj = nullptr;
									return PARSE_INVALID_NUMBER;
								}
							}
							break;
						}
						else if ( !std::isdigit(num_str[i])){
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}

					assert( i == expMarkerPos && expMarker.find(num_str[i]) != std::string::npos);
					num_str[i++] = 'e';

                    if (num_str[i] == '+' || num_str[i] == '-') ++i;
                    if (i == num_str.size()){
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }

                    for (; i < num_str.size(); ++i){
                        if (!std::isdigit(num_str[i])){
                            obj = nullptr;
                            return PARSE_INVALID_NUMBER;
                        }
                    }

					obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Real(false, num_str.c_str()));
					return PARSE_NORMAL_EXIT;
                }

                // . <digit 10>+ #* <suffix>
				// <digit 10>+ . <digit10>* #* <suffix>
				// <digit 10>+ #+ . #* <suffix>
                else{
                    std::string num_str;
                    unsigned i = 0;
                    if (hasSign && str[begin_of_number-1] == '-') {
                        num_str.push_back('-');
                        ++i;
                    }
                    if (str[begin_of_number] == '.') {
                        if (! std::isdigit(str[begin_of_number+1])){
                            obj = nullptr;
                            return PARSE_INVALID_NUMBER;
                        }
                        num_str.push_back('0');
                        ++expMarkerPos;     //added an extra zero
                        ++split;
                    }
                    num_str.append(str, begin_of_number, std::string::npos);

                    expMarkerPos = expMarkerPos - begin_of_number + i;
                    begin_of_number = i;
                    if ( !std::isdigit(num_str[i])){
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    for (++i; num_str[i] != '.'; ++i){
                        if (num_str[i] == '#'){
                            num_str[i] = '0';
                            for (++i; num_str[i] != '.'; ++i){
                                if (num_str[i] == '#'){
                                    num_str[i] = '0';
                                } else {
                                    obj = nullptr;
                                    return PARSE_INVALID_NUMBER;
                                }
                            }
                            num_str[i] = '#';
                            break;
                        }
                        else if ( !std::isdigit(num_str[i])){
                            obj = nullptr;
                            return PARSE_INVALID_NUMBER;
                        }
                    }

                    if (i+1 == expMarkerPos){   //<digit 10>+ #* . <ExpMarker> <sign> <digit 10>+
                        ++i;    // It seems that leaving the dot to mpf does not hurt
                    }
                    else if (num_str[i] == '#'){
                        num_str[i] = '.';
                        for (unsigned j = i + 1; j < expMarkerPos; ++j){
                            if (num_str[j] !='#'){
                                obj = nullptr;
                                return PARSE_INVALID_NUMBER;
                            }
                        }
                        num_str[i+1] = '0';
                        num_str.erase(i+2, expMarkerPos - i - 2);
                        expMarkerPos = i + 2;
                        i+=2;
                    } else {

                        for (++i; i < expMarkerPos; ++i){
                            if (num_str[i] == '#'){
                                num_str[i] = '0';
                                for (unsigned j = i+1; j < expMarkerPos; ++j){
                                    if (num_str[j] !='#'){
                                        obj = nullptr;
                                        return PARSE_INVALID_NUMBER;
                                    }
                                }
                                num_str.erase(i+1, expMarkerPos - i - 1);
                                expMarkerPos = i + 1;
                                i+=1;
                                break;
                            }
                            else if ( !std::isdigit(num_str[i])) {
                                obj = nullptr;
                                return PARSE_INVALID_NUMBER;
                            }
                        }
                    }

                    assert( i == expMarkerPos && expMarker.find(num_str[i]) != std::string::npos);
                    num_str[i++] = 'e';
                    if (num_str[i] == '-' || num_str[i] == '+') ++i;
                    if (i == num_str.size()){
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }

                    for (; i < num_str.size(); ++i){
                        if ( !std::isdigit(num_str[i])){
                            obj = nullptr;
                            return PARSE_INVALID_NUMBER;
                        }
                    }

                    obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Real(false, num_str.c_str()));
                    return PARSE_NORMAL_EXIT;
                }
			}
		}

		// <uinteger R>
		else if (split == std::string::npos){
			std::string num_str = (hasSign && str[begin_of_number-1] == '-') ? (str.substr(begin_of_number-1))
                                                                             : (str.substr(begin_of_number));

            unsigned i = 0;
            if (str[i] == '-') ++i;
			if ( (radix <= 10 && (num_str[i] < '0' || num_str[i] >= '0' + radix) )
						 || (radix == 16 && !( std::isdigit(num_str[i]) || (num_str[i] >= 'a' && num_str[i] <= 'f')))){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}
			for (++i; i < num_str.size(); ++i){
				if (num_str[i] == '#'){
					num_str[i] = '0';
                    for (++i; i < num_str.size(); ++i){
                        if (num_str[i] == '#'){
                            num_str[i] = '0';
                        } else {
                            obj = nullptr;
                            return PARSE_INVALID_NUMBER;
                        }
                    }
					if (exactness == DEFAULT_EXACTNESS) exactness = EXPLICIT_INEXACT;
					break;
				}
				else if ( (radix <= 10 && (num_str[i] < '0' || num_str[i] >= '0' + radix) )
						 || (radix == 16 && !( std::isdigit(num_str[i]) || (num_str[i] >= 'a' && num_str[i] <= 'f')))){
					obj = nullptr;
					return PARSE_INVALID_NUMBER;
				}
			}

			if (exactness == DEFAULT_EXACTNESS) exactness = EXPLICIT_EXACT;

            //try if it could fit in a Int
            int val;
            try{
                val = std::stoi(num_str, 0, radix);
                obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness == EXPLICIT_EXACT, val));
            }
            catch (std::out_of_range){
                obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::BigInt(exactness == EXPLICIT_EXACT, num_str, radix));
            }
			return PARSE_NORMAL_EXIT;
		}

		// . <digit 10>+ #*
		// <digit 10>+  . <digit 10>* #*
		// <digit 10>+ #+ . #*
		else if (str[split] == '.'){
			if (radix != 10){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}

            if (split == begin_of_number && split + 1 ==str.size()){
                obj = nullptr;
                return PARSE_INVALID_NUMBER;
            }

			if (split + 1 == str.size()){
				str.push_back('0');
			}

			// EXPLICIT_EXACT or a conversion could be used
			if (exactness == EXPLICIT_EXACT || str.size() <= _scm_inexact_conversion_to_rational_bound){
				//  . <digit 10>+ #*
				//  <digit 10>+ . <digit10>* #*
				//  <digit 10>+ #+ . #*
				{
					std::string num_str = (hasSign && str[begin_of_number-1] == '-') ?
										 ((split == begin_of_number) ? (std::string("-0")) : (str.substr(begin_of_number-1, split - begin_of_number + 1))):
										 ((split == begin_of_number) ? (std::string("0")) : (str.substr(begin_of_number, split - begin_of_number)) );

					unsigned i = 0;
					if ( num_str[i] == '-') ++i;
					begin_of_number = i;
					if ( !std::isdigit(num_str[i])){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}
					for (++i; i < num_str.size(); ++i){
						if (num_str[i] == '#'){
							num_str[i] = '0';
							for ( ++i; i < num_str.size(); ++i){
								if (num_str[i] == '#'){
									num_str[i] = '0';
								} else {
									obj = nullptr;
									return PARSE_INVALID_NUMBER;
								}
							}
							i = 0;
							break;
						}
						else if ( !std::isdigit(num_str[i])){
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}

					int exp_val = 0;
					if (i == num_str.size()){ // No # has ever been encountered
						i = split + 1;
						for (; i < str.size(); ++i){
							if (str[i] == '#'){
								for (++i; i < str.size(); ++i){
									if (str[i] != '#'){
										obj = nullptr;
										return PARSE_INVALID_NUMBER;
									}
								}
							}
							else if ( std::isdigit(str[i]) ) {
								if (exp_val == std::numeric_limits<int>::min()){
									obj = nullptr;
									return PARSE_NUMBER_OVERFLOW;
								}
								exp_val -= 1;
								num_str.push_back(str[i]);
							} else {
								obj = nullptr;
								return PARSE_INVALID_NUMBER;
							}
						}
					} else {
						i = split + 1;
						for (; i < str.size(); ++i){
							if (str[i] != '#'){
								obj = nullptr;
								return PARSE_INVALID_NUMBER;
							}
						}
					}

					//erase prefix 0
					i = begin_of_number;
					for (; i < num_str.size() && num_str[i] == '0' ; ++i);
					if (i == num_str.size()) --i;
					num_str.erase(begin_of_number, i - begin_of_number);

					if ((num_str.size() == 1 && num_str.back() == '0')
                        || (num_str.size() == 2 && num_str.front() == '-' && num_str.back() == '0')){
						obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness == EXPLICIT_EXACT, 0));
						return PARSE_NORMAL_EXIT;
					}

					assert(exp_val <= 0);
					if (exp_val < 0){
						while (exp_val < 0 && num_str.back() == '0'){
							num_str.pop_back();
							++exp_val;
						}
						if (exp_val < 0){
							mpz_class num(num_str);
							num_str.clear();
							num_str.push_back('1');
							num_str.append(-exp_val, '0');
							mpz_class den(num_str);
							obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Rational(exactness == EXPLICIT_EXACT, num, den));
							return PARSE_NORMAL_EXIT;
						}
					}

                    //try if it could fit in a Int
                    int val;
                    try{
                        val = std::stoi(num_str, 0, 10);
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness == EXPLICIT_EXACT, val));
                    }
                    catch (std::out_of_range){
                        obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::BigInt(exactness == EXPLICIT_EXACT, num_str));
                    }
					return PARSE_NORMAL_EXIT;

				}
			}

			// INEXACT
			else {
				std::string num_str;
				unsigned i = 0;
                if (hasSign && str[begin_of_number-1] == '-') {
                    num_str.push_back('-');
                    ++i;
                }
                if (str[begin_of_number] == '.') {
                    if (! std::isdigit(str[begin_of_number+1])){
                        obj = nullptr;
                        return PARSE_INVALID_NUMBER;
                    }
                    num_str.push_back('0');
                }
                num_str.append(str, begin_of_number, std::string::npos);

				begin_of_number = i;
				if (!std::isdigit(num_str[i])){
					obj = nullptr;
					return PARSE_INVALID_NUMBER;
				}
				for (++i; num_str[i] != '.'; ++i){
					if (num_str[i] == '#'){
						num_str[i] = '0';
						for (++i; num_str[i] != '.'; ++i){
							if (num_str[i] == '#'){
								num_str[i] = '0';
							} else {
								obj = nullptr;
								return PARSE_INVALID_NUMBER;
							}
						}
                        num_str[i] = '#';
                        break;
					}
					else if ( !std::isdigit(num_str[i])){
						obj = nullptr;
						return PARSE_INVALID_NUMBER;
					}
				}

				if (num_str[i] == '#'){
					num_str[i] = '.';
					for (unsigned j = i + 1; j < num_str.size(); ++j){
						if (num_str[j] !='#'){
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}
					num_str[i+1] = '0';
					num_str.erase(i+2, std::string::npos);
					i+=2;
				} else {
					num_str[i] = '.';
					for (++i; i < num_str.size(); ++i){
						if (num_str[i] == '#'){
							num_str[i] = '0';
							for (unsigned j = i+1; j < num_str.size(); ++j){
								if (num_str[j] !='#'){
									obj = nullptr;
									return PARSE_INVALID_NUMBER;
								}
							}
							num_str.erase(i+1, std::string::npos);
							i+=1;
							break;
						}
						else if ( !std::isdigit(num_str[i])) {
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}
				}

				assert(i == num_str.size());
				obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Real(false, num_str.c_str()));
				return PARSE_NORMAL_EXIT;
			}
		}

		//fraction
		else if (str[split] == '/'){
			if ( split == begin_of_number || str.size() - split <= 1){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}

			//Extract <uinteger R>1
			std::string num_str = (hasSign && str[begin_of_number-1] == '-') ?
								  (str.substr(begin_of_number-1, split - begin_of_number+1)) :
								  (str.substr(begin_of_number, split - begin_of_number)) ;

			unsigned i = 0;
			if (num_str[i] == '-') ++i;
			begin_of_number = i;
			if ((radix <= 10 && (num_str[i] < '0' || num_str[i] >= '0' + radix) )
								  || (radix == 16 && !( std::isdigit(num_str[i]) || (num_str[i] >= 'a' && num_str[i] <= 'f'))) ){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}
			for (++i; i < num_str.size(); ++i){
				if (num_str[i] == '#'){
					if (exactness == DEFAULT_EXACTNESS){
						exactness = EXPLICIT_INEXACT;
					}
					num_str[i] = '0';
					for (++i ; i < num_str.size(); ++i){
						if (num_str[i] == '#'){
							num_str[i] = '0';
						}
						else{
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}
					break;
				}
				else if ( (radix <= 10 && (num_str[i] < '0' || num_str[i] >= '0' + radix) )
						 || (radix == 16 && !( std::isdigit(num_str[i]) || (num_str[i] >= 'a' && num_str[i] <= 'f'))) ){
					obj = nullptr;
					return PARSE_INVALID_NUMBER;
				}
			}

			mpz_class num(num_str.c_str(), radix);

			//Extract <uinteger R>2
			num_str = str.substr(split+1, std::string::npos);

			i = 0;
			if ((radix <= 10 && (num_str[i] < '0' || num_str[i] >= '0' + radix) )
				 || (radix == 16 && !( std::isdigit(num_str[i]) || (num_str[i] >= 'a' && num_str[i] <= 'f'))) ){
				obj = nullptr;
				return PARSE_INVALID_NUMBER;
			}
			for (++i; i < num_str.size(); ++i){
				if (num_str[i] == '#'){
					if (exactness == DEFAULT_EXACTNESS){
						exactness = EXPLICIT_INEXACT;
					}
					num_str[i] = '0';
					for (++i ; i < num_str.size(); ++i){
						if (num_str[i] == '#'){
							num_str[i] = '0';
						}
						else{
							obj = nullptr;
							return PARSE_INVALID_NUMBER;
						}
					}
				}
				else if ( (radix <= 10 && (num_str[i] < '0' || num_str[i] >= '0' + radix) )
						 || (radix == 16 && !( std::isdigit(num_str[i]) || (num_str[i] >= 'a' && num_str[i] <= 'f'))) ){
					obj = nullptr;
					return PARSE_INVALID_NUMBER;
				}
			}

			mpz_class den(num_str.c_str(), radix);

			if (den == 0){
				obj = nullptr;
				return PARSE_DIVISION_BY_ZERO;
			}

			if (exactness == DEFAULT_EXACTNESS){
				exactness = EXPLICIT_EXACT;
			}

			if (num % den == 0){
				num /= den;
				if (num >= std::numeric_limits<ObjectDef::Int::integer>::min() && num <= std::numeric_limits<ObjectDef::Int::integer>::max()){
					obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Int(exactness == EXPLICIT_EXACT, num.get_si()));
				}
				else{
					obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::BigInt(exactness == EXPLICIT_EXACT, num));
				}
			} else {
				obj = std::shared_ptr<ObjectDef::Object>(new ObjectDef::Rational(exactness == EXPLICIT_EXACT, num, den));
			}
			return PARSE_NORMAL_EXIT;
		}

		return PARSE_NORMAL_EXIT;
	}

    /*
        Return value:
        -1: Error encountered.
        0: Normal exit.
        1: ')' encountered. No '(' to match leads to returning -1.
    */
    int Parser::parse(std::shared_ptr<ParseTree>& root, StreamReader& reader){
		int retCode = getNextExpression(root, reader);
		std::shared_ptr<ParseTree> cur = root;

		while (cur && retCode == 0){
			retCode = getNextExpression(cur->next, reader);
			cur = cur->next;
		}

		switch (retCode){
		case -1:
			root = std::shared_ptr<ParseTree>();
			return -1;
		case 1:
			lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
			lastErrorPos = reader.getCounter();
			root = std::shared_ptr<ParseTree>();
			return -1;
		}

		return 0;
	}

    bool StreamReader::hasNextToken(){
        char nextChar = getNextChar();
        while (nextChar != std::istream::traits_type::eof() && isspace(nextChar)) nextChar = getNextChar();
        if (nextChar != std::istream::traits_type::eof()){
            putBackLastChar();
            return true;
        }
        return false;
    }

    int StreamReader::getNextToken(std::string& token){
        token.clear();
        /*
            Skipping whitespaces and comment. Whitespaces include space(' '), newline('\n')
            and additional whitespace characters: '\f', '\r', '\t', '\v'
        */
        char nextChar = getNextChar();
        while (nextChar == ';' || isspace(nextChar)){
            if (nextChar == ';'){
                while (nextChar != '\n' && nextChar != std::istream::traits_type::eof())
                    nextChar = getNextChar();
            }
            else nextChar = getNextChar();
        }

        // end-of-file
        if (nextChar == std::istream::traits_type::eof()) ;

        // check reserved characters
        else if (reservedChar.find(nextChar) != std::string::npos){
            return PARSE_USE_OF_RESERVED;
        }

        //bracket
        else if (nextChar == '(' || nextChar == ')') token.push_back(nextChar);

        /*
            String.
        */
        else if (nextChar == '\"'){
            token.push_back(nextChar);
            while ((nextChar = getNextCharRaw()) != '\"'){
                //Unexpected eof
                if (nextChar == std::istream::traits_type::eof()){
                    return PARSE_UNEXPECTED_EOF;
                }

                //esacape character, without valitdity check
                if (nextChar == '\\'){
                    token.push_back(nextChar);
                    nextChar = getNextCharRaw();
                    //unexpected eof
                    if (nextChar == std::istream::traits_type::eof()){
                        return PARSE_UNEXPECTED_EOF;
                    }
                }

                token.push_back(nextChar);
            }
            token.push_back(nextChar);
        }

        //#, possibly char
        else if (nextChar == '#'){
            token.push_back(nextChar);
            nextChar = getNextChar();
            if (nextChar == std::string::traits_type::eof()){
                return PARSE_UNEXPECTED_EOF;
            }
            if (nextChar == '\\'){
                token.push_back(nextChar);
                char pendingChar = getNextCharRaw();
                if (pendingChar == std::string::traits_type::eof()){
                    return PARSE_UNEXPECTED_EOF;
                }
                nextChar = getNextChar();
                if (nextChar == std::string::traits_type::eof() || delimiter.find(nextChar) != std::string::npos){
                    token.push_back(pendingChar);
                    putBackLastChar();
                }
                else {
                    token.push_back(std::tolower(pendingChar));
                    while (nextChar != std::string::traits_type::eof()){
                        if (delimiter.find(nextChar) != std::string::npos){
                            putBackLastChar();
                            break;
                        }
                        token.push_back(nextChar);
                        nextChar = getNextChar();
                    }
                }
            }
            else {
                while (nextChar != std::string::traits_type::eof()){
                    if (delimiter.find(nextChar) != std::string::npos){
                        putBackLastChar();
                        break;
                    }
                    token.push_back(nextChar);
                    nextChar = getNextChar();
                }
            }
        }

        // ' `
        else if (nextChar == '\'' || nextChar == '`'){
            token.push_back(nextChar);
        }

        // , ,@

        else if (nextChar == ','){
            token.push_back(nextChar);
            nextChar = getNextChar();
            if (nextChar == '@'){
                token.push_back(nextChar);
            } else {
                putBackLastChar();
            }
        }

        //Identifier, literals
        else{
            while (nextChar != std::istream::traits_type::eof()){
                if (delimiter.find(nextChar) != std::string::npos){
                    putBackLastChar();
                    break;
                }
                token.push_back(nextChar);
                nextChar = getNextChar();
            }
        }

        return PARSE_NORMAL_EXIT;
    }

}
