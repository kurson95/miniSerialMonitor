#ifndef CSV_SAVE_H
#define CSV_SAVE_H
#include <QString>
#include <QDir>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QObject>

class csv_save:public QObject
{
public:
    csv_save(QObject *parent = nullptr);
    bool saveFile(const QString &dir, const QString &content,const QString &stop,const QString &split);
};

#endif // CSV_SAVE_H
