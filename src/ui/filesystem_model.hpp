#ifndef FILESYSTEM_MODEL
#define FILESYSTEM_MODEL


#include <QAbstractListModel>
#include <QVector>


/**
 * @brief TODO:
 */
struct FileItem {
  QString filename;
  QString icon;
  bool is_dir;
};


/**
 * @brief TODO:
 */
class FileSystemModel : public QAbstractListModel {
  Q_OBJECT


  private:
    QVector<FileItem> _files;


  public:
    enum Roles {
      FileNameRole = Qt::UserRole + 1,
      IconRole,
      IsDirRole
    };

    explicit FileSystemModel(QObject *parent = nullptr);

    /**
     * @brief TODO:
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;


     /**
     * @brief TODO:
     */
    QVariant data(const QModelIndex &index, int role) const override;


     /**
     * @brief TODO:
     */
    QHash<int, QByteArray> roleNames() const override;


    /**
     * @brief TODO:
     */
    void setFiles(QVector<FileItem> files);


    /**
     * @brief TODO:
     */
    Q_INVOKABLE void clear();
};


#endif // FILESYSTEM_MODEL