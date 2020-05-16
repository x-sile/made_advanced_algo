#include "parser.h"
#include <iostream>

// number starting with zero is considered to be valid
// expression starting with '+' is considered to be invalid

char Parser::get_next_char() {
    char next_char = parse_string[index];
    index++;
    return next_char;
}

int Parser::parse_expr() {
    // expression := term ( + | - ) term

    int op1 = parse_term();
    while ((parse_string[index] == '+') || (parse_string[index] == '-')) {
        char operation = get_next_char();
        int op2 = parse_term();
        if (operation == '+') {
            op1 = op1 + op2;
        } else {
            op1 = op1 - op2;
        }
    }
    return op1;
}

int Parser::parse_term() {
    // term := factor ( * | / ) factor

    int op1 = parse_factor();
    while ((parse_string[index] == '*') || (parse_string[index] == '/')) {
        char operation = get_next_char();
        int op2 = parse_factor();
        if (operation == '*') {
            op1 = op1 * op2;
        } else {
            if (op2 == 0) {
                throw std::invalid_argument("Division by zero");
            }
            op1 = op1 / op2;
        }
    }
    return op1;

}

int Parser::parse_factor() {
    // factor := NUMBER | (expression)

    std::string cur_string;

    char cur_char = get_next_char();
    while ((cur_char == '-') || (cur_char == ' ')) {
        if (cur_char == '-') {
            if (cur_string == "-") {
                cur_string = "";
            } else {
                cur_string = "-";
            }
        }
        cur_char = get_next_char();
    }
    if (isdigit(cur_char)) {
        cur_string += cur_char;
        while (isdigit(parse_string[index]) || (parse_string[index] == ' ')) {
            if (parse_string[index] == ' ') {
                index++;
            } else {
                cur_string += get_next_char();
            }
        }
        try {
            return std::stoi(cur_string);
        } catch (std::out_of_range &error_msg) {
            throw std::invalid_argument("Value " + cur_string + " is too large for int");
        }
    } else if (cur_char == '(') {
        int expr = parse_expr();
        if (parse_string[index] != ')') {
            throw std::invalid_argument("Missing ')'");
        } else {
            index++;
            while (parse_string[index] == ' ') {
                index++;
            }
        }
        return expr;
    } else {
        throw std::invalid_argument("Invalid expression");
    }

}

std::string Parser::parse_vars(const std::string &expr) {
    if (expr.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    // skip spaces
    while ((index < expr.size()) && (expr[index] == ' ')) {
        index++;
    }
    if (index == expr.size()) {
        throw std::invalid_argument("Empty expression");
    }

    // read variable name
    std::string var_name;
    while ((index < expr.size()) && isalpha(expr[index])) {
        var_name += expr[index];
        index++;
    }
    if (var_name.empty()) {
        throw std::invalid_argument("Invalid variable name");
    }

    // skip spaces
    while ((index < expr.size()) && (expr[index] == ' ')) {
        index++;
    }
    if ((index == expr.size()) || (expr[index] != '=')) {
        throw std::invalid_argument("Invalid expression");
    }
    index++; // skip "="

    // replace variables in expression
    std::string var;
    while (index < expr.size()) {
        if (isalpha(expr[index])) {
            var += expr[index];
        } else {
            if (!var.empty()) {
                if (vars.find(var) != vars.end()) {
                    parse_string += std::to_string(vars[var]);
                    var = "";
                } else {
                    throw std::invalid_argument("Unknown variable " + var);
                }
            } else {
                parse_string += expr[index];
            }
        }
        index++;
    }
    if (!var.empty()) {
        if (vars.find(var) != vars.end()) {
            parse_string += std::to_string(vars[var]);
        }
    }

    index = 0;
    vars[var_name] = parse_expr();

    return var_name + " = " + std::to_string(vars[var_name]);
}

std::string Parser::parse(const std::string &expr) {
    parse_string = "";
    index = 0;
    std::string result = parse_vars(expr);

    if (index != parse_string.length()) {
        throw std::invalid_argument("Invalid expression");
    }
    return result;
}
