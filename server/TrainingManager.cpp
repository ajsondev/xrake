#include "TrainingManager.h"

#include <vector>
#include <string>
#include <iostream>

#include "json/json.h"

#include "JsonCommon.h"
#include "Serialization.h"
#include "Histograms.hpp"

bool file_exists(const std::string& name) {
  FILE *file = fopen(name.c_str(), "r");
  bool exists = (file != NULL);
  if (exists)
    fclose(file);
  return exists;
}

Sample FetchSample(RPCMachine* rpc, int uid) {
  // Atributes that turned out to be poor predictors are commented.
  Sample s;
  //s.rect                   = rpc->GetRect(uid);
  //s.rect_norm              = rpc->GetNormalizedRect(uid);
  auto& r = s.children_rect_variance = rpc->GetChildrenRectVariance(uid);
  s.children_rect_min_variance = std::min(std::min(r[0], r[1]), std::min(r[2], r[3]));
  //s.text_color_hists       = rpc->GetHistogramTextColorGrayscale(uid);
  //s.text_size_hists        = rpc->GetHistogramTextSize(uid);
  s.pq_gram                = rpc->PqGramDistanceChildrenMean(uid);
  s.words_per_child        = rpc->GetWordCountPerChild(uid);

  return s;
}

TrainingManager::TrainingManager(RPCMachine* rpc) : rpc(rpc) {
}

void TrainingManager::StartLearning() {
  warcLoader.LoadNextArchive();

  vector<string> urls = rpc->GetWarcLoadedPages();
  for (std::string& url : urls) {
    rpc->OpenPage(url);
  }
   
}

void TrainingManager::StartSupervising() {
  while (warcLoader.LoadNextArchive()) {
    if (!file_exists(GetCurrentTrainingJsonPath())) {
      Sleep(1000); // Let supervisor to take a look.
      continue;
    }

    Json::Value root;
    vector<string> urls = rpc->GetWarcLoadedPages();
    for (std::string& url : urls) {
      rpc->OpenPage(url);
      Json::Value page_json;
      page_json["url"] = url;
      page_json["data"] = rpc->GetTrainingJson();
      rpc->ClosePage();

      root.append(page_json);
    }

    // Save data;
    ofstream file;
    file.open(GetCurrentTrainingJsonPath(), ios::out);
    file << root.toStyledString();
    file.close();
  }
  warcLoader.ResetState();
}

void TrainingManager::PresentTrainingMarks() {
  while (warcLoader.LoadNextArchive()) {
    if (!file_exists(GetCurrentTrainingJsonPath())) {
      Sleep(1000); // Let user to take a look.
      continue;
    }

    rpc->GetWarcLoadedPages(); // Added only to wait until reply server is fully loaded.

    TrainingSampleIterator it(rpc, GetCurrentTrainingJsonPath());
    while (!it.end()) {
      auto uid = it.nodeUid();

      rpc->MarkNode(uid);

      if (uid == RPCMachine::NonExsistentNode)
        cout << "PresentTrainingMarks: not matched "<< it.getMarkJson().toStyledString() << endl;

      Sleep(1000);
      if(it.last_on_page())
        Sleep(1000);

      ++it;
    }
  }
  rpc->ClosePage();
  warcLoader.ResetState();
}

void TrainingManager::TestTrainingMarksRecall() {
  int marksCount = 0;
  int marksNotMatched = 0;
  while (warcLoader.LoadNextArchive()) {
    if (!file_exists(GetCurrentTrainingJsonPath())) {
      Sleep(1000);
      continue;
    }

    cout << endl << endl << "Loading from: " << GetCurrentTrainingJsonPath() << endl << endl;
    rpc->GetWarcLoadedPages(); // Added only to wait until reply server is fully loaded.

    TrainingSampleIterator it(rpc, GetCurrentTrainingJsonPath());
    while (!it.end()) {
      auto uid = it.nodeUid();
      marksCount++;
      if (uid == RPCMachine::NonExsistentNode)
        marksNotMatched++;
      cout << "In progress... not matched " << marksNotMatched << " out of " << marksCount << " marks (" << ((double)marksNotMatched / marksCount * 100) << "%)." << endl;
      ++it;
    }
  }
  rpc->ClosePage();
  warcLoader.ResetState();

  cout << "Finally, not matched " << marksNotMatched << " out of " << marksCount << " marks (" << ((double)marksNotMatched / marksCount * 100) << "%)." << endl;
}

std::string TrainingManager::GetCurrentTrainingJsonPath() const {
  return warcLoader.GetCurrentArchivePath() + ".json";
}


void TrainingManager::VisualizeTrainingFeatures() {
  Json::Value json;
  SampleSet ss_positive;
  SampleSet ss_random;

  std::string cache_path_base = GlobalConfig::Get().cacheDirectory + "\\CachedVisualizeTrainingFeatures";
  std::string cache_positive = cache_path_base + "_lists_230.json";
  std::string cache_random = cache_path_base + "_random_5200.json";

  if (file_exists(cache_positive) && file_exists(cache_random)) {
    LoadSampleSet(cache_positive, ss_positive);
    LoadSampleSet(cache_random, ss_random);

    //// Trim
    //while (ss.size() > 250) {
    //  ss.erase(ss.begin() + (rand() % ss.size()));
    //}
    //SaveSampleSet(cache_path, ss);

  } else {
    while (warcLoader.LoadNextArchive()) {
      if (!file_exists(GetCurrentTrainingJsonPath())) {
        Sleep(1000);
        continue;
      }
      
      rpc->GetWarcLoadedPages(); // Added only to wait until reply server is fully loaded.

      TrainingSampleIterator it(rpc, GetCurrentTrainingJsonPath());
      while (!it.end()) {
        auto uid = it.nodeUid();
        // Save marked list.
        if (uid != RPCMachine::NonExsistentNode)
          ss_positive.push_back(FetchSample(rpc, uid));

        // Gather random.
        for (int i = 0; i < 20; ++i) {
          uid = rpc->GetRandomUid();
          if (uid != RPCMachine::NonExsistentNode)
            ss_random.push_back( FetchSample(rpc, uid) );
        }

        ++it;
      }
    }
    rpc->ClosePage(); // Close last, remaining page.
    warcLoader.ResetState();

    // Save cache.
    SaveSampleSet(ss_positive, cache_positive);
    SaveSampleSet(ss_random, cache_random);
  }

  // Save CSVs.
  SaveAllHistogramsData(ss_positive, cache_positive);
  SaveAllHistogramsData(ss_random, cache_random);

  cout << "SAMPLES LIST: " << ss_positive.size() << endl;
  cout << "SAMPLES RANDOM: " << ss_random.size() << endl;
}