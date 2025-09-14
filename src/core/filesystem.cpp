#include "filesystem.hpp"


namespace fs = std::filesystem;


/*************************************************************
*************************** Private **************************
*************************************************************/


fs::path Filesystem::_getRelativeOrAbsolutePath(const fs::path& path) {
  // If the path is absolute, then no changes need to happen. Assuming it exists.
  if (isPathAbsolute(path)) {
    return path;
  }
  // If the path is relative, then get the relative path and return that.
  else {
    return getAbsolutePathFromRelativePath(path);
  }
}


// ------------------------------------- //
// -------- Static Declarations -------- //
// ------------------------------------- //
std::stack<fs::path> Filesystem::_history = std::stack<fs::path>{};
fs::path Filesystem::_current_dir = fs::path{};
std::pair<Filesystem::ClipboardActionType, std::vector<fs::path>> Filesystem::_clipboard = {};


/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/


/*************************************************************
*************************** Public ***************************
*************************************************************/


void Filesystem::init() {
  // Config MUST be initialized before calling this function
  if (!Config::isInitialized()) {
    eprintln("Error: Please call 'Config::init()' before 'Filesystem::init()'");
    return;
  }

  // Setup clipboard and history
  _history = std::stack<fs::path>{}; // TODO: Setup history system.

  // Set the starting directory to the saved default.
  setCurrentDirectory(Config::getString(Config::Keys::DEFAULT_DIRECTORY));
}


void Filesystem::addToClipboard(ClipboardActionType type, const fs::path& path) {
  _clipboard = std::make_pair(type, std::vector{path});
}


void Filesystem::addToClipboard(ClipboardActionType type, const std::vector<fs::path>& paths) {
  _clipboard = std::make_pair(type, paths);
}


void Filesystem::applyClipboardAction(fs::path dest_path) {
  // Get path from relative/absolute
  dest_path = _getRelativeOrAbsolutePath(dest_path);

  ClipboardActionType& type = _clipboard.first;
  std::vector<fs::path>& paths = _clipboard.second;

  switch (type) {
    // Duplicate files into another location
    case ClipboardActionType::COPY: {
      // TODO: Implement

      break;
    }
    // Rename files into another location
    case ClipboardActionType::CUT: {
      // TODO: Implement

      break;
    }
    // Default statement is unreachable as ClipboardActionType is an ENUM CLASS not just an ENUM
    default: {
      assert(false && "Unreachable code!");
    }
  }
}


fs::path Filesystem::getExecutableDirectoryPath() {
  // NOTE: Hasn't been tested on other operating systems as of yet.
  // Just theoretically works, chatgpt'd the other OS's so idk for
  // sure.

  // WINDOWS
  #if defined(_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();

  // LINUX
  #elif defined(__linux__)
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
      buffer[len] = '\0';
      return std::filesystem::path(buffer).parent_path();
    }

    // Error
    return {};

  // APPLE
  #elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
      char resolved[PATH_MAX];
      realpath(buffer, resolved);
      return std::filesystem::path(resolved).parent_path();
    }

    // Error
    return {};

  // Fallback
  #else
    // This shouldn't be possible typically, but you never know
    return {};
  #endif
}


const fs::path Filesystem::getCurrentDirectoryPath() {
  return _current_dir;
}


bool Filesystem::setCurrentDirectory(fs::path dir_path) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path).lexically_normal();

  // If the path is NOT a directory, DO NOT reassign the _current_dir var.
  if (!exists(dir_path) || !isDirectory(dir_path)) return false;

  _current_dir = dir_path;
  return true;
}


std::vector<fs::path> Filesystem::getFilesInCurrentDirectory() {
  return getFilesInDirectory(_current_dir.string());
}


std::vector<fs::path> Filesystem::getFilesInDirectory(const std::string& dir_path) {
  // Special Case: OS is Windows and attempting to get the root's files
  #ifdef _WIN32
    // When on Windows, if the root is attempting to be accessed, instead return each drive's path
    if (dir_path == _SYS_ROOT || dir_path == _SYS_ROOT_ALT) {
      // DEBUG Print
      if (DEBUG) println("'dir_path' is ROOT!");

      std::vector<fs::path> roots;

      // Get the number of logical drives
      DWORD count = GetLogicalDriveStringsW(0, nullptr);
      if (count == 0) return roots; // Failed call

      // Get the actual drive data
      std::vector<wchar_t> buffer(count);
      DWORD ret = GetLogicalDriveStringsW(count, buffer.data());
      if (ret == 0) return roots; // Failed call

      // Place drives into the return value 'roots'
      for (wchar_t* p = buffer.data(); *p; p += std::wcslen(p) + 1) {
        roots.emplace_back(p); // Each root | Example: L"C:\\"
      }

      // Return gathered drives
      return roots;
    }
  #endif // _WIN32

  // Get path from relative/absolute
  const fs::path DIR_PATH = _getRelativeOrAbsolutePath(dir_path);

  // Return empty vector if the path is NOT a directory
  if (!isDirectory(DIR_PATH)) return {};

  // Return all paths in the directory
  return std::vector<fs::path>(fs::directory_iterator(DIR_PATH), {});
}


