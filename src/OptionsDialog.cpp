#include "OptionsDialog.h"
#include "Settings.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QStringList>
#include <QMessageBox>

// Public:

OptionsDialog::OptionsDialog(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
    QVBoxLayout *v_layout = new QVBoxLayout(this);
    QVBoxLayout *layout1 = new QVBoxLayout();
    QVBoxLayout *layout2 = new QVBoxLayout();
    QHBoxLayout *h_layout = new QHBoxLayout();
    QPushButton *ok_button = new QPushButton("Ok");
    QPushButton *cancel_button = new QPushButton("Cancel");
    QGroupBox *group1 = new QGroupBox("Root directories");
    QGroupBox *group2 = new QGroupBox("Launchers");

    this->setWindowTitle("ffinder - Options");
    this->setGeometry(0,0,500,500);

    launchersTextEdit = new QTextEdit(this);
    directoriesTextEdit = new QTextEdit(this);

    launchersTextEdit->setToolTip("Extension=Application\n\nExample:\ncpp=gedit\ntxt=leafpad\n");
    directoriesTextEdit->setToolTip("Name=Directory Path\n\nExample:\nmyproject=/home/foo/myproject/\n");
    
    v_layout->addWidget(group1);
    v_layout->addWidget(group2);
    v_layout->addLayout(h_layout);
    
    h_layout->addWidget(ok_button);
    h_layout->addWidget(cancel_button);
    
    group1->setLayout(layout1);
    group2->setLayout(layout2);
    
    layout1->addWidget(new QLabel("List:"));
    layout1->addWidget(directoriesTextEdit);

    layout2->addWidget(new QLabel("List:"));
    layout2->addWidget(launchersTextEdit);

    connect(ok_button, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    
    fillTextEdit();
}

OptionsDialog::~OptionsDialog()
{
}

// Protected:


// Private slots:

void OptionsDialog::okButtonClicked()
{
    if(saveSettings())
        this->close();
}

void OptionsDialog::cancelButtonClicked()
{
    this->close();
}

// Private:

void OptionsDialog::fillTextEdit()
{
    {
        QMapIterator<QString, QString> iterator(Settings::getInstance().getLauncherTable());
        QString line;

        launchersTextEdit->clear();

        while (iterator.hasNext()) {
            iterator.next();
            line = iterator.key() + "=" + iterator.value();
            launchersTextEdit->append(line);
        }
    }

    {
        QMapIterator<QString, QString> iterator(Settings::getInstance().getRootTable());
        QString line;
        
        directoriesTextEdit->clear();
        
        while (iterator.hasNext()) {
            iterator.next();
            line = iterator.key() + "=" + iterator.value();
            directoriesTextEdit->append(line);
        }
    }
}

bool OptionsDialog::saveSettings()
{
    QMap<QString,QString> data_table;

    if (parseText(data_table, directoriesTextEdit->toPlainText()))
        Settings::getInstance().saveRootTable(data_table);
    else {
        QMessageBox::critical(this, "ffinder - Error", "Cannot parse directories." );
        return false;
    }

    data_table.clear();
    
    if (parseText(data_table, launchersTextEdit->toPlainText()) )
        Settings::getInstance().saveLauncherTable(data_table);
    else {
        QMessageBox::critical(this, "ffinder - Error", "Cannot parse launchers." );
        return false;
    }

    Settings::getInstance().refreshTables();
    
    return true;
}

bool OptionsDialog::parseText(QMap<QString,QString> & data_table, const QString & text)
{
    QStringList lines = text.split("\n");

    foreach(const QString line, lines) {
        QStringList strings = line.split("=");
        if (strings.size() <= 1)
            return false;
        data_table.insert(strings[0], strings[1]);
    }
    
    return true;
}
