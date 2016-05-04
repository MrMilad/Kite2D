#include <QtWidgets>
#include "Kite/core/kany.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/math/kmathstructs.h"

QCheckBox *checkEdit(QFormLayout *Layout, QString Label, bool defval, bool ReadOnly);

QComboBox *comboEdit(QFormLayout *Layout, QString Label, const QStringList &Items, bool ReadOnly);

QLineEdit *lineEdit(QFormLayout *Layout, QString Label, QString Text, bool ReadOnly);

template <typename T>
T * singleSpin(QFormLayout *Layout, QString Label,
						   double sp1, bool ReadOnly,
						   int Min = 0, int Max = 0, bool unsig = false) {
	auto spin1 = new T();

	spin1->setFixedWidth(70);
	spin1->setStyleSheet("color: DarkViolet;");

	// is unsigned
	if (unsig) {
		if (Min != Max && Min < Max && Min >= 0) {
			spin1->setMinimum(Min);
			spin1->setMaximum(Max);
		} else {
			spin1->setMinimum(0);
			spin1->setMaximum(INT32_MAX);
		}
	} else {
		if (Min != Max && Min < Max) {
			spin1->setMinimum(Min);
			spin1->setMaximum(Max);
		} else {
			spin1->setMinimum(INT32_MIN);
			spin1->setMaximum(INT32_MAX);
		}
	}

	spin1->setValue(sp1);

	if (ReadOnly) {
		spin1->setDisabled(true);
	}

	auto hlayout = new QHBoxLayout();
	hlayout->addWidget(new QLabel(Label));
	hlayout->addWidget(spin1, 1);
	Layout->addRow(hlayout);
	return spin1;
}

template <typename T>
QPair<T *, T *> doubleSpin(QFormLayout *Layout, QString Label, QString Sub1, QString Sub2,
						   double sp1, double sp2, bool ReadOnly,
										 int Min = 0, int Max = 0, bool unsig = false) {

	auto spin1 = new T();
	auto spin2 = new T();

	spin1->setFixedWidth(70);
	spin1->setStyleSheet("color: DarkViolet;");
	spin2->setFixedWidth(70);
	spin2->setStyleSheet("color: DarkViolet;");

	// is unsigned
	if (unsig) {
		if (Min != Max && Min < Max && Min >= 0) {
			spin1->setMinimum(Min);
			spin1->setMaximum(Max);
			spin2->setMinimum(Min);
			spin2->setMaximum(Max);
		} else {
			spin1->setMinimum(0);
			spin1->setMaximum(INT32_MAX);
			spin2->setMinimum(0);
			spin2->setMaximum(INT32_MAX);
		}
	} else {
		if (Min != Max && Min < Max) {
			spin1->setMinimum(Min);
			spin1->setMaximum(Max);
			spin2->setMinimum(Min);
			spin2->setMaximum(Max);
		} else {
			spin1->setMinimum(INT32_MIN);
			spin1->setMaximum(INT32_MAX);
			spin2->setMinimum(INT32_MIN);
			spin2->setMaximum(INT32_MAX);
		}
	}

	spin1->setValue(sp1);
	spin2->setValue(sp2);

	if (ReadOnly) {
		spin1->setDisabled(true);
		spin2->setDisabled(true);
	}

	auto hlayout = new QHBoxLayout();
	hlayout->addWidget(new QLabel(Label));
	hlayout->addWidget(new QLabel("<font color=\"DeepSkyBlue\">" + Sub1 + "</font>"), 1, Qt::AlignRight);
	hlayout->addSpacing(2);
	hlayout->addWidget(spin1, 1);
	hlayout->addSpacing(3);
	hlayout->addWidget(new QLabel("<font color=\"DeepSkyBlue\">" + Sub2 + "</font>"), 0, Qt::AlignRight);
	hlayout->addSpacing(2);
	hlayout->addWidget(spin2, 1);
	Layout->addRow(hlayout);
	return QPair<T *, T *>(spin1, spin2);
}

class ComProperty : public QObject {
	Q_OBJECT
public:
	ComProperty(QObject *Parrent, QString CName, QString CType,
				QString PName, QString PType, Kite::KHandle EHandle);

