#include "application.hpp"

using namespace Qt::StringLiterals;


/*************************************************************
*************************** Private **************************
*************************************************************/


// ------------------------------------- //
// -------- Static Declarations -------- //
// ------------------------------------- //
std::unique_ptr<QGuiApplication> Application::_app = nullptr;
std::unique_ptr<QQmlApplicationEngine> Application::_engine = nullptr;
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

  // Setup app & engine
  _app = std::make_unique<QGuiApplication>(argc, argv);
  _engine = std::make_unique<QQmlApplicationEngine>();

  // Link C++ Logic here
  // static FileHandler fileHandler; 
  // _engine->rootContext()->setContextProperty("FileBackend", &fileHandler);

  // Url
  //const QUrl url(u"qrc:/qt/qml/vsfm/src/ui/app_interface.qml"_s); TODO: Set back to this for production
  const QUrl url = QUrl::fromLocalFile("H:/Dev/VSFM/src/ui/app_interface.qml");
  QObject::connect(_engine.get(), &QQmlApplicationEngine::objectCreated,
                   _app.get(), [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl) QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  _engine->load(url);
  
  // Ensure setup was valid
  if (_engine->rootObjects().isEmpty()) return false;
  return true;
}


void Application::run() {
  // Programmer did NOT run 'Application::init(argc, argv);'
  if ((_app == nullptr) || (_engine  == nullptr)) {
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
  if (_active) _app->exec();
  _active = false;
}