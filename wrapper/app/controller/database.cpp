#include "database.hpp"

Database::~Database() {
	std::cout << std::endl << "Disconnecting from server" << std::endl;
	conn.disconnect();
	return;
}

// Checks if table exists
bool Database::doesTableExist(const char *table_name) {
	std::string query = "";
	return false;
}