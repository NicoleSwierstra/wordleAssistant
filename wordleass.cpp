#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>
#include <conio.h>
#include <regex>

#define WINDOWS

#ifdef WINDOWS
#include <windows.h>
#endif

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define BACKSPACE 8
#define ESCAPE 27
#define SPACE ' '

std::vector<std::string> common;
std::vector<std::string> uncommon;

std::string input;
bool illegal[26];
bool inword[26];
bool mode;
int cursor = 0, ycursor = 0;

bool isIllegal(std::string s){
    for(char c : s){
        if(illegal[c - 'a']) return true;
    }
    return false;
}

bool isValid(std::string s){
    int i = 0;
    for(const char& c : input){
        if(c == '-');
        else if(c != s[i]) return false;
        i++;
    }
    for(i = 0; i < 26; i++){
        if(!inword[i]) continue;
        if(s.find('a' + i) == std::string::npos) return false;
    }
    return true;
}

void printBoardLine(std::string line){
    for(const char& c : line){
        if(c - 'a' >= 0 && c - 'a' < 26){
            if (illegal[c - 'a']) printf("\033[31m");
            else if (inword[c - 'a']) printf("\033[33m");
            else printf("\033[37m");
            std::cout << c << " ";
        }
        else std::cout << " ";
    }
    std::cout << "\n";
}

void getStrings(const std::string& filepathc, const std::string&  filepathu){
    std::ifstream isc(filepathc);
    std::ifstream isu(filepathu);

    std::string line;
    while(getline(isc, line))
        common.push_back(line);
    while(getline(isu, line))
        uncommon.push_back(line);
}

void search(std::string token, int display){
    if(token.length() == 0) return;

    int i = 0;
    
    printf("\033[38;5;76m");
    for(std::string s : common){
        if(isValid(s) && !isIllegal(s)){
            if(i >= ycursor) std::cout << s << "\n";
            i++;
        }
        if(i > display + ycursor) return;
    }
    printf("\033[38;5;70m");
    for(std::string s : uncommon){
        if(isValid(s) && !isIllegal(s)){
            if(i >= ycursor) std::cout << s << "\n";
            i++;
        }
        if(i > display + ycursor) return;
    }
}

void render(){
    printf("\033[2J\033[H\033[37m");

    if(!mode){
        std::cout << input;
        if(input.length() == 0) return;
        printf("\033[s\n");
        std::string token = input;
        token.resize(5, '-');

        search(token, 25);
        printf("\033[u");
    } 
    if(mode){
        std::cout << "uppercase = yellow\nlowercase = red\n";
        printBoardLine("qwertyuiop");
        printBoardLine(" asdfghjkl");
        printBoardLine("  zxcvbnm");
    }
}

int main(void){
    #ifdef WINDOWS
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO scbi;
    GetConsoleScreenBufferInfo(hOut, &scbi);
    COORD newSize;
    newSize.X = scbi.dwSize.X;
    newSize.Y =  scbi.srWindow.Bottom - scbi.srWindow.Top + 1;
    int Status = SetConsoleScreenBufferSize(hOut, newSize);
    DWORD cmode;
    GetConsoleMode(hOut, &cmode);
    SetConsoleMode(hOut, cmode | 0x0004);
    #else
    //idk linux

    #endif


    std::cout << "WELCOME TO WORDLE ASSISTANT!\n\n" <<
        "this program will help you in your escapades of wordle dominance by finding all 5 letter words that fit in a space\n" <<
        "to look up a word, type it in! for any missing letters use a \"-\"\n"
        "tab switches between search and remove mode with tab";


    getStrings("common.txt", "uncommon.txt");

    mode = true;
    while(1) {
        char c = getch();
        switch(c){
            case KEY_LEFT:
                if(cursor) cursor--;
                break;
            
            case KEY_RIGHT:
                if(input.length()-cursor) cursor++;
                break;
            
            case KEY_UP:
                if(ycursor) ycursor--;
                break;
            
            case KEY_DOWN:
                ycursor++;
                break;
            
            case BACKSPACE:
                if(!mode){
                    if(!cursor) break;
                    input.erase(cursor-1);
                    cursor--;
                }
                else
                    for(int i = 0; i < 26; i++) {illegal[i] = 0; inword[i] = 0;}
                break;
            case ESCAPE:
                return 0;
            case '\t':
                mode = !mode;
                break;
            default:
                if(!mode){
                    if(!(5-cursor)) break;
                    if(!isalpha(c) && c != '-') break;
                    if(cursor==input.length())
                        input = input + c;
                    else input[cursor] = c;
                    cursor++;
                    ycursor = 0;
                }
                else {
                    if(c >= 'A' && c < 'Z'){
                        inword[c - 'A'] = !inword[c - 'A'];
                        illegal[c - 'A'] = false;
                    }
                    else if (c >= 'a' && c < 'z'){
                        illegal[c - 'a'] = !illegal[c - 'a'];
                        inword[c - 'a'] = false;
                    }
                }
                break;
        }

        render();
    }
}