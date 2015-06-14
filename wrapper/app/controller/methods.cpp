#include "methods.hpp"

/*
	@FUNCTION - Checks if specified table exists in database
	@RETURNS - TRUE/FALSE
*/
bool db::check_exist::table(pqxx::connection *c, std::string table_name) {
	pqxx::work worker(*c); // create worker
	// lowercase table name
	std::string table_name_f = to_lowercase(table_name);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name=" + c->quote(table_name_f) + " AND table_schema='public')";
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			pqxx::tuple row = result[0];
			pqxx::field field = row.at(0);
			if(field.as<std::string>() == "t") {
				return true;
			} else {
				return false;
			}
			success = true;
		} catch(std::exception &e) {
			// if exception is caught on last try
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Checking if table exists :: " << e.what();
				error::send(ss.str());
				throw;
			}
		}
	}
	return false;
}

/*
	@FUNCTION - Determines if specified username exists in database
	@RETURNS - TRUE/FALSE
*/
bool db::check_exist::username(pqxx::connection *c, std::string username) {
	pqxx::work worker(*c); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM dmv_users_t WHERE username=" + c->quote(username_f) + " )";
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result count = worker.exec(query.c_str());
			pqxx::tuple row = count[0];
			pqxx::field field = row[0];
			if(field.as<std::string>() == "t") {
				return true;
			} else {
				return false;
			}
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Checking if username exists :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return false;
}

/*
	@FUNCTION - Determines if email exists in database
	@RETURNS - TRUE/FALSE
*/
bool db::check_exist::email(pqxx::connection *c, std::string email) {
	pqxx::work worker(*c); // create worker
	// lowercase email
	std::string email_f = to_lowercase(email);
	// format query
	std::string query = "SELECT EXISTS (SELECT 1 FROM dmv_users_t WHERE email=" + c->quote(email_f) + " OR secondary_email=" + c->quote(email_f) + ")";
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result count = worker.exec(query.c_str());
			pqxx::tuple row = count[0];
			pqxx::field field = row[0];
			if(field.as<std::string>() == "t") {
				return true;
			} else {
				return false;
			}
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Checking if email exists :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return false;
}

/*
	@FUNCTION - Determines if a specified forgot_token exists in database
	@RETURNS - TRUE/FALSE
*/
bool db::check_exist::forgot_token(pqxx::connection *c, std::string token) {
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "SELECT EXISTS ( SELECT 1 FROM dmv_users_t WHERE forgot_token=" + c->quote(token) + ")";
	// execute
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			pqxx::tuple row = result[0];
			pqxx::field field = row[0];
			if(field.as<std::string>() == "t") {
				return true;
			} else {
				return false;
			}
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Checking if forgot_token exists :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return false;
}

/*
	@FUNCTION - Determines if specified username & password is a valid combination in database
	@RETURNS - TRUE/FALSE
*/
bool db::try_login::with_username(pqxx::connection *c, std::string username, std::string password) {
	pqxx::work worker(*c); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	std::string password_f = crypto::gen_password::by_username(username, password);
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE username=" + c->quote(username_f) + " AND password=" + c->quote(password_f) + ")";
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			pqxx::tuple row = result[0];
			pqxx::field field = row[0];
			if(field.as<std::string>() == "t") {
				return true;
			} else {
				return false;
			}
			success = true;
		} catch(std::exception &e) {
			std::cout << "Error(" << e.what() << ")" << std::endl;
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Logging in with username :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return false;
}

/*
	@FUNCTION - Determines if specified email & password is a valid combination in database
	@RETURNS - TRUE/FALSE
*/
bool db::try_login::with_email(pqxx::connection *c, std::string email, std::string password) {
	// lowercase email
	std::string email_f = to_lowercase(email);
	// get user account
	std::map<std::string, std::string> user_account = db::get_user::by_email(c, email_f);
	pqxx::work worker(*c); // create worker
	if(user_account.empty()) {
		return false;
	}
	std::string password_f = crypto::gen_password::by_email(c, email, password);
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE email=" + c->quote(email_f) + " AND password=" + c->quote(password_f) + ")";
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			pqxx::tuple row = result[0];
			pqxx::field field = row[0];
			if(field.as<std::string>() == "t") {
				return true;
			} else {
				return false;
			}
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Logging in with email :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
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
bool db::create_table::user(pqxx::connection *c) {
	pqxx::work worker(*c); // create worker
	// prepare query
	std::string query = "CREATE TABLE dmv_users_t (id SERIAL PRIMARY KEY, firstname VARCHAR(32) NOT NULL, lastname VARCHAR(32) NOT NULL, username VARCHAR(16) UNIQUE NOT NULL, email VARCHAR(64) UNIQUE NOT NULL, secondary_email VARCHAR(64) UNIQUE DEFAULT '', password VARCHAR(256) NOT NULL, token VARCHAR(256) NOT NULL, zipcode INT NOT NULL, gender CHAR(1) NOT NULL, forgot_token VARCHAR(256) NOT NULL DEFAULT '', forgot_timestamp INT NOT NULL, timestamp INT NOT NULL)";
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			worker.commit();
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Creating user table :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
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
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
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
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Finding user by ID :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return info;
}

