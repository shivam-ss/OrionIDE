#include <windows.h>

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

HANDLE hConsole; //Default Console Details

void saveDefaultAttributes();
void changeTextColor(int color);

void saveDefaultAttributes(){
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    /* writeArrayOfStrings current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
}
void changeTextColor(int color){
	WORD COLOR = (color & 0x0F);
	SetConsoleTextAttribute(hConsole, COLOR);
}
void changeBackgroundColor(int BColor, int TColor){
	WORD COLOR = ((BColor & 0x0F) << 4) + (TColor & 0x0F);
	SetConsoleTextAttribute(hConsole, COLOR);
}
