#include "Calc.h"
#include <iostream>
#include <stack>
#include <string>
#include <conio.h>
#include "setcolor.h"

using namespace std;

enum class ParseError {
    DIVISION_BY_ZERO,
    SYNTAX_ERROR,
    UNCLOSED_BRACKETS,
    INVALID_CHARACTERS,
    EMPTY,
    NONE
};

class Operation {
public:
    virtual double Calculate() const = 0;
    virtual string GetStructure() const = 0;    // для тестирования   
    virtual ~Operation() {}
};

class Number : public Operation {
public:
    Number(double _value) : value(_value) {}
    double Calculate() const override {
        return value;
    }
    string GetStructure() const override {
        return "Number(" + to_string(value) + ")";   // для тестирования
    }
private:
    double value;
};

class Add : public Operation {
public:
    Add(Operation* _left, Operation* _right) : left(_left), right(_right) {}
    double Calculate() const override {
        return left->Calculate() + right->Calculate();
    }
    string GetStructure() const override {
        return "Add(" + left->GetStructure() + ", " + right->GetStructure() + ")";   // для тестирования
    }
private:
    unique_ptr<Operation> left;
    unique_ptr<Operation> right;
};

class Subtract : public Operation {
public:
    Subtract(Operation* _left, Operation* _right) : left(_left), right(_right) {}
    double Calculate() const override {
        return left->Calculate() - right->Calculate();
    }
    string GetStructure() const override {
        return "Subtract(" + left->GetStructure() + ", " + right->GetStructure() + ")";   // для тестирования
    }
private:
    unique_ptr<Operation> left;
    unique_ptr<Operation> right;
};

class Multiply : public Operation {
public:
    Multiply(Operation* _left, Operation* _right) : left(_left), right(_right) {}
    double Calculate() const override {
        return left->Calculate() * right->Calculate();
    }
    string GetStructure() const override {
        return "Multiply(" + left->GetStructure() + ", " + right->GetStructure() + ")";   // для тестирования
    }
private:
    unique_ptr<Operation> left;
    unique_ptr<Operation> right;
};

class Divide : public Operation {
public:
    Divide(Operation* _left, Operation* _right) : left(_left), right(_right) {}
    double Calculate() const override {
        return left->Calculate() / right->Calculate();
    }
    string GetStructure() const override {
        return "Divide(" + left->GetStructure() + ", " + right->GetStructure() + ")";   // для тестирования
    }
private:
    unique_ptr<Operation> left;
    unique_ptr<Operation> right;
};

class ExpressionParser {
public:
    ExpressionParser(const string& expr) : expression(expr), errorCode(ParseError::NONE) {
        removeSpaces();
    }

    unique_ptr<Operation> parse() {
        if (expression.empty()) {
            errorCode = ParseError::EMPTY;
            return nullptr;
        }
        tokenize();
        if (errorCode != ParseError::NONE) {
            return nullptr;
        }
        processOperators();
        return move(values.top());
    }

    ParseError getErrorCode() const {
        return errorCode;
    }

private:
    string expression;
    stack<unique_ptr<Operation>> values;
    stack<char> operators;
    ParseError errorCode;
    int parenthesesBalance = 0;

    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    int getPrecedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    unique_ptr<Operation> applyOperation(char op, unique_ptr<Operation> left, unique_ptr<Operation> right) {
        if (op == '/' && right->Calculate() == 0) {
            errorCode = ParseError::DIVISION_BY_ZERO;
            return nullptr;
        }
        switch (op) {
        case '+': return make_unique <Add>(left.release(), right.release());
        case '-': return make_unique <Subtract>(left.release(), right.release());
        case '*': return make_unique <Multiply>(left.release(), right.release());
        case '/': return make_unique <Divide>(left.release(), right.release());
        default: return nullptr;
        }
    }

    void processOperators() {
        while (!operators.empty()) {
            applyTopOperator();
        }
    }

