/**
 * 
 * TODO: Add info here...
 * 
 * 
 * 
 * 
 */

#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "core/config.hpp"
#include "core/filesystem.hpp"
#include "misc/clock.hpp"
#include "misc/interval_timer.hpp"
#include "misc/utils.hpp"


namespace fs = std::filesystem;


/**
 * @brief Sets general parameters and starts the program.
 * @param argc: Number of arguments passed to the executable.
 * @param argv: Arguments passed to the executable | NOTE: argv[0] is ALWAYS the command used to open this executable.
 */
int main(int argc, char* argv[]) {
  // Initialize 'globals'
  Config::init(fs::path("..")/Filesystem::getExecutableDirectoryPath()); // TESTING - Need to set custom path during testing. Modify if necessary.
  Filesystem::init();


  Filesystem::setCurrentDirectory(Filesystem::getExecutableDirectoryPath());

  while (true) {
    std::vector<fs::path> files = Filesystem::getFilesInCurrentDirectory();
    for (const auto& path : files) {
      println("Path: ", path);
    }

    println("\nEnter a filename: ");
    std::string filename;
    std::cin >> filename;

    println("Enter an option:\n 0 - Rename\n 1 - Copy\n 2 - Remove\n");
    int option;
    std::cin >> option;

    switch (option) {
      case 0: {
        println("Enter a new filename: ");
        std::string new_filename;
        std::cin >> new_filename;
        Filesystem::rename(filename, new_filename);
        break;
      }
      case 1: {
        println("Enter a destination path: ");
        std::string dest_path;
        std::cin >> dest_path;

        println("Enter a new filename: ");
        std::string new_filename;
        std::cin >> new_filename;
        Filesystem::copy(filename, dest_path, new_filename);
        break;
      }
      case 2: {
        Filesystem::remove(filename, true);
        break;
      }
      default:
        return 0;
    }
  }
  

  // // TESTING
  // println("[Test1]");
  // Filesystem::setCurrentDirectory(Config::getString(Config::Keys::DEFAULT_DIRECTORY));
  // std::vector<std::filesystem::path> dirs = Filesystem::getFilesInCurrentDirectory();
  // println("\nGathered Paths: ");
  // for (const auto path : dirs) {
  //   println("Path: ", path);
  // }
  // println("");


  // if (dirs.size() > 0) {
  //   println("[Test2]");
  //   println("Using path: '", dirs[0], "'");
  //   Filesystem::setCurrentDirectory(dirs[0]);
  //   std::vector<std::filesystem::path> dirs2 = Filesystem::getFilesInCurrentDirectory();
  //   println("\nGathered Paths: ");
  //   for (const auto path : dirs2) {
  //     println("Path: ", path);
  //   }
  //   println("");

  //   Filesystem::setCurrentDirectory(dirs2[5]);
  //   println(Filesystem::getCurrentDirectoryPath());
  // }


  // // TESTING
  // IntervalTimer t;
  // while (true) {
  //   print("Enter seconds to wait: ");
  //   double time;
  //   std::cin >> time;

  //   t.start();
  //   Clock::wait_s(time);
  //   t.end();

  //   println(std::to_string(t.diff_ms()) + "ms");
    
  //   t.reset();
  // }


  return EXIT_SUCCESS;
}
