#pragma once

#include "Poco/DirectoryIterator.h"
//#include "Poco/RecursiveDirectoryIterator.h"
//#include "Poco/FileStream.h"

#include "GlobalConfig.h"

class WarcArchivesLoader {
public:
  explicit WarcArchivesLoader();

  // Returns false when there is no next archive.
  bool LoadNextArchive();
  std::string GetCurrentArchivePath() const;
  void ResetState() {
    PositionToFirstFile();
    current_archive_path_ = "";
  }

private:
  bool IsArchive(const std::string& path) const;
  void PositionToFirstFile() {
    warcFilesIterator_ = Poco::DirectoryIterator(GlobalConfig::Get().warcDirectory);
  }

  Poco::DirectoryIterator warcFilesIterator_;
  std::string current_archive_path_;
};