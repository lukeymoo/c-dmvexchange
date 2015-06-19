#include "controller.hpp" // contains core files

int main(int argc, char **argv) {

	std::cout << "[+] Setting up database" << std::endl;

	// Connect to database
	std::shared_ptr<DatabaseClass> db(new DatabaseClass);

	// if the user table doesn't exist create it
	if(!db::check_exist::table(db, "dmv_users_t")) {
		std::cout << "[-] Table dmv_users_t does not exist" << std::endl;
		std::cout << "\t=> Creating table..." << std::endl;
		// Create user table
		try {
			db::create_table::user(db);
		} catch(std::exception &e) {
			std::cerr << "[-] Exception occurred => " << e.what() << std::endl;
			exit(1);
		}
		// successful creation
		std::cout << "\t=> Table created!" << std::endl;
	} else {
		std::cout << "[+] Table dmv_users_t exists" << std::endl;
	}

	// if product post table doesn't exist, create it
	// if comment post table doesn't exist, create it

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