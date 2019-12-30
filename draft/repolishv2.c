#include<stdio.h>
#define pushStack(stk, target, isChar) (stk.num[++stk.top] = (Node){(float)target, isChar})
#define popStack(stk) (--stk.top)
#define topStack(stk) (stk.num[stk.top])
#define resetStack(stk) (stk.top = -1)
#define StackIsEmpty(stk) (stk.top < 0)
#define OpLevel(c) (c == '+' || c == '-' ? 1 : (c == '*' || c == '/' ? 3 : (c == 'm' || c == '$' ? 2 : 0)))
#define IsNumber(c) (c >= '0' && c <= '9')
#define OneOp(c) (c == 'm' || c == '$')
#define bit int
typedef struct Node {
    float payload;
    int isChar;
} Node;
typedef struct {
    int top;
    Node num[10];
}Stk;


float Q_sqrt(float z)
{
    unsigned int b;
    float g;
    b = *(unsigned int *) &z;

    b = (b >> 1) + 0x1fbffb72;  //wtf
    g = *(float *) &b;
    g = (.5 * g + .5 * z / g);
    g = (.5 * g + .5 * z / g);
    return g;
}

void rePolish(char* str, Stk* stk) {
    Stk s;
    resetStack(s);
    bit isNum = 0;
    for (; *str; ++str) {
        char t = *str;
        if (t == ' ') {
            continue;
        }
        if (t == '(') {
            pushStack(s, t, 1);
        }
        else if (t == ')') {
            while (topStack(s).isChar && topStack(s).payload != '(') {
                pushStack((*stk), topStack(s).payload, 1);
                popStack(s);
            }
            popStack(s);
            while(!StackIsEmpty(s) && topStack(s).isChar && OneOp(topStack(s).payload)) {
                pushStack((*stk), topStack(s).payload, 1);
                popStack(s);
            }
        } else if (t == '$') {
            pushStack(s, '$', 1);
        }
        else if (t == '+' || t == '*' || t == '/' || t == 'd') {
            while (OpLevel(topStack(s).payload) >= OpLevel(t)) {
                pushStack((*stk), topStack(s).payload, 1);
                popStack(s);
            }
            pushStack(s, t, 1);
        } else if (t == '-') {
            if (isNum) {
                while (OpLevel(topStack(s).payload) >= OpLevel(t)) {
                    pushStack((*stk), topStack(s).payload, 1);
                    popStack(s);
                }
                pushStack(s, t, 1);
            }
            else {
                pushStack(s, 'm', 1);
            }
        } else {
            float strnum = 0;
            while (IsNumber(*str)) {
                isNum = 1;
                strnum *= 10;
                strnum += *str - '0';
                ++str;
            }
            if (*str == '.') {
                ++str;
                float loadBit = 0.1f;
                while(IsNumber(*str)) {
                    strnum = strnum + (*str - '0') * loadBit;
                    loadBit *= 0.1;
                    ++str;
                }
            }
            pushStack((*stk), strnum, 0);
            --str;
        }
    }
    while (!StackIsEmpty(s)) {
        pushStack((*stk), topStack(s).payload, topStack(s).isChar);
        popStack(s);
    }
}

float calc(Stk* stk) {
    Stk s;
    resetStack(s);
    float tmp;
    while (!StackIsEmpty((*stk))) {
        Node top = topStack((*stk));
        if (top.isChar && top.payload == '+') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = tmp + topStack(s).payload;
            popStack(s);
        }
        else if (top.isChar && top.payload == '-') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = topStack(s).payload - tmp;
            popStack(s);
        }
        else if (top.isChar && top.payload == '*') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = tmp * topStack(s).payload;
            popStack(s);
        }
        else if (top.isChar && top.payload == '/') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = topStack(s).payload / tmp;
            popStack(s);
        }
        else if (top.isChar && top.payload == 'm') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = -tmp;
        }
        else if (top.isChar && top.payload == '$') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = Q_sqrt(tmp);
        }
        else {
            tmp = topStack((*stk)).payload;
        }
        popStack((*stk));
        pushStack(s, tmp, 0);
    }
    return topStack(s).payload;
}

void reverseStack(Stk* stk) {
    int s = 0, e = (*stk).top;
    Node tmp;
    while(s < e) {
        tmp = (*stk).num[s];
        (*stk).num[s] = (*stk).num[e];
        (*stk).num[e] = tmp;
        ++s;
        --e;
    }
}

void calculate(char* str, char* output) {
    Stk stk;
    resetStack(stk);
    rePolish(str, &stk);
    reverseStack(&stk);
    // cout << tmp << endl;
    float res = calc(&stk);
    sprintf(output, "%.3f", res);
}

int main() {
    char str[1000], buffer[1000];
    while (gets(str)) {
        calculate(str, buffer);
        printf("%s\n", buffer);
    }
}