/*
	@FUNCTION - Retreives user account details specified by username which is unique
		if no user is found an empty std::map is returned 
	@RETURNS - std::map<std::string, std::string> - key,value pairs of user acc details
*/
std::map<std::string, std::string> db::get_user::by_username(pqxx::connection *c, std::string username) {
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(*c); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE username=" + c->quote(username_f);
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
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
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Finding user by username :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return info;
}

/*
	@FUNCTION - Retreives user account details specified by email address which is unique
		if no user is found an empty std::map is returned
	@RETURNS - std::map<std::string, std::string> - key,value pairs of user acc details
*/
std::map<std::string, std::string> db::get_user::by_email(pqxx::connection *c, std::string email) {
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(*c); // create worker
	// lowercase email
	std::string email_f = to_lowercase(email);
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE email=" + c->quote(email_f);
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
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
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Finding user by email :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return info;
}

std::map<std::string, std::string> db::get_user::by_forgot_token(pqxx::connection *c, std::string token) {
	std::map<std::string, std::string> info; // will contain the user info
	pqxx::work worker(*c); // create worker
	// prepare query
	std::string query = "SELECT * FROM dmv_users_t WHERE forgot_token=" + c->quote(token);
	// execute query
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
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
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Finding user by email :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return info;
}


/*
	@FUNCTION - sha512 a specifed string and base64 encode it
	@RETURNS - std::string - base64 encoded sha512 hash
*/
std::string crypto::sha512_enc(std::string input_s) {
	unsigned char obuf[64];
	memset(obuf, 0, 64);
	SHA512(reinterpret_cast<const unsigned char*>(input_s.c_str()), input_s.length(), obuf); // encrypt
	std::string encoded = base64_encode(obuf, 64); // encode
	return encoded;
}

/*
	@FUNCTION - Sha512 a specified string
	@RETURNS - std::string with no encoding, this results in bad characters
		and is why this function is generally unused
*/
std::string crypto::sha512_noenc(std::string input_s) {
	unsigned char obuf[64];
	memset(obuf, 0, 64);
	SHA512(reinterpret_cast<const unsigned char*>(input_s.c_str()), input_s.length(), obuf); // encrypt
	std::string hash_s = reinterpret_cast<const char *>(obuf);
	return hash_s;
}

/*
	@FUNCTION - Generates a Sha512 string base64 encoded
		takes 2 parameters and hashes them multiple times(7 hash functions)
	@RETURNS - std::string - base64 encoded string 
*/
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

// calls crypto::gen_password::by_username after recovering username from database
// with specified identifier ( id in this case )
std::string crypto::gen_password::by_id(pqxx::connection *dbconn, int id, std::string password) {
	std::map<std::string, std::string> info = db::get_user::by_id(dbconn, id);
	std::string password_f = "";
	if(!info.empty()) {
		password_f = crypto::gen_password::by_username(info["username"], password);
	}
	return password_f;
}

// calls crypto::gen_password::by_username after recovering username from database
// with specified identifier ( email in this case )
std::string crypto::gen_password::by_email(pqxx::connection *dbconn, std::string email, std::string password) {
	std::map<std::string, std::string> info = db::get_user::by_email(dbconn, email);
	std::string password_f = "";
	if(!info.empty()) {
		password_f = crypto::gen_password::by_username(info["username"], password);
	}
	return password_f;
}

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

/*
	@FUNCTION - Generates URI params from a register form to be appended to Url
		EX. &f=xxx&u=xxx
	@RETURNS - std::string - uri parameters
*/
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

/*
	@FUNCTION - Determines if specified string can be a valid name
	@SPECS - Regex, 2-32 characters
	@RETURNS - TRUE/FALSE
*/
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

/*
	@FUNCTION - Determines if a specified string can be a valid username
	@SPECS - Regex, 2-16 characters length
	@RETURNS - TRUE/FALSE
*/
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

/*
	@FUNCTION - Determines if a specified string can be a valid email
	@SPECS 	- Regex, 5-64 characters
	@RETURNS - TRUE/FALSE
*/
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