    void processCurrentOperator(char op) {
        while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(op)) {
            applyTopOperator();
        }
        operators.push(op);
    }

    void applyTopOperator() {
        char op = operators.top();
        operators.pop();

        unique_ptr<Operation> right = move(values.top());
        values.pop();
        unique_ptr<Operation> left = move(values.top());
        values.pop();

        values.push(applyOperation(op, move(left), move(right)));
    }

    void removeSpaces() {
        expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
    }

    bool isValidNonInteger(size_t index) {
        return expression[index] == '.' && index + 1 < expression.length() && isdigit(expression[index + 1]);
    }

    bool isValidNegative(size_t index) {
        return expression[index] == '-' && (index == 0 || expression[index - 1] == '(') && index + 1 < expression.length() && isdigit(expression[index + 1]);
    }

    bool isNumberStart(size_t index) {
        if (index >= expression.length()) return false; // защита от вылета с index + 1
        if (isdigit(expression[index])) return true;
        if (isValidNonInteger(index)) return true;
        if (isValidNegative(index)) {
            parenthesesBalance++;
            return true;
        }
        return false;
    }

    void handleNumber(size_t& index) {
        size_t length;
        double number = stod(expression.substr(index), &length);
        values.push(make_unique<Number>(number));
        if ((expression[index + length] == ')' && isValidNegative(index)) || (index == 0 && isValidNegative(index))) {
            parenthesesBalance--;
        }
        index += length;
    }

    bool isOperatorAtStartOfExpression(size_t index) {
        return values.empty() && expression[index] != '-';
    }

    bool isInvalidOperatorPrecedence(size_t& index) {
        return !values.empty() && !operators.empty() && isOperator(expression[index - 1]) && expression[index - 1] != '(';
    }

    void handleOperator(size_t& index) {
        if (isOperatorAtStartOfExpression(index) || isInvalidOperatorPrecedence(index)) {
            errorCode = ParseError::SYNTAX_ERROR;
        }
        else {
            processCurrentOperator(expression[index]);
        }
        index++;
    }

    void handleParentheses(size_t& index) {
        if (expression[index] == '(') {
            operators.push(expression[index]);
            parenthesesBalance++;
        }
        else if (expression[index] == ')') {
            parenthesesBalance--;
            while (!operators.empty() && operators.top() != '(') {
                applyTopOperator();
            }
            if (!(operators.empty() || parenthesesBalance < 0)) {
                operators.pop();
            }
        }
        index++;
    }

    bool isParentheses(size_t& index) {
        return expression[index] == '(' || expression[index] == ')';
    }

    bool isValidChar(char index) {
        return isdigit(index) || index == '+' || index == '-' || index == '*' || index == '/' || index == '(' || index == ')' || index == '.';
    }

    void tokenize() {
        for (size_t index = 0; index < expression.length();) {
            if (!isValidChar(expression[index])) {
                errorCode = ParseError::INVALID_CHARACTERS;
                return;
            }
            else if (isParentheses(index)) {
                handleParentheses(index);
            }
            else if (isNumberStart(index)) {
                handleNumber(index);
            }
            else if (isOperator(expression[index])) {
                handleOperator(index);
            }
        }
        if (parenthesesBalance != 0) {
            errorCode = ParseError::UNCLOSED_BRACKETS;
            return;
        }

        if (isOperator(expression.back())) {
            errorCode = ParseError::SYNTAX_ERROR;
            return;
        }
    }
};

void PrintError(ParseError error) {
    SetColor(31);
    switch (error) {
    case ParseError::DIVISION_BY_ZERO: cout << "Ошибка: Деление на ноль!" << endl; break;
    case ParseError::SYNTAX_ERROR: cout << "Ошибка: Неккоректное выражение!" << endl; break;
    case ParseError::UNCLOSED_BRACKETS: cout << "Ошибка: Незакрытые скобки в выражении!" << endl; break;
    case ParseError::INVALID_CHARACTERS: cout << "Ошибка: Некорректные символы в выражении!" << endl; break;
    case ParseError::EMPTY: cout << "Ошибка: Пустое выражение!" << endl; break;
    }
    SetColor(0);
}

void CalculatorMain() {
    string expression;
    cout << "Отрицательные числа должны быть заключены в скобки (кроме начала выражения)\n";
    cout << "Введите выражение: ";
    getline(cin, expression);

    ExpressionParser Tree(expression);
    unique_ptr<Operation> operation = Tree.parse();

    if (Tree.getErrorCode() != ParseError::NONE) {
        PrintError(Tree.getErrorCode());
    }
    else {
        cout << "Результат: " << operation->Calculate();
    }

    cout << "\nНажмите на любую кнопку для продолжения";
    _getch();
}