#ifndef __METHODS_H__
#define __METHODS_H__

#include <cppcms/util.h>
#include <iostream>
#include <cstdlib>
#include <pqxx/pqxx>
#include <boost/regex.hpp>
#include "user.hpp"
#include "base64.hpp"
#include <openssl/sha.h>

std::string get_time(); // gets current unix epoch in ms

namespace crypto {
	std::string sha512_enc(std::string input);
	std::string sha512_noenc(std::string input);
	std::string generate(std::string username, std::string password);
	namespace gen_password {
		// gets user account & generates a password as usual
		std::string by_username(std::string username, std::string password);
		std::string by_email(pqxx::connection *dbconn, std::string email, std::string password);
		std::string by_id(pqxx::connection *dbconn, int id, std::string password);
	};
};

namespace form {
	// Don't know how to get form fields using a function, for now
	// everything is done manually ex..
	// form["username"] = request().post("username")
	// ---
	//std::map<std::string, std::string> get_register_form();
	//std::map<std::string, std::string> get_login_form();
	
	std::string generate_query(std::map<std::string, std::string> form);
	bool validName(std::string name);
	bool validUsername(std::string username);
	bool validPassword(std::string password);
	bool validEmail(std::string email);
	bool validZipcode(std::string zipcode);
};

// contains methods for email & internal mailing/chat system
namespace mail {
	std::string generate_token(std::string email);
	namespace external {
		void send_registration(std::string email, std::string token); // activate account
		void send_pwd_reset(std::string email, std::string token); // forgot password
		void send_username(std::string email, std::string username); // forgot username
		void notice_password(std::string email); // update password change
	};
};

namespace error {
	void send(std::string err_msg);
};

// database related methods
namespace db {

	namespace update {
		namespace user {
			// Single key,value pairs
			void by_id(pqxx::connection *c, int id, std::pair<std::string, std::string> data);
			void by_username(pqxx::connection *c, std::string username, std::pair<std::string, std::string> data);
			void by_email(pqxx::connection *c, std::string email, std::pair<std::string, std::string> data);
			// double key,value pairs
			void by_id(pqxx::connection *c, int id, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2);
			void by_username(pqxx::connection *c, std::string username, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2);
			void by_email(pqxx::connection *c, std::string email, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2);
		};
	};

	namespace get_user {
		std::map<std::string, std::string> by_id(pqxx::connection *c, int id);
		std::map<std::string, std::string> by_username(pqxx::connection *c, std::string username);
		std::map<std::string, std::string> by_email(pqxx::connection *c, std::string email);
		std::map<std::string, std::string> by_forgot_token(pqxx::connection *c, std::string token);
	};

	namespace check_exist {
		bool table(pqxx::connection *c, std::string table_name);
		bool username(pqxx::connection *c, std::string username);
		bool email(pqxx::connection *c, std::string email);
		bool forgot_token(pqxx::connection *c, std::string token);
	};

	namespace try_login {
		bool with_username(pqxx::connection *c, std::string username, std::string password);
		bool with_email(pqxx::connection *c, std::string email, std::string password);
	};

	namespace create_table {
		bool user(pqxx::connection *c);
	};

};



#endif