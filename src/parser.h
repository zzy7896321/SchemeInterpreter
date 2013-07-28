#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>
#include <iostream>
#include <utility>
#include <cctype>

namespace ParserEnv{

using std::string;

struct ParseTree{
    ParseTree();
    ParseTree(const string& token, ParseTree* next, ParseTree* child);
    ParseTree(string&& token, ParseTree* next, ParseTree* child);

    ParseTree& operator=(const ParseTree&) = delete;
    ParseTree& operator=(ParseTree&&) = delete;
    ParseTree(const ParseTree&) = delete;
    ParseTree(ParseTree&&) = delete;

    string token;
    ParseTree* next;
    ParseTree* child;

    static void debugPrint(ParseTree*);
};

class StreamReader{
public:
    StreamReader() = delete;
    StreamReader(const StreamReader&) = delete;
    StreamReader& operator=(const StreamReader&) = delete;

    StreamReader(std::istream& _stream):stream(_stream), counter(0){}

    bool hasNextToken();

    int getNextToken(std::string&);

    void resetCounter(){
        counter = 0;
    }

    int getCounter() const{
        return counter-1;
    }

private:
    std::istream& stream;
    int counter;

    inline char getNextChar(){
        ++counter;
        return tolower(stream.get());
    }

    inline void putBackLastChar(){
        --counter;
        stream.unget();
    }

};

class Parser{
public:
    static ParseTree* parseFromString(const string& _expr);
    static ParseTree* parseFromString(string&& _expr);

    static ParseTree* parseFromFile(const string& _filename);

    static void deleteParseTree(ParseTree*);

    static int getLastReturnCode(){
        return lastReturnCode;
    }
    static int getLastErrorPos(){
        return lastErrorPos;
    }
private:
    static int parse(ParseTree*&, StreamReader&);

    static int lastReturnCode;
    static int lastErrorPos;

    static std::string token;   //as a shared temporary string
};

extern const std::string delimiter;
extern const std::string reservedChar;

constexpr int PARSE_NORMAL_EXIT = 0;
constexpr int PARSE_USE_OF_RESERVED = 1;
constexpr int PARSE_UNEXPECTED_EOF = 2;
constexpr int PARSE_UNEXPECTED_RBRACKET = 3;

}

#endif // PARSER_H_INCLUDED
