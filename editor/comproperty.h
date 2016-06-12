#ifndef COMPROPERTY_H
#define COMPROPERTY_H

#include <qtwidgets>
#include "Kite/core/kcomponent.h"
#include "Kite/core/kany.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/math/kmathstructs.h"
#include <Kite/meta/kmetamanager.h>
#include <Kite/meta/kmetabase.h>
#include <Kite/meta/kmetaclass.h>
#include <Kite/meta/kmetapod.h>
#include <kmeta.khgen.h>

namespace priv {
	template <typename T>
	T * singleSpin(int Min = 0, int Max = 0, bool unsig = false) {
		auto spin1 = new T();

		spin1->setFixedWidth(100);
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
		return spin1;
	}

	class KProp : public QFrame {
	public:
		KProp(QWidget *Parent):
			QFrame(Parent){}
		virtual void reset(Kite::KComponent *Comp) = 0;
	};

	// bool
	class KBOOL : public KProp {
		Q_OBJECT
	public:
		KBOOL(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			 bool ROnly = false) :
			KProp(Parent), pname(PName) {
			ctype = Comp->getType().c_str();
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			auto initeVal = Comp->getProperty(PName.toStdString());
			check = new QCheckBox();
			check->setChecked(initeVal.as<bool>());
			if (check->isChecked()) {
				check->setText("On");
			} else {
				check->setText("Off");
			}

			if (ROnly) {
				check->setCheckable(false);
			}
			check->setStyleSheet("color: DarkViolet;");
			connect(check, &QCheckBox::stateChanged, this, &KBOOL::valueChanged);
			hlayout->addWidget(check);
			hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			check->setChecked(val.as<bool>());
		}

signals:
		void propertyEdited(const QString &CType, const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(bool Val) {
			if (Val) {
				check->setText("On");
			} else {
				check->setText("Off");
			}
			Kite::KAny pval(Val);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(ctype, pname, v));
		}

	private:
		QCheckBox *check;
		QString pname;
		QString ctype;
	};

	// string
	class KSTR : public KProp {
		Q_OBJECT
	public:
		KSTR(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			 bool ROnly = false, const QString &ResType = "", bool IsRes = false,
			 const QHash<QString, Kite::KResource *> *Dictionary = nullptr) :
			KProp(Parent), pname(PName), isRes(IsRes), resDict(Dictionary){
			ptype = Comp->getType().c_str();
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			auto initeVal = Comp->getProperty(PName.toStdString());
			if (!isRes) {
				ledit = new QLineEdit(Parent);
				ledit->setStyleSheet("color: orange;");
				ledit->setText(initeVal.as<std::string>().c_str());
				ledit->setReadOnly(ROnly);
				if (ROnly) {
					ledit->setStyleSheet("color: orange;"
										 "border: none;");
				}
				connect(ledit, &QLineEdit::textChanged, this, &KSTR::valueChanged);
				connect(ledit, &QLineEdit::textChanged, ledit, &QLineEdit::setToolTip);
				hlayout->addWidget(ledit);
			} else {
				combo = new QComboBox(Parent);
				combo->setCurrentText(initeVal.as<std::string>().c_str());
				combo->setObjectName(ResType);
				combo->installEventFilter(this);
				connect(combo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
						this, &KSTR::valueChanged);

				hlayout->addWidget(combo);
			}
			
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			if (isRes) {

			} else {
				ledit->setText(val.as<std::string>().c_str());
			}
		}

signals:
		void propertyEdited(const QString &PType, const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(const QString &Val) {
			Kite::KAny pval(Val.toStdString());
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(ptype, pname, v));
		}

	protected:
		bool eventFilter(QObject *Obj, QEvent *Event) {
			if (resDict != nullptr) {
				if (Event->type() == QEvent::MouseButtonPress) {
					auto text = combo->currentText();
					combo->clear();
					auto type = combo->objectName();

					if (resDict != nullptr) {
						QStringList items;
						for (auto it = resDict->cbegin(); it != resDict->cend(); ++it) {
							if ((*it)->getResourceType().c_str() == type) {
								items.push_back(it.key());
							}
						}

						combo->addItems(items);
						combo->setCurrentText(text);
					}
				}
			}
			return false;
		}

	private:
		bool isRes;
		QLineEdit *ledit;
		QComboBox *combo;
		QString pname;
		QString ptype;
		const QHash<QString, Kite::KResource *> *resDict;
	};

	// I32
	class KI32 : public KProp {
		Q_OBJECT
	public:
		KI32(Kite::KComponent *Comp, const QString &PName, const QString &Label, QWidget *Parent,
			 bool ROnly = false, int Min = 0, int Max = 0) :
			KProp(Parent), pname(PName) {
			ptype = Comp->getType().c_str();
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			if (!Label.isEmpty()) {
				auto label = new QLabel(this);
				label->setText(Label);
				label->setStyleSheet("background-color: green; color: white;"
									 "border-top-left-radius: 4px;"
									 "border-bottom-left-radius: 4px;"
									 "margin-left: 2px;"
									 "margin-right: 3px;");
				hlayout->addWidget(label);
			}

			auto initeVal = Comp->getProperty(PName.toStdString());
			spin = singleSpin<QSpinBox>(Min, Max);
			spin->setValue(initeVal.as<int>());
			spin->setReadOnly(ROnly);
			connect(spin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KI32::valueChanged);
			hlayout->addWidget(spin);

			hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			spin->setValue(val.as<int>());
		}

