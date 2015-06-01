/**

	Nginx serves static files these are just fallbacks

**/





#ifndef __STATIC_H__
#define __STATIC_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class StaticMapper : public cppcms::application {
	public:
		StaticMapper(cppcms::service &srv) : cppcms::application(srv) {
			// Map /img/{file}
			mapper().assign("img", "/img/{1}");
			// img - png
			dispatcher().assign("/img/([a-zA-Z_0-9\\-\\.]+\\.png)", &StaticMapper::png, this, 1);
			// img - bmp
			dispatcher().assign("/img/([a-zA-Z_0-9\\-\\.]+\\.bmp)", &StaticMapper::bmp, this, 1);
			// img - jpeg
			dispatcher().assign("/img/([a-zA-Z_0-9\\-\\.]+\\.jpeg)", &StaticMapper::jpg, this, 1);
			// img - jpg
			dispatcher().assign("/img/([a-zA-Z_0-9\\-\\.]+\\.jpg)", &StaticMapper::jpeg, this, 1);
			// img - gif
			dispatcher().assign("/img/([a-zA-Z_0-9\\-\\.]+\\.gif)", &StaticMapper::gif, this, 1);

			// Map /font/{file}
			mapper().assign("font", "/font/{1}");
			// font - ttf
			dispatcher().assign("/font/([a-zA-Z_0-9\\-\\.]+\\.ttf)", &StaticMapper::ttf, this, 1);
			// font - otf
			dispatcher().assign("/font/([a-zA-Z_0-9\\-\\.]+\\.otf)", &StaticMapper::otf, this, 1);

			// Map /js/{file}
			mapper().assign("js", "/js/{1}");
			dispatcher().assign("/js/([a-zA-Z_0-9\\.]+\\.js)", &StaticMapper::js, this, 1);

			// Map /css/{file}
			mapper().assign("css", "/css/{1}");
			dispatcher().assign("/css/([a-zA-Z_0-9\\.]+\\.css)", &StaticMapper::css, this, 1);

			// Map /h/{file}
			mapper().assign("h", "/h/{1}");
			// html
			dispatcher().assign("/h/([a-zA-Z_0-9\\.]+\\.html)", &StaticMapper::html, this, 1);
			// zip
			dispatcher().assign("/h/([a-zA-Z_0-9\\.]+\\.zip)", &StaticMapper::html, this, 1);
		}

		// images
		void png(std::string filename);
		void bmp(std::string filename);
		void jpeg(std::string filename);
		void jpg(std::string filename);
		void gif(std::string filename);

		// fonts
		void otf(std::string filename);
		void ttf(std::string filename);

		// css
		void css(std::string filename);

		// js
		void js(std::string filename);

		// html
		void html(std::string filename);
};


#endif