#include "Calc.h"
#include <iostream>
#include <stack>
#include <string>
#include <conio.h>
#include "setcolor.h"
#include <iomanip>
#include "gtest/gtest.h"

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
        case '+': return make_unique <Add> (left.release(), right.release());
        case '-': return make_unique <Subtract> (left.release(), right.release());
        case '*': return make_unique <Multiply> (left.release(), right.release());
        case '/': return make_unique <Divide> (left.release(), right.release());
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

///////////////////////////////////////
//////////ТЕСТЫ ДЛЯ ДЕРЕВЬЕВ///////////
///////////////////////////////////////

TEST(ClassTest, NumberTest) {
    Number number(5.0);
    EXPECT_EQ(number.Calculate(), 5.0);
}

TEST(ClassTest, AddTest) {
    auto left = make_unique<Number>(5.0);
    auto right = make_unique<Number>(1.0);
    Add add(left.release(), right.release());
    EXPECT_EQ(add.Calculate(), 6.0);
}

TEST(ClassTest, SubtractTest) {
    auto left = make_unique<Number>(5.0);
    auto right = make_unique<Number>(1.0);
    Subtract sub(left.release(), right.release());
    EXPECT_EQ(sub.Calculate(), 4.0);
}

TEST(ClassTest, MultiplyTest) {
    auto left = make_unique<Number>(5.0);
    auto right = make_unique<Number>(2.0);
    Multiply mul(left.release(), right.release());
    EXPECT_EQ(mul.Calculate(), 10.0);
}

TEST(ClassTest, DivideTest) {
    auto left = make_unique<Number>(5.0);
    auto right = make_unique<Number>(2.0);
    Divide div(left.release(), right.release());
    EXPECT_EQ(div.Calculate(), 2.5);
}

TEST(CalculationTest, Expression1) {
    // (3 + (2 * 4)) = 11
    auto expr = make_unique<Add>(
        new Number(3),
        new Multiply(
            new Number(2),
            new Number(4)
        )
    );
    EXPECT_EQ(expr->Calculate(), 11);
}

TEST(CalculationTest, Expression2) {
    // ((7 - 2) * (8 / 4)) = 10
    auto expr = make_unique<Multiply>(
        new Subtract(
            new Number(7),
            new Number(2)
        ),
        new Divide(
            new Number(8),
            new Number(4)
        )
    );
    EXPECT_EQ(expr->Calculate(), 10);
}

TEST(CalculationTest, Expression3) {
    // (10 / (5 - 3)) + 6 = 11
    auto expr = make_unique<Add>(
        new Divide(
            new Number(10),
            new Subtract(
                new Number(5),
                new Number(3)
            )
        ),
        new Number(6)
    );
    EXPECT_EQ(expr->Calculate(), 11);
}

TEST(CalculationTest, ExpressionWithNegative) {
    // -10 * (3 + (-1)) / (-2) = 10
    auto expr = make_unique<Divide>(
        new Multiply(
            new Number(-10),
            new Add(
                new Number(3),
                new Number(-1)
            )
        ),
        new Number(-2)
    );
    EXPECT_EQ(expr->Calculate(), 10);
}

///////////////////////////////////////
//////////ТЕСТЫ ДЛЯ ПАРСЕРА////////////
///////////////////////////////////////

