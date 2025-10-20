#include "colors.hpp"
#include <string>
#include <sstream>

std::string bg_red(std::string text) {
	return (BG_RED + text + RESET);
}

std::string red(std::string text) {
	return (RED + text + RESET);
}

std::string bg_blue(std::string text) {
	return (BG_BLUE + text + RESET);
}

std::string blue(std::string text) {
	return (BLUE + text + RESET);
}

std::string bg_green(std::string text) {
	return (BG_GREEN + text + RESET);
}

std::string green(std::string text) {
	return (GREEN + text + RESET);
}

std::string bg_yellow(std::string text) {
	return (BG_YELLOW + text + RESET);
}

std::string yellow(std::string text) {
	return (YELLOW + text + RESET);
}


std::string welcome_message() {

	std::ostringstream msg;

	msg << "\n\n______welcome to\n";

	msg << "      $$\\                     \n";
	msg << "      \\__|                    \n";
	msg << "      $$\\  $$$$$$\\   $$$$$$$\\ \n";
	msg << "      $$ |$$  __$$\\ $$  _____|\n";
	msg << "      $$ |$$ |  \\__|$$ /      \n";
	msg << "      $$ |$$ |      $$ |      \n";
	msg << "      $$ |$$ |      \\$$$$$$$\\ \n";
	msg << "      \\__|\\__|       \\_______|\n";
	msg << "                              \n";
	msg << " 	  our InstantRegretChat	  \n";
	msg << "                              \n";
	return (msg.str());
}
