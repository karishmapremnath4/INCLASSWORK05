// Q1: infix expression evaluation using two stacks

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int MAX = 100;

class ValueStack
{
    int a[MAX];
    int t = -1;
public:
    bool empty() { return t == -1; }
    bool full() { return t == MAX - 1; }
    void push(int x)
    {
        if (full()) { cout << "push: overflow" << endl; return; }
        a[++t] = x;
    }
    int pop()
    {
        if (empty()) { cout << "pop: underflow" << endl; return 0; }
        return a[t--];
    }
};

class OpStack
{
    char a[MAX];
    int t = -1;
public:
    bool empty() { return t == -1; }
    bool full() { return t == MAX - 1; }
    char top() { return a[t]; }
    void push(char c)
    {
        if (full()) { cout << "push: overflow" << endl; return; }
        a[++t] = c;
    }
    char pop()
    {
        if (empty()) { cout << "pop: underflow" << endl; return 0; }
        return a[t--];
    }
};

bool isNumber(string s)
{
    int i = 0;
    if (s[0] == '-' && s.size() > 1) i = 1;
    while (i < (int)s.size())
    {
        if (s[i] < '0' || s[i] > '9') return false;
        i++;
    }
    return true;
}

int toInt(string s)
{
    int n = 0, i = 0;
    bool neg = false;
    if (s[0] == '-') { neg = true; i = 1; }
    while (i < (int)s.size())
    {
        n = n * 10 + (s[i] - '0');
        i++;
    }
    if (neg) n = -n;
    return n;
}

int precedence(char c)
{
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    return 1;
}

int floorDiv(int a, int b)
{
    int q = a / b;
    if (a % b != 0 && (a < 0) != (b < 0)) q--;
    return q;
}

int applyOp(int a, int b, char c)
{
    if (c == '+') return a + b;
    if (c == '-') return a - b;
    if (c == '*') return a * b;
    if (c == '/') return floorDiv(a, b);
    int p = 1;
    for (int i = 0; i < b; i++) p = p * a;
    return p;
}

int evaluate(vector<string> tok)
{
    ValueStack vals;
    OpStack ops;

    for (int i = 0; i < (int)tok.size(); i++)
    {
        if (isNumber(tok[i]))
        {
            vals.push(toInt(tok[i]));
        }
        else
        {
            char c = tok[i][0];
            int pc = precedence(c);
            while (!ops.empty() && (precedence(ops.top()) > pc ||
                                    (precedence(ops.top()) == pc && c != '^')))
            {
                char o = ops.pop();
                int b = vals.pop();
                int a = vals.pop();
                vals.push(applyOp(a, b, o));
            }
            ops.push(c);
        }
    }

    while (!ops.empty())
    {
        char o = ops.pop();
        int b = vals.pop();
        int a = vals.pop();
        vals.push(applyOp(a, b, o));
    }

    return vals.pop();
}

int main()
{
    vector<string> e1 = {"8", "+", "3", "*", "2", "^", "2", "-", "20", "/", "4"};
    vector<string> e2 = {"2", "^", "3", "^", "2"};
    vector<string> e3 = {"-7", "/", "2"};

    cout << "(b) 8 + 3 * 2 ^ 2 - 20 / 4 = " << evaluate(e1) << endl;
    cout << "(d) 2 ^ 3 ^ 2             = " << evaluate(e2) << endl;
    cout << "(d) -7 / 2                = " << evaluate(e3) << endl;
    return 0;
}
