#include "frmaddobj.h"
#include "ui_frmaddobj.h"
#include <qpushbutton.h>

frmAddObj::frmAddObj(const QStringList &Prefabs, bool Prefab, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::frmAddObj),
	ok(false),
	prefab(false)
{
    ui->setupUi(this);
	ui->cmbPrefab->addItems(Prefabs);
	ui->cmbPrefab->setDisabled(true);
	ui->txtObject->setPlaceholderText("Name (you can leave it empty!)");

	if (!Prefab) {
		ui->rdoPrefab->setDisabled(true);
	}
	connect(ui->rdoPrefab, &QRadioButton::toggled, this, &frmAddObj::checkType);
	connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &frmAddObj::okClicked);
	connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &frmAddObj::cancelClicked);
}

frmAddObj::~frmAddObj(){
    delete ui;
}

QString frmAddObj::objName() const {
	return ui->txtObject->text();
}

QString frmAddObj::preName() const {
	return ui->cmbPrefab->currentText();
}

void frmAddObj::checkType(bool Type) {
	prefab = Type;
	ui->cmbPrefab->setDisabled(!Type);
	ui->txtObject->setDisabled(Type);

	if (Type) {
		ui->cmbPrefab->setFocus();
	} else {
		ui->txtObject->setFocus();
	}
}

void frmAddObj::okClicked() {
	ok = true;
	this->close();
}

void frmAddObj::cancelClicked() {
	ok = false;
	this->close();
}

