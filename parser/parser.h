/** @file
 *	Parser.h contains the declaration of parser classes.
 */
#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>
#include <iostream>
#include <utility>
#include <cctype>
#include <memory>

#include "parsetree.h"

/**
 * Parser classes are all in namespace ParserEnv.
 */
namespace ParserEnv{

/**
 * StreamReader manage an std::istream object and consecutively extract tokens from it. It cannot be copied, copy-constructed, or default-constructed.
 */
class StreamReader{
public:

	/**
	 * No default constructor.
	 */
	StreamReader() = delete;

	/**
	 * No copy constructor.
	 */
	StreamReader(const StreamReader&) = delete;

	/**
	 * No assignement operator.
	 */
	StreamReader& operator=(const StreamReader&) = delete;

	/**
	 * The only constructor that constructs the StreamReader from a given std::istream. The stream has to be guaranteed valid before any call to members of StreamReader.
	 */
    StreamReader(std::istream& _stream):stream(_stream), counter(0){}

	/**
	 * Returns true if there still is a next token in the stream. Returns false otherwise.
	 */
    bool hasNextToken();

	/**
	 * Store the next token in the reference of std::string. Return value indicates errors encountered. If any error is encountered, the function immediately returns so that the position of the error in the stream could be retrieved by invoking getCounter().
	 */
    int getNextToken(std::string&);

	/**
	 * Reset the position counter.
	 */
    void resetCounter(){
        counter = 0;
    }

	/**
	 * Get the position counter. If previous getNextToken call encounters any error, the return value indicates the position of the error in the stream. The counter begins at 0.
	 */
    int getCounter() const{
        return counter-1;
    }

private:

	/**
	 * The input stream.
	 */
    std::istream& stream;

	/**
	 * The counter of inputed characters.
	 */
	int counter;

	/**
	 * Returns the next character in the stream. Returned character is turned into lower case. The counter is increased by 1.
	 */
    inline char getNextChar(){
        ++counter;
        return std::tolower(stream.get());
    }

    /**
     * Returns the next character in the stream. The counter is increased by 1.
     */
    inline char getNextCharRaw(){
        ++counter;
        return stream.get();
    }

	/**
	 * Put back the last extracted character. The counter is decreased by 1. Behaviour is unspecified if no character is ever extracted or this function is consecutively invoked.
	 */
    inline void putBackLastChar(){
        --counter;
        stream.unget();
    }

};

/**
 * Parser provide methods to parse Scheme expressions from given string, file and stream. If any error has been encounterd, the error code and position of the error in the stream could be retrieved by invoking getLastReturnCode() and getLastErrorPos().
 */
class Parser{
public:

	/**
	 * Parse the expressions in the std::string. The string is passed by const lvalue reference. If parsing is successful, a pointer to a dynamically allocated ParseTree using new operator is returned. Otherwise, return value would be nullptr. Error code and position could be retrieved by invoking getLastReturnCode() and getLastErrorPos().
	 */
	static std::shared_ptr<ParseTree> parseFromString(const std::string& _expr);

	/**
	 * Parse the expressions in the std::String. The string is passed by rvalue reference. If parsing is successful, a pointer to a dynamically allocated ParseTree using new operator is returned. Otherwise, return value would be nullptr. Error code and position could be retrieved by invoking getLastReturnCode() and getLastErrorPos().
	 */
	static std::shared_ptr<ParseTree> parseFromString(std::string&& _expr);

	/**
	 * Parse the expressions in the file indicated by the file name passed to the function. If parsing is successful, a pointer to a dynamically allocated ParseTree using new operator is returned. Otherwise, return value would be nullptr. Error code and position could be retrieved by invoking getLastReturnCode() and getLastErrorPos().
	 */
    static std::shared_ptr<ParseTree> parseFromFile(const std::string& _filename);

    /**
     * Parse only one expression in the given stream.
     */
    static std::shared_ptr<ParseTree> parseOneFromStreamReader(StreamReader& _stream);

	/**
	 * Returns the last parsing error code.
	 * Possible Codes are:
	 */
    static int getLastReturnCode(){
        return lastReturnCode;
    }

	/**
	 * Returns the position of the last error in the stream. Index starts at 0.
	 */
    static int getLastErrorPos(){
        return lastErrorPos;
    }

    /**
     * Try to convert the string into a number. Default radix has to be 2, 8, 10 or 16.
     */
	static int parseNumber(std::shared_ptr<ObjectDef::Object>& obj, const std::string& str, int default_radix = 10);
private:

	/**
	 * Internal implementation of parsing procedure. Returns error code.
	 */
    static int parse(std::shared_ptr<ParseTree>&, StreamReader&);

	/**
	 * Error code of last parsing.
	 */
    static int lastReturnCode;

	/**
	 * Error position of last parsing.
	 */
	static int lastErrorPos;

	/**
	 * A shared temporary string.
	 */
    static std::string token;

    /**
     * Extract next complete expression from the stream reader.
     */
	static int getNextExpression(std::shared_ptr<ParseTree>& root, StreamReader& reader);

    /**
     * Try to extract a real number from the string when gien the internal state. Only for internal use.
     */
    static int parseRealNumber(std::shared_ptr<ObjectDef::Object>& obj, std::string&& str,
                               int exactness, int radix);
};

/** Declaration of the string containing all the delimiters. */
extern const std::string delimiter;

/**	Declaration of the string containing all the reserved characters. */
extern const std::string reservedChar;

/** Declaration of the string containing all the special initials of identifiers. */
extern const std::string specialInitial;

/** Declaratoin of the string containing all the special subsequent of identifiers. */
extern const std::string specialSubsequent;

/** No error encountered.*/
constexpr int PARSE_NORMAL_EXIT = 0;

/** Reserved characters encountered. */
constexpr int PARSE_USE_OF_RESERVED = 1;

/** EOF encountered before string ends.*/
constexpr int PARSE_UNEXPECTED_EOF = 2;

/** Right bracket does not match with any left bracket.*/
constexpr int PARSE_UNEXPECTED_RBRACKET = 3;

/** Unrecognized literal */
constexpr int PARSE_UNRECOGNIZED_LITERAL = 4;

/** Invalid number for the given radix */
constexpr int PARSE_INVALID_NUMBER = 5;

/** Number overflow */
constexpr int PARSE_NUMBER_OVERFLOW = 6;

/** Division by zero */
constexpr int PARSE_DIVISION_BY_ZERO = 7;

/** Invalid Identifier */
constexpr int PARSE_INVALID_IDENTIFIER = 8;

/** Prompt for parsing error. */
extern const std::string parsingError[];

}

#endif // PARSER_H_INCLUDED
