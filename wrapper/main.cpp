#include "controller.hpp" // contains core files

int main(int argc, char **argv) {

	std::cout << "[+] Setting up database" << std::endl;

	// Connect to database
	std::shared_ptr<DatabaseClass> db(new DatabaseClass);

	// -- TABLES --

	// User Table
	if(!db::check_exist::table(db, "dmv_users_t")) {
		std::cout << "[-] Table dmv_users_t does not exist" << std::endl;
		std::cout << "\t=> Creating table..." << std::endl;
		// Create user table
		try {
			db::create::table::users(db);
		} catch(std::exception &e) {
			std::cerr << "[-] Exception occurred => " << e.what() << std::endl;
			exit(1);
		}
		// successful creation
		std::cout << "\t=> Table created!" << std::endl;
	} else {
		std::cout << "[+] Table dmv_users_t exists" << std::endl;
	}

	// Blocked List Table
	if(!db::check_exist::table(db, "dmv_blocked_list_t")) {
		std::cout << "[-] Table dmv_blocked_list_t does not exist" << std::endl;
		std::cout << "\t=> Creating table..." << std::endl;
		try {
			db::create::table::block_list(db);
		} catch(std::exception &e) {
			std::cerr << "[-] Exception occurred => " << e.what() << std::endl;
			exit(1);
		}
	} else {
		std::cout << "[+] Table dmv_blocked_list_t exists" << std::endl;
	}
	// Create block lists for all users
	db::validate(db);
	
	// Product Table
	if(!db::check_exist::table(db, "dmv_products_t")) {
		std::cout << "[-] Table dmv_products_t does not exist" << std::endl;
		std::cout << "\t=> Creating table..." << std::endl;
		try {
			db::create::table::products(db);
		} catch(std::exception &e) {
			std::cerr << "[-] Exception occurred => " << e.what() << std::endl;
			exit(1);
		}
	} else {
		std::cout << "[+] Table dmv_products_t exists" << std::endl;
	}

	// Comments Table

	// Free connection
	db.reset(); // remove connection
	std::cout << "[+] Database initialization complete" << std::endl << std::endl;
	
	//	Initialize server
	try {
		cppcms::service srv(argc, argv);
		srv.applications_pool().mount(cppcms::applications_factory<BaseController>());
		srv.run();
	} catch(std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}