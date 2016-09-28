#include "frmnewmap.h"
#include "ui_frmnewmap.h"

frmNewMap::frmNewMap(const QStringList &Types, int Width, int Height, int TWidth, int THeight,
					 const QStringList &TileSets, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::frmNewMap),
	misok(false)
{
    ui->setupUi(this);
	setWindowTitle("Create New Map");

	ui->cmbType->addItems(Types);
	ui->cmbTileset->addItems(TileSets);

	connect(ui->spnMWidth, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &frmNewMap::showInfo);
	connect(ui->spnMHeight, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &frmNewMap::showInfo);
	connect(ui->spnTWidth, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &frmNewMap::showInfo);
	connect(ui->spnTHeight, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &frmNewMap::showInfo);

	ui->spnMWidth->setValue(Width);
	ui->spnMHeight->setValue(Height);
	ui->spnTWidth->setValue(TWidth);
	ui->spnTHeight->setValue(THeight);

	connect(ui->btnOk, &QPushButton::clicked, this, &frmNewMap::ok);
	connect(ui->btnCancel, &QPushButton::clicked, this, &frmNewMap::close);
}

frmNewMap::~frmNewMap()
{
    delete ui;
}

void frmNewMap::showInfo(int Value) {
	ui->lblInfo->setText("Map Size(Pixel): " + QString::number(ui->spnMWidth->value() * ui->spnTWidth->value())+ " x "
						 + QString::number(ui->spnMHeight->value() * ui->spnTHeight->value()) 
						 + "\tTotal Tiles: " + QString::number(ui->spnMWidth->value() * ui->spnMHeight->value()));
}

void frmNewMap::ok() {
	misok = true;
	close();
}

int frmNewMap::getWidth() const {
	return ui->spnMWidth->value();
}

int frmNewMap::getHeight() const {
	return ui->spnMHeight->value();
}

int frmNewMap::getTWidth() const {
	return ui->spnTWidth->value();
}

int frmNewMap::getTHeight() const {
	return ui->spnTHeight->value();
}

QString frmNewMap::getTileSet() const {
	return ui->cmbTileset->currentText();
}