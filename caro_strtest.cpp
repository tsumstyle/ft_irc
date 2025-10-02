#include <string>
#include <iostream>

bool	valid_prefix(const std::string& cmd) {
	if (cmd[0] != '-' && cmd[0] != '+') {
		std::cout << "wrong prefix: " << cmd[0] << std::endl;
		return false;
	}
	for (size_t i = 1; i < cmd.size(); i++) {
		if (cmd[i] == '-' || cmd[i] == '+') {
			std::cout << "extra sign: " << cmd[i] << std::endl;
			return false;
		}
	}
	return true;
}

bool	is_in_str(const char c, const std::string& str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == c)
			return true;
	}
	return false;
}

// all options in itokl. not repeated
bool	valid_chars(const std::string& options) {
	for (size_t i = 1; i < options.size(); i++) {
		if (!is_in_str(options[i], "itokl")) {
			std::cout << "invalid char: " << options[i] << std::endl;
			return false; // not of valid chars
		}
		if (is_in_str(options[i], options.substr(i + 1, options.size() - 1))) {
			std::cout << "char repeated: " << options[i] << std::endl;
			return false; // if its repeated?
		}
	}
	return true;
}

int main (int argc, char* argv[]) {
	if (argc != 2)
		return 1;
	if (valid_prefix(argv[1]) && valid_chars(argv[1]))
		std::cout << "all good!" << std::endl;
	return 0;
}
