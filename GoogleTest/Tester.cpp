//              ВНИМАНИЕ!!! 
// ТЕРРИТОРИЯ АСБОЛЮТНОГО БЕЗУМСТВА И ХАОСА
// ВХОД СО ЗДРАВЫМ СМЫСЛОМ ВОСПРЕЩЁН

#include "gtest/gtest.h"
#include <iostream>
#include <stack>
#include <string>
#include <conio.h>
#include "setcolor.h"
#include <iomanip>

using namespace std;
int Priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

long double ApplyOperation(long double Operand1, long double Operand2, char Op, bool& error) {
    switch (Op) {
    case '+': return Operand1 + Operand2;
    case '-': return Operand1 - Operand2;
    case '*': return Operand1 * Operand2;
    case '/':
        if (Operand2 == 0) {
            error = true;
            return 0;
        }
        return Operand1 / Operand2;
    default:
        error = true;
        return 0;
    }
}

long double EvaluateExpression(string& Expression, bool& error) {
    stack<long double> Values;
    stack<char> Operators;
    bool negative = false;

    for (size_t i = 0; i < Expression.length(); ++i) {
        if (Expression[i] == ' ')
            continue;

        if (isdigit(Expression[i]) || Expression[i] == '.' || (Expression[i] == '-' && (i == 0 || Expression[i - 1] == '('))) {
            long double Val = 0;
            long double Fraction = 0.1;
            bool DecimalPointFound = false;

            if (Expression[i] == '-') {
                negative = true;
                continue;
            }

            while (i < Expression.length() && (isdigit(Expression[i]) || Expression[i] == '.')) {
                if (Expression[i] != '.') {
                    if (!DecimalPointFound) {
                        Val = Val * 10 + (Expression[i] - '0');
                    }
                    else {
                        Val += (Expression[i] - '0') * Fraction;
                        Fraction *= 0.1;
                    }
                }
                else {
                    DecimalPointFound = true;
                }
                ++i;
            }
            --i;
            Values.push(negative ? -Val : Val);
            negative = false;
        }
        else if (Expression[i] == '(') {
            Operators.push(Expression[i]);
        }
        else if (Expression[i] == ')') {
            while (!Operators.empty() && Operators.top() != '(') {
                long double Operand2, Operand1;
                char Op;
                if (!Values.empty()) {
                    Operand2 = Values.top();
                    Values.pop();
                }
                else {
                    error = true;
                    return 0;
                }
                if (!Values.empty()) {
                    Operand1 = Values.top();
                    Values.pop();
                }
                else {
                    error = true;
                    return 0;
                }
                if (!Operators.empty()) {
                    Op = Operators.top();
                    Operators.pop();
                }
                else {
                    error = true;
                    return 0;
                }

                Values.push(ApplyOperation(Operand1, Operand2, Op, error));
                if (error) return 0;
            }
            if (!Operators.empty()) Operators.pop();
        }
        else {
            while (!Operators.empty() && Priority(Operators.top()) >= Priority(Expression[i])) {
                long double Operand2, Operand1;
                char Op;
                if (!Values.empty()) {
                    Operand2 = Values.top();
                    Values.pop();
                }
                else {
                    error = true;
                    return 0;
                }
                if (!Values.empty()) {
                    Operand1 = Values.top();
                    Values.pop();
                }
                else {
                    error = true;
                    return 0;
                }
                if (!Operators.empty()) {
                    Op = Operators.top();
                    Operators.pop();
                }
                else {
                    error = true;
                    return 0;
                }

                Values.push(ApplyOperation(Operand1, Operand2, Op, error));
                if (error) return 0;
            }
            Operators.push(Expression[i]);
        }
    }

    while (!Operators.empty()) {
        long double Operand2, Operand1;
        char Op;
        if (!Values.empty()) {
            Operand2 = Values.top();
            Values.pop();
        }
        else {
            error = true;
            return 0;
        }
        if (!Values.empty()) {
            Operand1 = Values.top();
            Values.pop();
        }
        else {
            error = true;
            return 0;
        }
        if (!Operators.empty()) {
            Op = Operators.top();
            Operators.pop();
        }
        else {
            error = true;
            return 0;
        }

        Values.push(ApplyOperation(Operand1, Operand2, Op, error));
        if (error) return 0;
    }

    if (Values.size() != 1) {
        error = true;
        return 0;
    }

    return Values.top();
}

bool IsOnlySpaces(const string& str) {
    for (char ch : str) {
        if (!isspace(ch)) {
            return false;
        }
    }
    return true;
}

