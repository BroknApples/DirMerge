#include "app_backend.hpp"


/*************************************************************
*************************** Private **************************
*************************************************************/





/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/

AppBackend::AppBackend(QObject *parent) : QObject(parent) {

}

/*************************************************************
*************************** Public ***************************
*************************************************************/


Q_INVOKABLE void AppBackend::merge(const QString &dest_path, bool remove_sources) {
  // 1. Prepare the current config with UI values
  _active_merge_config.setDestinationDirectory(fs::path(dest_path.toStdString()));
  _active_merge_config.setRemoveSourcePostCopyFlag(remove_sources);
  
  // 2. Set a naming sequence if one isn't set (your Manager expects one)
  _active_merge_config.setNamingSequence(std::make_unique<NumericalNamingSequence>());

  // 3. MOVE the config into a unique_ptr to pass to the Manager
  // This avoids the "use of deleted function" error
  auto ptr = std::make_unique<MergeConfig>(std::move(_active_merge_config));
  
  // 4. Start the merge
  MergeManager::startMerge(std::move(ptr));
}


Q_INVOKABLE void AppBackend::addFileToMerge(const QString& path) {
  fs::path p(path.toStdString());
  _active_merge_config.addFileToMergeList(p);
}