std::vector<fs::path> Filesystem::getFilesInCurrentDirectoryRecursive() {
  return getFilesInDirectoryRecursive(_current_dir.string());
}


std::vector<fs::path> Filesystem::getFilesInDirectoryRecursive(const std::string& dir_path) {
  // NEVER run the recursive version when on the system root OR when the current
  // directory's size is over "<SOME_SIZE>"
  if (dir_path == _SYS_ROOT || dir_path == _SYS_ROOT_ALT) {
    // DEBUG Print
    if (DEBUG) println("'dir_path' is ROOT!");

    // TODO: Add some checker for file size. If the directory is over like 1GB
    // or something in size it would take forever to check.
    return getFilesInDirectory(dir_path);
  }  

  // Get path from relative/absolute
  const fs::path DIR_PATH = _getRelativeOrAbsolutePath(dir_path);

  // Return empty vector if the path is NOT a directory
  if (!isDirectory(DIR_PATH)) return {};

  std::vector<fs::path> files;
  for (const auto& file : fs::directory_iterator(DIR_PATH)) {
    // If the file is a directory, then gather all files in that directory and add them to the list.
    if (isDirectory(file)) {
      std::vector<fs::path> children = getFilesInDirectoryRecursive(file.path().string());
      for (const fs::path& child : children) {
        files.push_back(child);
      }
    }
    // Since the file is not a directory, just simply add the file.
    else {
      files.push_back(file);
    }
  }
  
  return files;
}


bool Filesystem::exists(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  return fs::exists(path);
}


bool Filesystem::existsInCurrentDirectory(fs::path path) {
  return existsInDirectory(path, _current_dir);
}


bool Filesystem::existsInDirectory(fs::path path, fs::path parent_dir_path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);
  parent_dir_path = _getRelativeOrAbsolutePath(parent_dir_path);

  // Check if the file/directory even exists
  if (!exists(path) || !exists(parent_dir_path)) return false;
  
  // Check if the parent of the path is the passed 'parent_dir_path'
  const fs::path PATH_PARENT = path.parent_path();
  return (PATH_PARENT == parent_dir_path);
}


bool Filesystem::isPathAbsolute(const fs::path& path) {
  return path.is_absolute();
}


bool Filesystem::isPathRelative(const fs::path& path) {
  return path.is_relative();
}


fs::path Filesystem::getAbsolutePathFromRelativePath(const fs::path& path, const fs::path& parent_dir_path) {
  // If the path is already absolute, then just simply return the given path.
  if (isPathAbsolute(path)) return path;

  // Get the base path
  fs::path base_path; 
  if (parent_dir_path == fs::path{} || !isPathAbsolute(parent_dir_path)) {
    // Default variable used OR the parent directory isn't absolute, so use '_current_dir'
    base_path = _current_dir;
  }
  else {
    base_path = parent_dir_path;
  }

  // Return the path with ./'s & ../'s removed
  return (base_path/path).lexically_normal();
}


bool Filesystem::isDirectory(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // Path must exist to be a directory.
  if (!exists(path)) return false;

  return fs::is_directory(path);
}


bool Filesystem::createDirectory(fs::path parent_dir_path, const std::string& new_dirname) {
  // Get path from relative/absolute
  parent_dir_path = _getRelativeOrAbsolutePath(parent_dir_path);

  // If there already exists a file with that name, return false early.
  if (fs::exists(parent_dir_path)) return false;

  fs::create_directory(parent_dir_path/new_dirname);
  return true;
}


