#ifndef C215_H
#define CC215_H
#include <Windows.h>
#include <time.h>
#include <dos.h>
#include <stdlib.h>

struct Location
{
	float x, y;

	Location(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Location()
	{
		x = y = 0;
	}
};

void goto_xy(int x, int y)
{
	COORD cr;
	cr.X = x;
	cr.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cr);
}

Location getConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return Location(rows, columns);
}

void clearScreen()
{
	system("cls");
}

int getRandomNumber(int max)
{
	return rand() % max + 1;
}

int getScreenWidth()
{
	return getConsoleSize().y;
}

int getScreenHeight()
{
	return getConsoleSize().x;
}

#endif