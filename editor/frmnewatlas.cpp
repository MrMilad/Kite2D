#include "frmnewatlas.h"
#include "ui_frmnewatlas.h"
#include <qfiledialog.h>
#include <qimage.h>

FrmNewAtlas::FrmNewAtlas(QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::FrmNewAtlas),
	ok(false),
	width(0),
	height(0)
{
    ui->setupUi(this);
	ui->btnOk->setDisabled(true);
	ui->spnHeight->setDisabled(true);
	ui->spnWith->setDisabled(true);

	connect(ui->spnWith, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FrmNewAtlas::showInfo);
	connect(ui->spnHeight, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FrmNewAtlas::showInfo);

	connect(ui->btnBrowse, &QPushButton::clicked, this, &FrmNewAtlas::browseImage);
	connect(ui->btnOk, &QPushButton::clicked, this, &FrmNewAtlas::setOk);
	connect(ui->btnCancel, &QPushButton::clicked, this, &FrmNewAtlas::close);
}

FrmNewAtlas::~FrmNewAtlas()
{
    delete ui;
}

QString FrmNewAtlas::getImageAddress() const {
	return ui->ledPath->text();
}

unsigned int FrmNewAtlas::getCellWidth() const {
	return ui->spnWith->value();
}

unsigned int FrmNewAtlas::getCellHeight() const {
	return ui->spnHeight->value();
}

void FrmNewAtlas::browseImage() {
	QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "PNG File (*.png)");
	ui->ledPath->setText(fileName);
	if (!fileName.isEmpty()) {
		QImage image(fileName);
		width = image.width();
		height = image.height();
		ui->spnHeight->setDisabled(false);
		ui->spnWith->setDisabled(false);
		showInfo(0);
	}
}

void FrmNewAtlas::setOk() {
	ok = true;
	this->close();
}

void FrmNewAtlas::showInfo(int Value) {
	ui->lblInfo->setText("Image Size: " + QString::number(width) + " x " + QString::number(height) +
						 " \tCell Size: " + QString::number(width / ui->spnWith->value()) + " x " +
						 QString::number(height / ui->spnHeight->value()));

	if ((width / ui->spnWith->value()) > 0 && (height / ui->spnHeight->value()) > 0) {
		ui->btnOk->setDisabled(false);
	} else {
		ui->btnOk->setDisabled(true);
	}
}