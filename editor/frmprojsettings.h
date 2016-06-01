#ifndef FRMPROJSETTINGS_H
#define FRMPROJSETTINGS_H

#include <QDialog>
#include <qstringlist.h>
#include "Kite/engine/kengine.h"

namespace Ui {
class frmProjSettings;
}

class frmProjSettings : public QDialog
{
    Q_OBJECT

public:
    explicit frmProjSettings(QWidget *parent, Kite::KConfig *Config, const QStringList &Scenes);
    ~frmProjSettings();

private slots:
	void okClicked();
	void cancelClicked();

private:
	Kite::KConfig *config;
    Ui::frmProjSettings *ui;
};

#endif // FRMPROJSETTINGS_H
