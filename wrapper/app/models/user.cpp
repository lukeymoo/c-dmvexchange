#include "user.hpp"

UserModel::UserModel() {
  return;
}

UserModel::~UserModel() {
  return;
}

/*
	@FUNCTION - Return specified string in lowercase format
*/
std::string to_lowercase(std::string word) {
	std::locale loc;
	std::stringstream ss;
	for(std::string::size_type i = 0; i < word.length(); i++) {
		ss << std::tolower(word[i], loc);
	}
	return ss.str();
}
