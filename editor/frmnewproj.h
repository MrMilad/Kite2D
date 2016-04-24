#ifndef FRMNEWPROJ_H
#define FRMNEWPROJ_H

#include <QDialog>

namespace Ui {
class frmNewProj;
}

class frmNewProj : public QDialog
{
    Q_OBJECT

public:
    explicit frmNewProj(QWidget *parent = 0);
    ~frmNewProj();

	inline bool isOk() const { return misok; }
	QString getName();
	QString getPath();

private:
    Ui::frmNewProj *ui;
	bool misok;

private slots:
	void authorizeForm();
	void browseFolder();
	void cancel();
	void ok();
};

#endif // FRMNEWPROJ_H
