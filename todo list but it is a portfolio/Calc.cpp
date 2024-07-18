#include "Calc.h"
#include <iostream>
#include <stack>
#include <string>
#include <conio.h>
#include "setcolor.h"
#include <iomanip>
#include "gtest/gtest.h"

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

void CalculatorMain() {
    string Expression;
    bool error = false;
    cout << "Отрицательные числа должны быть заключены в скобки (кроме начала выражения)\n";
    cout << "Введите выражение: ";
    getline(cin, Expression);
    string result = Calculator(Expression, error);
    cout << "Результат: " << result;
    cout << "\nНажмите на любую кнопку для продолжения";
    _getch();
}



TEST(CalculatorTest, RepeatedOperations) {
    bool Error = false;
    string Expression = "5 + 5 - 5 * 5 / 5";
    string result = "5";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, NegativeNumbers) {
    bool Error = false;
    string Expression = "(-5 + (-5)) * (-1)";
    string result = "10";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, LargeNumberDivision) {
    bool Error = false;
    string Expression = "1000000000 / 0.001";
    string result = "1000000000000";
    EXPECT_EQ(result, Calculator(Expression, Error));   // "double" problem
}

TEST(CalculatorTest, FloatingPointPrecision) {
    bool Error = false;
    string Expression = "0.1 + 0.2 - 0.3";
    string result = "0";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, NestedParentheses) {
    bool Error = false;
    string Expression = "(2 + (3 * (4 - 1))) / 2";
    string result = "5.500000";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, SmallNumbers) {
    bool Error = false;
    string Expression = "0.00001 + 0.000001";
    string result = "0.000011";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MixedIntegerAndFloatingPoint) {
    bool Error = false;
    string Expression = "5 + 2.5 * 2 - 3 / 1.5";
    string result = "8";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, VeryLargeNumbers) {
    bool Error = false;
    string Expression = "(1000000000 * 1000000000) / (1000000000 - 100000000)";
    string result = "1111111111.111111";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MixedOperationsPriority) {
    bool Error = false;
    string Expression = "6 + 4 / 2 * 3 - 1";
    string result = "11";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MultipleNestedOperations) {
    bool Error = false;
    string Expression = "(((5 + 2) * 3) - 4) / 2";
    string result = "8.500000";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, ZeroAndNegativeNumbers) {
    bool Error = false;
    string Expression = "0 * (-2 + 3) / 4";
    string result = "0";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, RepeatedSingleOperation) {
    bool Error = false;
    string expr = "1";
    for (int i = 0; i < 99; ++i) {
        expr += " - 1 + 1";
    }
    string Expression = expr;
    string result = "1";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, MixedPositiveAndNegativeFractions) {
    bool Error = false;
    string Expression = "-0.5 + 0.25 - 0.75 + 1";
    string result = "0";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, SequentialOperations) {
    bool Error = false;
    string Expression = "((5 + 3) * 2 - (4 / 2)) * 2";
    string result = "28";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, SmallFractions) {
    bool Error = false;
    string Expression = "(0.0000000001 + 0.0000000001) * 10000000000";
    string result = "2.000000";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, FractionsResultingInInteger) {
    bool Error = false;
    string Expression = "(2 / 3) * 3";
    string result = "2";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, ComplexOperationsCombination) {
    bool Error = false;
    string Expression = "(3 + 4 * 2 / (1 - 5) * 2) + 10 / (2 + 3)";
    string result = "1";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTest, DeepNestedParentheses) {
    bool Error = false;
    string Expression = "((((2 + 3) * 2) / 5) - 1) * 2";
    string result = "2";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, OpenedBrackets) {
    bool Error = false;
    string Expression = "3 + 4 * ( 5 - 1";
    string result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
    Expression = "3 + 4 * 5 - 1)";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, InvalidChar) {
    bool Error = false;
    string Expression = "1 _ 1";
    string result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
    Expression = "1 -_ 1";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
    Expression = "1 + 1 + d";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, DividedByZero) {
    bool Error = false;
    string Expression = "1 / 0";
    string result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
    Expression = "1 / ((-5) * (-1) - 5)";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, EmptyLine) {
    bool Error = false;
    string Expression = " ";
    string result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
    Expression = "        ";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}

TEST(CalculatorTestForErrors, IncompleteLine) {
    bool Error = false;
    string Expression = "1 +";
    string result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
    Expression = "(3 + 4 * 2 / (1  5) * 2) + 10 / (2 + 3)";
    result = "err";
    EXPECT_EQ(result, Calculator(Expression, Error));
}