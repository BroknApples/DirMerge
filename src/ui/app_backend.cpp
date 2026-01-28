#include "app_backend.hpp"


/*************************************************************
*************************** Private **************************
*************************************************************/





/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/

AppBackend::AppBackend(QObject *parent) : QObject(parent) {
  setPath(QDir::homePath());
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


void AppBackend::setPath(const QString &path) {
  _current_path = path;
  emit currentPathChanged();

  QDir dir(path);
  QVector<FileItem> items;

  for (const QFileInfo &info : dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
    items.push_back({
      info.fileName(),
      info.isDir() ? "📁" : "📄",
      info.isDir()
    });
  }

  _file_model.setFiles(items);
}