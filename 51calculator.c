#include<reg51.h>
#include "lcd.h"
#include "516_input.h"
#include<math.h>

#define pushStack(stk, target, _char) stk.num[++stk.top].payload = (float)target; \
														stk.num[stk.top].isChar = _char;
#define popStack(stk) (--stk.top)
#define topStack(stk) (stk.num[stk.top])
#define sizeOfStack(stk) (stk.top + 1)
#define resetStack(stk) (stk.top = -1)
#define StackIsEmpty(stk) (stk.top < 0)
#define OpLevel(c) (c == '+' || c == '-' ? 1 : (c == '*' || c == '/' ? 3 : (c == 'm' || c == '$' ? 2 : 0)))
#define TopLevel(c) (OpLevel(c) == 3)
#define IsNumber(c) (c >= '0' && c <= '9')
#define OneOp(c) (c == 'm' || c == '$' || c == 'u' || c == 'p')
#define TwoOp(ch) (OneOp(ch) != 1)
#define __CALC__

// used to store the final result
float result;

char error;

unsigned char iterate_lcd_ram(unsigned char read_pos)
{
	unsigned char _data;

	if ( read_pos > 0xA7 || read_pos < 0x80)
		return 0;

	LcdWriteCom( read_pos );
	_data = ReadLcdData();

	if ( _data == 0x20 )
		return 0;

	return _data;
}

typedef struct Node {
    float payload;
    char isChar;
} Node;

typedef struct {
    char top;
    Node num[13];
}Stk;


float Q_sqrt(float z)
{
    unsigned long b;
    float g;
    b = *(unsigned long *) &z;

    b = (b >> 1) + 0x1fbffb72;  //wtf
    g = *(float *) &b;
    g = (.5 * g + .5 * z / g);
    g = (.5 * g + .5 * z / g);
    return g;
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


float calc(Stk* stk, char* size) {
    Stk s;
    float tmp;
    resetStack(s);
    *size = 0;
    while (!StackIsEmpty((*stk))) {
        if (topStack((*stk)).isChar) {
            if ((TwoOp(topStack((*stk)).payload) && sizeOfStack(s) < 2)
                || (OneOp(topStack((*stk)).payload) && sizeOfStack(s) < 1)
                || (topStack(s).payload < 0 && topStack((*stk)).payload == '$')) {
                END:
                *size = 1;
                while(sizeOfStack(s) > 0) {
                    pushStack((*stk), topStack(s).payload, topStack(s).isChar);
                    popStack(s);
                }
                return 0;
            }
        }
        if (topStack((*stk)).isChar && topStack((*stk)).payload == '+') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = tmp + topStack(s).payload;
            popStack(s);
        }
        else if (topStack((*stk)).isChar && topStack((*stk)).payload == '-') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = topStack(s).payload - tmp;
            popStack(s);
        }
        else if (topStack((*stk)).isChar && topStack((*stk)).payload == '*') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = tmp * topStack(s).payload;
            popStack(s);
        }
        else if (topStack((*stk)).isChar && topStack((*stk)).payload == '/') {
            tmp = topStack(s).payload;
            if (fabs(tmp) < 1e-3) {
                goto END;
            }
            popStack(s);
            tmp = topStack(s).payload / tmp;
            popStack(s);
        }
        else if (topStack((*stk)).isChar && topStack((*stk)).payload == 'm') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = -tmp;
        }
        else if (topStack((*stk)).isChar && topStack((*stk)).payload == '$') {
            tmp = topStack(s).payload;
            if (tmp < 0) {
                goto END;
            }
            popStack(s);
            tmp = Q_sqrt(tmp);
        }
        else if (topStack((*stk)).isChar && topStack((*stk)).payload == 'u') {
            tmp = topStack(s).payload;
            popStack(s);
            tmp = pow(tmp, 1.0 / 3);
        }
        else if (!topStack((*stk)).isChar){
            tmp = topStack((*stk)).payload;
        }
        else {
            goto END;
        }
        popStack((*stk));
        pushStack(s, tmp, 0);
    }

    if( StackIsEmpty(s)) {
        *size = 1;
        return 0;
    }
    if (sizeOfStack(s) > 1) {
        *size = sizeOfStack(s);
        while(sizeOfStack(s) > 0) {
            pushStack((*stk), topStack(s).payload, topStack(s).isChar);
            popStack(s);
        }
        return 0;
    }
    else {
        return topStack(s).payload;
    }
}

