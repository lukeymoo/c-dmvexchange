#include "generic.h" // contains core files

int main(int argc, char **argv) {
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