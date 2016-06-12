#ifndef FRMEXEORDER_H
#define FRMEXEORDER_H

#include <QDialog>
#include <Kite/core/kentity.h>

namespace Ui {
class frmexeorder;
}

class frmexeorder : public QDialog
{
    Q_OBJECT

public:
    explicit frmexeorder(Kite::KEntity *Entity, QWidget *parent = 0);
    ~frmexeorder();

private slots:
	void reorder(const QModelIndex & sourceParent, int sourceStart, int sourceEnd,
				 const QModelIndex & destinationParent, int destinationRow);

private:
	Kite::KEntity *currEnt;
    Ui::frmexeorder *ui;
};

#endif // FRMEXEORDER_H
