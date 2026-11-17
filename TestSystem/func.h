
#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include "Settings.h"

using namespace std;

enum Colors
{
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

void setcolor(int text, int background)
{
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        (WORD)((background << 4) | text)
    );
}

void clear()
{
    system("cls");
    setcolor(White, Blue);
}

void pause()
{
    setcolor(White, Blue);
    cout << "\n\nНажмите Enter...";
    cin.get();
}

string readtext(string message)
{
    string value;

    cout << message;
    setcolor(Black, White);
    getline(cin, value);
    setcolor(White, Blue);

    return value;
}

int readnumber(string message)
{
    int number;

    cout << message;
    setcolor(Black, White);

    while (!(cin >> number))
    {
        cin.clear();
        cin.ignore(10000, '\n');

        setcolor(White, Blue);
        cout << "Введите число: ";

        setcolor(Black, White);
    }

    cin.ignore(10000, '\n');
    setcolor(White, Blue);

    return number;
}
