#ifndef ANSICOLOR_H
#define ANSICOLOR_H

#include <iostream>

#define ESCAPE_SEQ "\u001b["

namespace stratego::view{

    /**
    * Defines several colors that make use of the ANSI standard.
    *
    * These colors can be directly used in front of and behind a given string
    * of text (by calling the get() method associated with it) to color it or
    * several other methods allow you to directly color a whole given string without
    * having to surround it with the adequate colors.
    *
    * If for any reason, the ANSI colors want to be used directly by refering to their
    * enum litteral or by using the extended set of colors, the RESET field must always be
    * put behind the string to stop the colorization.
    */
    class AnsiColor{

        public:

            enum Value{

                /**
                * The reset color used to stop the colorization.
                */
                RESET = 0,

                /**
                * The black color.
                */
                BLACK = 30,

                /**
                * The red color.
                */
                RED = 31,

                /**
                * The green color.
                */
                GREEN = 32,

                /**
                * The yellow color.
                */
                YELLOW = 33,

                /**
                * The blue color.
                */
                BLUE = 34,

                /**
                * The magenta color.
                */
                MAGENTA = 35,

                /**
                * the cyan color.
                */
                CYAN = 36,

                /**
                * The white color.
                */
                WHITE = 37,

                /**
                * The black background.
                */
                BLACK_BACKGROUND = 40,

                /**
                * The red background.
                */
                RED_BACKGROUND = 41,

                /**
                * The green background.
                */
                GREEN_BACKGROUND = 42,

                /**
                * The yellow background.
                */
                YELLOW_BACKGROUND = 43,

                /**
                * The blue background.
                */
                BLUE_BACKGROUND = 44,

                /**
                * The magenta background.
                */
                MAGENTA_BACKGROUND = 45,

                /**
                * The cyan background.
                */
                CYAN_BACKGROUND = 46,

                /**
                * The white background.
                */
                WHITE_BACKGROUND = 47,

                /**
                * The bold text decoration.
                */
                BOLD = 1,

                /**
                * The underline text decoration.
                */
                UNDERLINE = 4,

                /**
                * The reversed text decoration, inversing the currently used color.
                */
                REVERSED = 7
            };

            /**
             * Constructs an AnsiColor using the given enum value.
             *
             * @param v the value this AnsiColor will take
             */
            AnsiColor(Value v);


            /**
            * Returns the ANSI color sequence associated with this enum litteral as a string.
            *
            * Be default, this method will returns the non-bold version of
            * the color. If the bold version want to be obtained, the get(bool) can
            * be called instead.
            *
            * @return the ANSI color sequence associated with this enum litteral.
            */
            std::string get() const noexcept;

            /**
            * Returns the ANSI color sequence associated with this enum litteral as a string.
            *
            * Using this method, you can specify wether the resulting color sequence will represent
            * the bold version of the color or not by setting the bold flag to true or false
            * respectively.
            *
            * @param bold the bold flag
            * @return the ANSI color sequence associated with this enum litteral.
            */
            std::string get(bool bold) const noexcept;

            /**
            * Colors the given text using the ANSI color used by this object.
            *
            * @param text the string text to color
            * @return the given string of text colored.
            */
            std::string colorize(const std::string& text) const noexcept;


            /**
            * Returns the ANSI color sequence from the extended set of 256 colors based on a given code point.
            * The code point must be included between 0 and 255, otherwise an invalid_argument will be thrown.
            *
            * @param codePoint the code point associated with the desired color
            * @throws invalid_argument if the given code point value is not included between 0 and 255.
            * @return the ANSI color sequence associated with the given code point.
            */
            static std::string extendedColor(int codePoint);

            /**
            * Returns the ANSI color sequence from the extended set of 256 colors based on a given code point.
            *
            * Using this version you can also specify wether the resulting color will be considered as a background
            * or not by setting the bg flag to true or false respectively. Note that the code point must be included
            * between 0 and 255, otherwise an invalid_argument will be thrown.
            *
            * @param codePoint the code point associated with the desired color
            * @param bg the background flag.
            * @throws invalid_argument if the given code point value is not included between 0 and 255.
            * @return the ANSI color sequence associated with the given code point.
            */
            static std::string extendedColor(int codePoint, bool bg);

            /**
            * Colors a given string using the extended set of 256 colors. Note that the code point must be included
            * between 0 and 255, otherwise an invalid_argument will be thrown.
            *
            * @param text the string text to color
            * @param codePoint the code point associated with the desired color
            * @throws invalid_argument if the given code point value is not included between 0 and 255.
            * @return the given string of text colored.
            */
            static std::string colorText(const std::string& text, int codePoint);

            /**
            * Colors a given string using the extended set of 256 colors. Using this version you can also specify
            * wether the resulting color will be considered as a background or not by setting the bg flag to true or
            * false respectively.
            *
            * Note that the code point must be included between 0 and 255, otherwise an invalid_argument will be thrown.
            *
            * @param text the string text to color
            * @param codePoint the code point associated with the desired color
            * @param bg the background flag.
            * @throws invalid_argument if the given code point value is not included between 0 and 255.
            * @return the given string of text colored.
            */
            static std::string colorText(const std::string& text, int codePoint, bool bg);

            /**
            * Colors a given string using the provided ANSI colors.
            *
            * @param text the string text to color
            * @param color the ANSI color with which to color the given text
            * @return the given string of text colored.
            */
            static std::string colorText(const std::string& text, Value color) noexcept;

            /**
            * Colors a given string using the provided ANSI colors.
            *
            * Using this version you can also specify wether the resulting color will be considered as a bold or not by
            * setting the boldflag to true or false respectively.
            *
            * @param text the string text to color
            * @param color the ANSI color with which to color the given text
            * @param bold the bold flag.
            * @return the given string of text colored.
            */
            static std::string colorText(const std::string& text, Value color, bool bold) noexcept;

            /**
            * User-defined conversion into the Value enumeration wrapped by this object. This overload
            * caracterizes AnsiColor class to be contextually implicitly convertible to an enumeration type,
            * allowing this class to be used in switch statement.
            */
            constexpr operator Value() const {return v_;}

            private:

                const Value v_;
                const std::string ansiColor_;
    };

    /**
    * Textual representation of an instance of the AnsiColor class when displaying it to the
    * standard output.
    *
    * @param os the standart output stream
    * @param color instance of the AnsiColor class to be displayed
    */
    std::ostream& operator<<(std::ostream& os, const AnsiColor& color);
};

#endif