string Calculator(string Expression, bool error) {
    if (Expression.empty() || IsOnlySpaces(Expression)) {
        return "err";
    }

    while (!Expression.empty() && Expression.back() == ' ')
        Expression.pop_back();

    if (!isdigit(Expression.back()) && Expression.back() != ')') {
        return "err";
    }

    int OpenBrackets = 0;
    int ClosedBrackets = 0;

    for (char Symbol : Expression) {
        if (Symbol == '(') {
            OpenBrackets++;
        }
        else if (Symbol == ')') {
            ClosedBrackets++;
        }

        if (!isdigit(Symbol) && Symbol != '+' && Symbol != '-' && Symbol != '*' && Symbol != '/' && Symbol != '(' && Symbol != ')' && Symbol != '.' && Symbol != ' ') {
            return "err";
        }
    }

    if (OpenBrackets != ClosedBrackets) {
        return "err";
    }

    if (error) {
        return "err";
    }
    else {
        long double result = EvaluateExpression(Expression, error);
        if (error) {
            return "err";
        }
        int precision = (result == static_cast<long long>(result)) ? 0 : 6;
        stringstream buffer;
        buffer << fixed << setprecision(precision) << result;
        string resultStr = buffer.str();                        // war crime zone
        return resultStr;
    }
    return "err";
}

bool Error = false;
string Expression;
string result;

TEST(CalculatorTest, RepeatedOperations) {
    Expression = "5 + 5 - 5 * 5 / 5";
    result = "5";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, NegativeNumbers) {
    Expression = "(-5 + (-5)) * (-1)";
    result = "10";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, LargeNumberDivision) {
    Expression = "1000000000 / 0.001";
    result = "1000000000000";
    EXPECT_EQ(result, Calculator(Expression, Error));   // "double" problem
}

TEST(CalculatorTest, FloatingPointPrecision) {
    Expression = "0.1 + 0.2 - 0.3";
    result = "0";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, NestedParentheses) {
    Expression = "(2 + (3 * (4 - 1))) / 2";
    result = "5.500000";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, SmallNumbers) {
    Expression = "0.00001 + 0.000001";
    result = "0.000011";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MixedIntegerAndFloatingPoint) {
    Expression = "5 + 2.5 * 2 - 3 / 1.5";
    result = "8";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, VeryLargeNumbers) {
    Expression = "(1000000000 * 1000000000) / (1000000000 - 100000000)";
    result = "1111111111.111111";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MixedOperationsPriority) {
    Expression = "6 + 4 / 2 * 3 - 1";
    result = "11";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MultipleNestedOperations) {
    Expression = "(((5 + 2) * 3) - 4) / 2";
    result = "8.500000";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, ZeroAndNegativeNumbers) {
    Expression = "0 * (-2 + 3) / 4";
    result = "0";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, RepeatedSingleOperation) {
    std::string expr = "1";
    for (int i = 0; i < 99; ++i) {
        expr += " - 1 + 1";
    }
    Expression = expr;
    result = "1";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MixedPositiveAndNegativeFractions) {
    Expression = "-0.5 + 0.25 - 0.75 + 1";
    result = "0";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, SequentialOperations) {
    Expression = "((5 + 3) * 2 - (4 / 2)) * 2";
    result = "28";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, SmallFractions) {
    Expression = "(0.0000000001 + 0.0000000001) * 10000000000";
    result = "2.000000";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, FractionsResultingInInteger) {
    Expression = "(2 / 3) * 3";
    result = "2";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, ComplexOperationsCombination) {
    Expression = "(3 + 4 * 2 / (1 - 5) * 2) + 10 / (2 + 3)";
    result = "1";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, DeepNestedParentheses) {
    Expression = "((((2 + 3) * 2) / 5) - 1) * 2";
    result = "2";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, OpenedBrackets1) {
    Expression = "3 + 4 * ( 5 - 1";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, OpenedBrackets2) {
    Expression = "3 + 4 * 5 - 1)";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, InvalidChar1) {
    Expression = "1 _ 1";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, InvalidChar2) {
    Expression = "1 -_ 1";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, InvalidChar3) {
    Expression = "1 + 1 + d";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, DividedByZero1) {
    Expression = "1 / 0";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, DividedByZero2) {
    Expression = "1 / ((-5) * (-1) - 5)";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, EmptyLine1) {
    Expression = " ";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, EmptyLine2) {
    Expression = "        ";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, IncompleteLine1) {
    Expression = "1 +";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, IncompleteLine2) {
    Expression = "(3 + 4 * 2 / (1  5) * 2) + 10 / (2 + 3)";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}