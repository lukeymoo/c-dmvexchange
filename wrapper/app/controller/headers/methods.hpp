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


class DatabaseClass {
	public:
		// open connection
		DatabaseClass() : conn("dbname=dmvexchange user=db password=f9a548add9f0007ca4071e06f04e3f81 hostaddr=173.66.17.87 port=5432") {
			std::cout << "[+] Connection opened" << std::endl;
		}
		~DatabaseClass(); // destroy connections

		pqxx::connection conn;

		void reconnect(); // reconnects inactive connection
};


std::string get_time(); // gets current unix epoch in ms

namespace crypto {
	std::string sha512_enc(std::string input);
	std::string sha512_noenc(std::string input);
	std::string generate(std::string username, std::string password);
	namespace gen_password {
		// gets user account & generates a password as usual
		std::string by_username(std::string username, std::string password);
		std::string by_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::string password);
		std::string by_id(std::shared_ptr<DatabaseClass> &db, int id, std::string password);
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
		void notice_password(std::string email); // notify of password change
		void notice_new_email(std::string email, std::string username); // notify secondary email added to specified username's account
	};
};

namespace error {
	void send(std::string err_msg);
};

// database related methods
namespace db {

	namespace block {
		void by_id(std::shared_ptr<DatabaseClass> &db, int owner_id, std::string to_block);
		void by_username(std::shared_ptr<DatabaseClass> &db, std::string owner_username, std::string to_block);
	};

	namespace update {
		namespace user {
			// Single key,value pairs
			void by_id(std::shared_ptr<DatabaseClass> &db, int id, std::pair<std::string, std::string> data);
			void by_username(std::shared_ptr<DatabaseClass> &db, std::string username, std::pair<std::string, std::string> data);
			void by_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::pair<std::string, std::string> data);
			// double key,value pairs
			void by_id(std::shared_ptr<DatabaseClass> &db, int id, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2);
			void by_username(std::shared_ptr<DatabaseClass> &db, std::string username, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2);
			void by_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2);
		};
	};

	namespace get_user {
		std::map<std::string, std::string> by_id(std::shared_ptr<DatabaseClass> &db, int id);
		std::map<std::string, std::string> by_username(std::shared_ptr<DatabaseClass> &db, std::string username);
		std::map<std::string, std::string> by_email(std::shared_ptr<DatabaseClass> &db, std::string email);
		std::map<std::string, std::string> by_forgot_token(std::shared_ptr<DatabaseClass> &db, std::string token);
	};

	namespace check_exist {
		bool table(std::shared_ptr<DatabaseClass> &db, std::string table_name);
		bool username(std::shared_ptr<DatabaseClass> &db, std::string username);
		bool email(std::shared_ptr<DatabaseClass> &db, std::string email);
		bool forgot_token(std::shared_ptr<DatabaseClass> &db, std::string token);
	};

	namespace check_in {
		namespace blocked_list {
			bool by_id(std::shared_ptr<DatabaseClass> &db, int owner_id, std::string blocked_username);
			bool by_username(std::shared_ptr<DatabaseClass> &db, std::string owner_username, std::string blocked_username);
		};
	};

	namespace get {
		namespace blocked_list {
			std::vector<std::string> by_id(std::shared_ptr<DatabaseClass> &db, int owner_id);
			std::vector<std::string> by_username(std::shared_ptr<DatabaseClass> &db, std::string owner_username);
		};
	};

	namespace try_login {
		// for use without pre-encrypted passwords
		bool with_username(std::shared_ptr<DatabaseClass> &db, std::string username, std::string password);
		bool with_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::string password);

		// already encrypted passwords
		bool with_username_enc(std::shared_ptr<DatabaseClass> &db, std::string username, std::string password);
		bool with_email_enc(std::shared_ptr<DatabaseClass> &db, std::string email, std::string password);
	};

	namespace create_table {
		void user(std::shared_ptr<DatabaseClass> &db);
		void blocked_list(std::shared_ptr<DatabaseClass> &db);
	};

};



#endif