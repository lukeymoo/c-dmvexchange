#include "methods.hpp"

DatabaseClass::~DatabaseClass() {
	conn.disconnect();
	std::cout << "[+] Connection closed" << std::endl;
	return;
}

void DatabaseClass::reconnect() {
	std::cout << "[+] Attempting to reconnect" << std::endl;
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			conn.activate();
			if(conn.is_open()) {
				success = true;
			} else {
				std::cout << "\t => Connection unopened, trying again..." << std::endl;
			}
		} catch(pqxx::broken_connection &e) {
			if(try_count == 1) {
				std::cerr << "[-] Failed to open connection, exception thrown" << std::endl;
				throw;
			}
		}
	}
	return;
}







// Ensure all users have a block list
void db::validate(std::shared_ptr<DatabaseClass> &db) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.quote(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// get list of ids
	std::vector<std::string> id_list = db::get::user::id_list(db);
	// iterate each id
	int id_int = 0;
	for(auto id : id_list) {
		// convert string id to integer
		id_int = std::stoi(id, nullptr, 10);
		// check exist
		if(db::check_exist::block_list(db, id_int)) {
			// do nothing
			std::cout << "\t[+] Block list for ID :: " << id << " exists" << std::endl;
		} else {
			// create list
			std::cout << "\t[+] Creating block list for ID :: " << id << std::endl;
			db::create::block_list(db, id_int);
		}
	}
	return;
}



// @FUNCTION - Place specified username in block list owned by specified id
// @RETURNS - NOTHING
void db::block::by_id(std::shared_ptr<DatabaseClass> &db, int owner_id, std::string target_username) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// format specified username
	std::string target_f = to_lowercase(target_username);
	// format query
	std::string query = "UPDATE dmv_blocked_list_t SET b_username=(SELECT array_append((SELECT b_username FROM dmv_blocked_list_t WHERE owner_id=" + db->conn.quote(owner_id) + "), " + db->conn.quote(target_f) + ")) WHERE owner_id=" + db->conn.quote(owner_id);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Inserting " + target_f + " into blocked list for id => " << owner_id << " :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}





















// @FUNCTION - Puts specified username into block list owned by another specified username
// @RETURNS - Nothing
void db::block::by_username(std::shared_ptr<DatabaseClass> &db, std::string owner_username, std::string target_username) {
	// grab account info and pass to by_id variation
	std::map<std::string, std::string> acc;
	try {
		acc = db::get::user::by_username(db, owner_username);
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Getting account id for block by id => " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	int owner_id = std::stoi(acc["id"]);
	db::block::by_id(db, owner_id, target_username);
	return;
}
























// @FUNCTION - Remove specified username from block list owned by user specified by id
// @RETURNS - NOTHING
bool db::unblock::by_id(std::shared_ptr<DatabaseClass> &db, int owner_id, std::string to_unblock) {
	// ensure connection open
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}

	// format username
	std::string target_f = to_lowercase(to_unblock);

	// fetch blocked list
	std::vector<std::string> block_list = db::get::blocked_list::by_id(db, owner_id);

	// search for specified username
	bool in_list = false;
	for(std::vector<std::string>::iterator it = block_list.begin(); it != block_list.end(); ++it) {
		// if the username is in the list, remove it & update status variable
		if(*it == target_f) {
			in_list = true;
			block_list.erase(it);
			break;
		}
	}

	// if not found return false to notify higher up of invalid specified username
	if(!in_list) {
		return false;
	}

	// perform update block_list query with modified vector
	db::update::block_list::by_id(db, owner_id, block_list);
	return true;
}


























// @FUNCTION - Fetch user id of account specified by username, invoke by_id variation using specified target
// @RETURNS - TRUE/FALSE
bool db::unblock::by_username(std::shared_ptr<DatabaseClass> &db, std::string owner_username, std::string target) {
	return true;
}



























// @FUNCTION - Checks if specified username is in specified users blocked list
// @RETURNS - TRUE/FALSE
bool db::check_in::blocked_list::by_id(std::shared_ptr<DatabaseClass> &db, int owner_id, std::string blocked_username) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// format inputs
	std::string blocked_f = "{" + to_lowercase(blocked_username) + "}";
	// format query
	std::string query = "SELECT EXISTS ( SELECT 1 FROM dmv_blocked_list_t WHERE owner_id=" + db->conn.quote(owner_id) + " AND b_username=" + db->conn.quote(blocked_f) + " )";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple tuple = result.at(0);
		pqxx::field field = tuple.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Error checking if blocked in by id => " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}












// @FUNCTION - Checks if specified username is in specified users blocked list
// @RETURNS - TRUE/FALSE
bool db::check_in::blocked_list::by_username(std::shared_ptr<DatabaseClass> &db, std::string owner_username, std::string blocked_username) {
	std::map<std::string, std::string> acc;
	try {
		acc = db::get::user::by_username(db, owner_username);
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Ensuring connection is open => " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}

	int id = -1;
	
	if(acc["id"] != "") {
		id = std::stoi(acc["id"]);
	}

	if(db::check_in::blocked_list::by_id(db, id, blocked_username)) {
		return true;
	} else {
		return false;
	}
	return false;
}












// @FUNCTION - Checks if specified table exists in database
// @RETURNS - TRUE/FALSE
bool db::check_exist::table(std::shared_ptr<DatabaseClass> &db, std::string table_name) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	pqxx::work worker(db->conn); // create worker
	// lowercase table name
	std::string table_name_f = to_lowercase(table_name);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name=" + db->conn.quote(table_name_f) + " AND table_schema='public')";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// if exception is caught on last try
		// report error to email
		std::ostringstream ss;
		ss << "Checking if table exists :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw;
	}
	return false;
}












