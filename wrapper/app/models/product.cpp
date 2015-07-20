#include "product.hpp"

ProductModel::ProductModel(int i_owner_id, std::string i_owner_username, std::string i_product_type, std::string i_post_type, std::string i_zipcode, std::string i_description) {
	owner_id = i_owner_id;
	owner_username = i_owner_username;
	post_type = i_post_type;
	product_type = i_product_type;
	zipcode = i_zipcode;

	description = i_description;
	return;
}

ProductModel::~ProductModel() {
	return;
}

void ProductModel::save(std::shared_ptr<DatabaseClass> &db) {
	std::string paths = "{";
	// create string from photo path vector
	for(auto path : photos) {
		paths += path + ",";
	}
	if(paths != "{") {
		paths.pop_back();
	}
	paths += "}";
	// create worker
	pqxx::work worker(db->conn);
	// prepare query
	std::string query = "INSERT INTO dmv_products_t (owner_id, owner_username, zipcode, product_type, post_type, description, photos) VALUES (" + db->conn.quote(owner_id) + ", " + db->conn.quote(owner_username) + ", " + db->conn.quote(zipcode) + ", " + db->conn.quote(product_type) + ", " + db->conn.quote(post_type) + ", " + db->conn.quote(description) + ", " + db->conn.quote(paths) + ")";
	// execute
	try {
		pqxx::result result = worker.exec(query.c_str());
		worker.commit();
	} catch(std::exception &e) {
		// report error to email
		std::ostringstream ss;
		ss << "Inserting product into database :: " << db->conn.esc(e.what());
		error::send(ss.str());
		throw; // bubble exception up
	}
	return;
}