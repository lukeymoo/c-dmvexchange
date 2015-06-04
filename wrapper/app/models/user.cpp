#include "user.hpp"

UserModel::UserModel() {
  return;
}

UserModel::~UserModel() {
  return;
}

/*
	@FUNCTION - Determines if specified string can be a valid name
	@SPECS - Regex, 2-32 characters
	@RETURNS - TRUE/FALSE
*/
bool UserModel::validName(std::string name) {
	boost::regex regex("^[A-Za-z]+(([\'-])?[A-Za-z]+$)");
	if(!boost::regex_match(name, regex)) {
		return false;
	}
	if(name.length() < 2 || name.length() > 32) {
		return false;
	}
	return true;
}

/*
	@FUNCTION - Determines if a specified string can be a valid username
	@SPECS - Regex, 2-16 characters length
	@RETURNS - TRUE/FALSE
*/
bool UserModel::validUsername(std::string username) {
	boost::regex regex("^[A-Za-z0-9_]+$");
	if(!boost::regex_match(username, regex)) {
		return false;
	}
	if(username.length() < 2 || username.length() > 16) {
		return false;
	}
	return true;
}

/*
	@FUNCTION - Determines if a specified string can be a valid email
	@SPECS 	- Regex, 5-64 characters
	@RETURNS - TRUE/FALSE
*/
bool UserModel::validEmail(std::string email) {
	boost::regex regex("^([a-zA-Z0-9_.+-])+\\@(([a-zA-Z0-9-])+\\.)+([a-zA-Z0-9]{2,4})+$");
	if(!boost::regex_match(email, regex)) {
		return false;
	}
	if(email.length() < 5 || email.length() > 64) {
		return false;
	}
	return true;
}

/*
	@FUNCTION - Determines if a specified string can be a valid password
	@SPECS - String length must be 2-32 characters
	@RETURNS - TRUE/FALSE
*/
bool UserModel::validPassword(std::string password) {
	if(password.length() < 2 || password.length() > 32) {
		return false;
	}
	return true;
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
