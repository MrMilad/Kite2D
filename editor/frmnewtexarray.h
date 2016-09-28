#ifndef FRMNEWTEXARRAY_H
#define FRMNEWTEXARRAY_H

#include <QDialog>

namespace Ui {
class frmNewTexArray;
}

class frmNewTexArray : public QDialog
{
    Q_OBJECT

public:
	explicit frmNewTexArray(unsigned int Width, unsigned int Height ,QWidget *parent = 0);
    ~frmNewTexArray();

	int getWidth();
	int getHeight();

	inline bool isOk() const { return misok; }

	private slots:
	void ok();


private:
    Ui::frmNewTexArray *ui;
	bool misok;
};

#endif // FRMNEWTEXARRAY_H
