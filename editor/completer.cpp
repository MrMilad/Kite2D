#include "completer.h"
#include <QStringList>

Completer::Completer(QObject *parent)
    : QCompleter(parent){}

Completer::Completer(QAbstractItemModel *model, QObject *parent)
    : QCompleter(model, parent){}

void Completer::setSeparator(const QString &separator){
    sep = separator;
}

QString Completer::separator() const
{
    return sep;
}

QStringList Completer::splitPath(const QString &path) const{
    if (sep.isNull()) {
        return QCompleter::splitPath(path);
    }

    return path.split(sep);
}

QString Completer::pathFromIndex(const QModelIndex &index) const{
    if (sep.isNull()) {
        return QCompleter::pathFromIndex(index);
    }

    // navigate up and accumulate data
    QStringList dataList;
    for (QModelIndex i = index; i.isValid(); i = i.parent()) {
        dataList.prepend(model()->data(i, completionRole()).toString());
    }

    return dataList.join(sep);
}

