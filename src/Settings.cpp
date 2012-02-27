#include "Settings.h"

#include <QSettings>
#include <QMetaType>
#include <QDataStream>

#define APPNAME "ffinder"
#define COMPANY "FishingCactus"

typedef QMap<QString,QString> QStringMap;

Q_DECLARE_METATYPE(QStringMap);

Settings & Settings::getInstance()
{
    static Settings instance;
    return instance;
}

void Settings::initialize()
{
    qRegisterMetaType<QStringMap>("QStringMap");
    qRegisterMetaTypeStreamOperators<QStringMap>("QStringMap");

    QSettings settings(COMPANY, APPNAME);

    rootTable = settings.value("rootTable").value<QStringMap>();
    launcherTable = settings.value("launcherTable").value<QStringMap>();

    lastRootKey = settings.value("lastRootKey").toString();

    if (lastRootKey.isEmpty() && rootTable.size() > 0)
        lastRootKey = rootTable.begin().key();

    refreshTables();
}

const QString Settings::getLauncherForExtension(const QString &extension) const
{
    return launcherTable[extension];
}

void Settings::saveMainDialogGeometry(QDialog *dialog)
{
     QSettings settings(COMPANY, APPNAME);
     settings.setValue("geometry", dialog->saveGeometry());
}

void Settings::saveLauncherTable(const QMap<QString,QString> & launcher_table)
{
    QSettings settings(COMPANY, APPNAME);

    launcherTable = launcher_table;

    settings.setValue("launcherTable",qVariantFromValue(launcherTable));
}

void Settings::saveRootTable(const QMap<QString,QString> & root_table)
{
    QSettings settings(COMPANY, APPNAME);

    rootTable = root_table;
    
    settings.setValue("rootTable",qVariantFromValue(rootTable));
}

void Settings::saveLastRootKey(const QString & key)
{
    QSettings settings(COMPANY, APPNAME);

    lastRootKey = key;

    settings.setValue("lastRootKey", lastRootKey);
}

void Settings::loadMainDialogGeometry(QDialog *dialog)
{
     QSettings settings(COMPANY, APPNAME);
     dialog->restoreGeometry(settings.value("geometry").toByteArray());
}
   
void Settings::refreshTables()
{
    QMapIterator<QString, QString> iterator(launcherTable);

    extensionTable.clear();
    
    while (iterator.hasNext()) {
        iterator.next();
        extensionTable.append(iterator.key());
    }
}
