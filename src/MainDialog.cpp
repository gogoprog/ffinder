#include "MainDialog.h"

#include "OptionsDialog.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QKeyEvent>
#include <QTextStream>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QUrl>

// Public:

class CustomListWidget : public QListWidget
{
    public :
        CustomListWidget( QWidget * parent = 0 ) : QListWidget( parent ) {}
    protected :
        QStringList mimeTypes() const
        {
            QStringList qstrList;
            qstrList.append("text/uri-list");
            return qstrList;
        }
        QMimeData * mimeData( const QList<QListWidgetItem *> items ) const
        {
            QMimeData *data = new QMimeData();
            QList< QUrl > urls;
            QUrl url;
            url.setPath( items[ 0 ]->toolTip() );
            urls.append( url );
            data->setUrls( urls );
            return data;
        }
};

MainDialog::MainDialog(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout *v_layout = new QVBoxLayout(this);
    QHBoxLayout *bottom_h_layout = new QHBoxLayout();
    QPushButton *options_button = new QPushButton("Options");
    QPushButton *open_button = new QPushButton("Open");
    QPushButton *about_button = new QPushButton("...");
    
    options_button->setMaximumWidth(80);
    about_button->setMaximumWidth(20);
    
    rootList = new QComboBox(this);
    listWidget = new CustomListWidget(this);
    lineInput = new QLineEdit(this);

    rootList->clear();

    v_layout->addWidget(rootList);
    v_layout->addWidget(lineInput);
    v_layout->addWidget(listWidget);
    v_layout->addLayout(bottom_h_layout);
    
    bottom_h_layout->addWidget(open_button);
    bottom_h_layout->addWidget(options_button);
    bottom_h_layout->addWidget(about_button);
    
    connect(lineInput, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(lineInput, SIGNAL(textEdited(const QString &)), this, SLOT(inputTextEdited(const QString &)));
    connect(listWidget, SIGNAL(itemDoubleClicked (QListWidgetItem *)), this, SLOT(returnPressed())); 
    connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));
    connect(options_button, SIGNAL(clicked()), this, SLOT(optionsButtonClicked()));
    connect(open_button, SIGNAL(clicked()), this, SLOT(returnPressed()));
    connect(about_button, SIGNAL(clicked()), this, SLOT(aboutButtonClicked()));
    
    lineInput->setFocus();
    userRegExp.setPatternSyntax(QRegExp::WildcardUnix);

    initializeRootList();
    connect(rootList, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(rootChanged(const QString &)));

    fileLister.initialize(Settings::getInstance().getLastRoot());
    fillList();

    listWidget->setDragEnabled( true );
}

MainDialog::~MainDialog()
{
    
}

int MainDialog::getCurrentFileIndex() const
{
    return listWidget->currentRow();
}

void MainDialog::fillList()
{
    listWidget->clear();
    fileLister.applyFilter();

    for( int i = 0; i < fileLister.getFileCount(); ++i )
    {
        const QFileInfo & file_info = fileLister.getFileInfo( i );
        QListWidgetItem * item = new QListWidgetItem( file_info.fileName(), listWidget );
        item->setToolTip( file_info.filePath() );
    }
}

void MainDialog::openFile(const QFileInfo & fileInfo) const
{
    QString command;

    command = Settings::getInstance().getLauncherForExtension(fileInfo.completeSuffix());

    qDebug() << command << "\n";
    QProcess::startDetached(command, QStringList(fileInfo.filePath()));
}

void MainDialog::initializeRootList()
{
    QMapIterator<QString, QString> iterator(Settings::getInstance().getRootTable());
    int index = 0;
    int indexToSelect = 0;

    rootList->clear();
 
    while (iterator.hasNext()) {
        iterator.next();
        rootList->addItem(iterator.key());
        
        if(iterator.key() == Settings::getInstance().getLastRootKey())
            indexToSelect = index;
        
        ++index;
    }
    
    rootList->setCurrentIndex(indexToSelect);
}

// Protected:

void MainDialog::keyPressEvent(QKeyEvent * e)
{
    if(e->key() == Qt::Key_Up) {
        if(getCurrentFileIndex()-1 >= 0)
            listWidget->setCurrentRow(getCurrentFileIndex()-1);
    }
    else if(e->key() == Qt::Key_Down) {
        if(getCurrentFileIndex()+1 <= fileLister.getFileCount())
            listWidget->setCurrentRow(getCurrentFileIndex()+1);
    }
    else if(e->key() == Qt::Key_Escape) {
        close();
    }
}

void MainDialog::closeEvent(QCloseEvent *event)
{
    Settings::getInstance().saveMainDialogGeometry(this);
    QDialog::closeEvent(event);
}

void MainDialog::showEvent(QShowEvent * /*event*/)
{
    Settings::getInstance().loadMainDialogGeometry(this);
}
 
// Private slots:

void MainDialog::returnPressed()
{
    if (getCurrentFileIndex() == -1)
        return;

    const QFileInfo & info = fileLister.getFileInfo(getCurrentFileIndex());
    openFile(info.absoluteFilePath());
}

void MainDialog::inputTextEdited(const QString &text)
{
    fileLister.setFilter(text);
    fillList();
    userRegExp.setPattern(text);
}

void MainDialog::currentRowChanged(int currentRow)
{
    if (currentRow == -1 )
        return;
        
    const QString & newString = fileLister.getString(currentRow);
    int index;

    index = newString.indexOf(userRegExp);
    index += userRegExp.matchedLength();

    Q_ASSERT(index != -1);
    
    lineInput->setText(newString);
    
    lineInput->setSelection(index, newString.size() - index);   
}

void MainDialog::optionsButtonClicked()
{
    OptionsDialog options_dialog;
    options_dialog.exec();
}

void MainDialog::aboutButtonClicked()
{
    QMessageBox::information(this, "ffinder - About", "ffinder by gogoprog" );
}

void MainDialog::rootChanged(const QString & text)
{
    if (!rootList->count() || text.isEmpty())
        return;

    fileLister.initialize(Settings::getInstance().getRootTable()[text]);
    fillList();
    Settings::getInstance().saveLastRootKey(text);
}
