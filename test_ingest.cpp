#include "client.h"
#include "client_json.h"
#include "client_manifest.h"
#include "client_registration.h"
#include <json/json.h>
#include <json/json_object.h>
#include <string>

#include "test_ingest.h"

std::string make_ingest_request(std::string ip_address, std::string path, std::string json){

  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(ip_address, "443");
  boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
  boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);

  client c(io_service, context, iterator, ip_address, path, json);

  io_service.run();

  std::string jstr = c.get_response_json();

  //std::cout << jstr << std::endl;

  return jstr;
}

std::string get_ingest_json(std::string content_provider_id, std::string access_token)
{
  //std::cout << "get reg json" << std::endl; 

  std::ostringstream oss;
  oss << "{" << "\"content_provider_id\"" ":" << "\"" << content_provider_id << "\"" << "," << "\"access_token\"" << ":" << "\"" << access_token <<"\"""}";    
  //std::cout << "strnew: " << oss.str() << std::endl;
  return oss.str();
}

int hello_ingest(std::string ip_address, std::string content_provider_id, std::string access_token)
{
  std::string path = "/ingest/v1/HelloVoC";
  //xtype type = xtype::ingest;
  std::string post_data_str = get_ingest_json(content_provider_id, access_token);

  std::cout << post_data_str << std::endl;
  std::string jstr = make_ingest_request(ip_address, path, post_data_str);
  std::cout << jstr  << std::endl;

  return 0;
}

int ingest_get_categories(std::string ip_address, std::string content_provider_id, std::string access_token)
{
  std::string path = "/ingest/v1/getContentCategories";
  //xtype type = xtype::ingest;
  std::string post_data_str = get_ingest_json(content_provider_id, access_token);

  std::cout << post_data_str << std::endl;
  std::string jstr = make_ingest_request(ip_address, path, post_data_str);
  std::cout << jstr  << std::endl;

  return 0;
}
