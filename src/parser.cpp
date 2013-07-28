#include "parser.h"

#include <cctype>
#include <cstring>
#include <utility>
#include <sstream>
#include <fstream>
#include <cassert>

namespace ParserEnv{

    const std::string delimeter(" ()\n\r\t\";\f\v");

    const std::string reservedChar("[]{}|");

    ParseTree::ParseTree():
        token(), next(nullptr), child(nullptr){}

    ParseTree::ParseTree(const string& _token, ParseTree* _next, ParseTree* _child):
        token(_token), next(_next), child(_child){}

    ParseTree::ParseTree(string&& _token, ParseTree* _next, ParseTree* _child):
        token(std::move(_token)), next(_next), child(_child){}

    void ParseTree::debugPrint(ParseTree* root){
    using std::cout;
    using std::endl;
        cout << '[';
        while (root){

            if (root->token.empty())
            cout << 'E'; else cout << root->token;

            if (root->child) debugPrint(root->child);
            root = root->next;
            if (root) cout << " , ";
        }
        cout << ']';
    }

    int Parser::lastReturnCode = 0;
    int Parser::lastErrorPos = 0;
    std::string Parser::token;

    ParseTree* Parser::parseFromString(const string& _expr){
        return parseFromString(string(_expr));
    }

    ParseTree* Parser::parseFromString(string&& _expr){
        lastReturnCode = 0;
        std::istringstream _sstream(_expr);
        StreamReader reader(_sstream);
        ParseTree* root = nullptr;
        if (parse(root, reader) == 1){
            deleteParseTree(root);
            lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
            lastErrorPos = reader.getCounter();
            root = nullptr;
        };
        return root;
    }

    ParseTree* Parser::parseFromFile(const string& _filename){
        lastReturnCode = 0;
        std::ifstream _fstream(_filename);
        StreamReader reader(_fstream);
        ParseTree* root = nullptr;
        if (parse(root, reader) == 1){
            deleteParseTree(root);
            lastReturnCode = PARSE_UNEXPECTED_RBRACKET;
            lastErrorPos = reader.getCounter();
            root = nullptr;
        }
        return root;
    }

    /*
        Return value:
        -1: Error encountered.
        0: Normal exit.
        1: ')' encountered. No '(' to match leads to returning -1.
    */
    int Parser::parse(ParseTree*& root, StreamReader& reader){
        lastReturnCode = reader.getNextToken(token);

        //Unexpected token
        if (lastReturnCode != PARSE_NORMAL_EXIT){
            lastErrorPos = reader.getCounter();
            root = nullptr;
            return -1;
        }

        //empty string
        else if (token.empty()){
            root = nullptr;
            return 0;
        }

        else if (token[0] == ')'){
            assert(token.size() == 1);
            root = nullptr;
            return 1;
        }

        else if (token[0] == '('){
            assert(token.size() == 1);

            root = new ParseTree;

            switch (parse(root->child, reader)){
            case -1:
                {
                    //error, discard changes
                    delete root;
                    root = nullptr;
                    return -1;
                }
               // break;
            case 0:
                {
                    //Unexpected end-of-file
                    lastReturnCode = PARSE_UNEXPECTED_EOF;
                    lastErrorPos = reader.getCounter();
                    delete root;
                    root = nullptr;
                    return -1;
                }
               // break;
            }
        }
        else {
            root = new ParseTree(token, 0, 0);
        }

        assert(root != nullptr);    //first element has to exist

        ParseTree* cur = root;
        while ( lastReturnCode = reader.getNextToken(token), !token.empty() ){
            if (lastReturnCode != PARSE_NORMAL_EXIT){
                deleteParseTree(root);
                root = nullptr;
                lastErrorPos = reader.getCounter();
                return -1;
            }

            else if (token[0] == ')'){
                assert(token.size() == 1);
                return 1;
            }

            else if (token[0] == '('){
                assert(token.size() == 1);
                cur->next = new ParseTree;
                cur = cur ->next;
                switch (parse(cur->child, reader)){
                case -1:
                    {
                        deleteParseTree(root);
                        root = nullptr;
                        return -1;
                    }
                    //break;
                case 0:
                    {
                        lastReturnCode = PARSE_UNEXPECTED_EOF;
                        lastErrorPos = reader.getCounter();
                        deleteParseTree(root);
                        root = nullptr;
                        return -1;
                    }
                    //break;
                }
            }
            else {
                cur->next = new ParseTree(token, 0, 0);
                cur = cur->next;
            }
        }

        return 0;
    }

    void Parser::deleteParseTree(ParseTree* root){
        while (root){
            ParseTree* tmp = root;
            root = root->next;
            deleteParseTree(tmp->child);
            delete tmp;
        }
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
            while ((nextChar = getNextChar()) != '\"'){
                //Unexpected eof
                if (nextChar == std::istream::traits_type::eof()){
                    return PARSE_UNEXPECTED_EOF;
                }

                //esacape character, without valitdity check
                if (nextChar == '\\'){
                    token.push_back(nextChar);
                    nextChar = getNextChar();
                    //unexpected eof
                    if (nextChar == std::istream::traits_type::eof()){
                        return PARSE_UNEXPECTED_EOF;
                    }
                }

                token.push_back(nextChar);
            }
            token.push_back(nextChar);
        }

        //Identifier, literals
        else{
            while (nextChar != std::istream::traits_type::eof()){
                if (delimeter.find(nextChar) != std::string::npos){
                    putBackLastChar();
                    break;
                }
                token.push_back(nextChar);
                nextChar = getNextChar();
            }
        }

        return 0;
    }

}
