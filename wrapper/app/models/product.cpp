#include "product.hpp"

ProductModel::ProductModel(int i_owner_id, std::string i_type, std::string i_zipcode, std::string i_description) {
	owner_id = i_owner_id;
	type = i_type;
	zipcode = i_zipcode;

	description = i_description;
	return;
}

ProductModel::~ProductModel() {
	return;
}

void ProductModel::save(std::shared_ptr<DatabaseClass> &db) {
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "INSERT INTO dmv_products_t (owner_id, zipcode, post_type, description) VALUES (" + db->conn.quote(owner_id) + ", " + db->conn.quote(zipcode) + ", " + db->conn.quote(type) + ", " + db->conn.quote(description) + ")";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Inserting product into database :: " << e.what();
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}