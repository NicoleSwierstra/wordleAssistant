#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctype.h>
#include <conio.h>
#include <regex>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define BACKSPACE 8
#define ESCAPE 27

std::vector<std::string> common;
std::vector<std::string> uncommon;

std::string input;
int cursor = 0;

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
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

void search(std::string token){
    if(token.length() == 0) return;
    std::regex regex = std::regex(token);

    int i = 0;
    for(std::string s : common){
        if(std::regex_match(s, regex)){
            std::cout << s << "\n";
            i++;
        }
        if(i > 20) return;
    }
}

void render(){
    printf("\033[2J\033[H");
    std::cout << input;
    printf("\033[s\n");
    std::string token = input;
    token.resize(5, '-');

    replaceAll(token, "-", "\\w");
    search(token);
    printf("\033[u");
}

int main(void){
    getStrings("common.txt", "uncommon.txt");

    std::cout << "\033[s"; //saves cursor pos

    while(1) {
        char c = getch();
        switch(c){
            /*
            case KEY_LEFT:
                if(cursor) cursor--;
                break;
            case KEY_RIGHT:
                if(4-cursor) cursor++;
                break;
            case KEY_UP:
                if(cursory) cursory--;
                break;
            case KEY_DOWN:
                cursory++;
                break;
            */
            case BACKSPACE:
                if(!cursor) break;
                input.erase(cursor-1);
                cursor--;
                break;
            case ESCAPE:
                return 1;
            default:
                if(!(5-cursor)) break;
                input = input + c;
                cursor++;
        }

        render();
    }
}