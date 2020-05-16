#include <windows.h>

extern WORD DEFAULT_COLOR;//Default color of text in console
extern HANDLE hConsole; //Default Console Details

void saveDefaultAttributes();
void changeText(WORD COLOR);

void saveDefaultAttributes(){
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    /* writeArrayOfStrings current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    DEFAULT_COLOR = consoleInfo.wAttributes;
}

void changeTextColor(WORD COLOR){
	SetConsoleTextAttribute(hConsole, COLOR);
}
