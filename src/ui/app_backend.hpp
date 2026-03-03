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

  Q_PROPERTY(QString current_filesystem_path READ getCurrentFilesystemPath NOTIFY currentPathChanged)
  Q_PROPERTY(FilesystemModel* filesystem_model READ getFilesystemModel CONSTANT)
  
  private:
    MergeConfig _active_merge_config;
    FilesystemModel _filesystem_model;
    QString _current_filesystem_path;

  public:
    /**
     * @brief Constructs parent object
     */
    explicit AppBackend(QObject *parent = nullptr);


    /**
     * @brief Returns the current path to the filesystem
     */
    QString getCurrentFilesystemPath() const {
      return _current_filesystem_path;
    }


    /**
     * @brief Gets the file model
     */
    FilesystemModel* getFilesystemModel() {
      return &_filesystem_model;
    }


    /**
     * @brief Merges the selected files into one directory
     */
    Q_INVOKABLE void merge();


    /**
     * @brief Merges the selected files into one directory
     * @param path: Path to add to "merge_list"
     */
    Q_INVOKABLE void addFileToMerge(const QString &filename) {
      std::string current = _current_filesystem_path.toStdString();
      std::string fullPath = (fs::path(current)/filename.toStdString()).string();
      _active_merge_config.addFileToMergeList(fullPath);
    }


    /**
     * @brief Joins the current path with a folder name and navigates into it
     */
    Q_INVOKABLE void enterDirectory(const QString &directory_name) {
      std::string current = _current_filesystem_path.toStdString();
      std::string newPath = (fs::path(current)/directory_name.toStdString()).string();
      setFilesystemPath(QString::fromStdString(newPath));
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
     * @brief Navigates to the parent directory of the current path
     */
    Q_INVOKABLE void goToParentDirectory() {
      const std::string CURRENT_STR = _current_filesystem_path.toStdString();
      const fs::path CURRENT_PATH(CURRENT_STR);

      // Check if a parent actually exists to avoid navigating into empty space
      if (CURRENT_PATH.has_parent_path() && CURRENT_PATH != CURRENT_PATH.root_path()) {
        fs::path parent = CURRENT_PATH.parent_path();
        
        // We use .string() here to extract the text from the path object
        setFilesystemPath(QString::fromStdString(parent.string()));
      } else {
        // If we're at C:/ or /, we might want to stay there or handle it specifically
        println("Already at root directory.");
      }
    }


    /**
     * @brief Refresh the current directory and scan for files
     */
    Q_INVOKABLE void refreshFileList() {
      setFilesystemPath(_current_filesystem_path);
    }


    /**
     * @brief Sets the path for the filesystem (this is the current path shown in the path bar)
     * @param path: New path as a QString
     */
    Q_INVOKABLE void setFilesystemPath(const QString &path);


  signals:
    void currentPathChanged();
};


#endif // APP_BACKEND_HPP
