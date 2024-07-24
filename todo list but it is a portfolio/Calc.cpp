#include "Calc.h"
#include <iostream>
#include <stack>
#include <string>
#include <conio.h>
#include "setcolor.h"
#include <iomanip>
#include "gtest/gtest.h"

using namespace std;
/*
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

string Calculator(string Expression, bool error) {
    Expression.erase(remove(Expression.begin(), Expression.end(), ' '), Expression.end()); // очистка от пробелов

    if (Expression.empty()) {
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
}*/

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
    ExpressionParser(const string& expr) : expression(expr) {}

    unique_ptr<Operation> parse() {
        tokenize();
        processOperators();
        return move(values.top());
    }

private:
    string expression;
    stack<unique_ptr<Operation>> values;
    stack<char> operators;
 
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    int getPrecedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    unique_ptr<Operation> applyOperation(char op, unique_ptr<Operation> left, unique_ptr<Operation> right) {
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

    void tokenize() {
        istringstream iss(expression);
        string token;

        while (iss >> token) {
            if (isdigit(token[0])) {
                values.push(make_unique<Number>(stod(token)));
            }
            else if (isOperator(token[0])) {
                processCurrentOperator(token[0]);
            }
        }
    }
};

///////////////////////////////////////
//////////ТЕСТЫ ДЛЯ ДЕРЕВЬЕВ///////////
///////////////////////////////////////

/*
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
}*/

///////////////////////////////////////
//////////ТЕСТЫ ДЛЯ ПАРСЕРА////////////
///////////////////////////////////////

TEST(ExpressionParserTest, SingleNumber) {
    ExpressionParser Tree ("42");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Number(42.000000)");
}

TEST(ExpressionParserTest, AdditionStructure) {
    ExpressionParser Tree("3 + 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(3.000000), Number(4.000000))");
}

TEST(ExpressionParserTest, SubtractionStructure) {
    ExpressionParser Tree("7 - 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Number(7.000000), Number(2.000000))");
}

TEST(ExpressionParserTest, MultiplicationStructure) {
    ExpressionParser Tree("2 * 3");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Multiply(Number(2.000000), Number(3.000000))");
}

TEST(ExpressionParserTest, DivisionStructure) {
    ExpressionParser Tree("8 / 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Divide(Number(8.000000), Number(4.000000))");
}

TEST(ExpressionParserTest, ComplexExpression) {
    ExpressionParser Tree("3 + 4 * 2");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(3.000000), Multiply(Number(4.000000), Number(2.000000)))");
}

TEST(ExpressionParserTest, ComplexExpression1) {
    ExpressionParser Tree("3 + 4 * 5 - 6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(3.000000), Multiply(Number(4.000000), Number(5.000000))), Number(6.000000))");
}

TEST(ExpressionParserTest, ComplexExpression2) {
    ExpressionParser Tree("7 * 8 / 2 + 3");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Divide(Multiply(Number(7.000000), Number(8.000000)), Number(2.000000)), Number(3.000000))");
}

TEST(ExpressionParserTest, ComplexExpression3) {
    ExpressionParser Tree("10 + 11 - 12 * 13 / 14");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(10.000000), Number(11.000000)), Divide(Multiply(Number(12.000000), Number(13.000000)), Number(14.000000)))");
}

TEST(ExpressionParserTest, ComplexExpression4) {
    ExpressionParser Tree("15 / 3 * 4 + 5 - 6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Multiply(Divide(Number(15.000000), Number(3.000000)), Number(4.000000)), Number(5.000000)), Number(6.000000))");
}

TEST(ExpressionParserTest, ComplexExpression5) {
    ExpressionParser Tree("2 + 3 * 4 - 5 / 6");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Add(Number(2.000000), Multiply(Number(3.000000), Number(4.000000))), Divide(Number(5.000000), Number(6.000000)))");
}

//TODO: Поддержка отрицательных чисел, скобок и ошибок
TEST(ExpressionParserTest, NegativeNumber) {
    ExpressionParser Tree("-3 + 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(-3.000000), Number(4.000000))");
}

TEST(ExpressionParserTest, NegativeNumber1) {
    ExpressionParser Tree("(-3) + 4");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(-3.000000), Number(4.000000))");
}

TEST(ExpressionParserTest, NegativeNumber2) {
    ExpressionParser Tree("(5 - (-3))");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Subtract(Number(5.000000), Number(-3.000000))");
}

TEST(ExpressionParserTest, ExpressionWithParentheses) {
    ExpressionParser Tree("3 + 4 * (2 - 1)");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Add(Number(3.000000), Multiply(Number(4.000000), Subtract(Number(2.000000), Number(1.000000))))");
}

TEST(ExpressionParserTest, NestedParentheses) {
    ExpressionParser parser("3 + (4 * (2 - 1))");
    auto operation = parser.parse();
    EXPECT_EQ(operation->GetStructure(), "Add(Number(3), Multiply(Number(4), Subtract(Number(2), Number(1))))");
}

TEST(ExpressionParserTest, MultipleParentheses) {
    ExpressionParser parser("((3 + 4) * (2 - 1)) / 2");
    auto operation = parser.parse();
    EXPECT_EQ(operation->GetStructure(), "Divide(Multiply(Add(Number(3), Number(4)), Subtract(Number(2), Number(1))), Number(2))");
}

TEST(ExpressionParserTest, NegativeNumbersWithParentheses) {
    ExpressionParser parser("(-3 + 5) * (-2)");
    auto operation = parser.parse();
    EXPECT_EQ(operation->GetStructure(), "Multiply(Add(Number(-3.000000), Number(5.000000)), Number(-2.000000))");
}

TEST(ExpressionParserTestForErrors, EmptyExpression) {
    ExpressionParser Tree("");
    unique_ptr<Operation> treeExpression = Tree.parse();
    EXPECT_EQ(treeExpression->GetStructure(), "Number(0.000000)");
}

///////////////////////////////////////
////////ТЕСТЫ ДЛЯ КАЛЬКУЛЯТОРА/////////
///////////////////////////////////////

/*
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
*/