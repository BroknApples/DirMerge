#ifndef MERGE_MANAGER_HPP
#define MERGE_MANAGER_HPP


#include <vector>
#include <algorithm>

#include "filesystem.hpp"
#include "merge_config.hpp"
#include "../misc/utils.hpp"
#include "../misc/interval_timer.hpp"


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


    /***************************************************************/
    /********************** Private Functions **********************/
    /***************************************************************/


    /**
     * @brief Restore all files included in the merge since there was some issue.
     *        Deletes the copied files and restores anything done to the original files.
     * @param original_paths: Paths of the files to restore.
     * @param dest_dir: Directory the files were copied to.
     */
    static void _restoreFiles(const std::vector<fs::path>& original_paths, const fs::path& dest_dir);
    

    /**
     * @brief Attempt to copy the files to the new location in the merge processe.
     * @param original_paths: Vector contatining the original paths of all the merged files
     * @param filename: File to attempt copying of.
     * @param dest_dir: Destination folder of the new filename.
     * @param naming_sequence: NamingSequence& class used for renaming.
     * @param overwrite_existing: Should the file overwrite an existing file.
     * @returns bool: True/False of success.
     */
    static bool _attemptFileCopying(std::vector<fs::path>& original_paths, const fs::path& filename, const fs::path& dest_dir, NamingSequence& naming_sequence, bool overwrite_existing);


  public:
    /**
     * @brief Enforce Static Class
     */
    MergeManager() = delete;


    /**
     * @brief Gets a list of the active merges and their completion progress.
     * @returns std::vector<std::pair<float, std::unique_ptr<MergeConfig>>>&: List of pairs formatted as such: { PercentComplete : MergeConfig }
     */
    static const std::vector<std::pair<float, std::unique_ptr<MergeConfig>>>& getActiveMerges() {
      return _active_merges;
    }


    /**
     * @brief Gets a list of the already completed merges
     * @returns std::vector<std::unique_ptr<MergeConfig>>&: List of past completed merges.
     */
    static const std::vector<std::unique_ptr<MergeConfig>>& getCompletedMerges() {
      return _completed_merges;
    }

    
    /**
     * @brief Start a new merge process.
     * @param merge_config: Config data for the merge.
     */
    static void startMerge(std::unique_ptr<MergeConfig> merge_config);
};


#endif // MERGE_MANAGER_HPP
