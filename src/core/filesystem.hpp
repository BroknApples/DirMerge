#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP


#include <filesystem>
#include <string>
#include <vector>
#include <stack>
#include <cassert>

#if defined(_WIN32)
  #include <windows.h>
  #include <cwchar>
#elif defined(__linux__)
  #include <sys/stat.h>
  #include <unistd.h>
  #include <pwd.h>
  #include <grp.h>
  #include <cstring>
  #include <climits>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <pwd.h>
  #include <grp.h>
  #include <cstring>
  #include <climits>
#endif

#include "config.hpp"


namespace fs = std::filesystem;


/**
 * @brief Basic implementation of a filesystem.
 * 
 * ------------------
 * 
 * STATIC ONLY CLASS
 */
class Filesystem {
  public:
    /**
     * @brief Determines the type of action the path saved in the clipboard is subject to
     */
    enum class ClipboardActionType {
      NONE,
      CUT,
      COPY
    };

  private:
    /** Root of the filesystem */
    inline static const std::string _SYS_ROOT = "/";

    /** Alternate root of the filesystem */
    inline static const std::string _SYS_ROOT_ALT = "\\";

    /** Appended to filenames that already exist when copying/renaming */
    inline static const std::string _EXISTING_FILE_APPENDAGE = " - Copy";

    static std::stack<fs::path> _history;
    static fs::path _current_dir;
    static std::pair<ClipboardActionType, std::vector<fs::path>> _clipboard;


    /**
     * @brief Determines whether or not a path is relative or absolute and returns the true path accordingly.
     * @param path: std::filesystem::path to get the full path of.
     * @returns fs::path: A complete std::filesystem::path object.
     */
    static fs::path _getRelativeOrAbsolutePath(const fs::path& path);


  public:
    /**
     * @brief Enforce Static Class
     */
    Filesystem() = delete;


    /**
     * @brief Initialize class defaults.
     *        NOTE: Config::init() must be called first.
     */
    static void init();


    /**
     * OVERLOADED FUNCTION.
     * 
     * @brief Adds an action to the clipboard.
     * @param type: Determines if the action should be a copy or cut.
     * @param path: Path of the file that will be affected. NOTE: CANNOT be relative.
     */
    static void addToClipboard(ClipboardActionType type, const fs::path& path);


    /**
     * OVERLOADED FUNCTION.
     * 
     * @brief Adds an action to the clipboard.
     * @param type: Determines if the action should be a copy or cut.
     * @param paths: Vector containing the path of the files that will be affected. NOTE: CANNOT be relative.
     */
    static void addToClipboard(ClipboardActionType type, const std::vector<fs::path>& paths);


    /**
     * @brief Applies the action stored in the clipboard. Equivalent to PASTE.
     * @param dest_dirpath: Path that the clipboard should be applied in. 
     * @param overwrite_existing: If a file already exists with the name in the destination 
     *                            directory, should it be overwritten?
     */
    static void applyClipboardAction(fs::path dest_dirpath, bool overwrite_existing = false);


    /**
     * @brief Clears the data stored in the clipboard
     */
    static void clearClipboard();

    /**
     * @brief Get the path to the current executable
     * @returns fs::path: Path to the executable on the filesystem.
     */
    static fs::path getExecutablePath();


    /**
     * @brief Get the path to the current executable's directory
     * @returns fs::path: Path to the executable's directory on the filesystem.
     */
    static fs::path getExecutableDirectoryPath();


    /**
     * @brief Get the path to the current directory the filesystem is set to.
     * @returns fs::path: Path to the '_current_dir' private var
     */
    static const fs::path getCurrentDirectoryPath();


    /**
     * @brief Go to a new directory.
     * @param dir_path: Path to the new directory. Can be relative or absolute
     * @returns bool: True/False of success.
     */
    static bool setCurrentDirectory(fs::path dir_path);


    /**
     * @brief Get all the files in the current directory. Does not get the contents of subdirectories.
     * @returns std:vector<fs::path>: List of files.
     */
    static const std::vector<fs::path> getFilesInCurrentDirectory();


    /**
     * @brief Get all the files in a given directory. Does not get the contents of subdirectories.
     * @param dir_path: Path to the directory to get the files from. Can be relative or absolute
     * @returns std:vector<fs::path>: List of files.
     */
    static const std::vector<fs::path> getFilesInDirectory(const std::string& dir_path);

    
    /**
     * @brief Get all the files in the current directory. Gets the contents of subdirectories.
     *        This does NOT return ANY directory type files, rather it will return the contents
     *        of those directories added into the vector.
     * @returns std:vector<fs::path>: List of files.
     */
    static const std::vector<fs::path> getFilesInCurrentDirectoryRecursive();


