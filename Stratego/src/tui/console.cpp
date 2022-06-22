#if defined __unix__ || defined __APPLE__
    #include <termios.h>
    #include <unistd.h>
#endif

#include <util.h>
#include <regex>
#include <iostream>

#include "console.h"

using namespace stratego::view;

std::array<std::string, 5092> Console::history_ {};
int Console::hist_p_ {};

void Console::clear() noexcept{
#if defined __unix__ || defined __APPLE__
    std::system("clear");
#else
    std::system("cls");
#endif
}

int Console::getch(void) noexcept{
    char ch;
#if defined __unix__ || defined __APPLE__
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO /* 0 */, &oldattr); // fetch terminal parameters

    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // ICANON unset for non-canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // update terminal settings

    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // recover old terminal parameters
    return ch;
#else
    std::cin >> ch;
    return ch;
#endif
}

bool Console::isArrowKey(int ch) noexcept{
    return ch >= KeyCode::VK_ARROW_UP && ch <= KeyCode::VK_ARROW_LEFT;
}

std::string Console::readline(const std::function<void(void)>& specialTabFunc) noexcept{
#if defined __unix__ || defined __APPLE__
    std::string input {};
    std::vector<int> linebuff {};
    int tmp_hist_p = hist_p_;
    int input_p {-1};
    int input_l {(int) input.length()};
    unsigned char tabf {0x00};

    int ch;
    while((ch = getch()) != '\n'){
        linebuff.push_back(ch);
        if(std::regex_match(std::string{static_cast<char>(ch)}, std::regex{"[[:alnum:] ]{1}"})){

            // arrow keys are considered valid
            if(isArrowKey(ch) && linebuff.size() > 2 && !(linebuff[linebuff.size() - 2] ^ KeyCode::VK_LSQUARE_BRACKET) && !(linebuff[linebuff.size() - 3] ^ KeyCode::VK_ESC)){ // valid key
                switch(ch){
                    case KeyCode::VK_ARROW_UP:
                        if(hist_p_ > 0){
                            replace(input, history_[--hist_p_], input_p);
                            input_p = input.length() - 1;
                            input_l = (int) input.length();
                        }

                        break;
                    case KeyCode::VK_ARROW_DOWN:
                        if(hist_p_ < tmp_hist_p){
                            replace(input, history_[++hist_p_], input_p);
                            input_p = input.length() - 1;
                            input_l = (int) input.length();
                        }

                        break;
                    case KeyCode::VK_ARROW_LEFT:
                        if(input_p > -1){
                            std::cout << '\b';
                            input_p--;
                        }

                        break;
                    case KeyCode::VK_ARROW_RIGHT:
                        if(input_p < input_l - 1){
                            std::cout << input[++input_p];
                        }

                        break;
                }
            }else{
                input_p++;
                input.insert(input_p, std::string{static_cast<char>(ch)});

                if(input_p < input_l){
                    for(int i = 0; i <= input_l - input_p; i++){
                        std::cout << input[input_p + i];
                    }
                    std::cout << util::strrepeat("\b", input_l - input_p);
                } else{
                    std::cout << input[input_p];
                }

                input_l = (int) input.length();
            }
        } else if(!(ch ^ KeyCode::VK_DEL) && ~input_p){
            input.erase(input_p, 1);
            std::cout << '\b'; // go back
            input_p--;
            input_l--;
            for(int i = 0; i < input_l - input_p - 1; i++){
                std::cout << input[input_p + i + 1];
            }
            std::cout << " "; // erase
            std::cout << util::strrepeat("\b", input_l - input_p); // replace the pointer
        } else if(ch == KeyCode::VK_HTAB){
            tabf |= 0x01;
            if(!(tabf ^ 0x03)){
                specialTabFunc();
                int i {};
                while(i < input_l){
                    std::cout << input[i];
                    i++;
                }
                std::cout << util::strrepeat("\b", input_l - input_p - 1);
                tabf ^= 0x03;
            }
            tabf <<= 1;
        } else if(ch == KeyCode::VK_DOLLAR){
            while(input_p < input_l - 1){
                std::cout << input[++input_p];
            }
        } else if(ch == KeyCode::VK_CIRCUMFLEX){
            while(input_p > -1){
                std::cout << '\b';
                input_p--;
            }
        }

    }

    std::cout << std::endl;
    hist_p_ = tmp_hist_p;
    if(!util::isblankstr(input)){
        history_[hist_p_] = input;
        hist_p_ = (hist_p_ + 1) % history_.size();
    }
    return input;
#else
    std::string input {};
    std::getline(std::cin, input);
    return input;
#endif
}

void Console::replace(std::string& input, const std::string& replacement, int input_p){
    int excess = (int) (input.length() - input_p - 1);
    for(int i = 0; i < excess; i++){
        std::cout << input[++input_p];
    }
    for(size_t i = 0; i < input.length(); i++){ // erase
        std::cout << '\b';
    }

    for(size_t i = 0; i < replacement.length(); i++){
        std::cout << replacement.at(i);
    }

    if(replacement.length() < input.length()){
        int excess = input.length() - replacement.length();
        std::cout << util::strrepeat(" ", excess);
        std::cout << util::strrepeat("\b", excess);
    }

    input = replacement;
}


