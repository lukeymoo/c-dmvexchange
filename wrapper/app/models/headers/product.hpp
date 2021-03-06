#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include <pqxx/pqxx>
#include <iostream>
#include <vector>
#include <memory>
#include "methods.hpp"

class DatabaseClass;

class ProductModel {
	public:
		ProductModel(int i_owner_id, std::string i_owner_username, std::string i_product_type, std::string i_post_type, std::string i_zipcode, std::string i_description);
		~ProductModel();

		// Meta
		int owner_id;
		std::string owner_username;
		std::string post_type;
		std::string product_type;
		std::string zipcode;

		// Photo locations
		std::vector<std::string> photos;

		std::string description;

		void save(std::shared_ptr<DatabaseClass> &db);
};



#endif