	inline void setCName(QString CName) { cname = CName; }
	inline QString getCName() const { return cname; }

	inline void setCType(QString CType) { ctype = CType; }
	inline QString getCType() const { return ctype; }

	inline void setPName(QString PName) { pname = PName; }
	inline QString getPName() const { return pname; }

	inline void setPType(QString PType) { ptype = PType; }
	inline QString getPType() const { return ptype; }

	inline void setEntity(Kite::KHandle EHandle) { entity = EHandle; }
	inline Kite::KHandle getEntity() const { return entity; }

	virtual Kite::KAny getValue() = 0;

Q_SIGNALS:
	void finishEdit();

private:
	QString cname;
	QString ctype;
	QString pname;
	QString ptype;
	Kite::KHandle entity;
};

class KV2F32 : public ComProperty {
	Q_OBJECT
public:
	KV2F32(QObject *Parrent, QString CName, QString CType,
		   QString PName, QString PType, Kite::KHandle EHandle) :
		ComProperty(Parrent, CName, CType, PName, PType, EHandle) {}

	Kite::KAny getValue() override {
		return Kite::KAny(value);
	}

public slots:
void editedX(float Val) {
	value.x = Val;
	emit(finishEdit());
}
void editedY(float Val) {
	value.y = Val;
	emit(finishEdit());
}

private:
	Kite::KVector2F32 value;
};

class KV2I32 : public ComProperty {
	Q_OBJECT
public:
	KV2I32(QObject *Parrent, QString CName, QString CType,
		   QString PName, QString PType, Kite::KHandle EHandle) :
		ComProperty(Parrent, CName, CType, PName, PType, EHandle) {}

	Kite::KAny getValue() override {
		return Kite::KAny(value);
	}

	public slots:
	void editedX(int Val) {
		value.x = Val;
		emit(finishEdit());
	}
	void editedY(int Val) {
		value.y = Val;
		emit(finishEdit());
	}

private:
	Kite::KVector2I32 value;
};

class KSTR : public ComProperty {
	Q_OBJECT
public:
	KSTR(QObject *Parrent, QString CName, QString CType,
		 QString PName, QString PType, Kite::KHandle EHandle) :
		ComProperty(Parrent, CName, CType, PName, PType, EHandle) {}

	Kite::KAny getValue() override {
		return Kite::KAny(value);
	}

	public slots:
	void editedStr(const QString & text) {
		if (text == "--EMPTY--") {
			value.clear();
		} else {
			value = text.toStdString();
		}
		emit(finishEdit());
	}

private:
	std::string value;
};

class KBOOL : public ComProperty {
	Q_OBJECT
public:
	KBOOL(QObject *Parrent, QString CName, QString CType,
		 QString PName, QString PType, Kite::KHandle EHandle) :
		ComProperty(Parrent, CName, CType, PName, PType, EHandle) {}

	Kite::KAny getValue() override {
		return Kite::KAny(value);
	}

	public slots:
	void editedBool(int val) {
		if (val == Qt::Unchecked) {
			auto obj = (QCheckBox *)sender();
			obj->setText("Off");
			value = false;
		} else if (val == Qt::Checked){
			auto obj = (QCheckBox *)sender();
			obj->setText("On");
			value = true;
		}
		emit(finishEdit());
	}

private:
	bool value;
};

class KFLT : public ComProperty {
	Q_OBJECT
public:
	KFLT(QObject *Parrent, QString CName, QString CType,
		 QString PName, QString PType, Kite::KHandle EHandle) :
		ComProperty(Parrent, CName, CType, PName, PType, EHandle) {}

	Kite::KAny getValue() override {
		return Kite::KAny(value);
	}

	public slots:
	void editedFloat(double val) {
		value = val;
		emit(finishEdit());
	}

private:
	float value;
};

class KINT : public ComProperty {
	Q_OBJECT
public:
	KINT(QObject *Parrent, QString CName, QString CType,
		  QString PName, QString PType, Kite::KHandle EHandle) :
		ComProperty(Parrent, CName, CType, PName, PType, EHandle) {}

	Kite::KAny getValue() override {
		return Kite::KAny(value);
	}

	public slots:
	void editedInt(int val) {
		value = val;
		emit(finishEdit());
	}

private:
	int value;
};
