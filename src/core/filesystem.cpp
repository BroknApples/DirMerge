#include "filesystem.hpp"


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


bool Filesystem::init() {
  // Config MUST be initialized before calling this function
  if (!Config::isInitialized()) {
    eprintln("Error: Please call 'Config::init()' before 'Filesystem::init()'");
    return false;
  }

  // Setup clipboard and history
  _history = std::stack<fs::path>{}; // TODO: Setup history system.
  _clipboard = std::make_pair(ClipboardActionType::NONE, std::vector<fs::path>{});

  // Set the starting directory to the saved default.
  if (!Filesystem::setCurrentDirectory(Config::getString(Config::Keys::DEFAULT_DIRECTORY))) return false;
  return true;
}


void Filesystem::addToClipboard(ClipboardActionType type, const fs::path& path) {
  _clipboard = std::make_pair(type, std::vector{path});
}


void Filesystem::addToClipboard(ClipboardActionType type, const std::vector<fs::path>& paths) {
  _clipboard = std::make_pair(type, paths);
}


void Filesystem::applyClipboardAction(fs::path dest_dirpath, bool overwrite_existing) {
  // Get path from relative/absolute
  dest_dirpath = _getRelativeOrAbsolutePath(dest_dirpath);

  // Destination path MUST be a directory
  if (!isDirectory(dest_dirpath)) {
    eprintln("Error: Cannot apply the clipboard onto a non-directory file.");
    return;
  }

  // Get clipboard data
  ClipboardActionType& type = _clipboard.first;
  std::vector<fs::path>& files = _clipboard.second;

  switch (type) {
    case ClipboardActionType::NONE: {
      // Do nothing.
      return;
    }
    // Duplicate files into another location
    case ClipboardActionType::COPY: {
      // For each file in the list, call copy() with:
      //  file = the original path
      //  (dest_path/file.filename()) = the original filename appended to the destination directory's path
      for (fs::path& file : files) {
        copy(file, (dest_dirpath/file.filename()), "", overwrite_existing);
      }

      break;
    }
    // Rename files into another location
    case ClipboardActionType::CUT: {
      // For each file in the list, call copy() with:
      //  file = the original path
      //  (dest_dirpath/file.filename()).string() = the original filename appended to the destination directory's path
      for (fs::path& file : files) {
        rename(file, (dest_dirpath/file.filename()).string(), overwrite_existing);
      }

      break;
    }
    // Default statement is unreachable as ClipboardActionType is an ENUM CLASS not just an ENUM
    default: {
      assert(false && "Unreachable code!");
    }
  }

  // Clear the clipboard
  clearClipboard();
}


void Filesystem::clearClipboard() {
  _clipboard.first = ClipboardActionType::NONE;
  _clipboard.second.clear();
}


fs::path Filesystem::getExecutablePath() {
  /**
   * NOTE: Hasn't been tested on other operating systems as of yet.
   * Just theoretically works, chatgpt'd the other OS's so idk for
   * sure.
   */

  // WINDOWS
  #if defined(_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer);

  // LINUX
  #elif defined(__linux__)
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
      buffer[len] = '\0';
      return std::filesystem::path(buffer);
    }

    // Error
    return fs::path{};

  // APPLE
  #elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
      char resolved[PATH_MAX];
      realpath(buffer, resolved);
      return std::filesystem::path(resolved);
    }

    // Error
    return fs::path{};

  // Fallback
  #else
    // This shouldn't be possible on 99.99% of machines, but you never know.
    eprintln("Error: Unsupported Operating System.");
    return fs::path{};
  #endif
}


