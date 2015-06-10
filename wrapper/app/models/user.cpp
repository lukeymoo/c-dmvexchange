#include "user.hpp"

UserModel::UserModel(std::string i_firstname, std::string i_lastname, std::string i_username, std::string i_email, std::string i_password, std::string i_token, int i_zipcode, std::string i_gender) {
 	firstname = to_lowercase(i_firstname);
 	lastname = to_lowercase(i_lastname);
 	username = to_lowercase(i_username);
 	email = to_lowercase(i_email);
 	password = "";
 	token = i_token;
 	zipcode = i_zipcode;
 	gender = to_lowercase(i_gender);


 	// generate password with crazy hashing technique
	std::string init = crypto::generate(username, i_password);
	std::string init_username;
	std::string init_password;
	// hash iteratively 2500 times with username
	for(int i = 0; i < 2500; i++) {
		init_username = crypto::generate(init, username);
	}
	// hash iteratively 2500 times with password
	for(int i = 0; i < 3000; i++) {
		init_password = crypto::generate(init, i_password);
	}
	// put them together with generate
	std::string init_final = crypto::generate(init_username, init_password);
	// hash iteratively 2500 times with init_final
	for(int i = 0; i < 2500; i++) {
		password = crypto::generate(init, init_final);
	}
 	return;
}

UserModel::~UserModel() {
  return;
}

/*
	@FUNCTION - Return specified string in lowercase format
*/
std::string to_lowercase(std::string word) {
	std::locale loc;
	std::stringstream ss;
	for(std::string::size_type i = 0; i < word.length(); i++) {
		ss << std::tolower(word[i], loc);
	}
	return ss.str();
}

// Execute PostgresQL command to insert a user into database
void UserModel::save(pqxx::connection *c) {
	std::time_t timestamp_t = std::time(0);
	std::ostringstream ss;
	ss << timestamp_t;
	std::string timestamp = ss.str();
	// create worker
	pqxx::work worker(*c);
	// prepare query
	std::string query = "INSERT INTO dmv_users_t (firstname, lastname, username, email, password, token, zipcode, gender, forgot_token, forgot_timestamp, timestamp) VALUES (" + c->quote(firstname) + ", " + c->quote(lastname) + ", " + c->quote(username) + ", " + c->quote(email) + ", " + c->quote(password) + ", " + c->quote(token) + ", " + c->quote(zipcode) + ", " + c->quote(gender) + ", '', 0, " + c->quote(timestamp) + ")";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Inserting user into database :: " << e.what();
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}