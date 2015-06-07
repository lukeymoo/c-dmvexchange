#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <iostream>
#include <cstdlib>
#include <pqxx/pqxx>
#include <boost/regex.hpp>
#include "user.hpp"
#include "base64.hpp"
#include <openssl/sha.h>

namespace crypto {
	std::string sha512_enc(std::string input);
	std::string sha512_noenc(std::string input);
	std::string generate(std::string username, std::string password);
};

// form related methods ** CANT FIGURE OUT HOW TO INTEGRATE YET **
namespace form {
	std::map<std::string, std::string> get_register_form();
	std::map<std::string, std::string> get_login_form();
	std::string generate_query(std::map<std::string, std::string> form);
	bool validName(std::string name);
	bool validUsername(std::string username);
	bool validPassword(std::string password);
	bool validEmail(std::string email);
	bool validZipcode(std::string zipcode);
};


// database related methods
namespace db {

	namespace get_user {
		std::map<std::string, std::string> by_id(pqxx::connection *c, int id);
		std::map<std::string, std::string> by_username(pqxx::connection *c, std::string username);
		std::map<std::string, std::string> by_email(pqxx::connection *c, std::string email);
	};

	namespace check_exist {
		bool table(pqxx::connection *c, std::string table_name);
		bool username(pqxx::connection *c, std::string username);
		bool email(pqxx::connection *c, std::string email);
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