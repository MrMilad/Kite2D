#ifndef FRMNEWATLAS_H
#define FRMNEWATLAS_H

#include <QDialog>

namespace Ui {
class FrmNewAtlas;
}

class FrmNewAtlas : public QDialog
{
    Q_OBJECT

public:
    explicit FrmNewAtlas(QWidget *parent = 0);
    ~FrmNewAtlas();

	QString getImageAddress() const;
	unsigned int getCellWidth() const;
	unsigned int getCellHeight() const;

	inline bool isOk() const { return ok; }

private slots:
void browseImage();
void setOk();
void showInfo(int);

private:
	int width;
	int height;
	bool ok;
    Ui::FrmNewAtlas *ui;
};

#endif // FRMNEWATLAS_H
