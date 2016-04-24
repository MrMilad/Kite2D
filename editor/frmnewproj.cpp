#include "frmnewproj.h"
#include "ui_frmnewproj.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

frmNewProj::frmNewProj(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::frmNewProj),
	misok(false)
{
    ui->setupUi(this);
	ui->btnOk->setEnabled(false);

	connect(ui->txtName, &QLineEdit::textChanged, this, &frmNewProj::authorizeForm);
	connect(ui->txtPath, &QLineEdit::textChanged, this, &frmNewProj::authorizeForm);

	connect(ui->btnBrowse, &QPushButton::clicked, this, &frmNewProj::browseFolder);
	connect(ui->btnCancel, &QPushButton::clicked, this, &frmNewProj::cancel);
	connect(ui->btnOk, &QPushButton::clicked, this, &frmNewProj::ok);
}

frmNewProj::~frmNewProj()
{
    delete ui;
}

QString frmNewProj::getName() {
	return ui->txtName->text();
}

QString frmNewProj::getPath() {
	return ui->txtPath->text();
}

void frmNewProj::authorizeForm() {
	bool name = false;
	bool path = false;

	if (!ui->txtName->text().isEmpty()) {
		name = true;
	}

	if (!ui->txtPath->text().isEmpty()) {
		path = true;
	}

	if (name && path) {
		ui->btnOk->setEnabled(true);
	} else {
		ui->btnOk->setEnabled(false);
	}
}

void frmNewProj::browseFolder() {
	QFileDialog fd;
	fd.setFileMode(QFileDialog::Directory);
	fd.setOption(QFileDialog::ShowDirsOnly);
	fd.setViewMode(QFileDialog::Detail);
	if (fd.exec()) {
		ui->txtPath->setText(fd.selectedFiles()[0]);
	}
}

void frmNewProj::cancel() {
	misok = false;
	this->close();
}

void frmNewProj::ok() {
	QFile file(ui->txtPath->text() + "/" + ui->txtName->text() + ".k2d");
	if (file.open(QIODevice::ReadWrite)) {
		misok = true;
		file.close();
		this->close();
		return;
	}

	QMessageBox msg;
	msg.setWindowTitle("Error");
	msg.setText("Incorrect project name\\directory or limited permission to read and write.");
	msg.exec();
}