    /**
     * @brief Get all the files in the current directory. Gets the contents of subdirectories.
     *        This does NOT return ANY directory type files, rather it will return the contents
     *        of those directories added into the vector.
     * @param dir_path: Path to the directory to get the files from. Can be relative or absolute
     * @returns std:vector<fs::path>: List of files.
     */
    static const std::vector<fs::path> getFilesInDirectoryRecursive(const std::string& dir_path);


    /**
     * @brief Gets the number of files in a directory. NOTE: Does not open subdirectories, instead counts a subdirectory as 1 file.
     * @param dir_path: Path to the directory to get the files from. Can be relative or absolute
     * @returns int: Number of files in the given directory.
     */
    static const int getTotalNumberOfFilesInDirectory(const std::string& dir_path);


    /**
     * @brief Gets the number of files in a directory and its subdirectories.
     * @param dir_path: Path to the directory to get the files from. Can be relative or absolute
     * @returns int: Number of files in the given directory and its subdirectories.
     */
    static const int getTotalNumberOfFilesInDirectoryRecursive(const std::string& dir_path);


    /***
     * @brief Checks if a given file exists in the filesystem.
     * @param path: Path to the file to check existence of.
     * @returns bool: True/False of existence.
     */
    static bool exists(fs::path path);


    /**
     * @brief Checks if a given file exists in the current directory. NOTE: Only
     *        checks the immediate parent, not two-plus levels up
     * @param path: Filename to check existence of.
     * @returns bool: True/False of existence.
     */
    static bool existsInCurrentDirectory(fs::path path);


    /**
     * @brief Checks if a given file exists in a given directory. NOTE: Only
     *        checks the immediate parent, not two-plus levels up
     * @param path: Filename to check existence of.
     * @param parent_dir_path: Path to the directory to scan. Can be relative or absolute.
     * @returns bool: True/False of existence.
     */
    static bool existsInDirectory(fs::path path, fs::path parent_dir_path);


    /**
     * @brief Determines whether or not a path is an Absolute Path ("C:/", "D:/", etc.)
     * @param path: std::filesystem::path to check.
     * @returns bool: True if the path is absolute, otherwise false.
     */
    static bool isPathAbsolute(const fs::path& path);


    /**
     * @brief Determines whether or not a path is a Relative Path ("dir1/file.txt", "text_doc.txt", etc.)
     * @param path: std::filesystem::path to check.
     * @returns bool: True if the path is relative, otherwise false.
     */
    static bool isPathRelative(const fs::path& path);


    /**
     * @brief Gets the full path given the relative path to a given/current directory.
     * @param path: std::filesystem::path to get the relative path of.
     * @param parent_dir_path: Path to the parent directory of the path. Default is the current directory of the filesystem.
     * @returns fs::path: Full path.
     */
    static fs::path getAbsolutePathFromRelativePath(const fs::path& path, const fs::path& parent_dir_path = fs::path{});


    /**
     * @brief Checks if a given file is a directory.
     * @param path: Path of the file to check.
     * @returns bool: True if the path is a directory, otherwise false. NOTE: Always false if the path doesn't exist.
     */
    static bool isDirectory(fs::path path);


    /**
     * @brief Checks if a given file is a directory.
     * @param status: fs::file_status of the file to check.
     * @returns bool: True if the path is a directory, otherwise false. NOTE: Always false if the path doesn't exist.
     */
    static bool isDirectory(const fs::file_status& status);


    /**
     * @brief Checks if a given file is a regular file.
     * @param path: Path of the file to check.
     * @returns bool: True if the path is a regular file, otherwise false. NOTE: Always false if the path doesn't exist.
     */
    static bool isRegularFile(fs::path path);


    /**
     * @brief Checks if a given file is a regular file.
     * @param status: fs::file_status of the file to check.
     * @returns bool: True if the path is a regular file, otherwise false. NOTE: Always false if the path doesn't exist.
     */
    static bool isRegularFile(const fs::file_status& status);


    /**
     * @brief Checks if a file is an operating-system file. 
     * @param path: Path to the file
     * @returns bool: True/False value.
     */
    static bool isSystemFile(fs::path path);


    /**
     * @brief Checks if a file is a protected file.
     * @param path: Path to the file
     * @returns bool: True/False value.
     */
    static bool isProtectedFile(fs::path path);


    /**
     * @brief Checks if a file is a hidden file.
     * @param path: Path to the file
     * @returns bool: True/False value.
     */
    static bool isHiddenFile(fs::path path);


