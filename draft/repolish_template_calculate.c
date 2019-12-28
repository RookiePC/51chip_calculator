#include<stdio.h>
#define MAXN 10000
typedef struct Stack
{
    int date[MAXN];
    int top;
} Stack;
struct BL
{
    int date;
    int ischar;
} a[1000];
Stack s1;
#define Stack_Init(X) (X.top = -1)
#define IsEmpty(X) (X.top == -1)
#define push(S, tmp) (S.date[++S.top] = tmp)
#define pop(S) (S.date[S.top--])
#define top(S) (S.date[S.top])

int main()
{
    char tmp;
    int i=0,j,num=0,t,t1,t2;
    Stack_Init(s1);
    char str[1000];
    gets(str);
    char* pt = str;
    while (tmp = *pt, pt++)
    {
        if (tmp==0)
        {
            if (num)
            {
                a[i].date=num;
                a[i++].ischar=0;
            }
            break;
        }
        if (tmp>='0'&&tmp<='9') num=num*10+tmp-48;
        else
        {
            if (num)
            {
                a[i].date=num;
                num=0;
                a[i++].ischar=0;
            }
            if (tmp=='(') push(s1,tmp);
            else if (tmp==')')
            {
                while (top(s1)!='(')
                {
                    t=pop(s1);
                    a[i].date=t;
                    a[i++].ischar=1;
                }
                pop(s1);
            }
            else if (tmp=='+'||tmp=='-')
            {
                while (IsEmpty(s1)!=1&&top(s1)!='(')
                {
                    t=pop(s1);
                    a[i].date=t;
                    a[i++].ischar=1;
                }
                push(s1,tmp);
            }
            else if (tmp=='*'||tmp=='/')
            {
                while (IsEmpty(s1)!=1&&(top(s1)!='+'&&top(s1)!='-')&&top(s1)!='(')
                {
                    t=pop(s1);
                    a[i].date=t;
                    a[i++].ischar=1;
                }
                push(s1,tmp);
            }
        }
    }
    while (IsEmpty(s1)!=1)
    {
        t=pop(s1);
        a[i].date=t;
        a[i++].ischar=1;
    }
    for (j=0; j<i; j++) {
        if (a[j].ischar) {
            t1 = pop(s1);
            t2 = pop(s1);
            if (a[j].date == '*') {
                t1 = t2 * t1;
                push(s1, t1);
            }
            if (a[j].date == '/') {
                t1 = t2 / t1;
                push(s1, t1);
            }
            if (a[j].date == '+') {
                t1 = t2 + t1;
                push(s1, t1);
            }
            if (a[j].date == '-') {
                t1 = t2 - t1;
                push(s1, t1);
            }
        } else {
            push(s1, a[j].date);
        }
    }
    printf("%d",top(s1));
    return 0;
}