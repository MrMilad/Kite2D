#include "comproperty.h"
#include <qobject.h>

QCheckBox *checkEdit(QFormLayout *Layout, QString Label, bool defval, bool ReadOnly) {
	auto check = new QCheckBox();
	check->setChecked(defval);
	if (defval) {
		check->setText("On");
	} else {
		check->setText("Off");
	}

	if (ReadOnly) {
		check->setCheckable(false);
	}
	check->setLayoutDirection(Qt::RightToLeft);
	check->setStyleSheet("color: DarkViolet;");
	Layout->addRow(Label, check);
	return check;
}

ComProperty::ComProperty(QObject *Parrent, QString CName, QString CType,
						 QString PName, QString PType, Kite::KHandle EHandle) :
	QObject(Parrent), cname(CName), ctype(CType),
	pname(PName), ptype(PType), entity(EHandle){}

QComboBox *comboEdit(QFormLayout *Layout, QString Label, const QStringList &Items, bool ReadOnly) {
	auto combo = new QComboBox();
	combo->addItems(Items);

	if (ReadOnly) {
		combo->setEditable(false);
	}

	Layout->addRow(Label, combo);
	return combo;
}

QLineEdit *lineEdit(QFormLayout *Layout, QString Label, QString Text, bool ReadOnly) {
	auto line = new QLineEdit();
	line->setText(Text);
	line->setStyleSheet("color: DarkViolet;");
	line->setToolTip(line->text());

	if (ReadOnly) {
		line->setDisabled(true);
		line->setStyleSheet("border: none; color: DarkViolet;");
	}
	QObject::connect(line, &QLineEdit::textChanged, line, &QLineEdit::setToolTip);
	Layout->addRow(Label, line);
	return line;
}