// @FUNCTION - Checks if activation token exists
// @RETURNS - TRUE/FALSE
bool db::check_exist::activation_token(std::shared_ptr<DatabaseClass> &db, std::string token) {
	// ensure connection open
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "SELECT EXISTS ( SELECT 1 FROM dmv_users_t WHERE token=" + db->conn.quote(token) + " )";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Checking if activation token exists :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}























// @FUNCTION - Determines if specified username exists in database
// @RETURNS - TRUE/FALSE
bool db::check_exist::username(std::shared_ptr<DatabaseClass> &db, std::string username) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	pqxx::work worker(db->conn); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM dmv_users_t WHERE username=" + db->conn.quote(username_f) + " )";
	// execute query
	try {
		pqxx::result count = worker.exec(query.c_str());
		pqxx::tuple row = count[0];
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Checking if username exists :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}
















// @FUNCTION - Determines if email exists in database
// @RETURNS - TRUE/FALSE
bool db::check_exist::email(std::shared_ptr<DatabaseClass> &db, std::string email) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	pqxx::work worker(db->conn); // create worker
	// lowercase email
	std::string email_f = to_lowercase(email);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM dmv_users_t WHERE email=" + db->conn.quote(email_f) + " OR secondary_email=" + db->conn.quote(email_f) + ")";
	// execute query
	try {
		pqxx::result count = worker.exec(query.c_str());
		pqxx::tuple row = count[0];
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Checking if email exists :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}





















// @FUNCTION - Determines if a specified forgot_token exists in database
// @RETURNS - TRUE/FALSE
bool db::check_exist::forgot_token(std::shared_ptr<DatabaseClass> &db, std::string token) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "SELECT EXISTS ( SELECT 1 FROM dmv_users_t WHERE forgot_token=" + db->conn.quote(token) + ")";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Checking if forgot_token exists :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}











bool db::check_exist::block_list(std::shared_ptr<DatabaseClass> &db, int owner_id) {
	// ensure connection open
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "SELECT EXISTS ( SELECT 1 FROM dmv_blocked_list_t WHERE owner_id=" + db->conn.quote(owner_id) + " )";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Checking if block_list exists for id => " << owner_id << " :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
}



























// @FUNCTION - Determines if specified username & password is a valid combination in database
// @RETURNS - TRUE/FALSE
bool db::try_login::with_username(std::shared_ptr<DatabaseClass> &db, std::string username, std::string password) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	pqxx::work worker(db->conn); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	std::string password_f = crypto::gen_password::by_username(username, password);
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE username=" + db->conn.quote(username_f) + " AND password=" + db->conn.quote(password_f) + ")";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Logging in with username :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}



















// @FUNCTION - Determines if specified email & password is a valid combination in database
// @RETURNS - TRUE/FALSE
bool db::try_login::with_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::string password) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// lowercase email
	std::string email_f = to_lowercase(email);
	// get user account
	std::map<std::string, std::string> user_account = db::get::user::by_email(db, email_f);
	pqxx::work worker(db->conn); // create worker
	if(user_account.empty()) {
		return false;
	}
	std::string password_f = crypto::gen_password::by_username(user_account["username"], password);
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE email=" + db->conn.quote(email_f) + " AND password=" + db->conn.quote(password_f) + ")";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Logging in with email :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}





















// @FUNCTION - Trys to login with a specified username and a pre-encrypted password
// @RETURNS - TRUE/FALSE
bool db::try_login::with_username_enc(std::shared_ptr<DatabaseClass> &db, std::string username, std::string password) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// lowercase email
	std::string username_f = to_lowercase(username);
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE username=" + db->conn.quote(username_f) + " AND password=" + db->conn.quote(password) + ")";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Logging in with username & pre-encrypted pwd :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}



















// @FUNCTION - Try to login with specified email and a pre-encrypted password
// @RETURNS - TRUE/FALSE
bool db::try_login::with_email_enc(std::shared_ptr<DatabaseClass> &db, std::string email, std::string password) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// lowercase email
	std::string email_f = to_lowercase(email);
	pqxx::work worker(db->conn); // create worker
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE email=" + db->conn.quote(email_f) + " AND password=" + db->conn.quote(password) + ")";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		pqxx::tuple row = result.at(0);
		pqxx::field field = row.at(0);
		if(field.as<std::string>() == "t") {
			return true;
		} else {
			return false;
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Logging in with email & pre-encrypted pwd :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return false;
}

















void db::create::block_list(std::shared_ptr<DatabaseClass> &db, int user_id) {
	// ensure connection
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "INSERT INTO dmv_blocked_list_t (owner_id) VALUES (" + db->conn.quote(user_id) + ")";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Creating blocked list for user id => " << user_id << " :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}
























