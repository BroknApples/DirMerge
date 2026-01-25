#ifndef APP_HPP
#define APP_HPP


#include <memory>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QString>
#include <QtQuickControls2/QQuickStyle>
#include <QLoggingCategory>

#include "app_backend.hpp"
#include "../core/config.hpp"
#include "../misc/printer.hpp"


/**
 * @brief GUI-Application entry-point class.
 * 
 * STATIC ONLY CLASS
 */
class Application {
  private:
    static std::unique_ptr<QGuiApplication> _app; /** Qt application object (Handles event loop and Widget module) */
    static std::unique_ptr<QQmlApplicationEngine> _engine; /** Qt QML Engine (Loads and manages the QML UI) */
    static bool _active; /** Is the application currently running? */


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
