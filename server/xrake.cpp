// xrake.cpp : Defines the entry point for the console application.

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Format.h"
#include <iostream>

#include "MainForTesting.h"
#include "ClientActionController.h"
#include "InputOutput.h"
#include "GlobalConfig.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


class WebSocketRequestHandler : public HTTPRequestHandler
  /// Handle a WebSocket connection.
{
public:
  void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
  {
    Application& app = Application::instance();
    try
    {
      WebSocket ws(request, response);
      InputOutputWebSocket io(&ws);
      ClientActionController client_action_controller(&io);
      app.logger().information("WebSocket connection established.");
      char buffer[1024];
      int flags;
      int n;
      while(1) {
        n = ws.receiveFrame(buffer, sizeof(buffer), flags);
        if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_CLOSE || n <= 0)
          break;
        std::string action_name(buffer, n);
        app.logger().information("Action: " + action_name);
        client_action_controller.RunAction(action_name);
      }
      app.logger().information("WebSocket connection closed.");
    }
    catch (WebSocketException& exc)
    {
      app.logger().log(exc);
      switch (exc.code())
      {
      case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
        response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
        // fallthrough
      case WebSocket::WS_ERR_NO_HANDSHAKE:
      case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
      case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
        response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentLength(0);
        response.send();
        break;
      }
    }
  }
};


class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
  {
    return new WebSocketRequestHandler;
  }
};


class WebSocketServer : public Poco::Util::ServerApplication
  /// The main application class.
  ///
  /// This class handles command-line arguments and
  /// configuration files.
  /// Start the WebSocketServer executable with the help
  /// option (/help on Windows, --help on Unix) for
  /// the available command line options.
  ///
  /// To use the sample configuration file (WebSocketServer.properties),
  /// copy the file to the directory where the WebSocketServer executable
  /// resides. If you start the debug version of the WebSocketServer
  /// (WebSocketServerd[.exe]), you must also create a copy of the configuration
  /// file named WebSocketServerd.properties. In the configuration file, you
  /// can specify the port on which the server is listening (default
  /// 5555) and the format of the date/time string sent back to the client.
{
public:
  WebSocketServer() : _helpRequested(false)
  {
  }

  ~WebSocketServer()
  {
  }

protected:
  void initialize(Application& self)
  {
    loadConfiguration(); // Load default configuration files, if present.
    ServerApplication::initialize(self);
  }

  void uninitialize()
  {
    ServerApplication::uninitialize();
  }

  void defineOptions(OptionSet& options)
  {
    ServerApplication::defineOptions(options);

    options.addOption(
      Option("help", "h", "display help information on command line arguments")
      .required(false)
      .repeatable(false));
  }

  void handleOption(const std::string& name, const std::string& value)
  {
    ServerApplication::handleOption(name, value);

    if (name == "help")
      _helpRequested = true;
  }

  void displayHelp()
  {
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A sample HTTP server supporting the WebSocket protocol.");
    helpFormatter.format(std::cout);
  }

  int main(const std::vector<std::string>& args)
  {
    if (_helpRequested)
    {
      displayHelp();
    }
    else
    {
      // Get parameters from configuration file.
      unsigned short port = (unsigned short)config().getInt("WebSocketServer.port", 5555);
      GlobalConfig::Get(&config());

      main_for_testing();

      // Set-up a server socket.
      ServerSocket svs(port);
      // Set-up a HTTPServer instance.
      HTTPServerParams::Ptr params(new HTTPServerParams);
      params->setKeepAlive(true);
      //params->setMaxKeepAliveRequests(1000);
      params->setKeepAliveTimeout(Poco::Timespan(1000 * 60, 0)); // 1000 minutes.
      params->setTimeout(params->getKeepAliveTimeout());
      HTTPServer srv(new RequestHandlerFactory, svs, params);
      // Start the HTTPServer.
      srv.start();
      // Wait for CTRL-C or kill.
      waitForTerminationRequest();
      // Stop the HTTPServer.
      srv.stop();
    }
    return Application::EXIT_OK;
  }

private:
  bool _helpRequested;
};


POCO_SERVER_MAIN(WebSocketServer)