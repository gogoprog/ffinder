#include "FileLister.h"

#include <QDirIterator>
#include "Settings.h"

bool simpleSort(const QFileInfo &file1, const QFileInfo &file2)
{
    return file1.fileName() < file2.fileName();
}

FileLister::FileLister()
{
    regExp.setPatternSyntax(QRegExp::WildcardUnix);
}

FileLister::~FileLister()
{
    
}

void FileLister::initialize(const QString & path)
{
    QDirIterator directory_walker(path, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    completeFileList.clear();

    while(directory_walker.hasNext()) {
        directory_walker.next();
        const QString suffix = directory_walker.fileInfo().completeSuffix();
        foreach(const QString & extension, Settings::getInstance().getExtensionTable()) {
            if(suffix == extension)
                completeFileList.push_back(directory_walker.fileInfo());
        }
    }

    qSort(completeFileList.begin(), completeFileList.end(), simpleSort);
}

void FileLister::applyFilter()
{
    fileList.clear();
    stringList.clear();
    regExp.setPattern(filter);

    if(!filter.isEmpty()) {
        foreach(const QFileInfo &fileInfo,completeFileList) {
            if(fileInfo.fileName().indexOf(regExp) == 0) {
                stringList.push_back(fileInfo.fileName());
                fileList.push_back(fileInfo);
            }
        }
    }
    else {
        foreach(const QFileInfo &fileInfo,completeFileList) {
            stringList.push_back(fileInfo.fileName());
            fileList.push_back(fileInfo);
        }
    }

}
