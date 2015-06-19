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
#include "methods.hpp"

#define ID_USERNAME 	500
#define ID_EMAIL		501

// Lowercase a string
std::string to_lowercase(std::string word);
std::string generate_token(std::string email);

class DatabaseClass;

class UserModel {
	public:
		UserModel(
			std::string firstname, std::string lastname, std::string username, std::string email,
			std::string password, std::string token, int zipcode, std::string gender
		);
		~UserModel();

		// Data to store about a user
		std::string firstname;
		std::string lastname;
		std::string username;
		std::string email;
		std::string password;
		std::string token;
		int zipcode;
		std::string gender;

		void save(std::shared_ptr<DatabaseClass> &db);
};

#endif
