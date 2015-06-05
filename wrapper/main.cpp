#include "generic.hpp" // contains core files

int main(int argc, char **argv) {
	// Connect to database
	pqxx::connection c("dbname=dmvexchange user=db password=f9a548add9f0007ca4071e06f04e3f81 hostaddr=127.0.0.1 port=5432");

	// Ensure connection is open
	if(c.is_open()) {
		std::cout << std::endl << "[+] Setting up database" << std::endl;

		// if the user table doesn't exist create it
		if(!db::check_exist::table(&c, "dmv_users_t")) {
			std::cout << "[-] Table dmv_users_t does not exist" << std::endl;
			std::cout << "\t=> Creating table..." << std::endl;
			// Create user table
			try {
				db::create_table::user(&c);
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

	} else {
		std::cerr << "[-] Failed to initialize database" << std::endl;
		exit(1);
	}
	// Free connection
	std::cout << "[+] Database initialization complete" << std::endl << std::endl;
	c.disconnect();
	/**
		Initialize server
	*/
	try {
		cppcms::service srv(argc, argv);
		srv.applications_pool().mount(cppcms::applications_factory<DXServer>());
		srv.run();
	} catch(std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}