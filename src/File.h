#ifndef FILE_H
#define FILE_H

#include <QFileInfo>
#include <QList>

class FileLister
{

public:
    FileLister();
    ~FileLister();
    
private:
    QList<File*> fileList;
};

#endif
