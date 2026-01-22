#ifndef MERGE_CONFIG_HPP
#define MERGE_CONFIG_HPP


#include <filesystem>
#include <vector>
#include <queue>
#include <algorithm>

#include "filesystem.hpp"
#include "merge_helpers.hpp"
#include "../misc/printer.hpp"


namespace fs = std::filesystem;


/**
 * @brief Defines the settings/toggles for a merge
 */
class MergeConfig {
  private:
    // Actual private variables
    std::vector<fs::path> _merge_list; /** List of all files to include in the merge. */
    std::priority_queue<int> _freed_indexes; /** Which indexes in _files are available? */

    // Merging Options
    std::unique_ptr<NamingSequence> _naming_sequence; /** Naming sequence to use in the renaming step. */
    fs::path _dest_dir; /** Destination directory of the merge. */
    
    // TODO: Look into making a bitmask type flag. Do after adding all options ofc tho.
    bool _unpack_subdirectories;            /** Should folders be expanded more than one time? */
    bool _remove_source_post_copy;          /** Should the original files be removed after copying them to the location? */
    bool _overwrite_existing;               /** Should the files overwrite existing files? */
    // TODO: List more options here.

  public:
    /**
     * @brief Default Constructor. Set all values to default saved in config.json
     * TODO: Implement defaults in 'config.json'
     */
    MergeConfig();


    /**
     * @brief Default Destructor.
     */
    ~MergeConfig();


    /**
     * @brief Adds a file to the merge list
     * @param file: Path to the file to add
     * @returns bool: True/False of success
     */
    bool addFileToMergeList(const fs::path& file);
    

    /**
     * @brief Adds multiple files to the merge list
     * @param files: List of files to add
     * @returns bool: True/False of success
     */
    bool addFilesToMergeList(const std::vector<fs::path>& files);


    /**
     * @brief Removes a file from the merge list ( if it exists)
     * @param file: Path to the to remove
     */
    void removeFileFromMergeList(const fs::path& file);


    /**
     * @brief Removes a list of files from the merge list (if they exist)
     * @param files: List of files to remove.
     */
    void removeFilesFromMergeList(const std::vector<fs::path>& files);

    /**
     * @brief Removes every path from the merge list
     */
    void clearMergeList();


    /**
     * OVERLOADED FUNCTION.
     * 
     * @brief Checks if a file exists in the merge list
     *        NOTE: Will NOT check the contents directories included in the merge list.
     * @param file: File to check.
     * @returns bool: True/False of existence.
     */
    bool existsInMergeList(const fs::path& file);


    /**
     * OVERLOADED FUNCTION.
     * 
     * @brief Checks if some number of files exists in the merge list
     *        NOTE: Will NOT check the contents directories included in the merge list.
     * @param files: Files to check
     * @returns bool: True/False of existence.
     */
    bool existsInMergeList(const std::vector<fs::path>& files);


    /**
     * @brief Gets the entire merge list
     * @returns std::vector<fs::path>&: List of files to be included in the merge.
     */
    const std::vector<fs::path>& getMergeList() const;


    /****************************************************
    ******************* Merge Options *******************
    ****************************************************/
    

    /**
     * @brief Set a new naming sequence.
     * @param naming_sequence: New naming sequence to set.
     */
    void setNamingSequence(std::unique_ptr<NamingSequence> naming_sequence) {
      _naming_sequence = std::move(naming_sequence);
    }

    
    /**
     * @brief Gets the naming sequence for usage.
     * @returns NamingSequence&: Reference to the naming sequence.
     */
    NamingSequence& getNamingSequence() const {
      return *_naming_sequence;
    }


    /**
     * @brief Set a new destination directory.
     * @param dest_dir: Path to the new destination directory.
     */
    void setDestinationDirectory(const fs::path& dest_dir) {
      _dest_dir = dest_dir;
    }


    /**
     * @brief Gets the destination directory for usage.
     * @returns fs::path&: Path of the destination directory.
     */
    const fs::path& getDestinationDirectory() const {
      return _dest_dir;
    }


    /**
     * @brief Set a new unpack subdirectories setting
     * @param value: True/False value
     */
    void setUnpackSubdirectoriesFlag(bool value) {
      _unpack_subdirectories = value;
    }


    /**
     * @brief Gets the unpack subdirectories value for usage
     * @returns bool: True/False flag value
     */
    const bool getUnpackSubdirectoriesFlag() const {
      return _unpack_subdirectories;
    }


    /**
     * @brief Sets the remove source post copy flag.
     * @param value: True/False value
     */
    void setRemoveSourcePostCopyFlag(bool value) {
      _remove_source_post_copy = value;
    }


    /**
     * @brief Gets the remove source post copy flag for usage
     * @returns bool: True/False flag value
     */
    const bool getRemoveSourcePostCopyFlag() const {
      return _remove_source_post_copy;
    }


    /**
     * @brief Sets the overwrite existing flag.
     * @param value: True/False value.
     */
    void setOverwriteExistingFlag(bool value) {
      _overwrite_existing = value;
    }


    /**
     * @brief Gets the overwrite existing flag for usage.
     * @returns bool: True/False falg value
     */
    const bool getOverwriteExistingFlag() const {
      return _overwrite_existing;
    }
};


#endif // MERGE_CONFIG_HPP
