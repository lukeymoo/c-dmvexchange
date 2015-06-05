#include "database.hpp"

Database::~Database() {
	std::cout << std::endl << "[+] Disconnecting from database" << std::endl;
	conn.disconnect();
	return;
}

/*
	@FUNCTION - Checks if specified table exists in database
	@RETURNS - TRUE/FALSE
*/
bool db::table_exist(pqxx::connection *c, std::string table_name) {
	pqxx::work worker(*c); // create worker
	// lowercase table name
	std::string table_name_f = to_lowercase(table_name);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name=" + c->quote(table_name_f) + " AND table_schema='public')";
	try {
		// execute query
		pqxx::result result = worker.exec(query.c_str());
		pqxx::result::tuple row = result[0];
		pqxx::result::field field = row[0];
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		throw;
	}
	return false;
}

/*
	@FUNCTION - Determines if specified username exists in database
	@RETURNS - TRUE/FALSE
*/
bool db::username_exist(pqxx::connection *c, std::string username) {
	pqxx::work worker(*c); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM dmv_users_t WHERE username=" + c->quote(username_f) + " )";
	try {
		// execute query
		pqxx::result count = worker.exec(query.c_str());
		pqxx::result::tuple row = count[0];
		pqxx::result::field field = row[0];
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return false;
}

/*
	@FUNCTION - Determines if email exists in database
	@RETURNS - TRUE/FALSE
*/
bool db::email_exist(pqxx::connection *c, std::string email) {
	pqxx::work worker(*c); // create worker
	// lowercase email
	std::string email_f = to_lowercase(email);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM dmv_users_t WHERE email=" + c->quote(email_f) + ")";
	try {
		// execute query
		pqxx::result count = worker.exec(query.c_str());
		pqxx::result::tuple row = count[0];
		pqxx::result::field field = row[0];
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return false;
}

/*
	@FUNCTION - Determines if specified username & password is a valid combination in database
	@RETURNS - TRUE/FALSE
*/
bool db::username_login(pqxx::connection *c, std::string username, std::string password) {
	pqxx::work worker(*c); // create worker
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE username=" + c->quote(username) + " AND password=" + c->quote(password) + ")";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::result::tuple row = result[0];
		pqxx::result::field field = row[0];
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return false;
}

/*
	@FUNCTION - Determines if specified email & password is a valid combination in database
	@RETURNS - TRUE/FALSE
*/
bool db::email_login(pqxx::connection *c, std::string email, std::string password) {
	pqxx::work worker(*c); // create worker
	// lowercase email
	std::string email_f = to_lowercase(email);
	/* Add password hashing here */

	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE email=" + c->quote(email_f) + " AND password=" + c->quote(password) + ")";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::result::tuple row = result[0];
		pqxx::result::field field = row[0];
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return false;
}

/*
	@FUNCTION - Creates a user table ( contains user account info )
	@RETURNS - TRUE/FALSE
	@MISC - 
		======================================================================
		==				 			TABLE STRUCTURE 						==
		======================================================================
		Field Name 		DATA TYPE 		Other Specs
		~~~~~~~~~~ 	 	~~~~~~~~~ 		~~~~~~~~~~~
		id  			SERIAL	 		PRIMARY KEY NOT NULL
		firstname 		VARCHAR(32) 	2-32 Characters Length
		lastname 		VARCHAR(32) 	2-32 Characters Length
		username 		VARCHAR(16) 	2-16 Characters Length
		email 			VARCHAR(64) 	5-64 Characters Length
		password 		VARCHAR(32) 	2-32 Characters Length
		token	 		VARCHAR(32) 	32 Character hash ( if empty, user is activated )
		zipcode 		VARCHAR(5) 		5 Characters Length
		timestamp 		INT 		 	Created in program std::time(0)
*/
bool db::create_user_table(pqxx::connection *c) {
	pqxx::work worker(*c); // create worker
	// prepare query
	std::string query = "CREATE TABLE dmv_users_t (id SERIAL PRIMARY KEY, firstname VARCHAR(32) NOT NULL, lastname VARCHAR(32) NOT NULL, username VARCHAR(16) UNIQUE NOT NULL, email VARCHAR(64) UNIQUE NOT NULL, password VARCHAR(32) NOT NULL, token VARCHAR(32) NOT NULL, zipcode INT NOT NULL, timestamp INT NOT NULL)";
	try {
		// execute query
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return true;
}

/*
	@FUNCTION - Grabs user info from database, user found by id, username or email info is returned into std::map
		If nothing was found an empty std::map is returned
	@RETURNS - TRUE/FALSE
*/
std::map<std::string, std::string> db::get_user::by_id(pqxx::connection *c, int id) {
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(*c); // create worker
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE id=" + c->quote(id);
	try {
		// execute query
		pqxx::result result = worker.exec(query.c_str());

		// return to prevent memory access error if no results
		if(result.empty()) {
			return info;
		}

		pqxx::result::const_iterator row_i = result.begin();

		pqxx::result::tuple field = row_i;
		field["id"] >> info["id"];
		field["firstname"] >> info["firstname"];
		field["lastname"] >> info["lastname"];
		field["username"] >> info["username"];
		field["email"] >> info["email"];
		field["zipcode"] >> info["zipcode"];
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return info;
}


std::map<std::string, std::string> db::get_user::by_username(pqxx::connection *c, std::string username) {
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(*c); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE username=" + c->quote(username_f);
	try {
		// execute query
		pqxx::result result = worker.exec(query.c_str());

		// return to prevent memory access error if no results
		if(result.empty()) {
			return info;
		}

		pqxx::result::const_iterator row_i = result.begin();

		pqxx::result::tuple field = row_i;
		field["id"] >> info["id"];
		field["firstname"] >> info["firstname"];
		field["lastname"] >> info["lastname"];
		field["username"] >> info["username"];
		field["email"] >> info["email"];
		field["zipcode"] >> info["zipcode"];
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return info;
}

std::map<std::string, std::string> db::get_user::by_email(pqxx::connection *c, std::string email) {
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(*c); // create worker
	// lowercase email
	std::string email_f = to_lowercase(email);
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE email=" + c->quote(email_f);
	try {
		// execute query
		pqxx::result result = worker.exec(query.c_str());

		// return to prevent memory access error if no results
		if(result.empty()) {
			return info;
		}

		pqxx::result::const_iterator row_i = result.begin();

		pqxx::result::tuple row = row_i;
		if(row["id"].is_null()) {
			info["id"] = "";
		} else {
			row["id"] >> info["id"];
		}
		if(row["firstname"].is_null()) {
			info["firstname"] = "";
		} else {
			row["firstname"] >> info["firstname"];
		}
		if(row["lastname"].is_null()) {
			info["lastname"] = "";
		} else {
			row["lastname"] >> info["lastname"];
		}
		if(row["username"].is_null()) {
			info["username"] = "";
		} else {
			row["username"] >> info["username"];
		}
		if(row["email"].is_null()) {
			info["email"] = "";
		} else {
			row["email"] >> info["email"];
		}
		if(row["zipcode"].is_null()) {
			info["zipcode"] = "";
		} else {
			row["zipcode"] >> info["zipcode"];
		}
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return info;
}
