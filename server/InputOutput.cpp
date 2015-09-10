
#include "InputOutput.h"


scoped_ptr<string> InputOutputWebSocket::Read() {
  int flags;
  int n;
  char buffer[MAX_FRAME_SIZE];
  scoped_ptr<string> result(new string());

  n = ws_->receiveFrame(buffer, sizeof(buffer), flags);
  int bytes_to_receive = stoi(string(buffer, n));

  do {
    n = ws_->receiveFrame(buffer, sizeof(buffer), flags);

    if (n <= 0)
      return result.Pass();

    result->append(buffer, n);
    bytes_to_receive -= n;
  } while (bytes_to_receive > 0);
  
  return result.Pass();
}

void InputOutputWebSocket::Write(const string& data) {
  ws_->sendFrame(data.c_str(), static_cast<int>(data.size()), WebSocket::FRAME_TEXT);
}


