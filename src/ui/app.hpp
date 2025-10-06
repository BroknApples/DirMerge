#ifndef APP_HPP
#define APP_HPP


#include <memory>
#include <QApplication>
#include <QMainWindow>

#include "../core/config.hpp"
#include "../misc/utils.hpp"


/**
 * @brief GUI-Application entry-point class.
 * 
 * STATIC ONLY CLASS
 */
class Application {
  private:
    /** Qt application object */
    static std::unique_ptr<QApplication> _app;


    /** Qt main window object */
    static std::unique_ptr<QMainWindow> _window;


    /** Is the application currently running? */
    static bool _active;


  public:
    /**
     * @brief Enfore Static Class
     */
    Application() = delete;


    /**
     * @brief Initialize all processes needed for the gui application to run.
     * 
     * NOTE: Please use as such:
     * 
     *  if (!Application::init(argc, argv)) {
     * 
     *    <Error stuff here>
     * 
     *  }
     * 
     *  Application::run(); // Or something similar
     * 
     * @param argc: Number of command-line arguments
     * @param argv: Command-line arguments
     * @returns bool: True/False of success.
     */
    static bool init(int argc, char* argv[]);


    /**
     * @brief Application loop.
     */
    static void run();


    /**
     * @brief Is the GUI Application currently active?
     * @returns bool: True/False of the application 'active' status.
     */
    static bool isActive() {
      return _active;
    }
};


#endif // APP_HPP
