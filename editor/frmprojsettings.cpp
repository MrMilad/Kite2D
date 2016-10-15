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
	connect(ui->chbFScreen, &QCheckBox::stateChanged, this, &frmProjSettings::fscreenState);

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

	// render config
	ui->chbCull->setChecked(Config->render.culling);
	ui->chbCamSort->setChecked(Config->render.camDepth);
	ui->chbZSort->setChecked(Config->render.zSorting);
	ui->spnIBuff->setValue(Config->render.indexSize);
	ui->spnVBuff->setValue(config->render.vertexSize);
	ui->spnOBuff->setValue(Config->render.objectSize);
}

frmProjSettings::~frmProjSettings(){
    delete ui;
}

void frmProjSettings::okClicked() {
	// window
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
	
	// render
	config->render.culling = ui->chbCull->isChecked();
	config->render.camDepth = ui->chbCamSort->isChecked();
	config->render.zSorting = ui->chbZSort->isChecked();
	config->render.indexSize = ui->spnIBuff->value();
	config->render.vertexSize = ui->spnVBuff->value();
	config->render.objectSize = ui->spnOBuff->value();
}

void frmProjSettings::cancelClicked() {
	this->close();
}

void frmProjSettings::fscreenState(int State) {
	if (State == Qt::CheckState::Checked) {
		ui->spnWidth->setDisabled(true);
		ui->spnHeight->setDisabled(true);
		ui->spnX->setDisabled(true);
		ui->spnY->setDisabled(true);
	} else {
		ui->spnWidth->setDisabled(false);
		ui->spnHeight->setDisabled(false);
		ui->spnX->setDisabled(false);
		ui->spnY->setDisabled(false);
	}
}