bool Filesystem::remove(fs::path path, bool force_remove) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // Can't remove what's not there :)
  if (!exists(path)) return false;
  
  try {
    if (force_remove) {
      // Removes any non-locked directory
      return (fs::remove_all(path) > 0);
    }
    else {
      // NOTE: Only removes EMPTY directories
      return fs::remove(path);
    }
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::removeDirectory(fs::path dir_path, bool force_remove) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // Can't remove what's not there :) | Also needs to be a directory
  if (!exists(dir_path) || !isDirectory(dir_path)) return false;

  try {
    if (force_remove) {
      // Removes any non-locked directory
      return (fs::remove_all(dir_path) > 0);
    }
    else {
      // NOTE: Only removes EMPTY directories
      return fs::remove(dir_path);
    }
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::removeFile(fs::path file_path) {
  // Get path from relative/absolute
  file_path = _getRelativeOrAbsolutePath(file_path);

  // Can't remove what's not there :) | Also cannot be a directory
  if (!exists(file_path) || isDirectory(file_path)) return false;

  try {
    return fs::remove(file_path);
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::rename(fs::path path, const std::string& new_name, bool overwrite_existing) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  try {
    fs::path modified_path = path;

    // If the new path name is absolute, then simply set it to that, otherwise change the filename itself.
    if (isPathAbsolute(new_name)) {
      modified_path = fs::path{new_name};
    }
    else {
      modified_path.replace_filename(new_name);
    }

    // TODO: Implement a renaming system for the file to overwrite, just in case the rename fails.
    //       It wasn't renamed anyways, so we shouldn't lose the original file too.
    if (overwrite_existing) {
      // Remove existing then rename
      remove(modified_path, true);
    }

    fs::rename(path, modified_path);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::renameDirectory(fs::path dir_path, const std::string& new_dirname, bool overwrite_existing) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  try {
    // MUST be a directory
    if (!isDirectory(dir_path)) return false;

    fs::path modified_dir_path = dir_path;

    // If the new path name is absolute, then simply set it to that, otherwise change the filename itself.
    if (isPathAbsolute(new_dirname)) {
      modified_dir_path = fs::path{new_dirname};
    }
    else {
      modified_dir_path.replace_filename(new_dirname);
    }

    // TODO: Implement a renaming system for the file to overwrite, just in case the rename fails.
    //       It wasn't renamed anyways, so we shouldn't lose the original file too.
    if (overwrite_existing) {
      // Remove existing then rename
      removeDirectory(modified_dir_path, true);
    }

    fs::rename(dir_path, modified_dir_path);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::renameFile(fs::path file_path, const std::string& new_filename, bool overwrite_existing) {
  // Get path from relative/absolute
  file_path = _getRelativeOrAbsolutePath(file_path);

  try {
    // Cannot be a directory
    if (isDirectory(file_path)) return false;

    fs::path modified_file_path = file_path;

    // If the new path name is absolute, then simply set it to that, otherwise change the filename itself.
    if (isPathAbsolute(new_filename)) {
      modified_file_path = fs::path{new_filename};
    }
    else {
      modified_file_path.replace_filename(new_filename);
    }

    // TODO: Implement a renaming system for the file to overwrite, just in case the rename fails.
    //       It wasn't renamed anyways, so we shouldn't lose the original file too.
    if (overwrite_existing) {
      // Remove existing then rename
      removeFile(modified_file_path);
    }

    fs::rename(file_path, modified_file_path);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::copy(fs::path src_path, fs::path dest_path, const std::string& new_name) {
  // Get path from relative/absolute
  src_path = _getRelativeOrAbsolutePath(src_path);
  dest_path = _getRelativeOrAbsolutePath(dest_path);

  // TODO: Limit src_path to some size like 500mb or something. (maybe make a setting in the json)

  // Replace the destination dir_path with new_name
  if (new_name != "") {
    dest_path.replace_filename(new_name);
  }
  // If the file exists, append something to the basename.
  else if (exists(new_name)) {
    std::string replacement_name = dest_path.stem().string() + _EXISTING_FILE_APPENDAGE;
    if (!isDirectory(dest_path)) replacement_name += dest_path.extension().string(); // Files that aren't directories also need to append the extension (which hopefully exists)
    dest_path.replace_filename(replacement_name);
  }
  
  try {
    // Plain copy; no check for file type or anything.
    fs::copy(src_path, dest_path, fs::copy_options::recursive);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::copyDirectory(fs::path src_dir_path, fs::path dest_dir_path, const std::string& new_dirname) {
  // Get path from relative/absolute
  src_dir_path = _getRelativeOrAbsolutePath(src_dir_path);
  dest_dir_path = _getRelativeOrAbsolutePath(dest_dir_path);

  // TODO: Limit Src_dir_path to some size like 500mb or something. (maybe make a setting in the json)

  // Replace the destination dir_path with new_dirname
  if (new_dirname != "") {
    dest_dir_path.replace_filename(new_dirname);
  }
  // If the file exists, append something to the basename.
  else if (exists(new_dirname)) {
    std::string replacement_name = dest_dir_path.stem().string() + _EXISTING_FILE_APPENDAGE;
    dest_dir_path.replace_filename(replacement_name);
  }
  
  try {
    // Src path is not a directory.
    if (!isDirectory(src_dir_path)) return false;

    fs::copy(src_dir_path, dest_dir_path, fs::copy_options::recursive);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::copyFile(fs::path src_file_path, fs::path dest_file_path, const std::string& new_filename) {
  // Get path from relative/absolute
  src_file_path = _getRelativeOrAbsolutePath(src_file_path);
  dest_file_path = _getRelativeOrAbsolutePath(dest_file_path);

  // Replace the destination dir_path with new_filename
  if (new_filename != "") {
    dest_file_path.replace_filename(new_filename);
  }
  // If the file exists, append something to the basename.
  else if (exists(new_filename)) {
    std::string replacement_name = dest_file_path.stem().string() + _EXISTING_FILE_APPENDAGE + dest_file_path.extension().string();
    dest_file_path.replace_filename(replacement_name);
  }
  
  try {
    // Src path is a directory.
    if (isDirectory(src_file_path)) return false;

    // Plain copy; no check for file type or anything.
    fs::copy_file(src_file_path, dest_file_path);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}
