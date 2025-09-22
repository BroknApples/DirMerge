#include "merge_config.hpp"


namespace fs = std::filesystem;


/*************************************************************
*************************** Private **************************
*************************************************************/


/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/


MergeConfig::MergeConfig()
  : _merge_list()
  , _freed_indexes()
  , _naming_sequence(nullptr)
  , _recursive_merge(false) {}


MergeConfig::~MergeConfig() {}


/*************************************************************
*************************** Public ***************************
*************************************************************/


void MergeConfig::addFileToMergeList(const fs::path& file) {
  // TODO: Add a setting that makes it so it doesn't check if
  // there are any empty index, but rather always pushes to the back

  // If the freed indexes queue isn't empty, then use a value from it.
  if (!_freed_indexes.empty()) {
    // Get the lowest empty index.
    int lowest_free_index = _freed_indexes.top();
    _freed_indexes.pop();

    // Set value at the free index to the file's path.
    _merge_list[lowest_free_index] = file;

    // Log data & return
    println("Added '", file, "' to the merge list at index [", lowest_free_index, "].");
    return;
  }

  // No empty index, so add to the back.
  _merge_list.push_back(file);

  // Log data
  println("Added '", file, "' to the merge list at index [", (_merge_list.size() - 1), "].");
}


void MergeConfig::addFilesToMergeList(const std::vector<fs::path>& files) {
  // Just call the singular file add function on each.
  for (const fs::path& file : files) {
    addFileToMergeList(file);
  }
}


void MergeConfig::removeFileFromMergeList(const fs::path& file) {
  // Check if the file exists before attempting to remove.
  if (!existsInMergeList(file)) return;

  // Get the iterator that points to the file's location in the vector
  auto it = std::find(_merge_list.begin(), _merge_list.end(), file);

  if (it != _merge_list.end()) {
    // Get index and remove file
    int index = std::distance(_merge_list.begin(), it);
    
    // TODO: Implement merge list shrinking option
    // if (SOME_OPTION) {
    //   _merge_list.erase(it);
    // }
    // else {
    //    *it = fs::path{};
    // }
    *it = fs::path{}; // Set value found to an empty fs::path

    // Add index to the freed indexes queue.
    _freed_indexes.push(index);

    // Log data
    println("Removed '", file, "' from the merge list at index [", index, "].");
  }
}


void MergeConfig::removeFilesFromMergeList(const std::vector<fs::path>& files) {
  // Check if the file exists before attempting to remove.
  if (!existsInMergeList(files)) return;

  for (const fs::path& file : files) {
    removeFileFromMergeList(file);
  }
}


void MergeConfig::clearMergeList() {
  _merge_list.clear();
  
  // Log process
  println("Emptied the merge list.");
}


bool MergeConfig::existsInMergeList(const fs::path& file) {
  if (std::find(_merge_list.begin(), _merge_list.end(), file) == _merge_list.end()) {
    return false;
  }
  
  return true;
}


bool MergeConfig::existsInMergeList(const std::vector<fs::path>& files) {
  for (const fs::path& file : files) {
    if (std::find(_merge_list.begin(), _merge_list.end(), file) == _merge_list.end()) {
      return false;
    }
  }

  return true;
}


const std::vector<fs::path>& MergeConfig::getMergeList() const {
  return _merge_list;
}