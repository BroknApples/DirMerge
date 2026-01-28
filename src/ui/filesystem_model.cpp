#include "filesystem_model.hpp"


FileSystemModel::FileSystemModel(QObject *parent)
: QAbstractListModel(parent) {}


int FileSystemModel::rowCount(const QModelIndex &) const {
  return _files.size();
}


QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return {};

  const FileItem &item = _files[index.row()];
  switch (role) {
    case FileNameRole: return item.filename;
    case IconRole:     return item.icon;
    case IsDirRole:    return item.is_dir;
  }

  return {};
}


QHash<int, QByteArray> FileSystemModel::roleNames() const {
  return {
    { FileNameRole, "filename" },
    { IconRole,     "icon" },
    { IsDirRole,    "is_dir" }
  };
}


void FileSystemModel::setFiles(QVector<FileItem> files) {
  beginResetModel();
  _files = std::move(files);
  endResetModel();
}


Q_INVOKABLE void FileSystemModel::clear() {
  beginResetModel();
  _files.clear();
  endResetModel();
}