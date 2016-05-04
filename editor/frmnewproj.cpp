#include "frmnewproj.h"
#include "ui_frmnewproj.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qdir.h>

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
	if (QDir(ui->txtPath->text() + "/" + ui->txtName->text()).exists()) {
		if (QFile(ui->txtPath->text() + "/" + ui->txtName->text() + "/" + ui->txtName->text() + ".k2d").exists()) {
			int ret = QMessageBox::warning(this, "Kite2D Editor",
										   ui->txtName->text() + " already exists.\nDo you want to replace it?",
										   QMessageBox::Yes | QMessageBox::No,
										   QMessageBox::No);

			if (ret == QMessageBox::No) {
				return;
			} 
		}
	}

	QDir maindir(ui->txtPath->text() + "/" + ui->txtName->text());
	if (!maindir.exists()) {
		maindir.mkdir(".");
	}

	QDir resdir(ui->txtPath->text() + "/" + ui->txtName->text() + "/resources");
	if (!resdir.exists()) {
		resdir.mkdir(".");
	}

	QFile file(ui->txtPath->text() + "/" + ui->txtName->text() + "/" + ui->txtName->text() + ".k2d");
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