// @FUNCTION - Creates a user table ( contains user account info )
// @RETURNS - TRUE/FALSE
// @MISC - 
//	======================================================================
//	==				 			TABLE STRUCTURE 						==
//	======================================================================
//	Field Name 		DATA TYPE 		Other Specs
//	~~~~~~~~~~ 	 	~~~~~~~~~ 		~~~~~~~~~~~
//	id  			SERIAL	 		PRIMARY KEY NOT NULL
//	firstname 		VARCHAR(32) 	2-32 Characters Length
//	lastname 		VARCHAR(32) 	2-32 Characters Length
//	username 		VARCHAR(16) 	2-16 Characters Length
//	email 			VARCHAR(64) 	5-64 Characters Length
//	password 		VARCHAR(32) 	2-32 Characters Length
//	token	 		VARCHAR(32) 	32 Character hash ( if empty, user is activated )
//	zipcode 		VARCHAR(5) 		5 Characters Length
//	timestamp 		INT 		 	Created in program std::time(0)
void db::create::table::users(std::shared_ptr<DatabaseClass> &db) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	pqxx::work worker(db->conn); // create worker
	// prepare query
	std::string query = "CREATE TABLE dmv_users_t (id SERIAL PRIMARY KEY, firstname VARCHAR(32) NOT NULL, lastname VARCHAR(32) NOT NULL, username VARCHAR(16) UNIQUE NOT NULL, email VARCHAR(64) UNIQUE NOT NULL, secondary_email VARCHAR(64) UNIQUE DEFAULT '', password VARCHAR(256) NOT NULL, token VARCHAR(256) NOT NULL, zipcode INT NOT NULL, gender CHAR(1) NOT NULL, forgot_token VARCHAR(256) NOT NULL DEFAULT '', forgot_timestamp INT NOT NULL, timestamp INT NOT NULL)";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Creating user table :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}

















// @FUNCTION - Create blocked list table in database
// @RETURNS - NOTHING
void db::create::table::block_list(std::shared_ptr<DatabaseClass> &db) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "CREATE TABLE dmv_blocked_list_t (owner_id INT NOT NULL UNIQUE, b_username VARCHAR(32)[])";
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Creating blocked table :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}










void db::create::table::products(std::shared_ptr<DatabaseClass> &db) {
	// Ensure connection is open
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "CREATE TABLE dmv_products_t (id SERIAL PRIMARY KEY, owner_id INT NOT NULL, zipcode CHAR(5) NOT NULL, priority INT NOT NULL DEFAULT '3', post_type VARCHAR(12) NOT NULL, product_type VARCHAR(64) NOT NULL DEFAULT '', description VARCHAR(1024) NOT NULL)";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Creating product table :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}


















// @FUNCTION - Returns list of all ids in database
// @RETURNS - std::vector<std::string> with all ids
std::vector<std::string> db::get::user::id_list(std::shared_ptr<DatabaseClass> &db) {
	// ensure active connection
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// Create vector container for std::string of returned ID's
	std::vector<std::string> list;
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "SELECT id FROM dmv_users_t";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		// iterate results print
		for(pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
			for(pqxx::tuple::const_iterator field = row.begin(); field != row.end(); ++field) {
				list.push_back(field.as<std::string>());
			}
		}
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Getting all user ids :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return list;
}









// @FUNCTION - Grabs user info from database, user found by id, username or email
//				info is returned into std::map If nothing was found an empty
// 				std::map is returned
// @RETURNS - TRUE/FALSE
std::map<std::string, std::string> db::get::user::by_id(std::shared_ptr<DatabaseClass> &db, int id) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(db->conn); // create worker
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE id=" + db->conn.quote(id);
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());

		// return to prevent memory access error if no results
		if(result.empty()) {
			return info;
		}

		pqxx::result::const_iterator row_i = result.begin();

		pqxx::tuple row = row_i;
		try {
			row["id"] >> info["id"];
		} catch(std::exception &e) {
			info["id"] = "";
		}

		try {
			row["firstname"] >> info["firstname"];
		} catch(std::exception &e) {
			info["firstname"] = "";
		}

		try {
			row["lastname"] >> info["lastname"];
		} catch(std::exception &e) {
			info["lastname"] = "";
		}

		try {
			row["username"] >> info["username"];
		} catch(std::exception &e) {
			info["username"] = "";
		}

		try {
			row["email"] >> info["email"];
		} catch(std::exception &e) {
			info["email"] = "";
		}

		try {
			row["secondary_email"] >> info["secondary_email"];
		} catch(std::exception &e) {
			info["secondary_email"] = "";
		}

		try {
			row["password"] >> info["password"];
		} catch(std::exception &e) {
			info["password"] = "";
		}

		try {
			row["token"] >> info["token"];
		} catch(std::exception &e) {
			info["token"] = "";
		}

		try {
			row["zipcode"] >> info["zipcode"];
		} catch(std::exception &e) {
			info["zipcode"] = "";
		}

		try {
			row["gender"] >> info["gender"];
		} catch(std::exception &e) {
			info["gender"] = "";
		}

		try {
			row["forgot_token"] >> info["forgot_token"];
		} catch(std::exception &e) {
			info["forgot_token"] = "";
		}

		try {
			row["forgot_timestamp"] >> info["forgot_timestamp"];
		} catch(std::exception &e) {
			info["forgot_timestamp"] = "";
		}

		try {
			row["timestamp"] >> info["timestamp"];
		} catch(std::exception &e) {
			info["timestamp"] = "";
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Finding user by ID :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return info;
}


















