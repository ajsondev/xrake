#include "Serialization.h"

#include "third_party/json/json.h"

#include "JsonCommon.h"
#include "RemoteProcedures.h"

#undef min

void LoadSampleSetFromJson(const Json::Value& jumbo, SampleSet& out) {
  auto v = ValueToValueVector(jumbo);
  out.reserve(v.size());
  for (const auto& jr : v) {
    Sample s;
    s.rect = ValueToDoubleVector(jr["r"]);
    s.rect_norm = ValueToDoubleVector(jr["rn"]);
    auto& r = s.children_rect_variance = ValueToDoubleVector(jr["crv"]);
    s.children_rect_min_variance = std::min(std::min(r[0], r[1]), std::min(r[2], r[3]));
    s.text_color_hists = ValueToIntVector(jr["tc"]);
    s.text_size_hists = ValueToIntVector(jr["ts"]);
    s.pq_gram = jr["pq"].asDouble();
    s.words_per_child = jr["wpc"].asInt();

    s.uid = jr["uid"].isInt() ? jr["uid"].asInt() : RPCMachine::NonExsistentNode;
    
    out.push_back(s);
  }
}

void LoadSampleSet(std::string json_path, SampleSet& out) {
  static Json::Value json;
  JsonFromFile(json, json_path);
  LoadSampleSetFromJson(json, out);
}

void SaveSampleSet(const SampleSet& in, std::string json_path) {
  static Json::Value json;
  for (int i = 0; i < in.size(); ++i) {
    Json::Value jr;
    jr["r"] = DoubleVectorToValue(in[i].rect);
    jr["rn"] = DoubleVectorToValue(in[i].rect_norm);
    jr["crv"] = DoubleVectorToValue(in[i].children_rect_variance);
    jr["tc"] = IntVectorToValue(in[i].text_color_hists);
    jr["ts"] = IntVectorToValue(in[i].text_size_hists);
    jr["pq"] = in[i].pq_gram;
    jr["wpc"] = in[i].words_per_child;

    json.append(jr);
  }
  JsonToFile(json, json_path);
}

