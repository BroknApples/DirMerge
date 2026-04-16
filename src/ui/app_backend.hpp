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
    std::vector<QString> _path_history;
    int _path_index;

  
    /**
     * @brief Refresh the files listed in the UI application
     */
    void _refreshFilesystemUi();

  
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
    Q_INVOKABLE void addFileToMerge(const QString &filename);


    /**
     * @brief Removes a file from the merg elist
     * @param path: Path to remove from "merge_list"
     */
    Q_INVOKABLE void removeFileFromMerge(const QString &filename);


    /**
     * @brief Joins the current path with a folder name and navigates into it
     */
    Q_INVOKABLE void enterSubdirectory(const QString &directory_name);


    /**
     * @brief Sets the destination directory for the merge
     * @param dest_path: Path to the directory they will merged into
     */
    Q_INVOKABLE void setDestinationPath(const QString& dest_path);


    /**
     * @brief Sets the naming sequence for the merge
     * @param naming_sequence: New naming sequence string key
     */
    Q_INVOKABLE void setNamingSequence(const QString& naming_sequence);


    /**
     * @brief Sets the "remove sources" flag for the merge
     * @param value: New flag value
     */
    Q_INVOKABLE void setRemoveSources(const bool value);


    /**
     * @brief Navigates to the previous entry in the path history
     */
    Q_INVOKABLE void goBack();


    /**
     * @brief Navigates to the next entry in the path history
     */
    Q_INVOKABLE void goForward();


    /**
     * @brief Navigates to the parent directory of the current path
     */
    Q_INVOKABLE void goToParentDirectory();


    /**
     * @brief Refresh the current directory and scan for files
     */
    Q_INVOKABLE void refreshFileList();


    /**
     * @brief Returns 1-based index, or 0 if not in the list
     * @param filename: Name of the file to check
     */
    Q_INVOKABLE int getFileOrder(QString filename);


    /**
     * @brief Sets the path for the filesystem (this is the current path shown in the path bar)
     * @param path: New path as a QString
     * @param update_history: Should the history be updated after navigating to the path
     */
    Q_INVOKABLE void setFilesystemPath(const QString &path, const bool update_history = true);


  signals:
    void currentPathChanged();
    void mergeListChanged();
};


#endif // APP_BACKEND_HPP
