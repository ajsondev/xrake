class RemoteProcedureCall {
public:
    RemoteProcedureCall(const string& function_name,
        int pageId);
    void AddArgument(string name, Json::Value value);

    string GetCallString() const;
    bool ParseResult(const string& result);
    const Json::Value& GetResult() const;

private:
    Json::Value json_in_;
    Json::Value json_out_;
};

class RPCMachine;

double RPCMachine::PqGramDistanceChildrenMean(int nodeUid,
    int pageId) {
  RemoteProcedureCall rpc("PqGramDistanceChildrenMean",
      pageId);
  rpc.AddArgument("uid", nodeUid);
  Call(rpc);
  return rpc.GetResult().asDouble();
}

void RPCMachine::Call(RemoteProcedureCall& rpc) {
  io_->Write(rpc.GetCallString());
  scoped_ptr<string> result = io_->Read();
  rpc.ParseResult(*result);
}