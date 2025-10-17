#ifndef COLORS_H
# define COLORS_H

# include <string>

# define RED "\033[1;31m"
# define BLUE "\033[1;34m"
# define YELLOW "\033[1;33m"
# define GREEN "\033[1;32m"
# define CYAN "\033[1;36m"
# define MAGENTA "\033[1;35m"
# define BLACK "\033[1;30m"
# define WHITE "\033[1;37m"

# define BG_RED "\033[1;31;7m"
# define BG_BLUE "\033[1;34;7m"
# define BG_YELLOW "\033[1;33;7m"
# define BG_GREEN "\033[1;32;7m"
# define BG_CYAN "\033[1;36;7m"
# define BG_MAGENTA "\033[1;35;7m"
# define BG_BLACK "\033[1;30;7m"
# define BG_WHITE "\033[1;37;7m"

# define RESET "\033[0m"

std::string	bg_red(std::string);
std::string red(std::string);
std::string bg_blue(std::string);
std::string blue(std::string);
std::string bg_green(std::string);
std::string green(std::string);
std::string bg_yellow(std::string);
std::string yellow(std::string);

std::string welcome_message();

#endif
