//I was annoyed i wasn't able to test my wordle program so i made wordle

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
//the function declaration begins
#ifdef __cplusplus
extern "C" {
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX
lpConsoleCurrentFontEx);
}
#endif

std::vector<std::string> common;
std::vector<std::string> uncommon;

void getStrings(const std::string& filepathc, const std::string&  filepathu){
    std::ifstream isc(filepathc);
    std::ifstream isu(filepathu);

    std::string line;
    while(getline(isc, line))
        common.push_back(line);
    while(getline(isu, line))
        uncommon.push_back(line);
}

void eraseLast(){
    printf("\033[1A\033[2K");
}

void checkLast(const std::string& last, const std::string& word){
    eraseLast();
    for(int i = 0; i < 5; i++){
        if(last[i] == word[i]) printf("\033[42;30m");
        else if(word.find(last[i]) != std::string::npos) printf("\033[43;30m");
        std::cout << last[i];
        printf("\033[40;37m");
    }
    std::cout << "\n";
}

int main(void){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO scbi;
    GetConsoleScreenBufferInfo(hOut, &scbi);
    COORD newSize{8, 9};
    int Status = SetConsoleScreenBufferSize(hOut, newSize);
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | 0x0004);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = 60;                  // Heights
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    std::cout << "WORDLE!\n";

    getStrings("common.txt", "uncommon.txt");

    while(1){
        std::cout << "press any key to continue\n";
        getchar();

        std::cout << "\033[2J\033[HWORDLE!\n";

        srand(time(0));

        std::string word = common[rand() % common.size()];

        int i = 0;
        while(i < 6){
            std::string in;
            std::getline(std::cin, in);
            if(in.length() != 5) {
                eraseLast();
                continue;
            }

            if(in == word) {
                checkLast(in, word);
                std::cout << "victory!!\n";
                goto r;
            }

            bool realword = false;
            for(const std::string& str : common){
                if(str == in){
                    realword = true;
                    break;
                }
            }

            if(!realword){
                eraseLast();
                continue;
            }

            checkLast(in, word);
            i++;
        }
        std::cout << "defeat!!\n" << word << "\n";
        r:;
    }
}