// @FUNCTION - Retreives user account details specified by username which is unique
// 				if no user is found an empty std::map is returned 
// @RETURNS - std::map<std::string, std::string> - key,value pairs of user acc details
std::map<std::string, std::string> db::get::user::by_username(std::shared_ptr<DatabaseClass> &db, std::string username) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(db->conn); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE username=" + db->conn.quote(username_f);
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());

		// return to prevent memory access error if no results
		if(result.empty()) {
			return info;
		}

		pqxx::result::const_iterator row_i = result.begin();

		pqxx::tuple row = row_i;
		try {
			row["id"] >> info["id"];
		} catch(std::exception &e) {
			info["id"] = "";
		}

		try {
			row["firstname"] >> info["firstname"];
		} catch(std::exception &e) {
			info["firstname"] = "";
		}

		try {
			row["lastname"] >> info["lastname"];
		} catch(std::exception &e) {
			info["lastname"] = "";
		}

		try {
			row["username"] >> info["username"];
		} catch(std::exception &e) {
			info["username"] = "";
		}

		try {
			row["email"] >> info["email"];
		} catch(std::exception &e) {
			info["email"] = "";
		}

		try {
			row["secondary_email"] >> info["secondary_email"];
		} catch(std::exception &e) {
			info["secondary_email"] = "";
		}

		try {
			row["password"] >> info["password"];
		} catch(std::exception &e) {
			info["password"] = "";
		}

		try {
			row["token"] >> info["token"];
		} catch(std::exception &e) {
			info["token"] = "";
		}

		try {
			row["zipcode"] >> info["zipcode"];
		} catch(std::exception &e) {
			info["zipcode"] = "";
		}

		try {
			row["gender"] >> info["gender"];
		} catch(std::exception &e) {
			info["gender"] = "";
		}

		try {
			row["forgot_token"] >> info["forgot_token"];
		} catch(std::exception &e) {
			info["forgot_token"] = "";
		}

		try {
			row["forgot_timestamp"] >> info["forgot_timestamp"];
		} catch(std::exception &e) {
			info["forgot_timestamp"] = "";
		}

		try {
			row["timestamp"] >> info["timestamp"];
		} catch(std::exception &e) {
			info["timestamp"] = "";
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Finding user by username :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return info;
}























// @FUNCTION - Retreives user account details specified by email address which is unique
// 				if no user is found an empty std::map is returned
// @RETURNS - std::map<std::string, std::string> - key,value pairs of user acc details
std::map<std::string, std::string> db::get::user::by_email(std::shared_ptr<DatabaseClass> &db, std::string email) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(db->conn); // create worker
	// lowercase email
	std::string email_f = to_lowercase(email);
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE email=" + db->conn.quote(email_f);
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());

		// return to prevent memory access error if no results
		if(result.empty()) {
			return info;
		}

		pqxx::result::const_iterator row_i = result.begin();

		pqxx::tuple row = row_i;
		try {
			row["id"] >> info["id"];
		} catch(std::exception &e) {
			info["id"] = "";
		}

		try {
			row["firstname"] >> info["firstname"];
		} catch(std::exception &e) {
			info["firstname"] = "";
		}

		try {
			row["lastname"] >> info["lastname"];
		} catch(std::exception &e) {
			info["lastname"] = "";
		}

		try {
			row["username"] >> info["username"];
		} catch(std::exception &e) {
			info["username"] = "";
		}

		try {
			row["email"] >> info["email"];
		} catch(std::exception &e) {
			info["email"] = "";
		}

		try {
			row["secondary_email"] >> info["secondary_email"];
		} catch(std::exception &e) {
			info["secondary_email"] = "";
		}

		try {
			row["password"] >> info["password"];
		} catch(std::exception &e) {
			info["password"] = "";
		}

		try {
			row["token"] >> info["token"];
		} catch(std::exception &e) {
			info["token"] = "";
		}

		try {
			row["zipcode"] >> info["zipcode"];
		} catch(std::exception &e) {
			info["zipcode"] = "";
		}

		try {
			row["gender"] >> info["gender"];
		} catch(std::exception &e) {
			info["gender"] = "";
		}

		try {
			row["forgot_token"] >> info["forgot_token"];
		} catch(std::exception &e) {
			info["forgot_token"] = "";
		}

		try {
			row["forgot_timestamp"] >> info["forgot_timestamp"];
		} catch(std::exception &e) {
			info["forgot_timestamp"] = "";
		}

		try {
			row["timestamp"] >> info["timestamp"];
		} catch(std::exception &e) {
			info["timestamp"] = "";
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Finding user by email :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return info;
}















