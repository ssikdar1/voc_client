#include "client.h"
#include "client_db.h"
#include "client_downloader.h"
#include <iostream>
#include <string>
#include <sstream>
#include <boost/uuid/sha1.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

//
//
//json-c interface file kept in 'c' style for portability
//
//

std::string quotesql( const std::string& s ) 
{
  return std::string("'") + s + std::string("'");
}

std::string quotesqlint(std::string s)
{
  return s;
}

bool quotsqlbool(bool b)
{
  return b;
}

std::string get_sha1(const std::string& str)
{
  boost::uuids::detail::sha1 sha1;
  sha1.process_bytes(str.data(), str.size());
  unsigned hash[5] = {0};
  sha1.get_digest(hash);

  // Back to string                                                                                                                                    
  char buf[41] = {0};

  for (int i = 0; i < 5; i++)
    {
      std::sprintf(buf + (i << 3), "%08x", hash[i]);
    }

  return std::string(buf);
}

std::string get_voc_id(json_object *j)
{
  std::string rc;
  std::cout << "vocid" << std::endl;
  json_object *tmp = json_object_object_get(j, "vocId");
  
  return (rc = json_object_get_string(tmp));
}

std::string get_platform(json_object *j)
{
  std::string rc;
  return (rc = "Linux");
}

std::string get_device_id(json_object *j)
{
  std::string rc;
  return (rc = "000000DEADBEEF");
}

std::string get_device_type(json_object *j)
{
  std::string rc;
  return (rc = "000000DEADBEEF");
}

std::string get_access_token(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "accessToken");
  return (rc = json_object_get_string(tmp));
}


std::string get_refresh_token(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "refreshToken");
  return (rc = json_object_get_string(tmp));
}

std::string get_congestion_detection(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "congestionDetection");
  if(!tmp)
    return "";
  return (rc = json_object_get_string(tmp));
}

std::string get_ads_frequency(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "adsFrequency");
  return (rc = json_object_get_string(tmp));
}

std::string get_daily_download_quota(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "dailyDownloadQuota");
  if(!tmp)
    return "";
  return (rc = json_object_get_string(tmp));
}

std::string get_daily_download_wifi(json_object *j)
{
  std::stringstream s;
  json_object *tmp = json_object_object_get(j, "dailyDownloadWifi");
  if(!tmp)
    return 0;
  s << json_object_get_int(tmp);
  return s.str();
}

std::string get_daily_download_cellular(json_object *j)
{
  std::stringstream s;
  json_object *tmp = json_object_object_get(j, "dailyDownloadCellular");
  if(!tmp){
    std::cout << "FAIL on get !!!!!!!!!!!!\n" << std::endl;
    std::string sc = "";
    return sc;
  }
  s << json_object_get_int64(tmp);
  return s.str();
}

std::string get_sdk_capabilities(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "sdkCapabilities");
  return (rc = json_object_get_string(tmp));
}

std::string get_daily_download_manifest(json_object *j)
{
  std::stringstream s;
  json_object *tmp = json_object_object_get(j, "dailyManifestCount");
  if(!tmp)
    return 0;
  s << json_object_get_int(tmp);
  return s.str();
}

std::string get_max_content_duration(json_object *j)
{
  std::stringstream s;
  json_object *tmp = json_object_object_get(j, "maxContentDuration");
  if(!tmp)
    return 0;
  s << json_object_get_int(tmp);
  return s.str();
}

std::string get_play_ads(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "playAds");
  return (rc = json_object_get_string(tmp));
}

std::string get_skip_policy(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "skipPolicyFirstTime");
  return (rc = json_object_get_string(tmp));
}

std::string get_tod_policy(json_object *j)
{
  std::string rc;
  json_object *tmp = json_object_object_get(j, "todPolicy");
  return (rc = json_object_get_string(tmp));
}


std::string get_token_expiration(json_object *j)
{
  std::stringstream s;
  json_object *tmp = json_object_object_get(j, "tokenExpiryDate");
  if(!tmp)
    return 0;
  s << json_object_get_int64(tmp);
  return s.str();
}

std::string get_server_state(json_object *j)
{
  std::string rc = "test";
  json_object *tmp = json_object_object_get(j, "serverState");
  if(!tmp)
    return rc;
  return (rc = json_object_get_string(tmp));
}

std::string get_local_file(json_object *j) //j is an array object
{
  std::string err = "error";
  std::string media_dir = "test/cache/"; //FIXME should pass this to downloader
  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      json_object *lfile;
      bool status;
      if(!(status = json_object_object_get_ex(lobj, "streams", &objtor))) //TBD check status
	std::cout << "Unable to retrieve stream from db" << std::endl;

      if(!(status = json_object_object_get_ex(lobj, "uniqueId", &lfile))) //TBD check status
	std::cout << "Unable to retrieve uniqueId from db" << std::endl;

      for(int i = 0; i < json_object_array_length(objtor); i++)
	{
	  if(i == 1)//just grab the first one for now FIXME
	    break;
	  json_object *pobj = json_object_array_get_idx(objtor, i);
	  std::string local_file = get_sha1(json_object_get_string(lfile));
	  json_object *str = json_object_object_get(pobj,"url");

	  downloader(str, 1, local_file + ".1"); //need this but not now
	  return (local_file + ".1");
	}
    }
  return err;
}

