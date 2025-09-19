#ifndef MERGE_CONFIG_HPP
#define MERGE_CONFIG_HPP


#include <filesystem>
#include <vector>
#include <queue>

#include "../misc/utils.hpp"


namespace fs = std::filesystem;


/**
 * @brief Defines the settings/toggles for a merge
 */
class MergeConfig {
  private:
    std::vector<fs::path> _merge_list; /** List of all files to include in the merge. */
    std::priority_queue<int> _freed_indexes; /** Which indexes in _files are available? */
    // TODO: List options here.

  public:
    MergeConfig();
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
     * @returns std::vector<fs::path>: List of files to be included in the merge.
     */
    const std::vector<fs::path> getMergeList() const;
};


#endif // MERGE_CONFIG_HPP


/*

Need these vars:

MergeConfig
vector<fs::path> _files;
queue<int> _empty_indexes;



Need these funcs:

addFilesToMergeList(file_list) {
  for file in file_list:
    addFileToMergeList(file)
}

addFileToMergeList(file) {
  if file is a directory, add all files within the directory and apply the "filter" to it (file types, sorting order, etc.)
  (add option for recursive directories for like 3-tiered directories too)


  add to the vector of files, which is used like a stack until numbers are removed.

  prob just create a vector<fs::path> , when a file/directory is removed, removed all corresponding files from the vector and 
  add the empty indexes to a queue which will be checked first before calling push_back(), if there is something in the queue
  do .insert() or something.

  ofc add a setting for whether or not to shift the later files left or to add to the empty indexes queue.
}

resetMergeList() {
  vector.erase(vector.begin(), vector.end());
}

etc.



*/