// @FUNCTION - Find and retrieve user by forgot_token
// @RETURNS - std::map<string, std> containing user information
std::map<std::string, std::string> db::get::user::by_forgot_token(std::shared_ptr<DatabaseClass> &db, std::string token) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(db->conn); // create worker
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE forgot_token=" + db->conn.quote(token);
	// execute query
	try {
		pqxx::result result = worker.exec(query.c_str());

		// return to prevent memory access error if no results
		if(result.empty()) {
			return info;
		}

		pqxx::result::const_iterator row_i = result.begin();

		pqxx::tuple row = row_i;
		try {
			row["id"] >> info["id"];
		} catch(std::exception &e) {
			info["id"] = "";
		}

		try {
			row["firstname"] >> info["firstname"];
		} catch(std::exception &e) {
			info["firstname"] = "";
		}

		try {
			row["lastname"] >> info["lastname"];
		} catch(std::exception &e) {
			info["lastname"] = "";
		}

		try {
			row["username"] >> info["username"];
		} catch(std::exception &e) {
			info["username"] = "";
		}

		try {
			row["email"] >> info["email"];
		} catch(std::exception &e) {
			info["email"] = "";
		}

		try {
			row["secondary_email"] >> info["secondary_email"];
		} catch(std::exception &e) {
			info["secondary_email"] = "";
		}

		try {
			row["password"] >> info["password"];
		} catch(std::exception &e) {
			info["password"] = "";
		}

		try {
			row["token"] >> info["token"];
		} catch(std::exception &e) {
			info["token"] = "";
		}

		try {
			row["zipcode"] >> info["zipcode"];
		} catch(std::exception &e) {
			info["zipcode"] = "";
		}

		try {
			row["gender"] >> info["gender"];
		} catch(std::exception &e) {
			info["gender"] = "";
		}

		try {
			row["forgot_token"] >> info["forgot_token"];
		} catch(std::exception &e) {
			info["forgot_token"] = "";
		}

		try {
			row["forgot_timestamp"] >> info["forgot_timestamp"];
		} catch(std::exception &e) {
			info["forgot_timestamp"] = "";
		}

		try {
			row["timestamp"] >> info["timestamp"];
		} catch(std::exception &e) {
			info["timestamp"] = "";
		}
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Finding user by email :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return info;
}
















// @FUNCTION - Generate random string
// @RETURNS - Random string in base64 encryption
std::string crypto::random() {
	std::stringstream ss;
	std::string epoch = get_time();
	int time_r_f = std::rand();
	int time_r_s = std::rand();
	int time_r_t = std::rand();
	ss << epoch << time_r_f << time_r_s << time_r_t;
	std::string final = crypto::sha512_hex(ss.str());
	return final;
}





















// @FUNCTION - sha512 a specified string & return it encoded in hex
// @RETURNS - hex encoded sha512 encrypted string in std::string
std::string crypto::sha512_hex(std::string input_s) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	unsigned char obuf[64];
	memset(obuf, 0, 64);
	SHA512(reinterpret_cast<const unsigned char*>(input_s.c_str()), input_s.length(), obuf); // encrypt
	// loop and convert to hex
	for(auto c : obuf) {
		ss << std::setw(2) << static_cast<unsigned>(c);
	}
	return ss.str();
}






















// @FUNCTION - sha512 a specifed string and base64 encode it
// @RETURNS - std::string - base64 encoded sha512 hash
std::string crypto::sha512_enc(std::string input_s) {
	unsigned char obuf[64];
	memset(obuf, 0, 64);
	SHA512(reinterpret_cast<const unsigned char*>(input_s.c_str()), input_s.length(), obuf); // encrypt
	std::string encoded = base64_encode(obuf, 64); // encode
	return encoded;
}













// @FUNCTION - Sha512 a specified string
// @RETURNS - std::string with no encoding, this results in bad characters
// 				and is why this function is generally unused
std::string crypto::sha512_noenc(std::string input_s) {
	unsigned char obuf[64];
	memset(obuf, 0, 64);
	SHA512(reinterpret_cast<const unsigned char*>(input_s.c_str()), input_s.length(), obuf); // encrypt
	std::string hash_s = reinterpret_cast<const char *>(obuf);
	return hash_s;
}















// @FUNCTION - Generates a Sha512 string base64 encoded
//		takes 2 parameters and hashes them multiple times(7 hash functions)
// @RETURNS - std::string - base64 encoded string
std::string crypto::generate(std::string username, std::string password) {
	std::string username_enc = crypto::sha512_enc(username);
	std::string password_enc = crypto::sha512_enc(password);
	std::string first = crypto::sha512_enc(username_enc + password_enc); // hu + hp
	std::string second = crypto::sha512_enc(first + username); // hfirst + username
	std::string third = crypto::sha512_enc(first + password); // hfirst + password
	std::string fourth = crypto::sha512_enc(second + third); // hsecond + hthird
	std::string final = crypto::sha512_enc(first + fourth); // hfirst + hfourth
	return final;
}














// @FUNCTION - Get user's username by id then generate password by_username
// @RETURNS - std::string of base64 encrypted password
std::string crypto::gen_password::by_id(std::shared_ptr<DatabaseClass> &db, int id, std::string password) {
	std::map<std::string, std::string> info = db::get::user::by_id(db, id);
	std::string password_f = "";
	if(!info.empty()) {
		password_f = crypto::gen_password::by_username(info["username"], password);
	}
	return password_f;
}












// @FUNCTION - Get user's username by email then generate password by_username
// @RETURNS - base64 encrypted password of type std::string
std::string crypto::gen_password::by_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::string password) {
	std::map<std::string, std::string> info = db::get::user::by_email(db, email);
	std::string password_f = "";
	if(!info.empty()) {
		password_f = crypto::gen_password::by_username(info["username"], password);
	}
	return password_f;
}
















// @FUNCTION - Generates a password using username
// @RETURNS - base64 encrypted password of type::string
std::string crypto::gen_password::by_username(std::string username, std::string password) {
	std::string password_f;
	// generate password with crazy hashing technique
	std::string init = crypto::generate(username, password);
	std::string init_username;
	std::string init_password;
	// hash iteratively 2500 times with username
	for(int i = 0; i < 2500; i++) {
		init_username = crypto::generate(init, username);
	}
	// hash iteratively 2500 times with password
	for(int i = 0; i < 3000; i++) {
		init_password = crypto::generate(init, password);
	}
	// put them together with generate
	std::string init_final = crypto::generate(init_username, init_password);
	// hash iteratively 2500 times with init_final
	for(int i = 0; i < 2500; i++) {
		password_f = crypto::generate(init, init_final);
	}
	return password_f;
}






















