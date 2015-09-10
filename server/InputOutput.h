#pragma once

#include <string>

#include "third_party/chromium/base/memory/scoped_ptr.h"

#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"

namespace {
  const int MAX_FRAME_SIZE = 102400;
}

using namespace Poco;
using namespace Poco::Net;
using namespace std;

class InputOutput {
public:
  InputOutput() {};
  virtual ~InputOutput() {};

  virtual scoped_ptr<string> Read() = 0;
  virtual void Write(const string& data) = 0;
};

class InputOutputWebSocket : public InputOutput {
public:
  InputOutputWebSocket(WebSocket* ws) : ws_(ws) {};
  virtual ~InputOutputWebSocket() {};

  virtual scoped_ptr<string> Read() override;
  virtual void Write(const string& data) override;
private:
  WebSocket* ws_;
};