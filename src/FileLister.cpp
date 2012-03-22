#include "FileLister.h"

#include <QDirIterator>
#include <QtDebug>
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

void FileLister::initialize(const QVector< QString > & filters)
{
    completeFileList.clear();

    foreach( const QString & filter, filters )
    {
        QRegExp exp( "^([\\+\\-])(.*) \\((.*)\\)$" );
        if( exp.indexIn( filter ) < 0 )
        {
            qWarning() << "invalid filter [" << filter << "]";
            continue;
        }

        const bool add = ( exp.cap( 1 ) == "+" );
        QDirIterator directory_walker(exp.cap( 2 ), QString( exp.cap( 3 ) ).split(' '), QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

        while(directory_walker.hasNext()) {
            directory_walker.next();

            if( add )
            {
                completeFileList.push_back(directory_walker.fileInfo());
            }
            else
            {
                completeFileList.removeOne(directory_walker.fileInfo());
            }
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
