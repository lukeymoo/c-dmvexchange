#include "database.hpp"

Database::~Database() {
	std::cout << std::endl << "Disconnecting from server" << std::endl;
	conn.disconnect();
	return;
}

// does table exist
bool Database::table_exist(pqxx::connection *c, std::string table_name) {
	pqxx::work worker(*c); // create worker
	// format query
	std::string query =	"SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name=" + c->quote(table_name) + " AND table_schema='public')";
	// execute
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
		throw;
	}
	return false;
}

// does username exist
bool Database::username_exist(pqxx::connection *c, std::string username) {
	pqxx::work worker(*c); // create worker
	std::string query = "SELECT COUNT(*) FROM dmv_users_t WHERE username=" 
		+ c->quote(username);
	// execute query
	try {
		pqxx::result count = worker.exec(query.c_str());
		std::cout << count[0][0] << std::endl;
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return false;
}

// does email exist
bool Database::email_exist(pqxx::connection *c, std::string email) {
	pqxx::work worker(*c); // create worker
	std::string query = "SELECT COUNT(*) FROM dmv_users_t WHERE email=" 
		+ c->quote(email) + "";
	// execute query
	try {
		pqxx::result count = worker.exec(query.c_str());
		std::cout << count[0][0] << std::endl;
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return false;
}

/*
	Field Name 		DATA TYPE 		Other Specs
	~~~~~~~~~~ 	 	~~~~~~~~~ 		~~~~~~~~~~~
	id  				INT 		PRIMARY KEY NOT NULL
	firstname 		VARCHAR(32) 	2-32 Characters Length
	lastname 		VARCHAR(32) 	2-32 Characters Length
	username 		VARCHAR(16) 	2-16 Characters Length
	email 			VARCHAR(64) 	5-64 Characters Length
	password 		VARCHAR(32) 	2-32 Characters Length
	token	 		VARCHAR(32) 	32 Character hash ( if empty, user is activated )
	zipcode 		VARCHAR(5) 		5 Characters Length
	timestamp 		INT 		 	Created in program std::time(0)
*/
bool Database::create_user_table(pqxx::connection *c) {
	pqxx::work worker(*c);
	// prepare query
	std::string query = "CREATE TABLE dmv_users_t (id INT, firstname VARCHAR(32), lastname VARCHAR(32), username VARCHAR(16), email VARCHAR(64), password VARCHAR(32), token VARCHAR(32), zipcode VARCHAR(5), timestamp INT, PRIMARY KEY(id, username, email))";
	try {
		// execute query
		pqxx:result result = worker.exec(query.c_str());
	} catch(std::exception &e) {
		throw; // bubble exception up
	}
	return false;
}