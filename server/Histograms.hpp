#pragma once

#include <string>
#include <iostream>
#include <algorithm>

#include "Serialization.h"

#undef max
#undef min

//
//void histPqGram(std::string output_file_base) {
//  std::ofstream file;
//  file.open(output_file_base + "-pq_gram.csv", ios::out);
//  for (auto& e : pq_gram) {
//    file << e << " "
//      << std::endl;
//  }
//  file.close();
//}
//
//ofstream file;
//file.open(cache_path+"-text_size_hists_sums.csv", ios::out);
//for (int i = 0; i < 256; ++i)
//  file << i << " ";
//file << std::endl;
//vector<unsigned long long int> ts_sums;
//ts_sums.resize(256);
//for (auto& e : text_size_hists) {
//  for (int i = 0; i < 256; ++i)
//    ts_sums[i] += e[i];
//}
//for (int i = 0; i < 256; ++i)
//  file << ts_sums[i] << " ";
//file << std::endl;
//file.close();
//
//
//ofstream file;
//file.open(cache_path + "-text_size_hists_sums_normalized.csv", ios::out);
//for (int i = 0; i < 256; ++i)
//  file << i << " ";
//file << std::endl;
//vector<double> ts_sums;
//ts_sums.resize(256);
//for (auto& e : text_size_hists) {
//  unsigned int sum = 0;
//  for (int i = 0; i < 256; ++i)
//    sum += e[i];
//  if (sum == 0)
//    continue;
//  for (int i = 0; i < 256; ++i)
//    ts_sums[i] += (e[i] / (double)sum);
//}
//for (int i = 0; i < 256; ++i)
//  file << ts_sums[i] << " ";
//file << std::endl;
//file.close();
//
//
//ofstream file;
//file.open(cache_path+"-text_color_hists.csv", ios::out);
//for (int i = 0; i < 256; ++i)
//  file << i << " ";
//file << std::endl;
//vector<unsigned long long int> tc_sums;
//tc_sums.resize(256);
//for (auto& e : text_color_hists) {
//  for (int i = 0; i < 256; ++i)
//    tc_sums[i] += e[i];
//}
//for (int i = 0; i < 256; ++i)
//  file << tc_sums[i] << " ";
//file << std::endl;
//file.close();

void saveHistMap(const map<int, int>& m, std::string file_path) {
  std::ofstream file;
  file.open(file_path, ios::out);
  for (auto it = m.begin(); it != m.end(); it++) {
    file << it->first << " " << it->second
      << std::endl;
  }
  file.close();
}

void histWordPerChildrenCount(const SampleSet& ss, std::string output_file_base) {
  map<int, int> m;
  for (int i = 0; i < 61; ++i)
    m[i] = 0;

  for (auto& e : ss) {
    m[e.words_per_child]++;
  }

  saveHistMap(m, output_file_base + "__wpc.csv");
}

void histChildrenRectVariance(const SampleSet& ss, std::string output_file_base) {
  vector<map<int, int>> m;
  m.resize(4);

  for (auto& e : ss) {
    auto r = e.children_rect_variance;
    for (int i = 0; i < 4; ++i)
      m[i][r[i]]++;
  }

  saveHistMap(m[0], output_file_base + "__crv_x.csv");
  saveHistMap(m[1], output_file_base + "__crv_y.csv");
  saveHistMap(m[2], output_file_base + "__crv_w.csv");
  saveHistMap(m[3], output_file_base + "__crv_h.csv");
}

void histChildrenRectVariance2(const SampleSet& ss, std::string output_file_base) {
  vector<map<int, int>> m;
  m.resize(3);

  for (auto& e : ss) {
    auto r = e.children_rect_variance;
    auto min_xy = std::min(r[0], r[1]);
    auto min_wh = std::min(r[2], r[3]);
    m[0][min_xy]++;
    m[1][min_wh]++;
    m[2][std::min(min_xy, min_wh)]++;
  }

  saveHistMap(m[0], output_file_base + "__crv_min_xy.csv");
  saveHistMap(m[1], output_file_base + "__crv_min_wh.csv");

  saveHistMap(m[2], output_file_base + "__crv_min_min_xywh.csv");
}


//#include "dlib-18.9/dlib/gui_widgets.h"
//#include "dlib-18.9/dlib/array2d.h"
//#include "dlib-18.9/dlib/image_transforms.h"
//// Make images
//const int size = 50;
//const int c = size - 1;
//dlib::array2d<int> img1(size, size);
//dlib::array2d<int> img2(size, size);
//
//for (auto& r : ss) {
//  // [y-axis, x-axis]
//  //img1[c*r.y][c*r.x]++;
//  //img2[c*(1-r.h)][c*r.w]++;
//}
//
//dlib::image_window win1(dlib::heatmap(img1), "x, y distribution");
//dlib::image_window win2(dlib::heatmap(img2), "w, h distribution");
////win1.wait_until_closed();



void SaveAllHistogramsData(const SampleSet& ss, std::string output_file_base) {
  histWordPerChildrenCount(ss, output_file_base);
  histChildrenRectVariance(ss, output_file_base);
  histChildrenRectVariance2(ss, output_file_base);
}
