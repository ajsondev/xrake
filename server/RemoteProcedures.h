#pragma once

#include <string>
#include <functional>

#include "InputOutput.h"

#include "json/json.h"

using namespace std::placeholders;
using namespace std;

namespace {

// Internal helper class.
class RemoteProcedureCall {
public:
    RemoteProcedureCall(const string& function_name, int pageId);
    void AddArgument(string name, Json::Value value);

    string GetCallString() const;
    bool ParseResult(const string& result);
    const Json::Value& GetResult() const {
      return json_out_;
    }

private:
    Json::Value json_in_;
    Json::Value json_out_;
};

}  // namespace

class RPCMachine {
public:
  explicit RPCMachine(InputOutput* io) : io_(io) {
  }

  // Avaliable remote procedures:
  int OpenPage(string url);
  void ClosePage(int pageId = LAST_PAGE);
  vector<string> GetWarcLoadedPages();
  Json::Value GetTrainingJson(int pageId = LAST_PAGE);
  void MarkNode(int nodeUid, int pageId = LAST_PAGE);
  int NodeFromMarker(Json::Value json, int pageId = LAST_PAGE);

  std::vector<int> GetHistogramTextColorGrayscale(int nodeUid, int pageId = LAST_PAGE);
  std::vector<int> GetHistogramTextSize(int nodeUid, int pageId = LAST_PAGE);
  double PqGramDistanceChildrenMean(int nodeUid, int pageId = LAST_PAGE);
  int GetWordCountPerChild(int nodeUid, int pageId = LAST_PAGE);
  std::vector<double> GetChildrenRectVariance(int nodeUid, int pageId = LAST_PAGE);

  std::vector<double> GetRect(int nodeUid, int pageId = LAST_PAGE);
  std::vector<double> GetNormalizedRect(int nodeUid, int pageId = LAST_PAGE);
  int GetRandomUid(int pageId = LAST_PAGE);

  Json::Value& GetFeaturesJumbo(int pageId = LAST_PAGE);

  enum { GLOBAL_PROCEDURE = -1, LAST_PAGE = -2 } SpecialPageId;

  static const int NonExsistentNode = -1;

private:
  void Call(RemoteProcedureCall& rpc);
  void TryRewrite(int& pageId);

  InputOutput* io_;
  int currentPageId = -1;
};