    /**
     * @brief Creates a new directory on the system.
     * @param parent_dir_path: Path to the directory the new directory shall be placed in.
     * @param new_dirname: Name of the newly created directory.
     * @returns bool: True/False of success.
     */
    static bool createDirectory(fs::path parent_dir_path, const std::string& new_dirname);


    /**
     * @brief Removes a file from the system. NOTE: Does NOT check if the file is a directory before proceeding.
     * @param path: Path to the file to remove.
     * @param force_remove: Force remove the file, even if its a non-empty directory. Default = false
     * @returns bool True/False of success.
     */
    static bool remove(fs::path path, bool force_remove = false);


    /**
     * @brief Removes an empty OR non-empty directory from the system. NOTE: Will not work if the file passed is not a directory.
     * @param dir_path: Path to the directory to remove.
     * @param force_remove: Force remove the directory, even if its not empty. Default = false
     * @returns bool True/False of success.
     */
    static bool removeDirectory(fs::path dir_path, bool force_remove = false);


    /**
     * @brief Removes a file from the system. NOTE: Will not work if the file passed is a directory.
     * @param file_path: Path to the file to remove.
     * @returns bool True/False of success.
     */
    static bool removeFile(fs::path file_path);


    /**
     * @brief Renames a file on the system. NOTE: Does NOT check if the file is a directory before proceeding.
     * @param path: Path to the file to rename.
     * @param new_name: New name of the file. Can be absolute or relative.
     * @param overwrite_existing: Should the file forcefully overwrite an existing file? Default = false
     * @returns bool: True/False of success.
     */
    static bool rename(fs::path path, const std::string& new_name, bool overwrite_existing = false);


    /**
     * @brief Renames a file on the system. NOTE: Will not work if the file passed is not a directory.
     * @param dir_path: Path to the directory to rename.
     * @param new_dirname: New name of the directory. Can be absolute or relative.
     * @param overwrite_existing: Should the file forcefully overwrite an existing file? Default = false
     * @returns bool: True/False of success.
     */
    static bool renameDirectory(fs::path dir_path, const std::string& new_dirname, bool overwrite_existing = false);


    /**
     * @brief Renames a file on the system. NOTE: Will not work if the file passed is a directory.
     * @param file_path: Path to the file to rename.
     * @param new_filename: New name of the file. Can be absolute or relative.
     * @param overwrite_existing: Should the file forcefully overwrite an existing file? Default = false
     * @returns bool: True/False of success.

     */
    static bool renameFile(fs::path file_path, const std::string& new_filename, bool overwrite_existing = false);


    /**
     * @brief Duplicates a file onto the system with another name.
     * @param src_path: Path to the file. Can be relative or absolute.
     * @param dest_path: Path to the destination location of the file. Can be relative or absolute.
     * @param new_name: New name of the file. NOTE: Optional. Default will be the same name OR "original_name - Copy.extension"
     *                  if there already exists a file with that name in the destination.
     * @param overwrite_existing: Should the file forcefully overwrite an existing file? Default = false
     * @returns bool: True/False of success.
     */
    static bool copy(fs::path src_path, fs::path dest_path, const std::string& new_name = "", bool overwrite_existing = false);


    /**
     * @brief Duplicates a directory onto the system with another name. NOTE: Automatically recursively copies files/subdirectories and their contents.
     * @param src_dir_path: Path to the directory. Can be relative or absolute.
     * @param dest_dir_path: Path to the destination location of the directory. Can be relative or absolute.
     * @param new_dirname: New name of the directory. NOTE: Optional. Default will be the same name OR "original_name - Copy"
     *                     if there already exists a directory with that name in the destination.
     * @param overwrite_existing: Should the file forcefully overwrite an existing file? Default = false
     * @returns bool: True/False of success.
     */
    static bool copyDirectory(fs::path src_dir_path, fs::path dest_dir_path, const std::string& new_dirname = "", bool overwrite_existing = false);


    /**
     * @brief Duplicates a file onto the system with another name.
     * @param src_file_path: Path to the file. Can be relative or absolute.
     * @param dest_file_path: Path to the destination location of the file. Can be relative or absolute.
     * @param new_filename: New name of the file. NOTE: Optional. Default will be the same name OR "original_name - Copy.extension"
     *                      if there already exists a file with that name in the destination.
     * @param overwrite_existing: Should the file forcefully overwrite an existing file? Default = false
     * @returns bool: True/False of success.
     */
    static bool copyFile(fs::path src_file_path, fs::path dest_file_path, const std::string& new_filename = "", bool overwrite_existing = false);
};


#endif // FILESYSTEM_HPP
