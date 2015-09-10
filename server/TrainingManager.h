#pragma once

#include <fstream>

#include "RemoteProcedures.h"
#include "WarcArchivesLoader.h"

class TrainingManager {
public:
  // RPCMachine must outlive TrainingManager
  explicit TrainingManager(RPCMachine* rpc);

  void StartSupervising();
  void PresentTrainingMarks();
  void TestTrainingMarksRecall();
  void StartLearning();

  // Visualizations
  void VisualizeTrainingFeatures();

private:
  std::string GetCurrentTrainingJsonPath() const;

  RPCMachine* rpc;
  WarcArchivesLoader warcLoader;
};


class TrainingSampleIterator {
public:
  TrainingSampleIterator(RPCMachine* rpc, std::string json_path) : rpc(rpc) {
    std::ifstream file(json_path);
    Json::Reader r;
    r.parse(file, json);
    Advance();
  }
  TrainingSampleIterator& operator++() {
    Advance();
    return *this;
  }

  bool end() const {
    return index >= json.size();
  }

  bool last_on_page() const {
    return sub_index == (json[index]["data"].size() - 1);
  }

  int nodeUid() const {
    return current_uid;
  }

  Json::Value getMarkJson() {
    return json[index]["data"][sub_index];
  }

private:
  void Advance()  {
    sub_index++;
    while (!end() && sub_index >= json[index]["data"].size()) {
      index++;
      sub_index = 0;
    }
    if (!end()) {
      if (!json[index]["data"].isArray()) {
        //DCHECK(json[index]["data"]["skip"].asBool() == true);
        return Advance();
      }
      if (sub_index == 0) {
        rpc->ClosePage(); // close current if any TODO: handle failure better in js
        rpc->OpenPage(json[index]["url"].asString());
      }

      current_uid = rpc->NodeFromMarker(getMarkJson());
    }
  }

  int current_uid = -1;
  int index = 0;
  int sub_index = -1;
  RPCMachine* rpc;
  Json::Value json;
};
