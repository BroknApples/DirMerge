#include "app_backend.hpp"


namespace fs = std::filesystem;


/*************************************************************
*************************** Private **************************
*************************************************************/


void AppBackend::_refreshFilesystemUi() {
  // Get files & fill vector that will be used in the UI
  std::vector<fs::path> files = Filesystem::getFilesInDirectory(_current_filesystem_path.toStdString());
  QVector<FileItem> items;
  items.reserve(files.size());
  for (const fs::path &path : files) {
    bool is_dir = Filesystem::isDirectory(path);

    FileItem item;
    item.filename = QString::fromUtf8(path.filename().u8string().c_str());
    item.full_path = QString::fromUtf8(path.u8string().c_str());
    item.icon = is_dir ? "📁" : "📄";
    item.is_dir = is_dir;

    items.push_back(item);
  }

  _filesystem_model.setFiles(items);
}


/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/


AppBackend::AppBackend(QObject *parent) : QObject(parent) {
  _active_merge_config = MergeConfig();
  _path_history.clear();
  _path_index = -1;

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


Q_INVOKABLE void AppBackend::addFileToMerge(const QString &filename) {
  std::string current = _current_filesystem_path.toStdString();
  std::string full_path = (fs::path(current)/filename.toStdString()).string();
  _active_merge_config.addFileToMergeList(full_path);
  emit mergeListChanged();
}


Q_INVOKABLE void AppBackend::removeFileFromMerge(const QString &filename) {
  std::string current = _current_filesystem_path.toStdString();
  std::string full_path = (fs::path(current)/filename.toStdString()).string();
  _active_merge_config.removeFileFromMergeList(full_path);
  emit mergeListChanged();
}


Q_INVOKABLE void AppBackend::enterSubdirectory(const QString &directory_name) {
  std::string current = _current_filesystem_path.toStdString();
  std::string newPath = (fs::path(current)/directory_name.toStdString()).string();
  setFilesystemPath(QString::fromStdString(newPath));
}


Q_INVOKABLE void AppBackend::setDestinationPath(const QString& dest_path) {
  _active_merge_config.setDestinationDirectory(dest_path.toStdString());
}


Q_INVOKABLE void AppBackend::setNamingSequence(const QString& naming_sequence) {
  _active_merge_config.setNamingSequence(std::make_unique<NumericalNamingSequence>());
};


Q_INVOKABLE void AppBackend::setRemoveSources(const bool value) {
  _active_merge_config.setRemoveSourcePostCopyFlag(value);
}


Q_INVOKABLE void AppBackend::goBack() {
  // Set new path index & update current path
  if (_path_index > 0 && _path_index < (int)_path_history.size()) {
    _path_index--;
    setFilesystemPath(_path_history[_path_index], false);
  }
}


Q_INVOKABLE void AppBackend::goForward() {
  // Set new path index & update current path
  if (_path_index + 1 < (int)_path_history.size()) {
    _path_index++;
    setFilesystemPath(_path_history[_path_index], false);
  }
}


Q_INVOKABLE void AppBackend::goToParentDirectory() {
  const std::string CURRENT_STR = _current_filesystem_path.toStdString();
  const fs::path CURRENT_PATH(CURRENT_STR);

  // Check if a parent actually exists to avoid navigating into empty space
  if (CURRENT_PATH.has_parent_path() && CURRENT_PATH != CURRENT_PATH.root_path()) {
    fs::path parent = CURRENT_PATH.parent_path();
    
    // We use .string() here to extract the text from the path object
    setFilesystemPath(QString::fromStdString(parent.string()));
  } else {
    // If we're at C:// or /, we might want to stay there or handle it specifically
    println("Already at root directory.");
  }
}


Q_INVOKABLE void AppBackend::refreshFileList() {
  setFilesystemPath(_current_filesystem_path);
}


Q_INVOKABLE int AppBackend::getFileOrder(QString filename) {
  std::string searchStr = filename.toStdString();
  const int idx = _active_merge_config.getIndexInMergeList(searchStr);
  
  // This will tell us if the paths look different (e.g., missing slashes or extensions)
  println("Checking: " + searchStr + " | Found at: " + std::to_string(idx));
  
  return (idx != -1) ? (idx + 1) : 0;
}


Q_INVOKABLE void AppBackend::setFilesystemPath(const QString &path, const bool update_history) {
  // Update the path in the filesystem model
  _current_filesystem_path = path;
  emit currentPathChanged();

  // Update the Filesystem singleton's path
  Filesystem::setCurrentDirectory(_current_filesystem_path.toStdString());

  // Wipe history past the current index if we are not at the end.
  if (update_history) {
    if (_path_index < _path_history.size() - 1) {
      _path_history.erase(_path_history.begin() + _path_index + 1, _path_history.end());
    }

    // Append current path to history
    _path_history.push_back(path);
    _path_index++;
  }

  // Refresh UI
  _refreshFilesystemUi();
}