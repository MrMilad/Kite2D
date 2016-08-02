#include "frmabout.h"
#include "ui_frmabout.h"
#include "Kite/core/kcoredef.h"

frmAbout::frmAbout(QWidget *parent) :
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::frmAbout)
{
    ui->setupUi(this);
	ui->lblVer->setText("ver: " +QString::number(K2D_VER_MAJ) +
						"." + QString::number(K2D_VER_MIN) +
						"." + QString::number(K2D_VER_BUILD));
}

frmAbout::~frmAbout()
{
    delete ui;
}
