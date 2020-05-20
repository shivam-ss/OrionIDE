#include <windows.h>

void setCursor(int X, int Y);
int getCursorX();
int getCursorY();

void setCursor(int X, int Y){
	//initialize objects for cursor manipulation
	HANDLE hConsole =  GetStdHandle(STD_OUTPUT_HANDLE);
	COORD destCoord;
	//position cursor at start of window
	destCoord.X = X;
	destCoord.Y = Y;
	SetConsoleCursorPosition(hConsole, destCoord);
}

int getCursorX(){
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
   GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
   return bufferInfo.dwCursorPosition.X;
}
int getCursorY(){
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
   GetConsoleScreenBufferInfo(hConsole, &bufferInfo);
   return bufferInfo.dwCursorPosition.Y;
}