fs::path Filesystem::getExecutableDirectoryPath() {
  /**
   * NOTE: Hasn't been tested on other operating systems as of yet.
   * Just theoretically works, chatgpt'd the other OS's so idk for
   * sure.
   */

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
    return fs::path{};

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
    return fs::path{};

  // Fallback
  #else
    // This shouldn't be possible on 99.99% of machines, but you never know.
    eprintln("Error: Unsupported Operating System.");
    return fs::path{};
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


// TODO: Ignore all files that are operating system-managed for the following few functions.
//
// Probaly wanna add a parameter to each function called like
// "ignore_os_files" and "ignore_hidden_files" with default values set to TRUE


const std::vector<fs::path> Filesystem::getFilesInCurrentDirectory() {
  return getFilesInDirectory(_current_dir.string());
}


const std::vector<fs::path> Filesystem::getFilesInDirectory(const std::string& dir_path) {
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


const std::vector<fs::path> Filesystem::getFilesInCurrentDirectoryRecursive() {
  return getFilesInDirectoryRecursive(_current_dir.string());
}


const std::vector<fs::path> Filesystem::getFilesInDirectoryRecursive(const std::string& dir_path) {
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

  // Return all paths in the directory
  return std::vector<fs::path>(fs::recursive_directory_iterator(DIR_PATH), {});
}


const int Filesystem::getTotalNumberOfFilesInDirectory(const std::string& dir_path) {
  // Get path from relative/absolute
  const fs::path DIR_PATH = _getRelativeOrAbsolutePath(dir_path);

  // Return 0 if the path is NOT a directory
  if (!isDirectory(DIR_PATH)) return 0;

  int count = 0;
  for (const auto& entry : fs::directory_iterator(DIR_PATH)) {
    if (isRegularFile(entry.status())) count++;
  }

  return count;
}


const int Filesystem::getTotalNumberOfFilesInDirectoryRecursive(const std::string& dir_path) {
  // NEVER run the recursive version when on the system root OR when the current
  // directory's size is over "<SOME_SIZE>"
  if (dir_path == _SYS_ROOT || dir_path == _SYS_ROOT_ALT) {
    // DEBUG Print
    if (DEBUG) println("'dir_path' is ROOT!");

    // TODO: Add some checker for file size. If the directory is over like 1GB
    // or something in size it would take forever to check.
    return getTotalNumberOfFilesInDirectory(dir_path);
  }  

  // Get path from relative/absolute
  const fs::path DIR_PATH = _getRelativeOrAbsolutePath(dir_path);

  // Return empty vector if the path is NOT a directory
  if (!isDirectory(DIR_PATH)) return 0;

  int count = 0;
  for (const auto& entry : fs::recursive_directory_iterator(DIR_PATH)) {
    if (isRegularFile(entry.status())) count++;
  }

  return count;
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


bool Filesystem::isDirectory(const fs::file_status& status) {
  return fs::is_directory(status);
}


bool Filesystem::isRegularFile(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // Path must exist to be a directory.
  if (!exists(path)) return false;

  return fs::is_regular_file(path);
}


bool Filesystem::isRegularFile(const fs::file_status& status) {
  return fs::is_regular_file(status);
}


bool Filesystem::isSystemFile(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);
  
  // File must exist to check.
  if (!exists(path)) return false;

  #if defined(_WIN32)
    DWORD attrs = GetFileAttributesA(path.string().c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) return false;
    return (attrs & FILE_ATTRIBUTE_SYSTEM) != 0;
  #else
    struct stat st;
    if (stat(path.string().c_str(), &st) != 0) return false;

    // Consider "system file" if:
    // - Owned by root (uid 0), OR
    // - Is a special file (not a regular file)
    if (st.st_uid == 0) return true;
    if (!S_ISREG(st.st_mode)) return true; // device, socket, fifo, symlink, etc.

    return false;
  #endif
}


bool Filesystem::isProtectedFile(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);
  
  // File must exist to check.
  if (!exists(path)) return false;

  #if defined(_WIN32)
  // Attempt to open the file
    HANDLE hFile = CreateFileA(
      path.string().c_str(),
      GENERIC_READ,
      0,                // no sharing: request exclusive access
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL
    );

    // Error during opening
    if (hFile == INVALID_HANDLE_VALUE) {
      DWORD err = GetLastError();

      // Locked/protected by another process
      if (err == ERROR_SHARING_VIOLATION) return true;

      // Some other failure (file not found, permission denied, etc.)
      return false; 
    }

    // Cleanup && Success, so return false
    CloseHandle(hFile);
    return false;
  #else
    int fd = open(path.string().c_str(), O_RDONLY);

    // Error during opening, so it must be protected somehow (permission denied, etc.)
    if (fd == -1) return true; 

    // Try a non-blocking advisory lock
    if ((flock(fd, LOCK_EX | LOCK_NB) != 0) && (errno == EWOULDBLOCK)) {
      // Another process has an advisory lock
      close(fd);
      return true; 
    }

    // Cleanup && Success,so return false.
    close(fd);
    return false;
  #endif
}


bool Filesystem::isHiddenFile(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // File must exist to check.
  if (!exists(path)) return false;

  #if defined(_WIN32)
    // Windows hidden files have a 'hidden' attribute attached to them
    DWORD attrs = GetFileAttributesA(path.string().c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) return false;
    return (attrs & FILE_ATTRIBUTE_HIDDEN) != 0;
  #else
    // On POSIX, hidden = starts with '.'
    const std::string filename = path.filename().string();
    return !filename.empty() && filename[0] == '.';
  #endif
}


bool Filesystem::createDirectory(fs::path parent_dir_path, const std::string& new_dirname) {
  // TODO: Create an overload for passing the full filepath to the function, not in two seperate parts.
  
  // Get path from relative/absolute
  parent_dir_path = _getRelativeOrAbsolutePath(parent_dir_path);

  // If there already exists a file with that name, return false early.
  if (exists(parent_dir_path/new_dirname)) return false;

  fs::create_directory(parent_dir_path/new_dirname);
  return true;
}


// TODO: Add overloads for all the functions below that take strings for names and paths


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
  if (!exists(file_path) || !isRegularFile(file_path)) return false;

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
    // Src path is not a regular file.
  if (!isRegularFile(file_path)) return false;

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


bool Filesystem::copy(fs::path src_path, fs::path dest_path, const std::string& new_name, bool overwrite_existing) {
  // Get path from relative/absolute
  src_path = _getRelativeOrAbsolutePath(src_path);
  dest_path = _getRelativeOrAbsolutePath(dest_path);

  // TODO: Limit src_path to some size like 500mb or something. (maybe make a setting in the json)

  // Replace the destination dir_path with new_name
  if (new_name != "") {
    dest_path.replace_filename(new_name);
  }

  // If the file exists, append something to the basename.
  if (exists(dest_path) && !overwrite_existing) {
    std::string replacement_name = dest_path.stem().string() + _EXISTING_FILE_APPENDAGE;
    if (!isDirectory(dest_path)) replacement_name += dest_path.extension().string(); // Files that aren't directories also need to append the extension (which hopefully exists)
    dest_path.replace_filename(replacement_name);
  }

  // TODO: Check if the extension types are the same.
  
  try {
    // Get the copy options
    fs::copy_options copy_options = fs::copy_options::recursive | (overwrite_existing ? fs::copy_options::overwrite_existing : fs::copy_options::recursive);

    // TODO: Create renaming system for the removed file so it can be recovered if copying fails.
    if (overwrite_existing) {
      remove(dest_path);
    }

    // Debug print
    //if (DEBUG) println("Copying '", src_path, "' to '", dest_path, "'");

    fs::copy(src_path, dest_path, copy_options);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::copyDirectory(fs::path src_dir_path, fs::path dest_dir_path, const std::string& new_dirname, bool overwrite_existing) {
  // Get path from relative/absolute
  src_dir_path = _getRelativeOrAbsolutePath(src_dir_path);
  dest_dir_path = _getRelativeOrAbsolutePath(dest_dir_path);

  // Src path is not a directory.
  if (!isDirectory(src_dir_path)) return false;

  // TODO: Limit Src_dir_path to some size like 500mb or something. (maybe make a setting in the json)

  // Replace the destination dir_path with new_dirname
  if (new_dirname != "") {
    dest_dir_path.replace_filename(new_dirname);
  }

  // If the file exists, append something to the basename.
  if (exists(dest_dir_path) && !overwrite_existing) {
    std::string replacement_name = dest_dir_path.stem().string() + _EXISTING_FILE_APPENDAGE;
    dest_dir_path.replace_filename(replacement_name);
  }
  
  try {
    // Get the copy options
    fs::copy_options copy_options = fs::copy_options::recursive | (overwrite_existing ? fs::copy_options::overwrite_existing : fs::copy_options::recursive);

    // TODO: Create renaming system for the removed file so it can be recovered if copying fails.
    if (overwrite_existing) {
      removeDirectory(dest_dir_path);
    }

    fs::copy(src_dir_path, dest_dir_path, copy_options);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}


bool Filesystem::copyFile(fs::path src_file_path, fs::path dest_file_path, const std::string& new_filename, bool overwrite_existing) {
  // Get path from relative/absolute
  src_file_path = _getRelativeOrAbsolutePath(src_file_path);
  dest_file_path = _getRelativeOrAbsolutePath(dest_file_path);

  // Src path is not a regular file.
  if (!isRegularFile(src_file_path)) return false;

  // Replace the destination dir_path with new_filename
  if (new_filename != "") {
    dest_file_path.replace_filename(new_filename);
  }

  // If the file exists, append something to the basename.
  if (exists(dest_file_path) && !overwrite_existing) {
    std::string replacement_name = dest_file_path.stem().string() + _EXISTING_FILE_APPENDAGE + dest_file_path.extension().string();
    dest_file_path.replace_filename(replacement_name);
  }
  
  try {
    // Get the copy options
    fs::copy_options copy_options = overwrite_existing ? fs::copy_options::overwrite_existing : fs::copy_options::none;

    // TODO: Create renaming system for the removed file so it can be recovered if copying fails.
    if (overwrite_existing) {
      removeFile(dest_file_path);
    }

    fs::copy_file(src_file_path, dest_file_path, copy_options);
    return true;
  }
  catch (const fs::filesystem_error& e) {
    eprintln("Error: ", e.what());
    return false;
  }
}
