#ifndef FRMADDOBJ_H
#define FRMADDOBJ_H

#include <QDialog>
#include <qstringlist.h>

namespace Ui {
class frmAddObj;
}

class frmAddObj : public QDialog
{
    Q_OBJECT

public:
    explicit frmAddObj(const QStringList &Prefabs, bool Prefab, QWidget *parent = 0);
    ~frmAddObj();

	inline bool isOk() const { return ok; }

	inline bool isPrefab() const { return prefab; }

	QString objName() const;

	QString preName() const;
private:
	void checkType(bool Type);
	void okClicked();
	void cancelClicked();

	bool ok;
	bool prefab;
    Ui::frmAddObj *ui;
};

#endif // FRMADDOBJ_H
