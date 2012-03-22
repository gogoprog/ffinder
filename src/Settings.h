#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QMap>
#include <QString>
#include <QStringList>
#include <QDialog>

class Settings
{
public:

    typedef QMap<QString, QVector< QString > > rootTable_t;

    static Settings & getInstance();
    void initialize();
    const QString getLauncherForExtension(const QString &extension) const;
    const QMap<QString,QString> & getLauncherTable() const {
        return launcherTable;
    }
    const rootTable_t & getRootTable() const {
        return rootTable;
    }
    const QString & getLastRootKey() const {
        return lastRootKey;
    }
    /*
    const QString getLastRoot() const {
        return rootTable[lastRootKey];
    }
    */
    const QStringList & getExtensionTable() const {
        return extensionTable;
    }

    void saveMainDialogGeometry(QDialog *dialog);
    void saveLauncherTable(const QMap<QString,QString> & launcher_table);
    void saveRootTable(const rootTable_t & root_table);
    void saveLastRootKey(const QString & key);
    void loadMainDialogGeometry(QDialog *dialog);
    void refreshTables();
    
private:
    
    QMap<QString,QString> launcherTable;
    rootTable_t rootTable;
    QStringList extensionTable;
    QString lastRootKey;
};

#endif // __SETTINGS_H__
