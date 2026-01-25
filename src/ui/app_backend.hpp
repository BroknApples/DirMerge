#ifndef APP_BACKEND_HPP
#define APP_BACKEND_HPP


#include <filesystem>

#include <QObject>
#include <QDebug>

#include "../core/merge_manager.hpp"
#include "../core/merge_config.hpp"


/**
 * @brief Defines the QML implementations of the C++ logic.
 * 
 * NOTE: This is where the outermost wrappers for code should go.
 * As such, this class will include most of the "core/" logic
 */
class AppBackend : public QObject {
  Q_OBJECT
  private:
    MergeConfig _active_merge_config;

  public:
    /**
     * @brief Constructs parent object
     */
    explicit AppBackend(QObject *parent = nullptr);

    /**
     * @brief Merges the selected files into one directory
     * @param dest_path: Path to the directory they will merged into
     * @param remove_sources: Should the original files be deleted post-merge
     */
    Q_INVOKABLE void merge(const QString &dest_path, bool remove_sources);


    /**
     * @brief Merges the selected files into one directory
     * @param path: Path to add to "merge_list"
     */
    Q_INVOKABLE void addFileToMerge(const QString &path);
};



#endif // APP_BACKEND_HPP
