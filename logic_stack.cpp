// Q2: evaluating the circuit expression using two stacks

#include <iostream>
#include <iomanip>
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

bool isVariable(string s)
{
    return s[0] >= 'A' && s[0] <= 'D';
}

int valueOf(string s, int A, int B, int C, int D)
{
    int v = 0;
    if (s[0] == 'A') v = A;
    else if (s[0] == 'B') v = B;
    else if (s[0] == 'C') v = C;
    else v = D;
    if (s.size() > 1 && s[1] == '\'') v = 1 - v;
    return v;
}

int precedence(char c)
{
    if (c == '&') return 2;
    return 1;
}

int applyOp(int a, int b, char c)
{
    if (c == '&') return a & b;
    return a | b;
}

int nand2(int a, int b) { return 1 - (a & b); }

int evaluate(vector<string> tok, int A, int B, int C, int D)
{
    ValueStack vals;
    OpStack ops;

    for (int i = 0; i < (int)tok.size(); i++)
    {
        if (isVariable(tok[i]))
        {
            vals.push(valueOf(tok[i], A, B, C, D));
        }
        else
        {
            char c = tok[i][0];
            while (!ops.empty() && precedence(ops.top()) >= precedence(c))
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
    vector<string> Zb   = {"A", "&", "B", "|", "A", "&", "C'", "|", "B", "&", "C",
                           "|", "B", "&", "D'", "|", "A", "&", "D"};
    vector<string> Zmin = {"B", "&", "D'", "|", "B", "&", "C",
                           "|", "A", "&", "D", "|", "A", "&", "C'"};

    cout << " A B C D | N1 N2  P  Q  R  S  T |  Z gates  Z (b)  Z (d)  Z nand" << endl;
    cout << "---------+----------------------+------------------------------" << endl;

    bool allMatch = true;
    for (int A = 0; A <= 1; A++)
      for (int B = 0; B <= 1; B++)
        for (int C = 0; C <= 1; C++)
          for (int D = 0; D <= 1; D++)
          {
              int N1 = !C, N2 = !D;
              int P = B | N1, Q = A & P;
              int R = C | N2, S = B & R;
              int T = A & D;
              int Zgates = Q | S | T;

              int Zexpanded = evaluate(Zb, A, B, C, D);
              int Zminimal  = evaluate(Zmin, A, B, C, D);

              int Cn = nand2(C, C), Dn = nand2(D, D);
              int g1 = nand2(B, Dn), g2 = nand2(B, C);
              int g3 = nand2(A, D),  g4 = nand2(A, Cn);
              int X = nand2(g1, g2), Y = nand2(g3, g4);
              int Znand = nand2(nand2(X, X), nand2(Y, Y));

              if (Zgates != Zexpanded || Zgates != Zminimal || Zgates != Znand)
                  allMatch = false;

              cout << " " << A << " " << B << " " << C << " " << D << " |"
                   << setw(3) << N1 << setw(3) << N2 << setw(3) << P
                   << setw(3) << Q  << setw(3) << R  << setw(3) << S
                   << setw(3) << T  << " |" << setw(6) << Zgates
                   << setw(8) << Zexpanded << setw(7) << Zminimal
                   << setw(8) << Znand << endl;
          }

    cout << endl;
    cout << "Z (b) = A & B | A & C' | B & C | B & D' | A & D" << endl;
    cout << "Z (d) = B & D' | B & C | A & D | A & C'" << endl << endl;
    cout << "expressions and NAND circuit match the gate circuit on all 16 rows: ";
    cout << (allMatch ? "yes" : "no") << endl;
    return 0;
}
