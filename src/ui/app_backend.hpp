#ifndef APP_BACKEND_HPP
#define APP_BACKEND_HPP


#include <filesystem>

#include <QObject>
#include <QDebug>
#include <QDir>

#include "../core/merge_manager.hpp"
#include "../core/merge_config.hpp"
#include "filesystem_model.hpp"


/**
 * @brief Defines the QML implementations of the C++ logic.
 * 
 * NOTE: This is where the outermost wrappers for code should go.
 * As such, this class will include most of the "core/" logic
 */
class AppBackend : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString current_path READ currentPath NOTIFY currentPathChanged)
  Q_PROPERTY(FileSystemModel* file_model READ fileModel CONSTANT)
  
  private:
    MergeConfig _active_merge_config;
    FileSystemModel _file_model;
    QString _current_path;

  public:
    /**
     * @brief Constructs parent object
     */
    explicit AppBackend(QObject *parent = nullptr);


    /**
     * @brief Merges the selected files into one directory
     */
    Q_INVOKABLE void merge();


    /**
     * @brief Merges the selected files into one directory
     * @param path: Path to add to "merge_list"
     */
    Q_INVOKABLE void addFileToMerge(const QString &path) {
      _active_merge_config.addFileToMergeList(path.toStdString());
    }


    /**
     * @brief Sets the destination directory for the merge
     * @param dest_path: Path to the directory they will merged into
     */
    Q_INVOKABLE void setDestinationPath(const QString& dest_path) {
      _active_merge_config.setDestinationDirectory(dest_path.toStdString());
    }


    /**
     * @brief Sets the naming sequence for the merge
     * @param naming_sequence: New naming sequence string key
     */
    Q_INVOKABLE void setNamingSequence(const QString& naming_sequence) {
      _active_merge_config.setNamingSequence(std::make_unique<NumericalNamingSequence>());
    };


    /**
     * @brief Sets the "remove sources" flag for the merge
     * @param value: New flag value
     */
    Q_INVOKABLE void setRemoveSources(const bool value) {
      _active_merge_config.setRemoveSourcePostCopyFlag(value);
    }


    /**
     * @brief TODO:
     */
    QString currentPath() const {
      return _current_path;
    }

    /**
     * @brief TODO:
     */
    FileSystemModel* fileModel() {
      return &_file_model;
    }

    /**
     * @brief TODO:
     */
    Q_INVOKABLE void setPath(const QString &path);


  signals:
    void currentPathChanged();
};



#endif // APP_BACKEND_HPP
