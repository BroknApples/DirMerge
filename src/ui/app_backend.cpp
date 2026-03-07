#include "app_backend.hpp"


/*************************************************************
*************************** Private **************************
*************************************************************/





/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/

AppBackend::AppBackend(QObject *parent) : QObject(parent) {
  setFilesystemPath(QDir::homePath());
}

/*************************************************************
*************************** Public ***************************
*************************************************************/


Q_INVOKABLE void AppBackend::merge() {
  // 1. Set a naming sequence if one isn't set (your Manager expects one)
  

  // 2. MOVE the config into a unique_ptr to pass to the Manager
  // This avoids the "use of deleted function" error
  auto ptr = std::make_unique<MergeConfig>(std::move(_active_merge_config));
  
  // 3. Start the merge
  MergeManager::startMerge(std::move(ptr));
}


void AppBackend::setFilesystemPath(const QString &path) {
  // Update th epath in the filesystem model
  _current_filesystem_path = path;
  emit currentPathChanged();

  // Update the Filesystem singleton's path
  Filesystem::setCurrentDirectory(_current_filesystem_path.toStdString());

  // Get files
  std::vector<fs::path> files = Filesystem::getFilesInDirectory(path.toStdString());
  QVector<FileItem> items;
  items.reserve(files.size());
  for (const fs::path &path : files) {
    // Leverage your class's helper methods for icons and types
    bool is_dir = Filesystem::isDirectory(path);
    
    items.push_back({
      QString::fromStdString(path.filename().string()),
      QString::fromStdString(path.string()),
      is_dir ? "📁" : "📄",
      is_dir
    });
  }

  _filesystem_model.setFiles(items);
}