#include "parser.h"
#include <iostream>
#include "cassert"

using namespace std;

bool base_test(Parser& parser, string& expr) {
    assert(parser.parse(expr.assign("a = 2")) == "a = 2");
    assert(parser.parse(expr.assign("b = 3")) == "b = 3");
    assert(parser.parse(expr.assign("c = 2 * 5")) == "c = 10");
    assert(parser.parse(expr.assign("d = c - a")) == "d = 8");
    return  true;
}

bool advanced_test(Parser& parser, string& expr) {
    assert(parser.parse(expr.assign("  a   =   2     +   2")) == "a = 4");
    assert(parser.parse(expr.assign("b = a + (2 + (10 / 4) * 3) + 5 / 1")) == "b = 17");
    assert(parser.parse(expr.assign("c = b --a + 03 * -1")) == "c = 18");
    assert(parser.parse(expr.assign("c = 5 / 1")) == "c = 5");
    assert(parser.parse(expr.assign("c = c * 2")) == "c = 10");
    return true;
}

bool test_unknown_var(Parser& parser, string& expr) {
    try {
        parser.parse(expr.assign("  a   =   e     +   2"));
    } catch (std::invalid_argument &err_msg) {
        return true;
    }
    return false;
}

bool test_invalid_var(Parser& parser, string& expr) {
    try {
        parser.parse(expr.assign("  a123   =   a     +   2"));
    } catch (std::invalid_argument &err_msg) {
        return true;
    }
    return false;
}

bool test_invalid_expr1(Parser& parser, string& expr) {
    try {
        parser.parse(expr.assign("c = 3.0"));
    } catch (std::invalid_argument &err_msg) {
        return true;
    }
    return false;
}

bool test_invalid_expr2(Parser& parser, string& expr) {
    try {
        parser.parse(expr.assign("c = a + (2 + 3"));
    } catch (std::invalid_argument &err_msg) {
        return true;
    }
    return false;
}

int main() {
    Parser parser;
    string expr;

    assert(base_test(parser, expr) == true);
    assert(advanced_test(parser, expr) == true);
    assert(test_unknown_var(parser, expr) == true);
    assert(test_invalid_var(parser, expr) == true);
    assert(test_invalid_expr1(parser, expr) == true);
    assert(test_invalid_expr2(parser, expr) == true);

    cout << "Tests passed!" << endl;
    return EXIT_SUCCESS;
}
