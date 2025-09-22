#ifndef MERGE_CONFIG_HPP
#define MERGE_CONFIG_HPP


#include <filesystem>
#include <vector>
#include <queue>
#include <algorithm>

#include "merge_helpers.hpp"
#include "../misc/utils.hpp"


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
    bool _recursive_merge; /** Should folder be expanded more than one time? */
    // TODO: List more options here.

  public:
    /**
     * @brief Default Constructor. Set all values to default saved in config.json
     * TODO: Implement defaults in config.ini
     */
    MergeConfig();


    /**
     * @brief Default Destructor.
     */
    ~MergeConfig();


    /**
     * @brief Adds a file to the merge list
     * @param file: Path to the file to add
     */
    void addFileToMergeList(const fs::path& file);
    

    /**
     * @brief Adds multiple files to the merge list
     * @param files: List of files to add
     */
    void addFilesToMergeList(const std::vector<fs::path>& files);


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
    const NamingSequence& getNamingSequence() const {
      return *_naming_sequence;
    }


    /**
     * @brief Set a new recursive merge settings
     * @param value: True/False value
     */
    void setRecursiveMergeFlag(bool value) {
      _recursive_merge = value;
    }


    /**
     * @brief Gets the recursive merge value for usage
     * @returns bool: True/False flag value
     */
    const bool getRecursiveMergeFlag() const {
      return _recursive_merge;
    }
};


#endif // MERGE_CONFIG_HPP
