#ifndef COMPLETER_H
#define COMPLETER_H

#include <QCompleter>

class Completer : public QCompleter{
    Q_OBJECT
    Q_PROPERTY(QString separator READ separator WRITE setSeparator)

public:
    explicit Completer(QObject *parent = 0);
    explicit Completer(QAbstractItemModel *model, QObject *parent = 0);

    QString separator() const;
public slots:
    void setSeparator(const QString &separator);

protected:
    QStringList splitPath(const QString &path) const Q_DECL_OVERRIDE;
    QString pathFromIndex(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QString sep;
};

#endif // COMPLETER_H

