#include "app.hpp"


/*************************************************************
*************************** Private **************************
*************************************************************/


// ------------------------------------- //
// -------- Static Declarations -------- //
// ------------------------------------- //
std::unique_ptr<QApplication> Application::_app = nullptr;
std::unique_ptr<QMainWindow> Application::_window = nullptr;
bool Application::_active = false;

/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/


/*************************************************************
*************************** Public ***************************
*************************************************************/


bool Application::init(int argc, char* argv[]) {
  // Must call Config::init() before this function.
  if (!Config::isInitialized()) {
    eprintln("Error: Please call Config::init() before Application::init()!");
    return false;
  }

  // Setup app
  _app = std::make_unique<QApplication>(argc, argv);

  // Setup Main Window
  _window = std::make_unique<QMainWindow>();
  const int DEFAULT_WIDTH = Config::getInt(Config::Keys::WINDOW_WIDTH);
  const int DEFAULT_HEIGHT = Config::getInt(Config::Keys::WINDOW_HEIGHT);
  _window->setWindowTitle("VSFM");
  _window->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

  // TODO: Initialize Widgets here

  return true;
}


void Application::run() {
  // Programmer did NOT run 'Application::init(argc, argv);'
  if ((_app == nullptr) || (_window  == nullptr)) {
    eprintln("Error: Application::run() called before Application::init()!");
    return;
  }

  // Programmer attempted to run this function twice at once somehow.
  if (_active) {
    eprintln("Error: Attempted to run multiple instances of the application at once!");
    return;
  }

  // Set blocking variable while the program is running.
  _active = true;
  _window->show();
  _app->exec();
  _active = false;
}