std::string get_local_file_size(json_object *j)
{
  std::string err = "error";
  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      json_object *lfile;
      bool status = json_object_object_get_ex(lobj, "streams", &objtor); //TBD check status         

      status = json_object_object_get_ex(lobj, "uniqueId", &lfile); //TBD check status
      
      for(int i = 0; i < json_object_array_length(objtor); i++)
        {
	  json_object *pobj = json_object_array_get_idx(objtor, i);
	  std::string local_file = get_sha1(json_object_get_string(lfile));
          json_object *str; 
	  if((status = json_object_object_get_ex(pobj,"size", &str)))
	    {
	      return json_object_get_string(str);
	    }                                                                                             
	}
    }
  return err;
}

std::string get_local_thumb_file(json_object *j) //j is an array object
{
  std::string err = "error";
  std::string media_dir = "test/cache/"; //FIXME should be passed to downloader.
  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      json_object *lfile;
      bool status;
      if((status = json_object_object_get_ex(lobj, "thumbFile", &objtor)))
	{
	  status = json_object_object_get_ex(lobj, "uniqueId", &lfile);
	  std::string local_thumb = json_object_get_string(lfile);
	  downloader(objtor, 2, local_thumb + ".2");
	  return (local_thumb + ".2"); 
	}  
    }
  return err;
}

std::string get_local_nfo(json_object *j)
{
  std::string err = "not done";
  return err;
}

std::string get_thumb_size(json_object *j)
{
  std::stringstream s;
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "thumbSize", &objtor)))
	{
	  s << json_object_get_int(objtor);
	  return s.str();
	}  
    }
  return err;
}

boost::posix_time::time_duration::tick_type milliseconds_since_epoch()
{
  using boost::gregorian::date;
  using boost::posix_time::ptime;
  using boost::posix_time::microsec_clock;

  static ptime const epoch(date(1970, 1, 1));
  return (microsec_clock::universal_time() - epoch).total_milliseconds();
}


std::string get_download_time()
{
  return std::to_string(milliseconds_since_epoch()/1000);
}

std::string get_content_provider(json_object *j) //j is an array object
{

  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "provider", &objtor)))
        {
	  return json_object_get_string(objtor);                                                                                          
        }
    }
  return err;
}

std::string get_category(json_object *j) //j is an array object 
{

  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "catId", &objtor)))
        {
	  json_object *larr = json_object_array_get_idx(objtor, 0);
	  return json_object_get_string(larr);
	}
    }
  return err;
}

std::string get_unique_id(json_object *j) //j is an array object  
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "uniqueId", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_summary(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "summary", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_title(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "title", &objtor)))
	{
          return json_object_get_string(objtor);
	}
    }
  return err;
}

std::string get_time_stamp(json_object *j) //j is an array object 
{

  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "timeStamp", &objtor)))
	{
          return json_object_get_string(objtor);
	}
    }
  return err;
}

static std::string parse_json_comma_list(std::string list, int item)
{
  std::string delimiter = "=";
  size_t pos = 0;
  std::string token;
  int item_count = 0;
  std::string err = "error";

  while ((pos = list.find(delimiter)) != std::string::npos) {
    token = list.substr(0, pos);
    if(item_count == item){
      std::string sub_delimiter = ",";
      size_t sub_pos = 0;
      while((sub_pos = token.find(sub_delimiter)) != std::string::npos){
	token.erase(sub_pos, sub_delimiter.length() + (token.length() - sub_pos));
	return token;
      }
    }
    item_count++;
    list.erase(0, pos + delimiter.length());
  }
  return err;
}

std::string get_duration(json_object *j)
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "objectAttrs", &objtor)))
        {
	  std::string attr = json_object_get_string(objtor);
	  attr.erase(std::remove(attr.begin(), attr.end(), ','), attr.end());
	  //std::cout << "attr " << attr << std::endl;
          return parse_json_comma_list(json_object_get_string(objtor), 1); //duration 1st item
        }
    }
  return err;
}

std::string get_sdk_metadata(json_object *j) //j is an array object 
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "sdkMetadataPassthrough", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_streams(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "streams", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_adserver_url(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "adServerUrl", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_tags(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "tags", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_priority(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "priority", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_object_type(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "objectType", &objtor)))
	{
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_thumb_attribs(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "thumbAttrs", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_object_attribs(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "objectAttrs", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_children(json_object *j) //j is an array object 
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "objectAttrs", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_policy_name(json_object *j) //j is an array object
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "policyName", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}

std::string get_key_server_url(json_object *j) //j is an array object 
{
  std::string err = "error";

  for(int i = 0; i < json_object_array_length(j); i++)
    {
      json_object *lobj = json_object_array_get_idx(j, i);
      json_object *objtor;
      bool status;
      if((status = json_object_object_get_ex(lobj, "keyServerUrl", &objtor)))
        {
          return json_object_get_string(objtor);
        }
    }
  return err;
}
