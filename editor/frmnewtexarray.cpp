#include "frmnewtexarray.h"
#include "ui_frmnewtexarray.h"

frmNewTexArray::frmNewTexArray(unsigned int Width, unsigned int Height, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::frmNewTexArray),
	misok(false)
{
    ui->setupUi(this);
	ui->spnWidth->setValue(Width);
	ui->spnHeight->setValue(Height);
	
	setWindowTitle("Create New Texture Group");
	connect(ui->btnOk, &QPushButton::clicked, this, &frmNewTexArray::ok);
	connect(ui->btnCancel, &QPushButton::clicked, this, &frmNewTexArray::close);
}

frmNewTexArray::~frmNewTexArray()
{
    delete ui;
}

void frmNewTexArray::ok() {
	misok = true;
	close();
}

int frmNewTexArray::getWidth() {
	return ui->spnWidth->value();
}

int frmNewTexArray::getHeight() {
	return ui->spnHeight->value();
}