	signals:
		void propertyEdited(const QString &PType, const QString &PName, QVariant &Val);

	private slots :
		void valueChanged(int Val) {
			Kite::KAny pval(Val);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(ptype, pname, v));
		}

	private:
		QSpinBox *spin;
		QString pname;
		QString ptype;
	};

	// F32
	class KF32 : public KProp {
		Q_OBJECT
	public:
		KF32(Kite::KComponent *Comp, const QString &PName, const QString &Label, QWidget *Parent,
			 bool ROnly = false, int Min = 0, int Max = 0):
			KProp(Parent),pname(PName)
		{
			ptype = Comp->getType().c_str();
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);
			
			if (!Label.isEmpty()) {
				auto label = new QLabel(this);
				label->setText(Label);
				label->setStyleSheet("background-color: green; color: white;"
									 "border-top-left-radius: 4px;"
									 "border-bottom-left-radius: 4px;"
									 "margin-left: 2px;"
									 "margin-right: 3px;");
				hlayout->addWidget(label);
			}

			auto initeVal = Comp->getProperty(PName.toStdString());
			spin = singleSpin<QDoubleSpinBox>(Min, Max);
			spin->setValue(initeVal.as<float>());
			spin->setReadOnly(ROnly);
			spin->setDecimals(2);
			connect(spin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KF32::valueChanged);
			hlayout->addWidget(spin);

			hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			spin->setValue(val.as<float>());
		}

	signals:
		void propertyEdited(const QString &PType, const QString &PName, QVariant &Val);

	private slots :
		void valueChanged(double Val) {
			Kite::KAny pval(Val);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(ptype, pname, v));
		}

	private:
		QDoubleSpinBox *spin;
		QString pname;
		QString ptype;
	};

	// KVector2F32
	class KV2F32 : public KProp {
		Q_OBJECT
	public:
		KV2F32(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			 bool ROnly = false, float Min = 0, float Max = 0) :
			KProp(Parent), pname(PName) {
			ptype = Comp->getType().c_str();
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			// X
			auto xlabel = new QLabel(this);
			xlabel->setText("X");
			xlabel->setStyleSheet("background-color: green; color: white;"
									"border-top-left-radius: 4px;"
									"border-bottom-left-radius: 4px;"
									"margin-left: 2px;"
									"margin-right: 3px;");
			hlayout->addWidget(xlabel);

			auto initeVal = Comp->getProperty(PName.toStdString());
			value = initeVal.as<Kite::KVector2F32>();
			xspin = singleSpin<QDoubleSpinBox>(Min, Max);
			xspin->setValue(initeVal.as<Kite::KVector2F32>().x);
			xspin->setReadOnly(ROnly);
			xspin->setDecimals(2);
			connect(xspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KV2F32::valueXChanged);
			hlayout->addWidget(xspin);

			hlayout->addSpacing(10);

			// Y
			auto ylabel = new QLabel(this);
			ylabel->setText("Y");
			ylabel->setStyleSheet("background-color: red; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "margin-left: 2px;"
								  "margin-right: 3px;");
			hlayout->addWidget(ylabel);

			yspin = singleSpin<QDoubleSpinBox>(Min, Max);
			yspin->setValue(initeVal.as<Kite::KVector2F32>().y);
			yspin->setReadOnly(ROnly);
			yspin->setDecimals(2);
			connect(yspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KV2F32::valueYChanged);
			hlayout->addWidget(yspin);

			hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			value = val.as<Kite::KVector2F32>();
			xspin->setValue(val.as<Kite::KVector2F32>().x);
			yspin->setValue(val.as<Kite::KVector2F32>().y);
		}

signals:
		void propertyEdited(const QString &PType, const QString &PName, QVariant &Val);

		private slots :
		void valueXChanged(double Val) {
			value.x = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(ptype, pname, v));
		}

		void valueYChanged(double Val) {
			value.y = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(ptype, pname, v));
		}

	private:
		QDoubleSpinBox *xspin;
		QDoubleSpinBox *yspin;
		Kite::KVector2F32 value;
		QString pname;
		QString ptype;
	};
}

// component view
class ComponentView : public QFrame {
	Q_OBJECT
public:
	ComponentView(Kite::KComponent *Component, QWidget *Parent, const QHash<QString, Kite::KResource *> *Dictionary);

Q_SIGNALS:
	void componentEdited(Kite::KHandle Chandle, const QString &CType, const QString &Pname, QVariant &Value);
	void resetSig(Kite::KComponent *Comp);

public slots:
void reset(Kite::KComponent *Comp);

private slots:
void propChanged(const QString &CType, const QString &Pname, QVariant &Value);

private:
	static Kite::KMetaManager *getKMeta() {
		static auto kmeta = Kite::KMetaManager();
		static bool inite = false;
		if (!inite) {
			Kite::registerKiteMeta(&kmeta);
			inite = true;
		}
		return &kmeta;
	}

	void createGUI(Kite::KComponent *Comp, const Kite::KMetaProperty *Meta, QFormLayout *Layout);
	Kite::KHandle compHandle;
	const QHash<QString, Kite::KResource *> *resDict;
};
#endif // COMPROPERTY_H