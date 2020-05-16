#include "parser.h"
#include <iostream>

int main(int argc, const char *argv[]) {
    Parser parser;

    int n_lines = std::stoi(argv[1]);
    try {
        std::string expr;
        for (int i = 0; i < n_lines; i++) {
            std::getline(std::cin, expr);
            std::cout << parser.parse(expr) << std::endl;
        }
        return EXIT_SUCCESS;
    } catch (const std::invalid_argument &error_msg) {
        std::cout << error_msg.what() << std::endl;
        return EXIT_FAILURE;
    }
}
