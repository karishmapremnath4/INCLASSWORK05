// Stacks and Queues - In-Class Work, Q1
// Infix expression evaluation with two stacks: one for integer values, one for operators.
// Operators: + - * / ^      Right associative: ^      Division: floor (toward -infinity)
// Assumes valid expressions, no parentheses, no division by zero, nonnegative exponents.

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

// values stack
// Array stack, same shape as the lecture's ArrayStack: top is the index of the
// top element and is -1 when the stack is empty.
class IntStack
{
private:
    static const int CAPACITY = 100;
    int data[CAPACITY];
    int top = -1;
public:
    bool isEmpty() const { return top == -1; }
    bool isFull() const { return top == CAPACITY - 1; }
    int size() const { return top + 1; }

    bool push(int x)
    {
        if (isFull()) { cout << "  push " << x << ": overflow" << endl; return false; }
        top++;
        data[top] = x;
        return true;
    }
    bool pop(int &x)
    {
        if (isEmpty()) { cout << "  pop: underflow" << endl; return false; }
        x = data[top];
        top--;
        return true;
    }
    bool peek(int &x) const
    {
        if (isEmpty()) return false;
        x = data[top];
        return true;
    }
    string toString() const               // bottom to top, so it reads like the trace table
    {
        string s;
        for (int i = 0; i <= top; i++)
        {
            if (i > 0) s += " ";
            s += to_string(data[i]);
        }
        return s;
    }
};

// operators stack
class CharStack
{
private:
    static const int CAPACITY = 100;
    char data[CAPACITY];
    int top = -1;
public:
    bool isEmpty() const { return top == -1; }
    bool isFull() const { return top == CAPACITY - 1; }
    int size() const { return top + 1; }

    bool push(char c)
    {
        if (isFull()) { cout << "  push " << c << ": overflow" << endl; return false; }
        top++;
        data[top] = c;
        return true;
    }
    bool pop(char &c)
    {
        if (isEmpty()) return false;
        c = data[top];
        top--;
        return true;
    }
    bool peek(char &c) const
    {
        if (isEmpty()) return false;
        c = data[top];
        return true;
    }
    string toString() const
    {
        string s;
        for (int i = 0; i <= top; i++)
        {
            if (i > 0) s += " ";
            s += data[i];
        }
        return s;
    }
};

// (a) identify numbers
// A token is a number if it is all digits, or a '-' followed by at least one
// digit. A lone "-" is the subtraction operator, not a number.
bool isNumber(const string &token)
{
    if (token.empty()) return false;
    int start = (token[0] == '-') ? 1 : 0;
    if (start == 1 && token.size() == 1) return false;
    for (int i = start; i < (int)token.size(); i++)
        if (token[i] < '0' || token[i] > '9') return false;
    return true;
}

// convert a number token to int, handling the leading '-'
int toInt(const string &token)
{
    bool negative = (token[0] == '-');
    int value = 0;
    for (int i = (negative ? 1 : 0); i < (int)token.size(); i++)
        value = value * 10 + (token[i] - '0');
    return negative ? -value : value;
}

// (a) determine precedence
int precedence(char op)
{
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// only ^ is right associative: 2 ^ 3 ^ 2 means 2 ^ (3 ^ 2)
bool isRightAssociative(char op) { return op == '^'; }

// (d) floor division
// C++ '/' truncates toward zero, so -7 / 2 gives -3, but the spec wants -4.
// Step down by one only when the signs differ and the division is not exact.
int floorDiv(int a, int b)
{
    int q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0)))
        q--;
    return q;
}

// exponent is a nonnegative integer, so a simple loop is enough
int intPow(int base, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++)
        result *= base;
    return result;
}

// (a) apply an operation
// a is the LEFT operand, b is the RIGHT one.
int applyOp(int a, int b, char op)
{
    switch (op)
    {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return floorDiv(a, b);
    case '^': return intPow(a, b);
    }
    return 0;
}

