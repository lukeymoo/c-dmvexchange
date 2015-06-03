#include "generic.hpp" // contains core files

int main(int argc, char **argv) {
	// Initialize database
	pqxx::connection c("dbname=dmvexchange user=db password=f9a548add9f0007ca4071e06f04e3f81 hostaddr=127.0.0.1 port=5432");
	if(c.is_open()) {
		std::cout << std::endl << "[INFO] Setting up database" << std::endl;
		// ensure tables exist
		if(!Database::table_exist(&c, "dmv_users_t")) {
			std::cout << "[NOTICE] Table dmv_users_t does not exist" << std::endl;
			// create table
		} else {
			std::cout << "[NOTICE] Table dmv_users_t exists" << std::endl;
		}
	} else {
		std::cerr << "[ERR] Failed to setup database" << std::endl;
		exit(1);
	}
	// Free connection
	std::cout << "[DONE] Destroyed connection handle for database initialization" << std::endl << std::endl;
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