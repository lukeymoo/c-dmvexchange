/*

Performs database queries to retrieve data about users
or to insert data about users

*/

#ifndef __USER_MODEL_H__
#define __USER_MODEL_H__

#define MALE 500
#define FEMALE 501

#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <boost/regex.hpp>

#define ID_USERNAME 	500
#define ID_EMAIL		501

// Lowercase a string
std::string to_lowercase(std::string word);

class UserModel {
	public:
		UserModel();
		~UserModel();

		// Data to store about a user
		std::string firstname;
		std::string lastname;
		std::string username;
		std::string email;

		int gender; // MALE | FEMALE ( 'm' | 'f' )
		int zipcode;

		// Not used currently <- will be collected at later date
		int age;

		// Validate name
		static bool validName(std::string name);
		// Validate username
		static bool validUsername(std::string username);
		// Validate email
		static bool validEmail(std::string email);
		// Validate password
		static bool validPassword(std::string password);
};

#endif
