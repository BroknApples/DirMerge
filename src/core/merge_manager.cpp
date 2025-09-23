#include "merge_manager.hpp"


namespace fs = std::filesystem;


/*************************************************************
*************************** Private **************************
*************************************************************/


void MergeManager::_restoreFiles(const std::vector<fs::path>& original_paths, const fs::path& dest_dir) {
  // Currently just force remove the destination directory.
  Filesystem::removeDirectory(dest_dir, true);
}


bool MergeManager::_attemptFileCopying(std::vector<fs::path>& original_paths, const fs::path& filename, const fs::path& dest_dir, NamingSequence& naming_sequence, bool overwrite_existing) {
  // Rename file passes. Add path to the original_paths vector
  if (Filesystem::copy(filename, dest_dir, naming_sequence.getNextName(filename), overwrite_existing)) {
    original_paths.push_back(filename);
    return true;
  }

  // Failure. Restore file paths here.
  _restoreFiles(original_paths, dest_dir);
  return false;
}


/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/


/*************************************************************
*************************** Public ***************************
*************************************************************/


void MergeManager::startMerge(std::unique_ptr<MergeConfig> merge_config) {
  /******************** Get merge config data ********************/

  const std::vector<fs::path> MERGE_LIST    = merge_config->getMergeList();
  NamingSequence& naming_sequence           = merge_config->getNamingSequence();
  const fs::path DEST_DIR                   = merge_config->getDestinationDirectory();
  const bool RECURSIVE_MERGE                = merge_config->getRecursiveMergeFlag();
  const bool REMOVE_SOURCE_POST_COPY        = merge_config->getRemoveSourcePostCopyFlag();
  const bool OVERWRITE_EXISTING             = merge_config->getOverwriteExistingFlag();

  /***************************************************************/

  // ----------------------------------------- //
  // ---------- Pre-merge processes ---------- //
  // ----------------------------------------- //

  // Append the new merge config to the 'active merges' list with a progress value of 0.0f
  _active_merges.emplace_back(0.0f, std::move(merge_config));

  // Get the iterator to the merge config in the list. Used later when cleaning up.
  auto config_iterator = std::prev(_active_merges.end());

  // TODO: Update the data within '_active_merges' as it goes on.

  // TODO: Check if any directory within the merge_list is the parent of the destination directory.
  // Cannot happen if REMOVE_SOURCE_POST_COPY is on.
  if (REMOVE_SOURCE_POST_COPY) {
    // TODO: Gonna have to check recursively too.
    
    // TODO: Update 'false' to some condition.
    if (false) return;
  }

  // TODO: Get the total number of files included in the merge. Update naming_sequence accordingly
  int total_files = 0;
  for (const auto& file : )

  // These are the paths to the original locations of the files. Used to remove/restore files.
  std::vector<fs::path> original_paths;

  // Start Timer
  IntervalTimer timer;
  timer.start();

  /********************* beg Merge Process *********************/

  bool success = true;
  for (const fs::path& file : MERGE_LIST) {
    // If file is a directory, don't add itself, but rather add its contents and its subdirectories' contents
    if (Filesystem::isDirectory(file)) {
      /** NOTE: Recursive merge flag setting */
      std::vector<fs::path> files;
      RECURSIVE_MERGE
        ? files = Filesystem::getFilesInDirectoryRecursive(file.string())
        : files = Filesystem::getFilesInDirectory(file.string());

      // Rename each file in the list.
      for (const fs::path& f : files) {
        if (!_attemptFileCopying(original_paths, f, DEST_DIR, naming_sequence, OVERWRITE_EXISTING)) goto END_MERGE;
      }
    }
    else if (Filesystem::isRegularFile(file)) {
      if (!_attemptFileCopying(original_paths, file, DEST_DIR, naming_sequence, OVERWRITE_EXISTING)) goto END_MERGE;
    }

    // Update completion percentage in the _active_merges vector.
  }
  END_MERGE: ; // Marks the end of the typical merge process.

  /********************* end Merge Process *********************/

  // End Timer
  timer.end();
  double diff = timer.diff_s();

  // ------------------------------------------ //
  // ----- Error during the merge process ----- //
  // ------------------------------------------ //
  if (!success) {
    // Do some error stuff here or whatev.
    return;
  }

  // ---------------------------------------------------------- //
  // ----- Merge was a success, so do post-merge content. ----- //
  // ---------------------------------------------------------- //

  /** NOTE: Remove source post-copy flag setting */
  if (REMOVE_SOURCE_POST_COPY) {
    for (const fs::path& file : MERGE_LIST) {
      // Force remove every file in the list.
      Filesystem::remove(file, true);
    }
  }



  // TODO: Write to the log file

  // Completed merge. Move from active->completed list
  _completed_merges.push_back(std::move(config_iterator->second));
  _active_merges.erase(config_iterator);
}
