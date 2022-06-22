#include "ansiColor.h"

using namespace stratego::view;

AnsiColor::AnsiColor(Value v) : v_ {v},  ansiColor_ {ESCAPE_SEQ + std::to_string(v_)}
{}

std::string AnsiColor::get() const noexcept{
    return get(false);    
}

std::string AnsiColor::get(bool bold) const noexcept{
#if defined __unix__ || defined __APPLE__
    switch(v_){
        case BOLD:
        case UNDERLINE:
        case REVERSED:
        case RESET: return ansiColor_ + "m";
        default:
            return ansiColor_ + (bold ? ";1m" : "m");
    }
#else
    return {};
#endif
}

std::string AnsiColor::colorize(const std::string& text) const noexcept{
    return colorText(text, v_);
}

std::string AnsiColor::extendedColor(int codePoint){
    return extendedColor(codePoint, false);
}

std::string AnsiColor::extendedColor(int codePoint, bool bg){
    if(codePoint < 0 || codePoint > 255){
        throw std::invalid_argument("Invalid codePoint");
    }

#if defined __unix__ || defined __APPLE__
    char buffer[50];
    std::sprintf(buffer, "%s%d;5;%dm", ESCAPE_SEQ, bg ? 48 : 38, codePoint);
    return std::string{buffer};
#else
    return {};
#endif
}

std::string AnsiColor::colorText(const std::string& text, int codePoint){
    return colorText(text, codePoint, false);
}

std::string AnsiColor::colorText(const std::string& text, int codePoint, bool bg){
#if defined __unix__ || defined __APPLE__
    AnsiColor stop {RESET};
    return extendedColor(codePoint, bg) + text + stop.get();
#else
    return text;
#endif
}

std::string AnsiColor::colorText(const std::string& text, Value color) noexcept{
    return colorText(text, color, false); 
}

std::string AnsiColor::colorText(const std::string& text, Value color, bool bold) noexcept{
#if defined __unix__ || defined __APPLE__
    AnsiColor col {color};
    AnsiColor stop {RESET};
    return col.get(bold) + text + stop.get(); 
#else
    return text;
#endif
}

std::ostream& stratego::view::operator<<(std::ostream& os, const AnsiColor& color){
    os << color.get();
    return os;
}

