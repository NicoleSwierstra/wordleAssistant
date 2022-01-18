//I was annoyed i wasn't able to test my wordle program so i made wordle

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

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
        if(last[i] == word[i]) printf("\033[43;30m");
        else if(word.find(last[i]) != std::string::npos) printf("\033[42;30m");
        std::cout << last[i];
        printf("\033[40;37m");
    }
    std::cout << "\n";
}

int main(void){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO scbi;
    GetConsoleScreenBufferInfo(hOut, &scbi);
    COORD newSize;
    newSize.X = scbi.dwSize.X;
    newSize.Y =  scbi.srWindow.Bottom - scbi.srWindow.Top + 1;
    int Status = SetConsoleScreenBufferSize(hOut, newSize);
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | 0x0004);

    getStrings("common.txt", "uncommon.txt");

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
            std::cout << "victory!!!";
            return 1;
        }

        bool realword = false;
        for(const std::string& str : common){
            if(str == word){
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
    std::cout << "defeat!!! the word was: " << word;
    return 0;
}