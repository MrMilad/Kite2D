#include "completer.h"
#include <QStringList>

Completer::Completer(QObject *parent)
    : QCompleter(parent){}

Completer::Completer(QAbstractItemModel *model, QObject *parent)
    : QCompleter(model, parent){}

QStringList Completer::splitPath(const QString &path) const{
	QRegExp rx("(\\.|\\:)");
	return path.split(rx);
}

QString Completer::pathFromIndex(const QModelIndex &index) const{
    // navigate up and accumulate data
    QStringList dataList;
    for (QModelIndex i = index; i.isValid(); i = i.parent()) {
        dataList.prepend(model()->data(i, completionRole()).toString());
    }

    return dataList.join(".");
}