void rePolish(Stk* stk) {
    Stk s;
    bit isNum = 0;
    char t;
    unsigned char str = 0x80;
    resetStack(s);
    for( ; iterate_lcd_ram( str ) ; str++) {
        t = iterate_lcd_ram( str );
        if ( t == 0 )
            break;
        else if (t == '(') {
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
        } else if (t == 'u') {
            pushStack(s, 'u', 1);
        }
        else if (t == '*' || t == '/') {
            while (OpLevel(topStack(s).payload) >= OpLevel(t)) {
                pushStack((*stk), topStack(s).payload, 1);
                popStack(s);
            }
            pushStack(s, t, 1);
        } else if (t == '+' || t == '-') {
            if (isNum) {
                while (OpLevel(topStack(s).payload) >= OpLevel(t)) {
                    pushStack((*stk), topStack(s).payload, 1);
                    popStack(s);
                }
                pushStack(s, t, 1);
            }
            else {
                if (t == '-') {
                    pushStack(s, 'm', 1);
                }
            }
        } else {
            float strnum = 0;
            float loadBit;
            if (!IsNumber(iterate_lcd_ram( str ))) {
                error = 1;
                return;
            }
            while (IsNumber(iterate_lcd_ram( str ))) {
                strnum *= 10;
                strnum += iterate_lcd_ram( str ) - '0';
                ++str;
            }
            if (iterate_lcd_ram( str ) == '.') {
                ++str;
                loadBit = 0.1f;
                while(IsNumber(iterate_lcd_ram( str ))) {
                    strnum = strnum + (iterate_lcd_ram( str ) - '0') * loadBit;
                    loadBit *= 0.1;
                    ++str;
                }
            }
            pushStack((*stk), strnum, 0);
            --str;
            t = iterate_lcd_ram(str);
        }
        isNum = IsNumber(t);
        if (sizeOfStack((*stk)) >= 3) {
            if (!StackIsEmpty(s) && !topStack((*stk)).isChar && TopLevel(topStack(s).payload)) {
                pushStack((*stk), topStack(s).payload, topStack(s).isChar);
                popStack(s);
            }
            reverseStack(stk);
            result = calc(stk, &error);
            reverseStack(stk);
            if (error == 0) {
                pushStack((*stk), result, 0);
            }
        }
    }
    while (!StackIsEmpty(s)) {
        pushStack((*stk), topStack(s).payload, topStack(s).isChar);
        popStack(s);
    }
}

void calculate() {
    Stk stk;
    result = 0;
    resetStack(stk);
    rePolish(&stk);
    reverseStack(&stk);
    result = calc(&stk, &error);
}

void display_string()
{
	unsigned char counter;
	char trailZero;
	float weight;
    weight = 1e16;
    if (error || result / weight >= 10) {
        LcdWriteData('E');
        LcdWriteData('r');
        LcdWriteData('r');
        LcdWriteData('o');
        LcdWriteData('r');
        return;
    }
    if (result < 0) {
        LcdWriteData('-');
        result = -result;
        weight = 1e15;
    }
    if (fabs(result) > 1e-3) {
        result += 0.0005;
        while ((int) (result / weight) == 0) {
            weight /= 10;
        }
    }
    else {
        weight = 1;
    }
    trailZero = 0;
    if (weight < 1) {
        trailZero = 1;
        LcdWriteData('0');
        LcdWriteData('.');
    }
    for( counter = 0; counter < 16 && trailZero < 4; counter++)
    {
        LcdWriteData((int)( result / weight) + '0');
        result = result - ((int)(result / weight)) * weight;
        if (trailZero) {
            ++trailZero;
        }
        if (((int)weight) == 1) {
            LcdWriteData('.');
            trailZero = 1;
        }
        weight /= 10;
        //LcdWriteData( Lcd_result_string[counter] );
    }
}


			
void main()
{
	unsigned char key;
	char counter = 0;
	LcdInit();
	while(1)
	{
		if ( get_key( &key ) == 1 )
		{
			RESTART:
		 	if ( key == '=')
			{
				
#ifdef __CALC__
				if ( counter == 0)
						continue;
				calculate();
				if ( counter > 16 )
				{
						LcdWriteCom( 0xC0 + counter-16 );
						LcdWriteCom( 0x06 );
				}
				else
				{
					LcdWriteCom( 0xC0 );
					LcdWriteCom( 0x06 );
				}
				display_string();
				counter = 0;
				while( 1 )
				{
					if ( get_key( &key ) == 1)
					{
						LcdWriteCom( 0x01 );
						LcdWriteCom( 0x06 );
						goto RESTART;
					}
				}
#endif

			}
			else	if ( key == 'C' )
			{
				LcdWriteCom( 0x01 );
				LcdWriteCom( 0x06 );
				counter = 0;
			}
			else
			{
				if ( counter == 16 )
				{
					LcdWriteCom( 0x07 ); 	
				}
				else if ( counter == 38 )
				{
					continue;
				}
				LcdWriteData( key );
				
				if ( key == '$' || key == 'u' )
				{
						LcdWriteData( '(' );
						counter++;
				}

				counter++;
			}

			
		}	
	}
}