// @FUNCTION - Generates URI params from a register form to be appended to Url
// 				EX. &f=xxx&u=xxx
// @RETURNS - std::string - uri parameters
std::string form::generate_query(std::map<std::string, std::string> form) {
	std::string query = "";
	// iterate through ignoring p, pa, ea
	for(auto &kv : form) {
		if(kv.first != "p" && kv.first != "pa" && kv.first != "ea" && kv.first != "tos") {
			query += "&" + kv.first + "=" + kv.second;
		}
	}
	return query;
}















// @FUNCTION - Determines if specified string can be a valid name
// @SPECS - Regex, 2-32 characters
// @RETURNS - TRUE/FALSE
bool form::validName(std::string name) {
	boost::regex regex("^[A-Za-z]+(([\'-])?[A-Za-z]+$)");
	if(!boost::regex_match(name, regex)) {
		return false;
	}
	if(name.length() < 2 || name.length() > 32) {
		return false;
	}
	return true;
}















// @FUNCTION - Determines if a specified string can be a valid username
// @SPECS - Regex, 2-16 characters length
// @RETURNS - TRUE/FALSE
bool form::validUsername(std::string username) {
	boost::regex regex("^[A-Za-z0-9_]+$");
	if(!boost::regex_match(username, regex)) {
		return false;
	}
	if(username.length() < 2 || username.length() > 16) {
		return false;
	}
	return true;
}
















// @FUNCTION - Determines if a specified string can be a valid email
// @SPECS 	- Regex, 5-64 characters
// @RETURNS - TRUE/FALSE
bool form::validEmail(std::string email) {
	boost::regex regex("^([a-zA-Z0-9_.+-])+\\@(([a-zA-Z0-9-])+\\.)+([a-zA-Z0-9]{2,4})+$");
	if(!boost::regex_match(email, regex)) {
		return false;
	}
	if(email.length() < 5 || email.length() > 64) {
		return false;
	}
	return true;
}



















// @FUNCTION - Determines if a specified string can be a valid password
// @SPECS - String length must be 2-32 characters
// @RETURNS - TRUE/FALSE
bool form::validPassword(std::string password) {
	if(password.length() < 2 || password.length() > 32) {
		return false;
	}
	return true;
}



















// @FUNCTION - Determines if specified string can be a valid zipcode
// @SPECS - String must be 5 numbers
// @RETURNS - TRUE/FALSE
bool form::validZipcode(std::string zipcode) {
	boost::regex regex("^[0-9]{5}$");
	if(!boost::regex_match(zipcode, regex)) {
		return false;
	}
	return true;
}
















// @FUNCTION - Send registration token to specified email
// @RETURNS - NOTHING
void mail::external::send_registration(std::string email_f, std::string token) {
	std::string subject = "DMV Exchange :: Registration";
	std::string token_f = cppcms::util::urlencode(token);
	std::string message = "Thank you for registering at DMV Exchange click <a href='http://dmv-exchange.com/activate?token=" + token_f + "'>here</a> to activate your account<br><br>If the link does not work copy and paste this link into the address bar<br>http://dmv-exchange.com/activate?token=" + token_f;
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange'\\<no-reply@dmv-exchange.com\\> -s \'" + subject + "\' " + email_f;
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to send email to " + email_f + " with token " + token;
		// report error to email
		error::send(err);
		std::cerr << err << std::endl;
		return;
	}
	pclose(pHandle);
	return;
}



















// @FUNCTION - Send username to specified email
// @RETURNS - NOTHING
void mail::external::send_username(std::string email, std::string username) {
	std::string username_f = to_lowercase(username);
	std::string email_f = to_lowercase(email);
	std::string subject = "Username reminder";
	std::string message = "You have requested a reminder of your username<br>Your username is => " + username_f;
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange'\\<no-reply@dmv-exchange.com\\> -s '" + subject + "' " + email_f;
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to send username to " + email_f + " username was " + username_f;
		// report error to email
		error::send(err);
		std::cerr << err << std::endl;
		return;
	}
	pclose(pHandle);
	return;
}



















// @FUNCTION - Send password reset token to specified email
// @RETURNS - NOTHING
void mail::external::send_pwd_reset(std::string email, std::string token) {
	std::string email_f = to_lowercase(email);
	std::string token_f = cppcms::util::urlencode(token);
	std::string subject = "Password Reset";
	std::string message = "You have requested to reset your password click <a href=\"http://dmv-exchange.com/reset?token=" + token_f + "\">here</a> to reset your password<br>If the link does not work copy and paste this link into the address bar<br>http://dmv-exchange.com/reset?token=" + token_f;
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange'\\<no-reply@dmv-exchange.com\\> -s '" + subject + "' " + email_f;
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to send password reset to " + email_f + " with token " + token;
		// report error to email
		error::send(err);
		std::cerr << err << std::endl;
		return;
	}
	pclose(pHandle);
	return;
}
















