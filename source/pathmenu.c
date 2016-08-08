#include "fs.h"
#include "draw.h"
#include "pathmenu.h"

u32 PathMenu()
{

	
	int lsize = 0x1000;
	char* list = (char*) 0x21000000;
	memcpy(list, "\0", 0x1000);
	
	
	GetFileList("/Launcher_A9LH/payload", list, lsize, false);
	
	int j = 0;
	int countpay = 0;
	

	for(int i = 0; i < 23; i++)
	{	
		list++;
	}

	while(*list != '\0')
	{

		if(*list == '\n')
		{
			
			c[countpay][j-4] = '\0';
			j = 0;
			countpay++;
			
			for(int g = 0; g < 24; g++)
			{	
				list++;
			}
		}
		c[countpay][j] = list[0];
		list++;
		j++;
		
	}

  return countpay;
}