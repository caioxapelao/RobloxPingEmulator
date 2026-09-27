#pragma once
#include <curl/curl.h>
#include <string>

namespace curl_wrapper {
	namespace shared {
		bool init = false;
	}
	namespace callbacks {
		size_t writeCb(char* ptr, size_t size, size_t nmemb, void* userdata) {
			((std::string*)userdata)->append(ptr, size * nmemb);
			return size * nmemb;
		}
	}
	namespace http {
		std::string get(std::string url) {
			std::string body;
			if (!shared::init) curl_global_init(CURL_GLOBAL_DEFAULT);
			CURL* curl = curl_easy_init();
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbacks::writeCb);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
			curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			return body;
		}
	}
}