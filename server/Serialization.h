#pragma once

#include <vector>
#include <string>

#include "json/json.h"

struct Sample {
  std::vector<double> rect;
  std::vector<double> rect_norm;
  std::vector<double> children_rect_variance; // x y w h
  std::vector<int> text_color_hists;
  std::vector<int> text_size_hists;
  double pq_gram;
  int words_per_child;
  double children_rect_min_variance;

  // optional, for marking in live demonstrations
  int uid;
};

typedef std::vector<Sample> SampleSet;

void LoadSampleSetFromJson(const Json::Value& jumbo, SampleSet& out);
void LoadSampleSet(std::string json_path, SampleSet& out);
void SaveSampleSet(const SampleSet& in, std::string json_path);
