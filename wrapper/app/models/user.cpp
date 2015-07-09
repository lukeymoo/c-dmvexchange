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

// Execute PostgresQL command to insert a user into database
void UserModel::save(std::shared_ptr<DatabaseClass> &db) {
	std::time_t timestamp_t = std::time(0);
	std::ostringstream ss;
	ss << timestamp_t;
	std::string timestamp = ss.str();
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "INSERT INTO dmv_users_t (firstname, lastname, username, email, password, token, zipcode, gender, forgot_token, forgot_timestamp, timestamp) VALUES (" + db->conn.quote(firstname) + ", " + db->conn.quote(lastname) + ", " + db->conn.quote(username) + ", " + db->conn.quote(email) + ", " + db->conn.quote(password) + ", " + db->conn.quote(token) + ", " + db->conn.quote(zipcode) + ", " + db->conn.quote(gender) + ", '', 0, " + db->conn.quote(timestamp) + ")";
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