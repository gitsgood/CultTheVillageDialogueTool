#include "Core.h"

void CoreHelpers::ClearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	// Assume POSIX (Linux/Mac)
	system("clear");
#endif
}

void CoreHelpers::UserConfirmation(const int Key)
{
	while (true)
	{
		int PressedKey{ _getch() };
#if DEBUG == 1
		std::cout << PressedKey << "\n";
#endif
		if (PressedKey == Key)
		{
			break;
		}
	}
}

bool CoreHelpers::YesOrNo()
{
	while(true)
	{
        char PressedKey = std::tolower(_getch());

        if (PressedKey == 'y') return true;
        if (PressedKey == 'n') return false;

        std::cout << "\nInvalid key. Please press Y or N.\n";
	}
}

void CoreHelpers::SafeIntegerInput(int& outReturnedInt)
{
    using namespace std;

    constexpr int INTMIN_32 = -2147483648;
    constexpr int INTMAX_32 = 2147483647;

    string userInput{};
    cout << "Please, input thine integer...\n";
    while (true) {
        long long extractedInt{ 0 };
        int signInt{ 1 };
        int signOffset{ 0 };
        bool invalidInput{ false };

        getline(cin, userInput);
        if (userInput.empty()) { cout << "Don't just sit there doing nothing! Try again...\n"; continue; }
        if (userInput[0] == '-') { signInt = -1; signOffset = 1; }
        if (userInput[0] == '+') { signOffset = 1; }
        if (signOffset == 1 && userInput.size() == 1) { cout << "Cheeky little bugger, don't just throw signs at me, INPUT SOMETHING!\n"; continue; }
        if (userInput.size() - signOffset > 10) { cout << "WE CANNOT CONTAIN THIS MANY NUMBERS IN AN INTEGER SPELL\n\tTry again...\n"; continue; }
        for (int i = 0 + signOffset; i < userInput.size(); i++) {
            if ((userInput[i] > '9' || userInput[i] < '0')) {
                cout << "Thine input contains a non digit, this shall not pass! -> '" << userInput[i] << "'\n";
                invalidInput = true;
                continue;
            }
            extractedInt = extractedInt * 10 + (userInput[i] - '0');
        }
        if (invalidInput) { cout << "\tTRY AGAIN YOU RASCAL!\n"; continue; }
        if (extractedInt > INTMAX_32 || extractedInt * signInt < INTMIN_32) {
            cout << "Thine number is an affront to GOD, show some HUMILITY.\n";
            continue;
        }
        outReturnedInt = extractedInt * signInt;
        break;
    }
}
