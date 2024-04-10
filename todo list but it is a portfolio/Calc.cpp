#include "Calc.h"
#include <iostream>
#include <stack>
#include <string>
#include <conio.h> 
#include "setcolor.h"

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

long double EvaluateExpression(const string& Expression, bool& error) {
    stack<long double> Values;
    stack<char> Operators;

    for (size_t i = 0; i < Expression.length(); ++i) {
        if (Expression[i] == ' ')
            continue;

        if (isdigit(Expression[i]) || Expression[i] == '.') {
            long double Val = 0;
            long double Fraction = 0.1;
            bool DecimalPointFound = false;

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
            Values.push(Val);
        }
        else if (Expression[i] == '(') {
            Operators.push(Expression[i]);
        }
        else if (Expression[i] == ')') {
            while (!Operators.empty() && Operators.top() != '(') {
                long double Operand2 = Values.top();
                Values.pop();

                long double Operand1 = Values.top();
                Values.pop();

                char Op = Operators.top();
                Operators.pop();

                Values.push(ApplyOperation(Operand1, Operand2, Op, error));
                if (error) return 0;
            }
            if (!Operators.empty()) Operators.pop();
        }
        else {
            while (!Operators.empty() && Priority(Operators.top()) >= Priority(Expression[i])) {
                long double Operand2 = Values.top();
                Values.pop();

                long double Operand1 = Values.top();
                Values.pop();

                char Op = Operators.top();
                Operators.pop();

                Values.push(ApplyOperation(Operand1, Operand2, Op, error));
                if (error) return 0; 
            }
            Operators.push(Expression[i]);
        }
    }

    while (!Operators.empty()) {
        long double Operand2 = Values.top();
        Values.pop();

        long double Operand1 = Values.top();
        Values.pop();

        char Op = Operators.top();
        Operators.pop();

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

int Calculator() {
    string Expression;
    bool error = false;

    cout << "Введите выражение: ";
    getline(cin, Expression);

    if (Expression.empty() || IsOnlySpaces(Expression)) {
        SetColor(31);
        cout << "Пустое выражение!" << endl;
        SetColor(0);
        _getch();
        return 1;
    }

    while (!Expression.empty() && Expression.back() == ' ')
        Expression.pop_back();

    if (!isdigit(Expression.back()) && Expression.back() != ')') {
        SetColor(31);
        cout << "Незавершенное выражение!" << endl;
        SetColor(0);
        _getch();
        return 1;
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
            SetColor(31);
            cout << "Некорректные символы в выражении" << endl;
            SetColor(0); 
            _getch();
            return 1;
        }
    }

    if (OpenBrackets != ClosedBrackets) {
        SetColor(31);
        cout << "Незакрытые скобки в выражении!" << endl;
        SetColor(0);
        _getch();
        return 1;
    }

    if (error) {
        SetColor(31);
        cout << "Некорректное выражение!" << endl;
        SetColor(0);
    }
    else {
        long double result = EvaluateExpression(Expression, error);
        cout << "Результат: " << fixed << result << endl; // это было настолько просто............
    }

    cout << "\nНажмите на любую кнопку для продолжения";
    _getch();
    return 0;
}
