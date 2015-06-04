#include "user.hpp"

UserModel::UserModel() {
  return;
}

UserModel::~UserModel() {
  return;
}

/** Helpers **/
/*

Username Allowed Characters

A-Z => 65 -> 90
a-z => 97 -> 122
0-9 => 48 -> 57
_ 	=> 95 		( underscore character )

*/
bool UserModel::validUsername(std::string word) {
	int length = 0;
	// lower case string
	std::string lowercase = to_lowercase(word);
	// iterate and test characters ( if i makes it through check its GOOD )
	for(char &c : lowercase) {
		// a-z ?
		if(c >= 97 && c <= 122) {
			++length;
			continue;
		} else {
			// 0-9 ?
			if(c >= 48 && c <= 57) {
				++length;
				continue;
			} else {
				// is it an underscore ?
				if(c == 95) {
					++length;
					continue;
				} else {
					// No match == bad character
					return false;
				}
			}
		}
	}
	// if the length is below 2 or over 16 == bad username
	if(length < 2 || length > 16) {
		return false;
	}
	return true;
}

// fuck checking lol, just make sure no spaces and less than 65 characters
// must only contain 1 `@` symbol & there must be at least 3 characters AFTER `@`
bool UserModel::validEmail(std::string word) {
	int cafter = 0;
	int numat = 0;
	bool afterat = false;
	// lower case string
	std::string lowercase = to_lowercase(word);
	// space checking
	for(char &c : lowercase) {
		if(afterat) {
			++cafter;
		}
		if(c == '@') {
			++numat;
			afterat = true;
		}
		if(c == ' ') {
			return false;
		}
	}
	// `@` checker -> needs only 1
	if(numat != 1) {
		return false;
	}
	// ensure cafter >= 3
	if(cafter < 3) {
		return false;
	}
	// length check
	if(lowercase.length() > 64) {
		return false;
	}
	return true;
}

/**
	Validate password
	Must be 2-32 characters
*/
bool UserModel::validPassword(std::string word) {
	if(word.length() < 2 || word.length() > 32) {
		return false;
	}
	return true;
}

/**
	Lowercase characters in string
*/
std::string UserModel::to_lowercase(std::string word) {
	std::locale loc;
	std::stringstream ss;
	for(std::string::size_type i = 0; i < word.length(); i++) {
		ss << std::tolower(word[i], loc);
	}
	return ss.str();
}


