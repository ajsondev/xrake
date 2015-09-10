#pragma once

#include <string>
#include <iostream>

#include "RemoteProcedures.h"
#include "TrainingManager.h"

class InputOutput;

class ClientActionController {
public:
  explicit ClientActionController(InputOutput* io) : rpc(io) {
  }
  ~ClientActionController() {
  }
  void RunAction(std::string& action);

private:
  RPCMachine rpc;
  TrainingManager trainingManager = TrainingManager(&rpc);
};
