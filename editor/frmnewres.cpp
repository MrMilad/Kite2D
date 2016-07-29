#include "frmnewres.h"
#include "ui_frmnewres.h"
#include "qpushbutton.h"
#include "qmessagebox.h"

frmnewres::frmnewres(const QStringList &Types, QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::frmnewres),
	ok(false)
{
    ui->setupUi(this);
	setWindowTitle("Add New Resource");

	if (Types.empty()) {
		ui->cmbType->clear();
		ui->cmbType->setDisabled(true);
	} else {
		ui->cmbType->clear();
		ui->cmbType->addItems(Types);
	}

	connect(ui->btnOk, &QPushButton::clicked, this, &frmnewres::okClicked);
	connect(ui->btnCancel, &QPushButton::clicked, this, &frmnewres::cancelClicked);
}

frmnewres::~frmnewres()
{
    delete ui;
}

QString frmnewres::getName() const {

	return ui->txtName->text();
}

QString frmnewres::getType() const {
	return ui->cmbType->currentText();
}

void frmnewres::okClicked() {
	if (ui->txtName->text().isEmpty()) {
		QMessageBox msg;
		msg.setWindowTitle("Message");
		msg.setText("resource name is empty!");
		msg.exec();

	} else {
		ok = true;
		this->close();
	}
}

void frmnewres::cancelClicked() {
	ok = false;
	this->close();
}
