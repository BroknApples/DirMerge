#ifndef MERGE_MANAGER_HPP
#define MERGE_MANAGER_HPP


#include <vector>
#include <algorithm>

#include "filesystem.hpp"
#include "merge_config.hpp"
#include ".../misc/utils.hpp"


/**
 * @brief Does the actual merging work for a MergeConfig. Responsible for writing logfiles as well.
 *  
 * ------------------
 * 
 * STATIC ONLY CLASS
 */
class MergeManager {
  private:
    /** 
     * Active merge processes running on some number of threads.
     * 
     * NOTE:
     *      float = percentage of merge completion
     *      MergeConfig = merge process
     */
    static std::vector<std::pair<float, std::unique_ptr<MergeConfig>>> _active_merges;

    /**
     * Completed merge processes. Useful for checking the past history of merges that ran while the program was active.
     * 
     * TODO: 
     *       Load data into this on startup from some log file.
     *       The log file should be overwritten completely every time.
     *       Data can be removed from this list by simply clicking delete on it.
     *       This data needs to include the files included, what they were renamed to and stuff like that.
     */
    static std::vector<std::unique_ptr<MergeConfig>> _completed_merges;


  public:
    /**
     * @brief Enforce Static Class
     */
    MergeManager() = delete;


    /**
     * @brief Gets a list of the active merges and their completion progress.
     * @returns std::vector<std::pair<float, std::unique_ptr<MergeConfig>>>&: List of pairs formatted as such: { PercentComplete : MergeConfig }
     */
    const std::vector<std::pair<float, std::unique_ptr<MergeConfig>>>& getActiveMerges() const {
      return _active_merges;
    }


    /**
     * @brief Gets a list of the already completed merges
     * @returns std::vector<std::unique_ptr<MergeConfig>>&: List of past completed merges.
     */
    const std::vector<std::unique_ptr<MergeConfig>>& getCompletedMerges() const {
      return _completed_merges;
    }

    
    /**
     * @brief Start a new merge process.
     * @param merge_config: Config data for the merge.
     */
    static void startMerge(std::unique_ptr<MergeConfig> merge_config) {
      /******************** Get merge config data ********************/

      const std::vector<fs::path> MERGE_LIST    = merge_config->getMergeList();
      NamingSequence naming_sequence      = merge_config->getNamingSequence();
      const bool RECURSIVE_MERGE                = merge_config->getRecursiveMergeFlag();
      
      /******************************************************/

      // Append the new merge config to the 'active merges' list with a progress value of 0.0f
      _active_merges.emplace_back(0.0f, std::move(merge_config));
      
      // Get the iterator to the merge config in the list. Used later when cleaning up.
      auto config_iterator = std::prev(_active_merges.end());

      // TODO: Start the actual merge loop here and update the data within '_active_merges' as it goes on.
      
      // Get and set the total number of files included in the merge.
      for (const fs::path& file : MERGE_LIST) {
        // If file is a directory, don't add itself, but rather add its contents and its subdirectories' contents
        if (Filesystem::isDirectory(file)) {
          std::vector<fs::path> files;

          /** NOTE: Recursive merge flag setting */ 
          if (RECURSIVE_MERGE) {
            files = Filesystem::getFilesInDirectoryRecursive(file.string());
          }
          else {
            files = Filesystem::getFilesInDirectory(file.string());
          }

          // Rename each file in the list.
          for (const fs::path& f : files) {
            Filesystem::rename(f, naming_sequence.getNextName(f), true);
          }
        }
        else if (Filesystem::isRegularFile(file)) {
          // Rename file.
          Filesystem::renameFile(file, naming_sequence.getNextName(file), true);
        }
      }


      // Completed merge. Move from active->completed list
      _completed_merges.push_back(std::move(config_iterator->second));
      _active_merges.erase(config_iterator);
    }
};


#endif // MERGE_MANAGER_HPP
