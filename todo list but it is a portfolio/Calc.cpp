#include "Calc.h"
#include <iostream>
#include <stack>
#include <string>
#include <conio.h>
#include "setcolor.h"
#include <iomanip>
#include <algorithm>

using namespace std;

enum ErrorCode { // список ошибок
    NO_ERROR,
    DIVISION_BY_ZERO,
    SYNTAX_ERROR,
    UNCLOSED_BRACKETS,
    INVALID_CHARACTERS,
    NOT_FULL,
    EMPTY
};

void PrintError(ErrorCode error) {
    SetColor(31);
    switch (error) {
    case DIVISION_BY_ZERO: cout << "Ошибка: Деление на ноль!" << endl; break;
    case SYNTAX_ERROR: cout << "Ошибка: Синтаксическая ошибка!" << endl; break;
    case UNCLOSED_BRACKETS: cout << "Ошибка: Незакрытые скобки в выражении!" << endl; break;
    case INVALID_CHARACTERS: cout << "Ошибка: Некорректные символы в выражении!" << endl; break;
    case NOT_FULL: cout << "Ошибка: Незавершенное выражение!" << endl; break;
    case EMPTY: cout << "Ошибка: Пустое выражение!" << endl; break;
    default: cout << "Ошибка: Некорректное выражение!" << endl; break;
    }
    SetColor(0);

    cout << "\nНажмите на любую кнопку для продолжения";
    _getch();
}

int Priority(char op) {
    return (op == '+' || op == '-') ? 1 : (op == '*' || op == '/') ? 2 : 0;
}

long double ApplyOperation(long double operand1, long double operand2, char op, ErrorCode& error) {
    switch (op) {
    case '+': return operand1 + operand2;
    case '-': return operand1 - operand2;
    case '*': return operand1 * operand2;
    case '/':
        if (operand2 == 0) {
            error = DIVISION_BY_ZERO;
            return 0;
        }
        return operand1 / operand2;
    default:
        error = SYNTAX_ERROR;
        return 0;
    }
}

bool IsDigitOrDot(char ch) {
    return isdigit(ch) || ch == '.';
}

bool IsValidChar(char ch) {
    return isdigit(ch) || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '.' || ch == ' ';
}

long double EvaluateExpression(const string& expression, ErrorCode& error) {
    stack<long double> values;
    stack<char> operators;
    bool negative = false;

    auto applyTopOperation = [&](stack<long double>& values, stack<char>& operators, ErrorCode& error) {
        if (values.size() < 2 || operators.empty()) {
            error = SYNTAX_ERROR;
            return;
        }
        long double operand2 = values.top(); values.pop();
        long double operand1 = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(ApplyOperation(operand1, operand2, op, error));
    };

    for (size_t i = 0; i < expression.length(); ++i) {
        if (isspace(expression[i])) continue;

        if (IsDigitOrDot(expression[i]) || (expression[i] == '-' && (i == 0 || expression[i - 1] == '('))) {
            long double val = 0;
            long double fraction = 0.1;
            bool decimalPointFound = false;

            if (expression[i] == '-') {
                negative = true;
                continue;
            }

            while (i < expression.length() && IsDigitOrDot(expression[i])) {
                if (expression[i] != '.') {
                    if (!decimalPointFound) {
                        val = val * 10 + (expression[i] - '0');
                    }
                    else {
                        val += (expression[i] - '0') * fraction;
                        fraction *= 0.1;
                    }
                }
                else {
                    decimalPointFound = true;
                }
                ++i;
            }
            --i;
            values.push(negative ? -val : val);
            negative = false;
        }
        else if (expression[i] == '(') {
            operators.push(expression[i]);
        }
        else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                applyTopOperation(values, operators, error);
                if (error != NO_ERROR) return 0;
            }
            if (!operators.empty()) operators.pop();
        }
        else {
            while (!operators.empty() && Priority(operators.top()) >= Priority(expression[i])) {
                applyTopOperation(values, operators, error);
                if (error != NO_ERROR) return 0;
            }
            operators.push(expression[i]);
        }
    }

    while (!operators.empty()) {
        applyTopOperation(values, operators, error);
        if (error != NO_ERROR) return 0;
    }

    if (values.size() != 1) {
        error = SYNTAX_ERROR;
        return 0;
    }

    return values.top();
}

void Calculator() {
    string expression;
    ErrorCode error = NO_ERROR;

    cout << "Отрицательные числа должны быть заключены в скобки (кроме начала выражения)\n";
    cout << "Введите выражение: ";
    getline(cin, expression);

    expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

    if (expression.empty()) {
        PrintError(EMPTY);
        return;
    }

    if (!isdigit(expression.back()) && expression.back() != ')') {
        PrintError(NOT_FULL);
        return;
    }

    int openBrackets = count(expression.begin(), expression.end(), '(');
    int closedBrackets = count(expression.begin(), expression.end(), ')');

    if (openBrackets != closedBrackets) {
        PrintError(UNCLOSED_BRACKETS);
        return;
    }

    if (any_of(expression.begin(), expression.end(), [](char ch) { return !IsValidChar(ch); })) {
        PrintError(INVALID_CHARACTERS);
        return;
    }

    long double result = EvaluateExpression(expression, error);
    if (error != NO_ERROR) {
        PrintError(error);
        return;
    }

    int precision = (result == static_cast<long long>(result)) ? 0 : 6;
    cout << "Результат: " << fixed << setprecision(precision) << result << endl;

    cout << "\nНажмите на любую кнопку для продолжения";
    _getch();
}