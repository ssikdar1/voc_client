#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <string.h>
#include "json/json.h"
#include "json/json_object.h"
#include <sqlite3.h>
#include <string>
#include <sstream>

std::string make_ingest_request(std::string ip_address, std::string path, std::string json);
int hello_ingest(std::string ip_address, std::string content_provider_id, std::string access_token);
