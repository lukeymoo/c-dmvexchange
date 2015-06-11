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
 	password = crypto::gen_password::by_username(username, i_password);
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