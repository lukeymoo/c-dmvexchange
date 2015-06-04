#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <iostream>
#include <cstdlib>
#include <pqxx/pqxx>
#include "user.hpp"

class Database {
	public:
		Database(const char *db_options)
		try : conn(db_options) {
			// test connection
			if(conn.is_open()) {
				std::cout << "[+] Connected to database `" << conn.dbname() << "`" << std::endl;
			} else {
				std::cout << "[-] Failed to connect to database" << std::endl;
				exit(1);
			}
		} catch(std::exception &e) {
			std::cout << "[-] Server error => " << e.what() << std::endl;
			exit(1);
		}
		~Database();

		// does table exist
		static bool table_exist(pqxx::connection *c, std::string table_name);

		// does username exist
		static bool username_exist(pqxx::connection *c, std::string username);

		// does email exist
		static bool email_exist(pqxx::connection *c, std::string email);

		// is username + password a valid login
		static bool username_login(pqxx::connection *c, std::string username, std::string password);

		// is email + password a valid login
		static bool email_login(pqxx::connection *c, std::string email, std::string password);

		// create user table
		static bool create_user_table(pqxx::connection *c);

		// get user info from database returns a std::map of data, users found by id, username, email
		static std::map<std::string, std::string> getUser(pqxx::connection *c, int id);
		//static std::map<std::string, std::string> getUser(pqxx::connection *c, std::string username);
		//static std::map<std::string, std::string> getUser(pqxx::connection *c, std::string email);

		pqxx::connection conn;
};



#endif