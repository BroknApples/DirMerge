#ifndef MERGE_HELPERS_HPP
#define MERGE_HELPERS_HPP


#include <filesystem>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdarg>

#include "../misc/utils.hpp"


namespace fs = std::filesystem;


/***********************************************************
******************* beg Naming Sequences *******************
***********************************************************/


/**
 * @brief Defines the sequence used for naming files in a merge.
 * 
 * NOTE: This can be used within plugins to add custom file merging naming sequences
 * 
 * ----------
 * 
 * NON-CONSTRUCTABLE CLASS - Use as a blueprint.
 */
class NamingSequence {
  private:
    /** How many files will be included in the merge. */
    int _total_files;


  protected:
    /**
     * @brief Enforce Non-Constructable Class
     */
    NamingSequence() = default;


  public:
    /**
     * @brief Sets the total number of files that will be included in the merge.
     * @param total_files: New total number of files.
     * 
     * ---------------
     * 
     * VIRTUAL FUNCTION. Default version simply sets the variable.
     */
    virtual void setTotalFiles(int total_files) {
      _total_files = total_files;
    }


    /**
     * @brief Gets the next name in the sequence
     * @param filepath: Path of the file to rename.
     * 
     * ---------------
     * 
     * VIRTUAL FUNCTION. Default version returns the inputted value.
     */
    virtual const std::string getNextName(const fs::path& filepath) {
      return filepath.string();
    }
};


/**
 * @brief Naming sequence which renames files to incrementing numerical values.
 */
class NumericalNamingSequence : public NamingSequence {
  private:
    /** Number of zeroes used to pad the filename. */
    int _zero_padding;

    /** Current number to use for the filename. */
    int _current_num;


  public:
    /**
     * @brief Constructs a new object with '_current_num' and '_zero_padding' both set to 1.
     */
    NumericalNamingSequence() : _current_num(1), _zero_padding(1) {}


    /**
     * @brief Sets the total number of files and calculates zero padding at the same time.
     * @param total_files: Total number of files included in the merge.
     * 
     * ---------
     *
     * OVERRIDE FUNCTION
     */
    void setTotalFiles(int total_files) override {
      // Call base method
      NamingSequence::setTotalFiles(total_files);

      // Calculate the zero padding
      int _zero_padding = std::to_string(std::abs(total_files)).size();
    }


    /**
     * @brief Gets the next name in the sequence
     * @param filepath: Path of the file to rename.
     */
    const std::string getNextName(const fs::path& filepath) override {
      // Write the number using an ostringstream
      std::ostringstream oss;
      oss << std::setw(_zero_padding) << std::setfill('0') << _current_num;

      // Increment '_current_num' and return the string with the correct file extension appended
      _current_num++;
      return (oss.str() + filepath.extension().string());
    }
};


/***********************************************************
******************* end Naming Sequences *******************
***********************************************************/


#endif // MERGE_HERLPERS_HPP
