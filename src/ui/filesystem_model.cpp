#include "filesystem_model.hpp"


FilesystemModel::FilesystemModel(QObject *parent)
: QAbstractListModel(parent) {}


int FilesystemModel::rowCount(const QModelIndex &) const {
  return _files.size();
}


QVariant FilesystemModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return {};

  const FileItem &item = _files[index.row()];
  switch (role) {
    case FileNameRole: return item.filename;
    case FullPathRole: return item.full_path;
    case IconRole:     return item.icon;
    case IsDirRole:    return item.is_dir;
  }

  return {};
}


QHash<int, QByteArray> FilesystemModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[FileNameRole] = "filename";
  roles[FullPathRole] = "full_path";
  roles[IconRole] = "icon";
  roles[IsDirRole] = "is_dir";
  return roles;
}


void FilesystemModel::setFiles(QVector<FileItem> files) {
  beginResetModel();
  _files = std::move(files);
  endResetModel();
}


Q_INVOKABLE void FilesystemModel::clear() {
  beginResetModel();
  _files.clear();
  endResetModel();
}