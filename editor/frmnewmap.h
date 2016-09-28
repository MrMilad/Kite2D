#ifndef FRMNEWMAP_H
#define FRMNEWMAP_H

#include <QDialog>
#include <qstringlist.h>

namespace Ui {
class frmNewMap;
}

class frmNewMap : public QDialog
{
    Q_OBJECT

public:
    explicit frmNewMap(const QStringList &Types, int Width, int Height, int TWidth, int THeight,
					   const QStringList &TileSets, QWidget *parent = 0);
    ~frmNewMap();

	int getWidth() const;
	int getHeight() const;
	int getTWidth() const;
	int getTHeight() const;
	QString getTileSet() const;

	inline bool isOk() const { return misok; }

private slots:
void showInfo(int Value);
void ok();

private:
    Ui::frmNewMap *ui;
	bool misok;
};

#endif // FRMNEWMAP_H
