#include "client.h"
#include "http_client.h"
#include <string>
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

void parse_url(const std::string& url, std::string& domain, std::string& path, std::string& proto)
{
  boost::regex ex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
  boost::cmatch what;
  if(regex_match(url.c_str(), what, ex))
    {
      proto    = std::string(what[1].first, what[1].second);
      domain   = std::string(what[2].first, what[2].second);
      //std::string port     = std::string(what[3].first, what[3].second);
      path     = std::string(what[4].first, what[4].second);
      //std::string query    = std::string(what[5].first, what[5].second);
      //std::cout << "[" << url << "]" << std::endl;
      //std::cout << proto << std::endl;
      //std::cout << "domain " << domain << std::endl;
      //std::cout << port << std::endl;
      //std::cout << "path " << path << std::endl;
      //std::cout << query << std::endl;
      //std::cout << "-------------------------------" << std::endl;
    }
}


int https_downloader(std::string domain, std::string path, int file_type, std::string suffix)
{
  std::string media_path = "test/cache/";
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(domain, "443");
  boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
  boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);

  xtype type = xtype::download;
  client c(io_service, context, iterator, domain, "test", "test", "test", path, type);

  io_service.run();
  if(file_type == 1)
    {
      boost::asio::streambuf hold;
      std::ofstream outfile(media_path + suffix, std::ios::out | std::ios::app | std::ios::binary);
      outfile << &(c.get_raw_response_strm());
    }
  if(file_type == 2)
    {
      std::ofstream outfile(media_path + suffix, std::ios::out | std::ios::app | std::ios::binary);
      outfile << &(c.get_raw_response_strm());
    }
  return 0;
}

int http_downloader(std::string domain, std::string path, int file_type, std::string suffix)
{
  std::string media_path = "test/cache/";
  boost::asio::io_service io_service;
  httpclient c(io_service, domain, path);
  io_service.run();

  if(file_type == 1)
    {
      boost::asio::streambuf hold;
      std::ofstream outfile(media_path + suffix, std::ios::out | std::ios::app | std::ios::binary);
      outfile << &(c.get_raw_response_strm());
    }
  if(file_type == 2)
    {
      std::ofstream outfile(media_path + suffix, std::ios::out | std::ios::app | std::ios::binary);
      outfile << &(c.get_raw_response_strm());
    }
  return 0;
}


int downloader(json_object *str, int file_type, std::string suffix)
{
  boost::filesystem::path dir("test/cache/"); //since standard c++ sucks about this.
  std::string path;
  std::string domain;
  std::string proto;
  parse_url(json_object_get_string(str), domain, path, proto);

  if(!(boost::filesystem::exists(dir))){
    std::cout<<"Doesn't Exists"<<std::endl;
    
    if (boost::filesystem::create_directory(dir))
      std::cout << "....Successfully Created !" << std::endl;
  }

  if(proto.compare("https") == 0)
    https_downloader(domain, path, file_type, suffix);
  if(proto.compare("http") == 0)
    http_downloader(domain, path, file_type, suffix);
  return 0;
}
