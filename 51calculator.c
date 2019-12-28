#include<reg51.h>
#include"lcd.h"
#include "516_input.h"


unsigned char _characters[24] = {
 '0', '1', '2', '3',
 '4', '5', '6', '7',
 '8', '9', '.', 'C',
 '+', '-', '*', '/',
 '(', ')', '#', '=',
 'A', 'B', 'C', 'D'
};
			
void main()
{
	unsigned char key;
	LcdInit();
	while(1)
	{
		//keyscan();
		if ( get_key( &key ) == 1 )
		{
		 	LcdWriteData( _characters[key] );
		}	
	}
}
