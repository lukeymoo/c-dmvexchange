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
bool db::check_exist::username(pqxx::connection *c, std::string username) {
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
bool db::check_exist::email(pqxx::connection *c, std::string email) {
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
bool db::try_login::with_username(pqxx::connection *c, std::string username, std::string password) {
	pqxx::work worker(*c); // create worker
	// lowercase username
	std::string username_f = to_lowercase(username);
	// generate inital hash
	std::string init = crypto::generate(username_f, password);
	std::string init_username;
	std::string init_password;
	// hash iteratively 2500 times with username
	for(int i = 0; i < 2500; i++) {
		init_username = crypto::generate(init, username_f);
	}
	// hash iteratively 2500 times with password
	for(int i = 0; i < 3000; i++) {
		init_password = crypto::generate(init, password);
	}
	// put them together with generate
	std::string init_final = crypto::generate(init_username, init_password);
	std::string password_f;
	// hash iteratively 2500 times with init_final
	for(int i = 0; i < 2500; i++) {
		password_f = crypto::generate(init, init_final);
	}
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE username=" + c->quote(username_f) + " AND password=" + c->quote(password_f) + ")";
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
bool db::try_login::with_email(pqxx::connection *c, std::string email, std::string password) {
	// lowercase email
	std::string email_f = to_lowercase(email);
	// get user account
	std::map<std::string, std::string> user_account = db::get_user::by_email(c, email_f);
	pqxx::work worker(*c); // create worker
	if(user_account.empty()) {
		return false;
	}
	// generate inital hash
	std::string init = crypto::generate(user_account["username"], password);
	std::string init_username;
	std::string init_password;
	// hash iteratively 2500 times with username
	for(int i = 0; i < 2500; i++) {
		init_username = crypto::generate(init, user_account["username"]);
	}
	// hash iteratively 3000 times with password
	for(int i = 0; i < 3000; i++) {
		init_password = crypto::generate(init, password);
	}
	// put them together with generate
	std::string init_final = crypto::generate(init_username, init_password);
	std::string password_f;
	// hash iteratively 2500 times with init_final
	for(int i = 0; i < 2500; i++) {
		password_f = crypto::generate(init, init_final);
	}
	
	// prepare query
	std::string query = "SELECT EXISTS (SELECT * FROM dmv_users_t WHERE email=" + c->quote(email_f) + " AND password=" + c->quote(password_f) + ")";
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
bool db::create_table::user(pqxx::connection *c) {
	pqxx::work worker(*c); // create worker
	// prepare query
	std::string query = "CREATE TABLE dmv_users_t (id SERIAL PRIMARY KEY, firstname VARCHAR(32) NOT NULL, lastname VARCHAR(32) NOT NULL, username VARCHAR(16) UNIQUE NOT NULL, email VARCHAR(64) UNIQUE NOT NULL, password VARCHAR(256) NOT NULL, token VARCHAR(32) NOT NULL, zipcode INT NOT NULL, timestamp INT NOT NULL)";
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
		if(row["password"].is_null()) {
			info["password"] = "";
		} else {
			row["password"] >> info["password"];
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
		if(row["password"].is_null()) {
			info["password"] = "";
		} else {
			row["password"] >> info["password"];
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
		if(row["password"].is_null()) {
			info["password"] = "";
		} else {
			row["password"] >> info["password"];
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