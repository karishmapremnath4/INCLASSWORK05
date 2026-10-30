# EECE 7205 In-Class Work 05: Stacks and Queues

Two problems, both solved with a pair of hand-written array stacks: one holding
integer values, one holding operators. No `std::stack`.

## Files

| file | what it is |
|---|---|
| `INCLASSWORK05.pdf` | the written answers for both problems |
| `q1.cpp` | Problem 1, infix expression evaluation |
| `q2.cpp` | Problem 2, the logic circuit expression evaluated on the same stacks |

## Problem 1

Evaluates an infix expression stored as a `vector<string>` of tokens, using a
values stack and an operators stack. Supports `+ - * / ^`, standard precedence,
right associativity for `^`, negative integer tokens, and division rounded down
toward negative infinity.

```
g++ -std=c++17 -o q1 q1.cpp && ./q1
```

```
(b) 8 + 3 * 2 ^ 2 - 20 / 4 = 15
(d) 2 ^ 3 ^ 2             = 512
(d) -7 / 2                = -4
```

`2 ^ 3 ^ 2` gives 512 rather than 64 because `^` is right associative, so on a
tie the waiting operator is left alone. `-7 / 2` gives -4 rather than -3 because
C++ integer division truncates toward zero and the spec asks for the floor.

## Problem 2

A four-input logic circuit is read off its gate list into a Boolean expression,
minimized with a Karnaugh map, and then evaluated with the same two stacks:

```
Z = A(B + C') + B(C + D') + AD = BD' + BC + AD + AC'
```

Because the minimized form is a sum of products it needs no parentheses, so the
Problem 1 evaluator runs it directly with `&` in place of `*` and `|` in place
of `+`. The program prints all 16 rows and checks four things against each
other: the gate network, the part (b) expression, the minimized expression, and
an 11-gate NAND implementation.

```
g++ -std=c++17 -o q2 q2.cpp && ./q2
```

```
expressions and NAND circuit match the gate circuit on all 16 rows: yes
```

Worst-case propagation delay of the NAND version is 5 gates at 20 ns, so 100 ns,
along the path `C -> C' -> g4 -> Y -> Y' -> Z`.
