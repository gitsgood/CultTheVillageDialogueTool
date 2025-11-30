#pragma once

#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <cstdlib>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
char getch() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}
#endif

constexpr const char* YesOrNoStr{ "\n\t\tPress Y to continue, or N to repeat this step...\n" };

#define DEBUG 1

// ENTER _getch() = 13
// Y _getch() = 121
// N _getch() = 110

class CoreHelpers
{
public:

    static void ClearScreen();

    static void UserConfirmation(const int Key);

    static bool YesOrNo();

    static void SafeIntegerInput(int& outReturnedInt);
};

#endif // !CORE.H