#include "WarcArchivesLoader.h"

#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"

#include <Windows.h>
#include <Processthreadsapi.h> // win8+

#include "third_party/chromium/base/strings/utf_string_conversions.h"

namespace {
  void CmdExecuteHelper(const char* cmd, const char* working_dir) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process. 
    ::CreateProcessA(NULL,   // No module name (use command line)
      const_cast<char*>(cmd),        // Command line
      NULL,           // Process handle not inheritable
      NULL,           // Thread handle not inheritable
      FALSE,          // Set handle inheritance to FALSE
      0,              // No creation flags
      NULL,           // Use parent's environment block
      const_cast<char*>(working_dir), // Use starting directory 
      &si,            // Pointer to STARTUPINFO structure
      &pi);           // Pointer to PROCESS_INFORMATION structure

    // Wait until child process exits.
    //WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }

}

WarcArchivesLoader::WarcArchivesLoader() {
  PositionToFirstFile();
}

bool WarcArchivesLoader::LoadNextArchive() {
  // Kill exe with previous archive (we are using the same localhost server port).
  const std::string kill_cmd = "taskkill /IM " + Poco::Path(GlobalConfig::Get().archiverExePath).getFileName();
  system(kill_cmd.c_str());
  
  Poco::DirectoryIterator end;
  do {
    if (warcFilesIterator_ == end) {
      // No more archives.
      return false;
    }
    current_archive_path_ = warcFilesIterator_.path().toString();
    ++warcFilesIterator_;
  } while (!IsArchive(current_archive_path_));

  const std::string cmd = GlobalConfig::Get().archiverExePath + " " + current_archive_path_;
  const std::string dir = Poco::Path(GlobalConfig::Get().archiverExePath).makeParent().toString();
  CmdExecuteHelper(cmd.c_str(), dir.c_str());
  return true;
}

bool WarcArchivesLoader::IsArchive(const std::string& path) const {
  const char* kArchiveSufix = ".warc.gz";
  return path.size() > sizeof(kArchiveSufix) &&
      path.substr(path.size() - sizeof(kArchiveSufix)) == kArchiveSufix;
}

std::string WarcArchivesLoader::GetCurrentArchivePath() const {
  return current_archive_path_;
}