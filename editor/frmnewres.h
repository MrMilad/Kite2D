#ifndef FRMNEWRES_H
#define FRMNEWRES_H

#include <QDialog>
#include <qstringlist.h>

namespace Ui {
class frmnewres;
}

class frmnewres : public QDialog
{
    Q_OBJECT

public:
    explicit frmnewres(const QStringList &Types, QWidget *parent = 0);
    ~frmnewres();

	QString getName() const;

	QString getType() const;

	inline bool isOk() const { return ok; }

private slots:
	void okClicked();
	void cancelClicked();

private:
    Ui::frmnewres *ui;
	bool ok;
};

#endif // FRMNEWRES_H