// Pop one operator and its two operands, push the result back.
// Returns a readable description such as "3 * 4 = 12" for the trace.
string applyTop(IntStack &values, CharStack &ops)
{
    char op;
    int a, b;
    ops.pop(op);
    values.pop(b);                        // the RIGHT operand comes off first
    values.pop(a);                        // the one popped second is the LEFT operand
    int result = applyOp(a, b, op);
    values.push(result);
    return to_string(a) + " " + op + " " + to_string(b) + " = " + to_string(result);
}

// trace table layout
const int W_TOKEN = 7, W_ACTION = 48, W_VALUES = 24, W_OPS = 10;

void printHeader()
{
    cout << left
         << setw(W_TOKEN)  << "Token"
         << setw(W_ACTION) << "Action"
         << setw(W_VALUES) << "Values (bottom to top)"
         << setw(W_OPS)    << "Operators" << endl;
    cout << string(W_TOKEN + W_ACTION + W_VALUES + W_OPS, '-') << endl;
}

void printRow(const string &token, const string &action,
              const IntStack &values, const CharStack &ops)
{
    string v = values.toString();
    string o = ops.toString();
    cout << left
         << setw(W_TOKEN)  << token
         << setw(W_ACTION) << action
         << setw(W_VALUES) << (v.empty() ? "(empty)" : v)
         << setw(W_OPS)    << (o.empty() ? "(empty)" : o) << endl;
}

// (a) evaluate the expression
int evaluate(const vector<string> &tokens, bool trace)
{
    IntStack values;
    CharStack ops;

    if (trace) printHeader();

    for (int i = 0; i < (int)tokens.size(); i++)
    {
        string token = tokens[i];
        string action;

        if (isNumber(token))
        {
            values.push(toInt(token));
            action = "number, push " + token;
        }
        else
        {
            char op = token[0];
            char topOp;
            // Run every waiting operator that must go first: one that is strictly
            // stronger, or one of equal strength when the incoming operator is
            // left associative. Equal strength plus right associative means wait.
            while (ops.peek(topOp) &&
                   (precedence(topOp) > precedence(op) ||
                    (precedence(topOp) == precedence(op) && !isRightAssociative(op))))
            {
                if (!action.empty()) action += ", ";
                action += applyTop(values, ops);
            }
            ops.push(op);
            if (!action.empty()) action += ", ";
            action += "push ";
            action += op;
        }

        if (trace) printRow(token, action, values, ops);
    }

    // The tokens are done. Show the remaining calculations.
    char topOp;
    if (trace && ops.peek(topOp))
        cout << "Remaining calculations:" << endl;

    while (ops.peek(topOp))
    {
        string action = applyTop(values, ops);
        if (trace) printRow("", action, values, ops);
    }

    int result;
    values.pop(result);
    return result;
}

void run(const string &title, const vector<string> &tokens)
{
    cout << title << endl;
    cout << "expression: ";
    for (int i = 0; i < (int)tokens.size(); i++)
        cout << tokens[i] << " ";
    cout << endl << endl;

    int answer = evaluate(tokens, true);

    cout << endl << "Result: " << answer << endl << endl;
}

int main()
{
    // (b) and (c)
    vector<string> e1 = {"8", "+", "3", "*", "2", "^", "2", "-", "20", "/", "4"};
    run("Q1 (b, c)   8 + 3 * 2 ^ 2 - 20 / 4", e1);

    // (d) right associativity: the answer must be 512, not 64
    vector<string> e2 = {"2", "^", "3", "^", "2"};
    run("Q1 (d)      2 ^ 3 ^ 2        expect 512, not 64", e2);

    // (d) floor division: the answer must be -4, not -3
    vector<string> e3 = {"-7", "/", "2"};
    run("Q1 (d)      -7 / 2           expect -4, not -3", e3);

    return 0;
}
