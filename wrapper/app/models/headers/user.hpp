/*

Performs database queries to retrieve data about users
or to insert data about users

*/

#ifndef __USER_MODEL_H__
#define __USER_MODEL_H__

#define MALE 500
#define FEMALE 501

#include <string>

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

};

#endif
