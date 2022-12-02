
// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "hw5server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "hw5client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <stdio.h>

// ecs36b
#include "IOT_Thing.h"
#include "Person.h"
#include "Thing.h"
#include <time.h>
#include "Shadow_Thing.h"

using namespace jsonrpc;
using namespace std;

// std::map (key, value pairs)
std::map<std::string, Thing *> Thing_Map;
std::map<std::string, Person *> Person_Map;

// my location
GPS process_here;

class Myhw5Server : public hw5Server
{
public:
  Myhw5Server(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value move(const std::string& action,
			   const std::string& class_id,
			   const Json::Value& json_object,
			   const Json::Value& location,
			   const std::string& object_id);
  virtual Json::Value search(const std::string& action,
			     const std::string& class_id,
			     const std::string& object_id);
  virtual Json::Value dump2JSON(const std::string& action,
				const std::string& arguments,
				const std::string& class_id,
				const std::string& host_url,
				const std::string& object_id);
};

Myhw5Server::Myhw5Server(AbstractServerConnector &connector, serverVersion_t type)
  : hw5Server(connector, type)
{
  std::cout << "Myhw5GrandmaHome Server Object created" << std::endl;
}

int main()
{
  /*GPS_DD gps_Home_Woodland { 38.672215864622636, -121.72280111121437 };
  GPS_DD gps_TLC_UCDavis   { 38.53874868013882,  -121.7542091083306 };
  GPS_DD gps_IKEA_Sacramento { 38.58681641563053, -121.55296296578501};*/
  GPS LittleRedHouse = GPS ("Red's and her Mother's House", "Village", 0.0);
  GPS meetingSpot = GPS ("entrance of the woods", "Forest");
  GPS grandmotherHouse = GPS("Grandma's house", "end of Woods", "three large oak", 0.5);
  Person LittleRedRidingHood = Person("Little Red Riding Hood", LittleRedHouse);
  LittleRedRidingHood.setLocation(meetingSpot, *getNowJvTime());
  Thing Cap = Thing("Cap", "a head covering");
  Cap.setOwner(LittleRedRidingHood);
  Cap.setLocation(meetingSpot);
  Thing Cake = Thing("Cake", "a sweet dessert");
  Cake.setLocation(meetingSpot);
  Thing Wine = Thing("Wine", "a drink");
  Wine.setLocation(meetingSpot);

  // set location here
  //process_here = gps_Home_Woodland; // another, grandma
  process_here = grandmotherHouse;
  
  /*Person Felix { "987654321", "Felix", gps_Home_Woodland };
  Felix.setLocation(gps_IKEA_Sacramento, (*getNowJvTime()));

  IP_Address meatball_IP { 2850883174 };
  IOT_Thing Swedish_Meatball { Felix , meatball_IP };
  
  Swedish_Meatball.model = "HUVUDROLL";
  Swedish_Meatball.sequence_num = "123456-0000";
  Swedish_Meatball.description = "frozen";
  Swedish_Meatball.location = gps_Home_Woodland;
  Swedish_Meatball.purchased = *getNowJvTime();
  Swedish_Meatball.sold = *getNowJvTime();
  Swedish_Meatball.owner = Felix;

  int rc;
  char name_buf[256];
  
  bzero(name_buf, 256);
  snprintf(name_buf, 256, "./json_objects/%s_%s.json", "Thing", "000000001");
  // "./json_objects/Thing_00000001.json"
  Json::Value jv = Swedish_Meatball.dump2JSON();
  rc = myJSON2File(name_buf, &jv); // char *, Json::Value *
  if (rc != 0)
    {
      std::cout << "myJSON2File error " << rc << std::endl;
      exit(-1);
    }

  Thing_Map["00000001"] = (Thing *) (&Cap);
  for (const auto& [key, value] : Thing_Map)
    std::cout << '[' << key << "] = " << value << ";\n ";*/

#define _GPS_STUFF_
#ifdef _GPS_STUFF_
  
  /*bzero(name_buf, 256);
  snprintf(name_buf, 256, "./json_objects/%s_%s.json", "GPS", "000000001");
  Json::Value gps_jv = gps_IKEA_Sacramento.dump2JSON();
  rc = myJSON2File(name_buf, &gps_jv);
  if (rc != 0)
    {
      std::cout << "myJSON2File error " << rc << std::endl;
      exit(-1);
    }

  Json::Value gps_jv_2;
  rc = myFile2JSON(name_buf, &gps_jv_2);
  if (rc != 0)
    {
      std::cout << "myJSON2File error " << rc << std::endl;
      exit(-1);
    }
  GPS_DD new_one {};
  new_one.JSON2Object(gps_jv_2);
  std::cout << (new_one.dump2JSON()).toStyledString() << std::endl;*/

#endif /* _GPS_STUFF_ */
  
  HttpServer httpserver(7375);
  Myhw5Server s(httpserver,
		JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();
  return 0;
}

// member functions

Json::Value
Myhw5Server::move
(const std::string& action, const std::string& class_id,
 const Json::Value& json_object, const Json::Value& location,
 const std::string& object_id)
{
  int error_code = 0;
  Json::Value result;
  std::string strJson;

  if (class_id != "Person" && class_id != "Thing") 
    {
      result["status"] = "failed";
      strJson = ( "class " + class_id + " unknown");
      result["reason"] = strJson;
      error_code = -1;
    }

  if (object_id.empty())
    {
      result["status"] = "failed";
      strJson = ( "object_id null ");
      result["reason"] = strJson;
      error_code = -2;
    }

  GPS incoming_location;
  if ((location.isNull() == true) ||
      (location.isObject() == false))
    {
      result["status"] = "failed";
      strJson = ( "location parsing error " );
      result["reason"] = strJson;
      error_code = -3;
    }

  if ((json_object.isNull() == true) ||
      (json_object.isObject() == false))
    {
      result["status"] = "failed";
      strJson = ( "person parsing error " );
      result["reason"] = strJson;
      error_code = -4;
    }

  if (error_code == 0)
    {
      incoming_location.JSON2Object(location);
      if (!(process_here == incoming_location))
	{
	  result["status"] = "failed";
	  strJson = ( "location mismatched" );
	  result["reason"] = strJson;
	  error_code = -5;
	}
      else
	{
	  Person * lv_person_ptr;
	  if (Person_Map.find(object_id) != Person_Map.end())
	    {
	      lv_person_ptr = Person_Map[object_id];
	    }
	  else
	    {
	      lv_person_ptr = new Person();
	      Person_Map[object_id] = lv_person_ptr;
	    }
	  result["status"] = "successful";
	}
    }

  std::cout << result.toStyledString() << std::endl;
  return result;
}

Json::Value
Myhw5Server::search
(const std::string& action, const std::string& class_id,
 const std::string& object_id)
{
  int error_code = 0;
  int rc;
  Json::Value result;
  std::string strJson;

  if (class_id != "Person")
    {
      result["status"] = "failed";
      strJson = ( "class " + class_id + " unknown");
      result["reason"] = strJson;
      error_code = -1;
    }

  if (object_id.empty())
    {
      result["status"] = "failed";
      strJson = ( "object_id null ");
      result["reason"] = strJson;
      error_code = -2;
    }

  Person * lv_person_ptr;
  if (Person_Map.find(object_id) != Person_Map.end())
    {
      lv_person_ptr = Person_Map[object_id];
      Json::Value json_object = lv_person_ptr->dump2JSON();
      json_object["current location"] = process_here.dump2JSON();
      result["object"] = json_object;
      result["status"] = "successful";
    }
  else
    {
      result["status"] = "failed";
      strJson = ( "object_id not found ");
      result["reason"] = strJson;
    }

  return result;
}

Json::Value
Myhw5Server::dump2JSON
(const std::string& action, const std::string& arguments,
 const std::string& class_id, const std::string& host_url,
 const std::string& object_id)
{
  // a pointer to Thing
  Thing *thing_ptr; // now a local variable

  Json::Value result;
  std::cout << action << " " << arguments << std::endl;
  std::string strJson;

  if (class_id != "Thing")
    {
      result["status"] = "failed";
      strJson = ( "class " + class_id + " unknown");
      result["reason"] = strJson;
    }
  else
    {
      if (Thing_Map.find(object_id) != Thing_Map.end())
	{
	  thing_ptr = Thing_Map[object_id];
	  result = thing_ptr->dump2JSON();
	  result["status"] = "successful";
	}
      else
	{
	  result["status"] = "failed";
	  strJson = ( "object " + object_id + " unknown");
	  result["reason"] = strJson;
	}
    }

  return result;
}