/*
	@FUNCTION - Determines if a specified string can be a valid password
	@SPECS - String length must be 2-32 characters
	@RETURNS - TRUE/FALSE
*/
bool form::validPassword(std::string password) {
	if(password.length() < 2 || password.length() > 32) {
		return false;
	}
	return true;
}

/*
	@FUNCTION - Determines if specified string can be a valid zipcode
	@SPECS - String must be 5 numbers
	@RETURNS - TRUE/FALSE
*/
bool form::validZipcode(std::string zipcode) {
	boost::regex regex("^[0-9]{5}$");
	if(!boost::regex_match(zipcode, regex)) {
		return false;
	}
	return true;
}

// Send registration token to specified email address
// token is generated before calling this function because it
// uses timestamps and is inserted into database before being sent
// to the user, this ensures consistency between token sent & token
// in the database.
// Email address is already formatted to lowercase
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

// notify the user that their password has been changed
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

// find user by id & update it's contents based on specified std::pair
// std::pair data contains key,value pair specifying column name & new value
// returns true/false on success/fail
void db::update::user::by_id(pqxx::connection *c, int id, std::pair<std::string, std::string> data) {
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + c->esc(data.first) + "=" + c->quote(data.second) + " WHERE id=" + c->quote(id);
	// execute
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			worker.commit();
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Updating user by id :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return;
}

// find user by email & update it's contents based on specified std::pair
// std::pair data contains key,value pair specifying column name & new value
// returns true/false on success/fail
void db::update::user::by_email(pqxx::connection *c, std::string email, std::pair<std::string, std::string> data) {
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + c->esc(data.first) + "=" + c->quote(data.second) + " WHERE email=" + c->quote(email);
	// execute
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			worker.commit();
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Updating user by email :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return;
}

// find user by username & update it's contents based on specified std::pair
// std::pair data contains key,value pair specifying column name & new value
// returns true/false on success/fail
void db::update::user::by_username(pqxx::connection *c, std::string username, std::pair<std::string, std::string> data) {
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + c->esc(data.first) + "=" + c->quote(data.second) + " WHERE username=" + c->quote(username);
	// execute
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			worker.commit();
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Updating user by username :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return;
}


void db::update::user::by_id(pqxx::connection *c, int id, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2) {
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + c->esc(data1.first) + "=" + c->quote(data1.second) + ", " + c->esc(data2.first) + "=" + c->quote(data2.second) + " WHERE id=" + c->quote(id);
	// execute
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			worker.commit();
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Updating user by id :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return;
}


void db::update::user::by_username(pqxx::connection *c, std::string username, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2) {
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + c->esc(data1.first) + "=" + c->quote(data1.second) + ", " + c->esc(data2.first) + "=" + c->quote(data2.second) + " WHERE username=" + c->quote(username);
	// execute
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			worker.commit();
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Updating user by username :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return;
}


void db::update::user::by_email(pqxx::connection *c, std::string email, std::pair<std::string, std::string> data1, std::pair<std::string, std::string> data2) {
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "UPDATE dmv_users_t SET " + c->esc(data1.first) + "=" + c->quote(data1.second) + ", " + c->esc(data2.first) + "=" + c->quote(data2.second) + " WHERE email=" + c->quote(email);
	// execute
	bool success = false;
	for(int try_count = 3; try_count > 0 && !success; --try_count) {
		try {
			pqxx::result result = worker.exec(query.c_str());
			worker.commit();
			success = true;
		} catch(std::exception &e) {
			if(try_count == 1) {
				// report error to email
				std::ostringstream ss;
				ss << "Updating user by email :: " << e.what();
				error::send(ss.str());
				throw; // bubble exception up
			}
		}
	}
	return;
}

// Generates a token with a formatted email & timestamp
// should be used for activating newly registered account
std::string mail::generate_token(std::string email) {
	std::string email_f = to_lowercase(email);
	std::time_t timestamp = std::time(0);
	std::ostringstream ss;
	ss << timestamp;
	std::string token = crypto::sha512_enc(email_f + ss.str());
	return token;
}

std::string get_time() {
	std::time_t timestamp = std::time(0);
	std::ostringstream ss;
	ss << timestamp;
	return ss.str();
}

// Get register and login form cannot figure
// out how to integrate as the request context is private
/*
std::map<std::string, std::string> form::get_register_form() {
	std::map<std::string, std::string> form;
	return form;
}

std::map<std::string, std::string> form::get_login_form() {
	std::map<std::string, std::string> form;
	return form;
}
*/