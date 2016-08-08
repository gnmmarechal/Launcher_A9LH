#include "menu.h"
#include "draw.h"
#include "hid.h"
#include "fs.h"
#include "tga.h"
#include "payload.h"
#include "fatfs/ff.h"
#include "pathmenu.h"
u32 Unlock()
{	
	u32 unlockSequence[] = { BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A, BUTTON_START, BUTTON_SELECT },
        sequenceSize = sizeof(unlockSequence) / sizeof(u32);

    for(u32 correctPresses = 0; correctPresses < sequenceSize; correctPresses++)
    {
        if(InputWait() != unlockSequence[correctPresses])
            return 1;
    }
}
//int count
void DrawMenu(u32 count, u32 index, bool fullDraw)
{
    bool top_screen = true;
   
    u32 menu_x0 = (top_screen) ? 76 : 36;
    u32 menu_x1 = (top_screen) ? 76 : 36;
    u32 menu_y0 = 50;
    u32 menu_y1 = menu_y0 + count * 13;
    
	char pathtga[60];
	
	if (fullDraw) { // draw full menu
        
		ClearScreenFull(true, !top_screen);
		
		snprintf(pathtga, 60, "Launcher_A9LH/menu/menu.tga");
		loadtga(true,false,pathtga,0,0);
	
		for (u32 i = 0; i < count; i++) 
		{
			loadtga(true,false,"Launcher_A9LH/Menu/barre.tga",menu_x0,menu_y0 + (i*13));
		}
		
		DrawStringFColor(RED  , TRANSPARENT, menu_x0, menu_y0 - 20, top_screen, "Launcher A9LH v1.0");
		DrawStringFColor(WHITE, TRANSPARENT, menu_x0, menu_y0 - 10, top_screen, "------------------------------");
        DrawStringFColor(WHITE, TRANSPARENT, menu_x0, menu_y1 +  0, top_screen, "------------------------------");
        DrawStringFColor(WHITE, TRANSPARENT, menu_x0, menu_y1 + 10, top_screen, "START:Poweroff   SELECT:Reboot");
		DrawStringFColor(WHITE, TRANSPARENT, menu_x0, menu_y1 + 20, top_screen, "A:Launch");
        
		if (CheckFS()) {
			DrawStringFColor(WHITE, TRANSPARENT, menu_x1, SCREEN_HEIGHT - 20, top_screen, "SD storage: %lluMB", TotalStorageSpace() / (1024*1024));
			DrawStringFColor(WHITE, TRANSPARENT, menu_x1, SCREEN_HEIGHT - 10, top_screen, "Used: %lluMB/Free: %lluMB", TotalStorageSpace() / (1024*1024) - RemainingStorageSpace() / (1024*1024), RemainingStorageSpace() / (1024*1024));
		} else {
            DrawStringFColor(RED  , TRANSPARENT, menu_x1, SCREEN_HEIGHT - 20, top_screen, "SD storage: unknown filesystem");
        }
		
    }
	
	ClearScreenFull(false, true);
	snprintf(pathtga, 60, "%s/%s%s", PATHLOGO, c[index], TGA);
	loadtga(false,true,pathtga,0,0);
    
	for (u32 i = 0; i < count; i++) {
        
		char* name = c[i];
		
		if(i != index)
		DrawStringFColor(WHITE, TRANSPARENT, menu_x0 + 16, menu_y0 + (i*13 + 2), top_screen, " %s ", name);
		
		if(i == index)
		DrawStringFColor(BLACK, TRANSPARENT, menu_x0 + 16, menu_y0 + (i*13 + 2), top_screen, " %s ", name);
		
	}
}

u32 ProcessEntry(u32 index)
{
    loadPayload(index);
}


void BatchScreenshot(u32 count, bool full_batch)
{
    for (u32 idx_m = 0; c[idx_m] != NULL; idx_m++) {
        for (u32 idx_s = 0; idx_s < ((full_batch) ? count : 1); idx_s++) {
            char filename[16];
            snprintf(filename, 16, "menu%04lu.bmp", (idx_m * 100) + idx_s);
           DrawMenu(count + idx_m, idx_s, true);
           
            Screenshot(filename);
        }
    }
}
u32 ProcessMenu()
{
    
	//Mise en memoire des noms des payload dans le "char c"
	//et retourne "count" le nombre de payload present
	
	u32 count = PathMenu();
    
	u32 index = 0;
    u32 result = MENU_EXIT_REBOOT;
   
	char* filename;
	int screenshot = 0;
	DrawMenu(count, index, true);
   
 // main processing loop
    while (true) 
	{
        
		
		bool full_draw = true;
        u32 pad_state = InputWait();
        
		if (pad_state & BUTTON_A) {
            
			pad_state = ProcessEntry(index);
			
		} else if (pad_state & BUTTON_DOWN) {
           
		   index = (index == count - 1) ? 0 : index + 1;
           full_draw = false;
			
		} else if (pad_state & BUTTON_UP) {
            
			index = (index == 0) ? count - 1 : index - 1;
			full_draw = false;
			
		} else if (pad_state & BUTTON_X) {
            
			(pad_state & (BUTTON_LEFT | BUTTON_RIGHT)) ?
			BatchScreenshot(count, pad_state & BUTTON_RIGHT) : Screenshot(NULL);
			
		} else {
			
			full_draw = false;
			
		}
		
		if (pad_state & BUTTON_START) {
           PowerOff();
        }
        if (pad_state & BUTTON_SELECT) {
           Reboot();
        }
       
		
		DrawMenu(count, index, full_draw);
        
    }
    
    return result;
}
