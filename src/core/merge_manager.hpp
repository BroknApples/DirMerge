#ifndef MERGE_MANAGER_HPP
#define MERGE_MANAGER_HPP


#include <vector>

#include "merge_config.hpp"


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
    static std::vector<std::pair<float, MergeConfig&>> _active_merges;


  public:
    /**
     * @brief Enforce Static Class
     */
    MergeManager() = delete;

    
    /**
     * @brief Start a new merge process.
     * @param merge_config: Config data for the merge.
     */
    static void startMerge(MergeConfig& merge_config);
};


#endif // MERGE_MANAGER_HPP
