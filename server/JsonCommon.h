#pragma once

#include <vector>
#include <string>

#include "third_party/json/json.h"

std::vector<Json::Value> ValueToValueVector(const Json::Value& value);
std::vector<int> ValueToIntVector(const Json::Value& value);
std::vector<double> ValueToDoubleVector(const Json::Value& value);
std::vector<std::string> ValueToStringVector(const Json::Value& value);

Json::Value IntVectorToValue(const std::vector<int>& v);
Json::Value DoubleVectorToValue(const std::vector<double>& v);

void JsonToFile(Json::Value root, std::string path);
void JsonFromFile(Json::Value& root, std::string path);