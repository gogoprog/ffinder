#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QRegExp>
#include <QFileInfo>
#include <QComboBox>
#include "FileLister.h"
#include "Settings.h"

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainDialog();
    int getCurrentFileIndex() const;
    void fillList();
    void openFile(const QFileInfo & fileInfo) const;
    void initializeRootList();

protected:
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event); 

private slots:
    void returnPressed();
    void inputTextEdited(const QString &text);
    void currentRowChanged(int currentRow);
    void optionsButtonClicked();
    void aboutButtonClicked();
    void rootChanged(const QString & text);

private:
    FileLister fileLister;
    QListWidget *listWidget;
    QLineEdit *lineInput;
    QRegExp userRegExp;
    QComboBox *rootList;
};

#endif
