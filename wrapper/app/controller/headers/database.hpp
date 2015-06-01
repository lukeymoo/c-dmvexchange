#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <iostream>
#include <pqxx/pqxx>

class Database {
	public:
		Database(const char *db_options)
		try : conn(db_options) {
			std::cout << "Connecting to database..." << std::endl;
			// test connection
			if(conn.is_open()) {
				std::cout << "Connected to database!" << std::endl;
				std::cout << "Database name => " << conn.dbname() << std::endl;
			} else {
				std::cout << "Failed to connect to database!" << std::endl;
				exit(1);
			}
		} catch(std::exception &e) {
			std::cout << "[-] Server error => " << e.what() << std::endl;
			exit(1);
		}
		~Database();

		static bool doesTableExist(const char *table_name);

		pqxx::connection conn;
};



#endif