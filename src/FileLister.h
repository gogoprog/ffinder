#ifndef FILELISTER_H
#define FILELISTER_H

#include <QDir>
#include <QList>
#include <QStringList>
#include <QRegExp>

class FileLister
{

public:
    FileLister();
    ~FileLister();
    
    QStringList & getStringList() { return stringList; }
    const QFileInfo & getFileInfo(int index) { return fileList[index]; }
    const QString & getString(int index) { return stringList[index]; }
    int getFileCount() const { return stringList.size(); }
    void setFilter(const QString & _filter) { filter = _filter; }
    void initialize(const QString & path);
    void applyFilter();

private:
    QList<QFileInfo> completeFileList;
    QStringList stringList;
    QList<QFileInfo> fileList;
    QString filter;
    QRegExp regExp;
};

#endif