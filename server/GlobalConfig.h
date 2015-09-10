#pragma once

#include <string>

#include "Poco/Util/LayeredConfiguration.h"

struct GlobalConfig;

// Not (yet) thread-safe class.
struct GlobalConfig {
  static GlobalConfig& Get(Poco::Util::LayeredConfiguration* config = nullptr) {
    static GlobalConfig* gc = new GlobalConfig();
    if (config)
      Init(*config, gc);
    return *gc;
  }
  
  std::string archiverExePath;
  std::string warcDirectory;

  std::string cacheDirectory;
  std::string cacheListsFeatures;
  std::string cacheRandomFeatures;

private:
  static void Init(Poco::Util::LayeredConfiguration& config, GlobalConfig* gc) {
    gc->archiverExePath = config.getString("Crawling.archiverExePath");
    gc->warcDirectory = config.getString("Crawling.warcDirectory");

    gc->cacheDirectory = config.getString("Crawling.cacheDirectory");
    gc->cacheListsFeatures = gc->cacheDirectory + "\\" + config.getString("Crawling.cacheListsFeatures");
    gc->cacheRandomFeatures = gc->cacheDirectory + "\\" + config.getString("Crawling.cacheRandomFeatures");
  }
};