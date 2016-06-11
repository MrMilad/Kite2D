#ifndef COMPLETER_H
#define COMPLETER_H

#include <QCompleter>

class Completer : public QCompleter{
    Q_OBJECT
public:
    explicit Completer(QObject *parent = 0);
    explicit Completer(QAbstractItemModel *model, QObject *parent = 0);

protected:
    QStringList splitPath(const QString &path) const Q_DECL_OVERRIDE;
    QString pathFromIndex(const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // COMPLETER_H

