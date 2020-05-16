#include <unordered_map>
#include <iostream>

class Parser {
    int index;
    std::string parse_string;
    std::unordered_map<std::string, int> vars;

private:
    char get_next_char();
    int parse_expr();
    int parse_term();
    int parse_factor();
    std::string parse_vars(const std::string& expr);

public:
    std::string parse(const std::string& expr);
};
