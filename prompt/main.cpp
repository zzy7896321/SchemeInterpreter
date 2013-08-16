#include "../config.h"
#include "prompt_declaration.h"

#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::string help_prompt =
R"(Welcome to scheme interpreter. (PPCA 2013 Summer Project)
An implementation to a subset of R5RS.

Author: Zhuoyue Zhao
Email: zzy7896321@163.com

Usage: schemeInterpreter [OPTIONS]...

Valid options:
-h                              Show this help and exit.

-l <filename>                   Load the specified file before interaction begins.
                                Multiple files can be loaded by providing multiple -l parameters.
                                Files would be loaded in the sequence they're provided.
                                By default, no files would be loaded.

-f <precision>                  Set the floating point precision. Input that is less than 64 will be ignored.
                                The default precision is 128. Only the last valid input will take effect.

-cr <length>                    Set the maximum length of inexact number literal that would be
                                stored as integer or rational. The default length is 10.
                                Set length to 0 to disable such conversion.

-mp <length>                    Set the maximum length of prefix zero that a real number will be displayed. If
                                a real number cannot be displayed in fixed point form without exceeding the limit,
                                it will be displayed in scientific form. The default length is 10.
                                Set length to 0 to disable prefix zero.

-np                             Disable protection of builtin symbols.

)";

std::string welcome_prompt =
R"(Welcome to scheme interpreter. (PPCA 2013 Summer Project)
An implementation to a subset of R5RS.
Author: Zhuoyue Zhao
Email: zzy7896321@163.com


)";

int load_arguments(int argc, char* argv[]){
    std::vector<std::string> filesToLoad;

    for (int i = 1; i < argc; ++i){
        if (strcmp("-h", argv[i]) == 0){
            std::cout << help_prompt;
            return 1;
        }
        else if (strcmp("-l", argv[i]) == 0){
            if (++i < argc){
                filesToLoad.push_back(argv[i]);
            }
        }
        else if (strcmp("-f", argv[i]) == 0){
            if (++i < argc){
                int val;
                try{
                    val = std::stoi(argv[i]);
                    if (val < 64) continue;
                    _scm_mpf_default_precision = val;
                }
                catch (...) {
                    // just ignore it
                }
            }
        }
        else if (strcmp("-cr", argv[i]) == 0){
            if (++i < argc){
                int val;
                try{
                    val = std::stoi(argv[i]);
                    if (val < 0) val = 0;
                    _scm_inexact_conversion_to_rational_bound = val;
                }
                catch (...) {
                    // just ignore it
                }
            }
        }
        else if (strcmp("-mp", argv[i]) == 0){
            if (++i < argc){
                int val;
                try{
                    val = std::stoi(argv[i]);
                    if (val < 0) val = 0;
                    _scm_maximum_prefix_zero_of_fixed = val;
                }
                catch (...) {
                    // just ignore it
                }
            }
        }
        else if (strcmp("-np", argv[i]) == 0){
            _scm_protect_builtin = false;
        }
    }

    _scm_init_all();

    std::cout << welcome_prompt;
    for (auto iter = filesToLoad.begin(); iter != filesToLoad.end(); ++iter){
        std::cout << "Loading " << *iter << std::endl;
        std::ifstream input(*iter);
        if (!input.is_open()){
            std::cout << "Failed to open the file." << std::endl << std::endl;
        }

        read_eval_loop(input, std::cout);
    }

    return 0;
}

int main(int argc, char* argv[]){

    if (load_arguments(argc, argv) != 0){
        return 0;
    }

	return read_eval_loop(std::cin, std::cout);
}
