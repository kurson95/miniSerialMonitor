#include "csv_save.h"

csv_save::csv_save(QObject *parent): QObject(parent) {}
bool csv_save::saveFile(const QString &dir, const QString &content,const QString &stop,const QString &split){
    QFile file(dir);

    // Attempt to open the file in WriteOnly mode (overwrites existing content)
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for writing:" << dir;
        return false;
    }

    QTextStream out(&file);
    QStringList lines = content.split(stop);


    for (const QString &line : lines) {
        if (!line.trimmed().isEmpty()) {
            if (line.contains(split)) {
                out << "\"" << line << "\"\n";
            } else {
                out << line << "\n";
            }
        }
    }

    file.close();

    return true;
}
