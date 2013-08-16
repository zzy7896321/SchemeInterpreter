#include <string>
#include "parser.h"

namespace ParserEnv{

const std::string parsingError[] = {
									"",
									"Reserved character encountered.",
									"Unexpected eof.",
									"unexpected ')'.",
									"Unknown literal.",
									"Invalid number literal.",
									"Exact number too large.",
									"Division by 0 in fraction.",
									"Invalid identifier"
									};

}