// @FUNCTION - Send notification that password has been changed
// @RETURNS - NOTHING
void mail::external::notice_password(std::string email) {
	std::string email_f = to_lowercase(email);
	std::string subject = "Password Changed!";
	std::string message = "Your password has been changed!";
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange'\\<no-reply@dmv-exchange.com\\> -s '" + subject + "' " + email_f;
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to send password change notice to " + email_f;
		// report error to email
		error::send(err);
		std::cerr << err << std::endl;
		return;
	}
	pclose(pHandle);
	return;
}



















// @FUNCTION - Send notification that email has been added to specified account
// @RETURNS - NOTHING
void mail::external::notice_new_email(std::string email, std::string username) {
	std::string email_f = to_lowercase(email);
	std::string username_f = to_lowercase(username);
	std::string subject = "Email Added!";
	std::string message = "This email has been added as secondary email to user " + username_f + "<br>Go to the <a href='http://dmv-exchange.com'>dmv-exchange</a>";
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange'\\<no-reply@dmv-exchange.com\\> -s '" + subject + "' " + email_f;
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to send new email notice to " + email_f + " for user " + username_f;
		// report error to email
		error::send(err);
		std::cerr << err << std::endl;
		return;
	}
	pclose(pHandle);
	return;
}










// @FUNCTION - sends notice to specified email it's been removed from account
// @RETURNS - NOTHING
void mail::external::notice_remove_email(std::string email, std::string username) {
	std::string email_f = to_lowercase(email);
	std::string username_f = to_lowercase(username);
	std::string subject = "Email Removed!";
	std::string message = "This secondary email has been removed from user " + username_f + "<br>Go to the <a href='http://dmv-exchange.com'>dmv-exchange</a>";
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange'\\<no-reply@dmv-exchange.com\\> -s '" + subject + "' " + email_f;
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to send remove email notice to " + email_f + " for user " + username_f;
		// report error to email
		error::send(err);
		std::cerr << err << std::endl;
		return;
	}
	pclose(pHandle);
	return;
}













// @FUNCTION - Send email containing error message to my email
// 				`termiosx@gmail.com`
// @RETURNS - NOTHING
void error::send(std::string err_msg) {
	std::string subject = "DMV Exchange :: Error";
	std::string message = "<span style=\"color:#fa6400;\">A server error has occurred..</span><br>" + err_msg;
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange - Bot'\\<bot@dmv-exchange.com\\> -s '" + subject + "' termiosx@gmail.com";
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to send error message";
		std::cerr << err << std::endl;
		return;
	}
	pclose(pHandle);
	return;
}




















// @FUNCTION - Update a field in user database entry specified by id
// @RETURNS - NOTHING
void db::update::user::by_id(std::shared_ptr<DatabaseClass> &db, int id, std::pair<std::string, std::string> data) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + db->conn.esc(data.first) + "=" + db->conn.quote(data.second) + " WHERE id=" + db->conn.quote(id);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Updating user by id :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}





















// @FUNCTION - Update field in user database entry specified by email
// @RETURNS - NOTHING
void db::update::user::by_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::pair<std::string, std::string> data) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + db->conn.esc(data.first) + "=" + db->conn.quote(data.second) + " WHERE email=" + db->conn.quote(email);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Updating user by email :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}






















// @FUNCTION - Update user database entry specified by username
// @RETURNS - NOTHING
void db::update::user::by_username(std::shared_ptr<DatabaseClass> &db, std::string username, std::pair<std::string, std::string> data) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + db->conn.esc(data.first) + "=" + db->conn.quote(data.second) + " WHERE username=" + db->conn.quote(username);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Updating user by username :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}











void db::update::user::by_activation_token(std::shared_ptr<DatabaseClass> &db, std::string token, std::pair<std::string, std::string> data) {
	// ensure connection open
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + db->conn.esc(data.first) + "=" + db->conn.quote(data.second) + " WHERE token=" + db->conn.quote(token);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Updating user by activation token :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}














// @FUNCTION - Update 2 fields in user database entry specified by id
// @RETURNS - NOTHING
void db::update::user::by_id(std::shared_ptr<DatabaseClass> &db, int id, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + db->conn.esc(data1.first) + "=" + db->conn.quote(data1.second) + ", " + db->conn.esc(data2.first) + "=" + db->conn.quote(data2.second) + " WHERE id=" + db->conn.quote(id);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Updating user by id :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}





















// @FUNCTION - Update 2 fields in user database entry specified by username
// @RETURNS - NOTHING
void db::update::user::by_username(std::shared_ptr<DatabaseClass> &db, std::string username, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + db->conn.esc(data1.first) + "=" + db->conn.quote(data1.second) + ", " + db->conn.esc(data2.first) + "=" + db->conn.quote(data2.second) + " WHERE username=" + db->conn.quote(username);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Updating user by username :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}





















// @FUNCTION - Update 2 fields in user database entry specified by email
// @RETURNS - NOTHING
void db::update::user::by_email(std::shared_ptr<DatabaseClass> &db, std::string email, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2) {
	// ensure connection is active
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + db->conn.esc(data1.first) + "=" + db->conn.quote(data1.second) + ", " + db->conn.esc(data2.first) + "=" + db->conn.quote(data2.second) + " WHERE email=" + db->conn.quote(email);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Updating user by email :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}

















