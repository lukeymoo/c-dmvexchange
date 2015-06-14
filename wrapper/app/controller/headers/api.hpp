#ifndef __API_H__
#define __API_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include "view.hpp"
#include "user.hpp"
#include "methods.hpp"

namespace json {
	void send(std::string status, std::string message, std::ostream &stream);
}

class api : public cppcms::application {
	public:
		api(cppcms::service &srv, pqxx::connection *conn) : cppcms::application(srv) {
			dbconn = conn;
			// Account page -- Add new email
			dispatcher().assign("/add_email(/)?", &api::new_email, this);

			// Returns session values
			dispatcher().assign("/session/state(/)?", &api::session_handler, this);
		
		}
		~api();

		pqxx::connection *dbconn;

		// add new email
		void new_email();

		void session_handler();
};

#endif