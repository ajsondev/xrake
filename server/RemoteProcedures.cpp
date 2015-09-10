#include "RemoteProcedures.h"
#include "JsonCommon.h"

void InsertValue(Json::Value& root, string name, const Json::Value& value) {
  root[name] = value;
}

RemoteProcedureCall::RemoteProcedureCall(const std::string& function_name, int pageId) {
  InsertValue(json_in_, "fun", function_name);
  InsertValue(json_in_, "forwardTo", pageId);
}

void RemoteProcedureCall::AddArgument(string name, Json::Value value) {
  InsertValue(json_in_["xargs"], name, value);
}

bool RemoteProcedureCall::ParseResult(const std::string& result) {
  Json::Reader reader;
  bool success = reader.parse(result, json_out_);
  return success;
}

std::string RemoteProcedureCall::GetCallString() const {
  Json::FastWriter writer;
  return writer.write(json_in_);
}

void RPCMachine::Call(RemoteProcedureCall& rpc) {
  io_->Write(rpc.GetCallString());
  scoped_ptr<string> result = io_->Read();
  rpc.ParseResult(*result);
}

void RPCMachine::TryRewrite(int& pageId) {
  if (pageId == LAST_PAGE)
    pageId = currentPageId;
  else
    currentPageId = pageId;
}

// -------------------- Remote procedures calling code------------------------

int RPCMachine::OpenPage(string url) {
  RemoteProcedureCall rpc("OpenPage", GLOBAL_PROCEDURE);
  rpc.AddArgument("url", url);
  Call(rpc);
  currentPageId = rpc.GetResult()["pageId"].asInt();
  return currentPageId;
}

void RPCMachine::ClosePage(int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("ClosePage", GLOBAL_PROCEDURE);
  rpc.AddArgument("pageId", pageId);
  Call(rpc);
}

vector<string> RPCMachine::GetWarcLoadedPages() {
  RemoteProcedureCall rpc("GetWarcLoadedPages", GLOBAL_PROCEDURE);
  Call(rpc);
  return ValueToStringVector(rpc.GetResult()["urls"]);
}

Json::Value RPCMachine::GetTrainingJson(int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetTrainingJson", pageId);
  Call(rpc);
  return rpc.GetResult();
}

void RPCMachine::MarkNode(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("MarkNode", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return;
}

int RPCMachine::NodeFromMarker(Json::Value json, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("NodeFromMarker", pageId);
  Json::FastWriter w;
  rpc.AddArgument("str", w.write(json));
  Call(rpc);
  return rpc.GetResult()["uid"].asInt();
}


std::vector<double> RPCMachine::GetRect(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetRect", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return ValueToDoubleVector(rpc.GetResult());
}

std::vector<double> RPCMachine::GetChildrenRectVariance(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetChildrenRectVariance", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return ValueToDoubleVector(rpc.GetResult());
}

std::vector<double> RPCMachine::GetNormalizedRect(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetNormalizedRect", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return ValueToDoubleVector(rpc.GetResult());
}


int RPCMachine::GetRandomUid(int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetRandomUid", pageId);
  Call(rpc);
  return rpc.GetResult().asInt();
}


std::vector<int> RPCMachine::GetHistogramTextColorGrayscale(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetHistogramTextColorGrayscale", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return ValueToIntVector(rpc.GetResult());
}

std::vector<int> RPCMachine::GetHistogramTextSize(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetHistogramTextSize", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return ValueToIntVector(rpc.GetResult());
}

double RPCMachine::PqGramDistanceChildrenMean(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("PqGramDistanceChildrenMean", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return rpc.GetResult().asDouble();
}

int RPCMachine::GetWordCountPerChild(int nodeUid, int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall rpc("GetWordCountPerChild", pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return rpc.GetResult().asInt();
}

Json::Value& RPCMachine::GetFeaturesJumbo(int pageId) {
  TryRewrite(pageId);
  RemoteProcedureCall* rpc = new RemoteProcedureCall("GetFeaturesJumbo", pageId);
  Call(*rpc);
  return const_cast<Json::Value&>(rpc->GetResult());
  // Leak rpc !!! (for speed)
}