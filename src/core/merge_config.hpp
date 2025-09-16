#ifndef MERGE_CONFIG_HPP
#define MERGE_CONFIG_HPP


#include <filesystem>
#include <vector>
#include <queue>


namespace fs = std::filesystem;


/**
 * @brief Defines the settings/toggles for a merge
 */
class MergeConfig {
  private:
    std::vector<fs::path> _files; /** List of all files to include in the merge */
    std::queue<int> _empty_indexes; /** Which indexes in _files are empty. */
    // TODO: List options here.

  public:
    
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
