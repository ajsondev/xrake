#include "JsonCommon.h"

#include <fstream>

using namespace std;

//TODO: implement as templates?

vector<Json::Value> ValueToValueVector(const Json::Value& value) {
  vector<Json::Value> result;
  if (value.isNull())
    return result;
  result.reserve(value.size());
  for (auto it = value.begin(); it != value.end(); ++it)
    result.push_back((*it));
  return result;
}

vector<int> ValueToIntVector(const Json::Value& value) {
  vector<int> result;
  if (value.isNull())
    return result;
  result.reserve(value.size());
  for (auto it = value.begin(); it != value.end(); ++it)
    result.push_back((*it).asInt());
  return result;
}

vector<double> ValueToDoubleVector(const Json::Value& value) {
  vector<double> result;
  if (value.isNull())
    return result;
  result.reserve(value.size());
  for (auto it = value.begin(); it != value.end(); ++it)
    result.push_back((*it).asDouble());
  return result;
}

Json::Value IntVectorToValue(const std::vector<int>& v) {
  Json::Value result;
  for (auto& e : v)
    result.append(e);
  return result;
}

Json::Value DoubleVectorToValue(const std::vector<double>& v) {
  Json::Value result;
  for (auto& e : v)
    result.append(e);
  return result;
}

vector<string> ValueToStringVector(const Json::Value& value) {
  vector<string> result;
  if (value.isNull())
    return result;
  result.reserve(value.size());
  for (auto it = value.begin(); it != value.end(); ++it)
    result.push_back((*it).asString());
  return result;
}

void JsonToFile(Json::Value root, std::string path) {
  ofstream file;
  file.open(path, ios::out);
  file << root.toStyledString();
  file.close();
}

void JsonFromFile(Json::Value& root, std::string path) {
  ifstream file(path);
  file >> root;
}


