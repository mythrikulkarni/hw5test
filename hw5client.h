/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */

#ifndef JSONRPC_CPP_STUB_HW5CLIENT_H_
#define JSONRPC_CPP_STUB_HW5CLIENT_H_

#include <jsonrpccpp/client.h>

class hw5Client : public jsonrpc::Client
{
    public:
        hw5Client(jsonrpc::IClientConnector &conn, jsonrpc::clientVersion_t type = jsonrpc::JSONRPC_CLIENT_V2) : jsonrpc::Client(conn, type) {}

        Json::Value move(const std::string& action, const std::string& class_id, const Json::Value& json_object, const Json::Value& location, const std::string& object_id) 
        {
            Json::Value p;
            p["action"] = action;
            p["class_id"] = class_id;
            p["json_object"] = json_object;
            p["location"] = location;
            p["object_id"] = object_id;
            Json::Value result = this->CallMethod("move",p);
            if (result.isObject())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value search(const std::string& action, const std::string& class_id, const std::string& object_id) 
        {
            Json::Value p;
            p["action"] = action;
            p["class_id"] = class_id;
            p["object_id"] = object_id;
            Json::Value result = this->CallMethod("search",p);
            if (result.isObject())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value dump2JSON(const std::string& action, const std::string& arguments, const std::string& class_id, const std::string& host_url, const std::string& object_id) 
        {
            Json::Value p;
            p["action"] = action;
            p["arguments"] = arguments;
            p["class_id"] = class_id;
            p["host_url"] = host_url;
            p["object_id"] = object_id;
            Json::Value result = this->CallMethod("dump2JSON",p);
            if (result.isObject())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
};

#endif //JSONRPC_CPP_STUB_HW5CLIENT_H_
