/**
 * 
 * TODO: Add info here...
 * 
 * 
 * 
 * 
 */


// Platform specific includes
#if defined(_WIN32)
  #define NOMINMAX
  #include <windows.h>
#endif // Platform specific includes


#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>

#include "ui/application.hpp"
#include "core/config.hpp"
#include "core/filesystem.hpp"
#include "core/merge_config.hpp"
#include "core/merge_manager.hpp"
#include "core/merge_helpers.hpp"
#include "misc/clock.hpp"
#include "misc/interval_timer.hpp"
#include "misc/printer.hpp"


namespace fs = std::filesystem;


/**
 * @brief Sets general parameters and starts the program.
 * @param argc: Number of arguments passed to the executable.
 * @param argv: Arguments passed to the executable | NOTE: argv[0] is ALWAYS the command used to open this executable.
 */
int main(int argc, char* argv[]) {
  // TESTING - Need to set custom path during testing. Modify if necessary.
  const fs::path exe_dir = Filesystem::getExecutableDirectoryPath();
  const fs::path modified_path = exe_dir/fs::path("..")/fs::path("config/config.json");

  // ---------------------------------------- //
  // --------- Initialize 'globals' --------- //
  // ---------------------------------------- //

  if (!Config::init(modified_path)) {
    eprintln("Error: Could not initialize the configuration.");
    return EXIT_FAILURE;
  }

  if (!Filesystem::init()) {
    eprintln("Error: Could not initialize the filesystem.");
    return EXIT_FAILURE;
  };

  if (!Application::init(argc, argv)) {
    eprintln("Error: Could not initialize the application.");
    return EXIT_FAILURE;
  }

  Application::run();

  /********************************************************/
  /********************************************************/
  /********************************************************/

  // Filesystem::setCurrentDirectory(Filesystem::getExecutableDirectoryPath());

  // // Setup config
  // auto merge_config = std::make_unique<MergeConfig>();
  // while (true) {
  //   print("Add more? (1 for yes, 0 for no): ");
  //   int _continue;
  //   std::cin >> _continue;
  //   if (_continue == 0) break;

  //   println("");
  //   std::vector<fs::path> files = Filesystem::getFilesInCurrentDirectory();
  //   for (const auto& path : files) {
  //     println("Path: ", path);
  //   }

  //   print("Add a filename to the merge: ");
  //   std::string filename;
  //   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  //   std::getline(std::cin, filename);

  //   merge_config->addFileToMergeList(filename);
  // }
  // // TESTING
  // // Set merge config data
  // merge_config->setNamingSequence(std::make_unique<NumericalNamingSequence>());
  
  // const fs::path dest_path = exe_dir/fs::path("test_dir");
  // println("Setting destination path to: '", dest_path, "'");
  // merge_config->setDestinationDirectory(dest_path);


  // // Do merge
  // MergeManager::startMerge(std::move(merge_config));

  /********************************************************/
  /********************************************************/
  /********************************************************/

  // while (true) {
  //   std::vector<fs::path> files = Filesystem::getFilesInCurrentDirectory();
  //   for (const auto& path : files) {
  //     println("Path: ", path);
  //   }

  //   println("\nEnter a filename: ");
  //   std::string filename;
  //   std::cin >> filename;

  //   println("Enter an option:\n 0 - Rename\n 1 - Copy\n 2 - Remove\n");
  //   int option;
  //   std::cin >> option;

  //   switch (option) {
  //     case 0: {
  //       println("Enter a new filename: ");
  //       std::string new_filename;
  //       std::cin >> new_filename;
  //       Filesystem::rename(filename, new_filename);
  //       break;
  //     }
  //     case 1: {
  //       println("Enter a destination path: ");
  //       std::string dest_path;
  //       std::cin >> dest_path;

  //       println("Enter a new filename: ");
  //       std::string new_filename;
  //       std::cin >> new_filename;
  //       Filesystem::copy(filename, dest_path, new_filename);
  //       break;
  //     }
  //     case 2: {
  //       Filesystem::remove(filename, true);
  //       break;
  //     }
  //     default:
  //       return 0;
  //   }
  // }
  
  /********************************************************/
  /********************************************************/
  /********************************************************/

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

  /********************************************************/
  /********************************************************/
  /********************************************************/

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
