#include "Core.h"
#include "JsonFileManagerStatics.h"										// Includes DialogueContent, so we dont include that here again.

auto main() -> int
{
    std::cout
        << "Welcome to the dialogue generation tool ULTIMATE edition!\n"
        << "\tThe program would first like to check the filesystem for the target directory in which to create the JSON.\n"
        << "\n\t\tPlease press ENTER to confirm...\n";
    CoreHelpers::UserConfirmation(13);
    CoreHelpers::ClearScreen();

#pragma region Dialogue Folder Check
    if (!JsonFileManagerStatics::FindTargetDirectory())
    {
        std::cout
            << "I couldn't find CultTheVillage's dialogue folder in the filesystem below me.\n"
            << "\t- You could try to ensure you have the project in your computer...\n"
            << "\t- Or ensure I am placed inside (or above) the project folder.\n"
            << "\n\t\tPlease press ENTER to close this window...\n";
        CoreHelpers::UserConfirmation(13);
        return 0;
    }
    std::cout << "Folder found, proceeding...\n\n";

#pragma endregion

#pragma region Naming File
    while(true)
    {
        std::cout
            << "\tWould you be so kind and enter a name for the dialogue file itself?\n"
            << "Current file name is:\t" << JsonFileManagerStatics::JsonFileName << "\n"
            << "New file name shall be?\n";

        std::getline(std::cin, JsonFileManagerStatics::JsonFileName);

        std::cout
            << "New file name is:\t" << JsonFileManagerStatics::JsonFileName << "\n"
            << YesOrNoStr;
        if (CoreHelpers::YesOrNo())
        {
            break;
        }
        CoreHelpers::ClearScreen();
    }

#pragma endregion

#pragma region NPCId Set
    while(true)
    {
        CoreHelpers::ClearScreen();
        std::cout
            << "Would you be so kind and enter an identification string for the NPC for whom this dialogue shall be attributed?\n"
            << "New NPCId shall be?\n";
        std::getline(std::cin, DialogueTree::NPCId);

        std::cout
            << "New NPCId is:\t" << DialogueTree::NPCId << "\n"
            << YesOrNoStr;
        if (CoreHelpers::YesOrNo())
        {
            break;
        }
    }

#pragma endregion

#pragma region Recursion Starts
    while(true)
    {
        CoreHelpers::ClearScreen();
        DialogueTree::NodeInput();

        std::cout
            << "\n\nYou have just finished writing a dialogue! Congratulations.\n"
            << "\tYou may now press Y and create your JSON... or press N and start the entirety of this step from scratch.\n"
            << "\t\t\t(Choose wisely)\n";
        if (CoreHelpers::YesOrNo())
        {
            break;
        }
    }

#pragma endregion

#pragma region File Gets Created
    CoreHelpers::ClearScreen();
    JsonFileManagerStatics::CreateTheDialogueJsonFile();

    std::cout << "\nThe process has been finished! You may now press ENTER and enjoy your new dialogue...\n";
    CoreHelpers::UserConfirmation(13);

#pragma endregion
    return 0;
}