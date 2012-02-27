#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QMap>
#include <QString>

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~OptionsDialog();

protected:

private slots:
    void okButtonClicked();
    void cancelButtonClicked();
    
private:
    void fillTextEdit();
    bool saveSettings();
    bool parseText(QMap<QString,QString> & data_table, const QString & text);

    QTextEdit *launchersTextEdit;
    QTextEdit *directoriesTextEdit;
};

#endif