TEST(ExpressionParserTest, NoSpaces) { // я проглядел самое очевидное
    ExpressionParser Tree("3+4*5-6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(3.000000), Multiply(Number(4.000000), Number(5.000000))), Number(6.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, СombinedSpaces) {
    ExpressionParser Tree("3+4 * 5- 6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(3.000000), Multiply(Number(4.000000), Number(5.000000))), Number(6.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, SingleNumber) {
    ExpressionParser Tree ("42");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Number(42.000000)");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, AdditionStructure) {
    ExpressionParser Tree("3 + 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(3.000000), Number(4.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, SubtractionStructure) {
    ExpressionParser Tree("7 - 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Number(7.000000), Number(2.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, MultiplicationStructure) {
    ExpressionParser Tree("2 * 3");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Multiply(Number(2.000000), Number(3.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, DivisionStructure) {
    ExpressionParser Tree("8 / 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Divide(Number(8.000000), Number(4.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ComplexExpression) {
    ExpressionParser Tree("3 + 4 * 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(3.000000), Multiply(Number(4.000000), Number(2.000000)))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ComplexExpression1) {
    ExpressionParser Tree("3 + 4 * 5 - 6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(3.000000), Multiply(Number(4.000000), Number(5.000000))), Number(6.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ComplexExpression2) {
    ExpressionParser Tree("7 * 8 / 2 + 3");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Divide(Multiply(Number(7.000000), Number(8.000000)), Number(2.000000)), Number(3.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ComplexExpression3) {
    ExpressionParser Tree("10 + 11 - 12 * 13 / 14");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(10.000000), Number(11.000000)), Divide(Multiply(Number(12.000000), Number(13.000000)), Number(14.000000)))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ComplexExpression4) {
    ExpressionParser Tree("15 / 3 * 4 + 5 - 6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Multiply(Divide(Number(15.000000), Number(3.000000)), Number(4.000000)), Number(5.000000)), Number(6.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ComplexExpression5) {
    ExpressionParser Tree("2 + 3 * 4 - 5 / 6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(2.000000), Multiply(Number(3.000000), Number(4.000000))), Divide(Number(5.000000), Number(6.000000)))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ComplexExpression6) {
    ExpressionParser Tree("2.1+3.5* 4.7-5/6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(2.100000), Multiply(Number(3.500000), Number(4.700000))), Divide(Number(5.000000), Number(6.000000)))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, NegativeNumber) {
    ExpressionParser Tree("-3 + 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(-3.000000), Number(4.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, NegativeNumber1) {
    ExpressionParser Tree("(-3) + 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(-3.000000), Number(4.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, NegativeNumber2) {
    ExpressionParser Tree("(5 - (-3))");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Number(5.000000), Number(-3.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, ExpressionWithParentheses) {
    ExpressionParser Tree("3 + 4 * (2 - 1)");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(3.000000), Multiply(Number(4.000000), Subtract(Number(2.000000), Number(1.000000))))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, NestedParentheses) {
    ExpressionParser Tree("3 + (4 * (2 - 1))");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(3.000000), Multiply(Number(4.000000), Subtract(Number(2.000000), Number(1.000000))))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, MultipleParentheses) {
    ExpressionParser Tree("((3 + 4) * (2 - 1)) / 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Divide(Multiply(Add(Number(3.000000), Number(4.000000)), Subtract(Number(2.000000), Number(1.000000))), Number(2.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserTest, NegativeNumbersWithParentheses) {
    ExpressionParser Tree("((-3) + 5) * (-2)");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Multiply(Add(Number(-3.000000), Number(5.000000)), Number(-2.000000))");
    EXPECT_EQ(Tree.getErrorCode(), ParseError::NONE);
}

TEST(ExpressionParserErrorTest, EmptyExpression) {
    ExpressionParser Tree("");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::EMPTY);
}

TEST(ExpressionParserErrorTest, IncorrectExpression) {
    ExpressionParser Tree("1 - + 3 - 1 + 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::SYNTAX_ERROR);
}

TEST(ExpressionParserErrorTest, IncorrectExpression1) {
    ExpressionParser Tree("1 + - 3 - 1 + 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::SYNTAX_ERROR);
}

TEST(ExpressionParserErrorTest, IncorrectExpressionLetters) {
    ExpressionParser Tree("1 + 5 + k - 1");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::INVALID_CHARACTERS);
}

TEST(ExpressionParserErrorTest, IncorrectExpressionOperators) {
    ExpressionParser Tree("1 - 3 _ 1 + 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::INVALID_CHARACTERS);
}

TEST(ExpressionParserErrorTest, IncorrectParetheses) {
    ExpressionParser Tree("(1 + 3) + 1)");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::UNCLOSED_BRACKETS);
}

TEST(ExpressionParserErrorTest, IncorrectParetheses1) {
    ExpressionParser Tree("((1 + 3) + 1");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::UNCLOSED_BRACKETS);
}

TEST(ExpressionParserErrorTest, IncorrectParetheses2) {
    ExpressionParser Tree("4 + (-1 + 1");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::UNCLOSED_BRACKETS);
}

TEST(ExpressionParserErrorTest, IncorrectParetheses3) {
    ExpressionParser Tree("4 + -1) + 1");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::UNCLOSED_BRACKETS);
}

TEST(ExpressionParserErrorTest, IncorrectParetheses4) { // new edge case
    ExpressionParser Tree("456 + (-1215 + 16)");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::UNCLOSED_BRACKETS);
}

TEST(ExpressionParserErrorTest, IncorrectParetheses5) { // new edge case...
    ExpressionParser Tree("14 + 56) * (100 - 86");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::UNCLOSED_BRACKETS);
}

TEST(ExpressionParserErrorTest, IncorrectExpressionMixed) {
    ExpressionParser Tree(" (4 + k _ 1");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::INVALID_CHARACTERS);
}

TEST(ExpressionParserErrorTest, IncompletedExpression) {
    ExpressionParser Tree(" (4 + 1 +");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::UNCLOSED_BRACKETS);
}

TEST(ExpressionParserErrorTest, IncompletedExpression1) {
    ExpressionParser Tree("4 + 1 +");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression, nullptr);
    EXPECT_EQ(Tree.getErrorCode(), ParseError::SYNTAX_ERROR);
}


///////////////////////////////////////
////////ТЕСТЫ ДЛЯ КАЛЬКУЛЯТОРА/////////
///////////////////////////////////////

TEST(CalculatorTest, RepeatedOperations) {
    string Expression = "5 + 5 - 5 * 5 / 5";
    double result = 5;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, NegativeNumbers) {
    string Expression = "((-5) + (-5)) * (-1)";
    double result = 10;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, LargeNumberDivision) {
    string Expression = "1000000000 / 0.001";
    double result = 1000000000000;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, FloatingPointPrecision) {
    string Expression = "0.1 + 0.2 - 0.3";
    double result = 0;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_NEAR(result, Tree->Calculate(), 1e-9); // some dude from github said 5.5511151231257827e-17 You possibly computed 3 * 0.1 - 0.3 (or something similar) in double precision floating point arithmetic. It's close enough to zero that you can probably stop worrying and get on with life.
}

TEST(CalculatorTest, NestedParentheses) {
    string Expression = "(2 + (3 * (4 - 1))) / 2";
    double result = 5.5;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, SmallNumbers) {
    string Expression = "0.00001 + 0.000001";
    double result = 0.000011;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_DOUBLE_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, MixedIntegerAndFloatingPoint) {
    string Expression = "5 + 2.5 * 2 - 3 / 1.5";
    double result = 8;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, VeryLargeNumbers) {
    string Expression = "(1000000000 * 1000000000) / (1000000000 - 100000000)";
    double result = 1111111111.111111;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_DOUBLE_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, MixedOperationsPriority) {
    string Expression = "6 + 4 / 2 * 3 - 1";
    double result = 11;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, MultipleNestedOperations) {
    string Expression = "(((5 + 2) * 3) - 4) / 2";
    double result = 8.5;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, ZeroAndNegativeNumbers) {
    string Expression = "0 * ((-2) + 3) / 4";
    double result = 0;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, RepeatedSingleOperation) {
    string expr = "1";
    for (int i = 0; i < 99; ++i) {
        expr += " - 1 + 1";
    }
    string Expression = expr;
    double result = 1;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, MixedPositiveAndNegativeFractions) {
    string Expression = "-0.5 + 0.25 - 0.75 + 1";
    double result = 0;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, SequentialOperations) {
    string Expression = "((5 + 3) * 2 - (4 / 2)) * 2";
    double result = 28;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, SmallFractions) {
    string Expression = "(0.0000000001 + 0.0000000001) * 10000000000";
    double result = 2.000000;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, FractionsResultingInInteger) {
    string Expression = "(2 / 3) * 3";
    double result = 2;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, ComplexOperationsCombination) {
    string Expression = "(3 + 4 * 2 / (1 - 5) * 2) + 10 / (2 + 3)";
    double result = 1;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTest, DeepNestedParentheses) {
    string Expression = "((((2 + 3) * 2) / 5) - 1) * 2";
    double result = 2;
    unique_ptr<Operation> Tree = ExpressionParser(Expression).parse();
    ASSERT_TRUE(Tree != nullptr);
    EXPECT_EQ(result, Tree->Calculate());
}

TEST(CalculatorTestForErrors, OpenedBrackets) {
    string Expression = "3 + 4 * ( 5 - 1";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::UNCLOSED_BRACKETS, Tree.getErrorCode());
}

TEST(CalculatorTestForErrors, OpenedBrackets1) {
    string Expression = "3 + 4 * 5 - 1)";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::UNCLOSED_BRACKETS, Tree.getErrorCode());
}

TEST(CalculatorTestForErrors, OpenedBrackets2) {
    string Expression = "3 + (-4 * 5 - 1";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::UNCLOSED_BRACKETS, Tree.getErrorCode());
}

TEST(CalculatorTestForErrors, InvalidChar) {
    string Expression = "1 _ 1";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::INVALID_CHARACTERS, Tree.getErrorCode());
}
TEST(CalculatorTestForErrors, InvalidChar1) {
    string Expression = "1 -_ 1";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::INVALID_CHARACTERS, Tree.getErrorCode());
}
TEST(CalculatorTestForErrors, InvalidChar2) {
    string Expression = "1 + 1 + d";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::INVALID_CHARACTERS, Tree.getErrorCode());
}


TEST(CalculatorTestForErrors, DividedByZero) {
    string Expression = "1 / 0";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::DIVISION_BY_ZERO, Tree.getErrorCode());
}
TEST(CalculatorTestForErrors, DividedByZero1) {
    string Expression = "1 / ((-5) * (-1) - 5)";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::DIVISION_BY_ZERO, Tree.getErrorCode());
}

TEST(CalculatorTestForErrors, EmptyLine) {
    string Expression = " ";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::EMPTY, Tree.getErrorCode());
}

TEST(CalculatorTestForErrors, EmptyLine1) {
    string Expression = "        ";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::EMPTY, Tree.getErrorCode());
}

TEST(CalculatorTestForErrors, IncompleteLine) {
    string Expression = "1 +";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::SYNTAX_ERROR, Tree.getErrorCode());
}
TEST(CalculatorTestForErrors, IncompleteLine1) {
    string Expression = "((3 + 4 * 2 / (1 - * 5) * 2) + 10 / 2 + 3)";
    ExpressionParser Tree(Expression);
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(ParseError::SYNTAX_ERROR, Tree.getErrorCode());
}