void db::update::block_list::by_id(std::shared_ptr<DatabaseClass> &db, int owner_id, std::vector<std::string> list) {
	// ensure connection is open
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}

	// create worker
	pqxx::work worker(db->conn);
	
	// format list to string
	std::string list_f = "{";
	for(std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
		list_f += to_lowercase(*it) + ",";
	}

	// remove last comma & append closing bracket `}`
	if(list_f != "{") {
		list_f.pop_back();
	}

	// close array
	list_f += "}";

	// update block list in database
	// prepare query
	std::string query = "UPDATE dmv_blocked_list_t SET b_username=" + db->conn.quote(list_f) + " WHERE owner_id=" + db->conn.quote(owner_id);

	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Updating block list for user id => " << owner_id << " with list => " << list_f << " :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}
























// @FUNCTION - Gets blocked list of specified id
// @RETURNS - Vector of usernames of type std::string within std::vector<std::string>
std::vector<std::string> db::get::blocked_list::by_id(std::shared_ptr<DatabaseClass> &db, int owner_id) {
	// Ensure connection is open
	if(!db->conn.is_open()) {
		try {
			db->reconnect();
		} catch(std::exception &e) {
			std::ostringstream ss;
			ss << "Ensuring connection is open => " << db->conn.esc(e.what());
			error::send(ss.str());
			throw; // bubble exception up
		}
	}
	// create container
	std::vector<std::string> list;
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "SELECT array_to_string(b_username, ',') AS b_name FROM dmv_blocked_list_t WHERE owner_id=" + db->conn.quote(owner_id);
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		for(pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
			for(pqxx::tuple::const_iterator field = row->begin(); field != row->end(); ++field) {
				if(!field->is_null()) {
					// split string
					std::string string_array = field->as<std::string>();
					std::string temp; // container for substring
					while(string_array.find(",", 0) != std::string::npos) {
						size_t pos = string_array.find(",", 0); // grab delimiter pos
						temp = string_array.substr(0, pos); // grab token
						string_array.erase(0, pos + 1); // remove delimiter + token from original string
						list.push_back(temp); // place token into vector
					}

					if(string_array != "") {
						// add last part of string to vector as well
						list.push_back(string_array);
					}
				}
			}
		}
	} catch(std::exception &e) {
		std::ostringstream ss;
		ss << "Getting blocked list by id :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return list;
}





















// @FUNCTION - Gets blocked list of specified username
// @RETURNS - Vector of usernames of type std::string within std::vector<std::string>
std::vector<std::string> db::get::blocked_list::by_username(std::shared_ptr<DatabaseClass> &db, std::string owner_username) {
	std::vector<std::string> list;
	return list;
}














// @FUNCTION - Generates a token using email and time since unix epoch(in ms)
// @RETURNS - Generated token as type std::string
std::string mail::generate_token(std::string email) {
	std::string email_f = to_lowercase(email);
	std::time_t timestamp = std::time(0);
	std::ostringstream ss;
	ss << timestamp;
	std::string token = crypto::sha512_enc(email_f + ss.str());
	return token;
}















// @FUNCTION - Emails me user submitted tip
// @RETURNS - NOTHING
void mail::tip(std::string tip) {
	std::string subject = "DMV Exchange :: Tip";
	std::string message = "<span style=\"color:#fa6400;\">User submitted tip</span><br>" + tip;
	std::string command = "echo '" + message + "' | mail -aContent-Type:text/html -aFrom:'DMV Exchange - Tip'\\<tip@dmv-exchange.com\\> -s '" + subject + "' termiosx@gmail.com";
	FILE *pHandle = popen(command.c_str(), "w");
	if(!pHandle) {
		std::string err = "Failed to submit tip";
		error::send(err);
		return;
	}
	pclose(pHandle);
	return;
}























// @FUNCTION - Gets time since unix epoch in milliseconds
// @RETURNS - time since unix epoch as type std::string
std::string get_time() {
	std::time_t timestamp = std::time(0);
	std::ostringstream ss;
	ss << timestamp;
	return ss.str();
}













// @FUNCTION - Check file mime type
// @RETURNS - Mime Type in std::string format
std::string file::get_mime(std::string filename) {
	std::stringstream ss;
	char line[256];
	memset(line, 0, 80);
	std::string command = "file --mime-type " + filename;
	FILE *pHandle = popen(command.c_str(), "r");
	if(!pHandle) {
		std::string err = "Failed to submit tip";
		error::send(err);
		return "";
	}
	// Write return to console
	fgets(line, 256, pHandle);
	ss << line;
	std::string final = ss.str();
	// Remove filename, colon and space after colon
	final.erase(0, filename.length()+2);
	// Remove last character(\n)
	final.pop_back();
	pclose(pHandle);
	return final;
}


















// @FUNCTION - Checks if specified mime type is a valid image
// @RETURNS - TRUE/FALSE
bool file::valid_image(std::string mime_type) {
	if(mime_type == "image/jpeg") {
		return true;
	}
	if(mime_type == "image/png") {
		return true;
	}
	if(mime_type == "image/bmp") {
		return true;
	}
	if(mime_type == "image/gif") {
		return true;
	}
	if(mime_type == "image/jpg") {
		return true;
	}
	return false;
}












// @FUNCTION - Return specified string in lowercase format
std::string to_lowercase(std::string word) {
	std::locale loc;
	std::stringstream ss;
	for(std::string::size_type i = 0; i < word.length(); i++) {
		ss << std::tolower(word[i], loc);
	}
	return ss.str();
}