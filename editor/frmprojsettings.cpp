#include "frmprojsettings.h"
#include "ui_frmprojsettings.h"
#include "qpushbutton.h"

frmProjSettings::frmProjSettings(QWidget *parent, Kite::KConfig *Config, const QStringList &Scenes) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint), config(Config),
    ui(new Ui::frmProjSettings)
{
    ui->setupUi(this);
	setWindowTitle("Project Settings");

	connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &frmProjSettings::okClicked);
	connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &frmProjSettings::cancelClicked);

	// window config
	ui->txtTitle->setText(Config->window.title.c_str());
	ui->spnWidth->setValue(Config->window.width);
	ui->spnHeight->setValue(Config->window.height);
	ui->spnX->setValue(Config->window.xpos);
	ui->spnY->setValue(Config->window.ypos);
	ui->chbFScreen->setChecked(Config->window.fullscreen);
	ui->chbCorsor->setChecked(Config->window.showCursor);
	ui->chbResize->setChecked(Config->window.resizable);
	ui->cmbScene->addItems(Scenes);
	if (Config->startUpScene.empty()) {
		ui->cmbScene->setCurrentText("<default>");
	} else {
		ui->cmbScene->setCurrentText(Config->startUpScene.c_str());
	}
	
}

frmProjSettings::~frmProjSettings(){
    delete ui;
}

void frmProjSettings::okClicked() {
	config->window.title = ui->txtTitle->text().toStdString();
	config->window.width = ui->spnWidth->value();
	config->window.height = ui->spnHeight->value();
	config->window.xpos = ui->spnX->value();
	config->window.ypos = ui->spnY->value();
	config->window.fullscreen = ui->chbFScreen->isChecked();
	config->window.showCursor = ui->chbCorsor->isChecked();
	config->window.resizable = ui->chbResize->isChecked();
	if (ui->cmbScene->currentText() == "<default>") {
		config->startUpScene = "";
	} else {
		config->startUpScene = ui->cmbScene->currentText().toStdString();
	}
	
}

void frmProjSettings::cancelClicked() {
	this->close();
}
