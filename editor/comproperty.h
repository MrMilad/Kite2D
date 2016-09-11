#ifndef COMPROPERTY_H
#define COMPROPERTY_H

#include <qtwidgets>
#include "Kite/core/kcomponent.h"
#include "Kite/core/kany.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/katlastexture.h"
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

		spin1->setMinimumWidth(30);

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
		KProp(QWidget *Parent) :
			QFrame(Parent) {}
		virtual void reset(Kite::KComponent *Comp) = 0;
	};

	// enum
	class KENUM : public KProp {
		Q_OBJECT
	public:
		KENUM(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			  const QStringList &Items, bool ROnly = false) :
			KProp(Parent), pname(PName) {
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			auto initeVal = Comp->getProperty(PName.toStdString());
			combo = new QComboBox();
			combo->addItems(Items);
			combo->setCurrentIndex(initeVal.asFreeCast<int>());

			if (ROnly) {
				combo->setDisabled(true);
			}
			connect(combo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &KENUM::valueChanged);
			hlayout->addWidget(combo);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			combo->setCurrentIndex(val.asFreeCast<int>());
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(int Val) {
			Kite::KAny pval(Val, true);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QComboBox *combo;
		QString pname;
	};

	// color
	class KCOLOR : public KProp {
		Q_OBJECT
	public:
		KCOLOR(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			   bool ROnly = false) :
			KProp(Parent), pname(PName) {
			ronly = ROnly;
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			auto initeVal = Comp->getProperty(PName.toStdString()).as<Kite::KColor>();
			color.setRgb(initeVal.getR(), initeVal.getG(), initeVal.getB(), initeVal.getA());
			btnBrowse = new QToolButton(this);
			btnBrowse->setAutoRaise(true);
			btnBrowse->setFixedHeight(24);
			btnBrowse->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
			btnBrowse->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextOnly);
			btnBrowse->setText("");
			btnBrowse->setStyleSheet("QToolButton { border: 2px solid black;\n"
									 "border-radius: 4px;\n"
									 "background-color: rgb("
									 + QString::number(initeVal.getR()) + ", "
									 + QString::number(initeVal.getG()) + ", "
									 + QString::number(initeVal.getB()) + "); }");
			connect(btnBrowse, &QToolButton::clicked, this, &KCOLOR::valueChanged);
			hlayout->addWidget(btnBrowse, 1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
		}

		void reset(Kite::KComponent *Comp) override {
			auto initeVal = Comp->getProperty(pname.toStdString()).as<Kite::KColor>();
			color.setRgb(initeVal.getR(), initeVal.getG(), initeVal.getB(), initeVal.getA());
			btnBrowse->setStyleSheet("QToolButton { border: 2px solid black;\n"
									 "border-radius: 4px;\n"
									 "background-color: rgb("
									 + QString::number(initeVal.getR()) + ", "
									 + QString::number(initeVal.getG()) + ", "
									 + QString::number(initeVal.getB()) + "); }");
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueChanged() {
			if (!ronly) {
				color = QColorDialog::getColor(color, this, "Color Picker", QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
				if (color.isValid()) {
					Kite::KAny pval(Kite::KColor(color.red(), color.green(), color.blue(), color.alpha()));
					QVariant v = qVariantFromValue((void *)&pval);
					btnBrowse->setStyleSheet("QToolButton { border: 2px solid black;\n"
											 "border-radius: 4px;\n"
											 "background-color: rgb("
											 + QString::number(color.red()) + ", "
											 + QString::number(color.green()) + ", "
											 + QString::number(color.blue()) + "); }");

					emit(propertyEdited(pname, v));
				}
			}
		}

	private:
		bool ronly;
		QToolButton *btnBrowse;
		QColor color;
		QString pname;
	};

	// bool
	class KBOOL : public KProp {
		Q_OBJECT
	public:
		KBOOL(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			  bool ROnly = false) :
			KProp(Parent), pname(PName) {
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
			//check->setStyleSheet("color: DarkViolet;");
			connect(check, &QCheckBox::stateChanged, this, &KBOOL::valueChanged);
			hlayout->addWidget(check);
			hlayout->addStretch(1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			check->setChecked(val.as<bool>());
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(bool Val) {
			if (Val) {
				check->setText("On");
			} else {
				check->setText("Off");
			}
			Kite::KAny pval(Val);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QCheckBox *check;
		QString pname;
	};

	// atlas item
	class KATLASITEM : public KProp {
		Q_OBJECT
	public:
		KATLASITEM(Kite::KComponent *Comp, const QString &PName, QWidget *Parent, bool ROnly = false) :
			KProp(Parent), pname(PName), fh(false), fv(false){

			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			auto initeVal = Comp->getProperty(PName.toStdString());
			currItem = initeVal.as<Kite::KAtlasItem>();

			combo = new QComboBox(Parent);
			combo->setCurrentIndex(refillCombo(currItem));
			combo->installEventFilter(this);
			if (ROnly) {
				combo->setDisabled(true);
			}
			connect(combo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &KATLASITEM::valueChanged);

			hlayout->addWidget(combo, 1);

			// fliph
			btnFliph = new QToolButton(Parent);
			btnFliph->setIcon(QIcon(":/icons/fliph"));
			btnFliph->setAutoRaise(true);
			btnFliph->setToolTip("Flip Horizontal");
			btnFliph->setCheckable(true);
			btnFliph->setChecked(currItem.getFlipH());
			fh = currItem.getFlipH();
			hlayout->addWidget(btnFliph);
			connect(btnFliph, &QToolButton::clicked, this, &KATLASITEM::fliph);

			// flipv
			btnFlipv = new QToolButton(Parent);
			btnFlipv->setIcon(QIcon(":/icons/flipv"));
			btnFlipv->setAutoRaise(true);
			btnFlipv->setToolTip("Flip Vertical");
			btnFlipv->setCheckable(true);
			btnFlipv->setChecked(currItem.getFlipV());
			fv = currItem.getFlipV();
			hlayout->addWidget(btnFlipv);
			connect(btnFlipv, &QToolButton::clicked, this, &KATLASITEM::flipv);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}

		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			currItem = val.as<Kite::KAtlasItem>();
			disconnect(combo, 0, 0, 0);
			disconnect(btnFlipv, 0, 0, 0);
			disconnect(btnFliph, 0, 0, 0);

			btnFliph->setChecked(currItem.getFlipH());
			btnFlipv->setChecked(currItem.getFlipV());
			fv = currItem.getFlipV();
			fh = currItem.getFlipH();

			combo->setCurrentIndex(refillCombo(currItem));
			connect(combo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &KATLASITEM::valueChanged);

			connect(btnFlipv, &QToolButton::clicked, this, &KATLASITEM::flipv);
			connect(btnFliph, &QToolButton::clicked, this, &KATLASITEM::fliph);
		}

	signals:
		void propertyEdited(const QString &PName, QVariant &Val);
		Kite::KResource *requestRes(const QString &Name);
		Kite::KAny requestPropValue(Kite::CTypes Type, const QString &ComName, const QString &PropName);

	private slots :
		void valueChanged(int Index) {
			if (items != nullptr) {
				btnFliph->setChecked(false);
				btnFlipv->setChecked(false);
				fv = false;
				fh = false;
				currItem = items->at(Index);
				Kite::KAny pval(currItem);
				QVariant v = qVariantFromValue((void *)&pval);
				emit(propertyEdited(pname, v));
			}
		}

		void fliph(bool Flip) {
			if (items != nullptr) {
				currItem = items->at(combo->currentIndex());
				if (Flip) currItem.flipH();
				if (fv) currItem.flipV();
				fh = Flip;
				Kite::KAny pval(currItem);
				QVariant v = qVariantFromValue((void *)&pval);
				emit(propertyEdited(pname, v));
			}
		}

		void flipv(bool Flip) {
			if (items != nullptr) {
				currItem = items->at(combo->currentIndex());
				if (Flip) currItem.flipV();
				if (fh) currItem.flipH();
				fv = Flip;
				Kite::KAny pval(currItem);
				QVariant v = qVariantFromValue((void *)&pval);
				emit(propertyEdited(pname, v));
			}
		}

	protected:
		bool eventFilter(QObject *Obj, QEvent *Event) {
			static QStringList resList;
			if (Event->type() == QEvent::MouseButtonPress) {
				disconnect(combo, 0, 0, 0);
				combo->setCurrentIndex(refillCombo(currItem));
				connect(combo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
						this, &KATLASITEM::valueChanged);
			}
			return false;
		}

	private:
		// return current matched item after refill
		unsigned int refillCombo(Kite::KAtlasItem CurrItem) {
			// set atlas attributes(flip) to default
			if (CurrItem.getFlipH()) CurrItem.flipH();
			if (CurrItem.getFlipV()) CurrItem.flipV();

			// get atlas items
			Kite::KAny atlasName;
			Kite::KAtlasTexture *atlas = nullptr;
			items = nullptr;
			auto currIndex = 0;
			combo->clear();
			emit(atlasName = requestPropValue(Kite::CTypes::RenderMaterial, "", "atlasTexture"));
			if (!atlasName.is_null()) {
				emit(atlas = (Kite::KAtlasTexture *)requestRes(atlasName.as<Kite::KStringID>().str.c_str()));
				if (atlas != nullptr) {
					items = atlas->getItemContiner();
				}

				auto idcounter = 0;
				if (items != nullptr) {
					for (auto it = items->begin(); it != items->end(); ++it) {
						// add items to list
						combo->addItem("ID: " + QString::number(idcounter));

						// find current item index
						if ((*it) == CurrItem) {
							currIndex = idcounter;
						}
						++idcounter;
					}
				}
			}
			return currIndex;
		}
		QToolButton *btnFliph;
		QToolButton *btnFlipv;
		QLineEdit *ledit;
		QComboBox *combo;
		QString pname;
		Kite::KAtlasItem currItem;
		std::vector<Kite::KAtlasItem> *items;
		bool fh, fv;
	};

	// string id
	class KSTRID : public KProp {
		Q_OBJECT
	public:
		KSTRID(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			   Kite::RTypes ResType, bool ROnly = false) :
			KProp(Parent), pname(PName), restype(ResType) {

			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			auto initeVal = Comp->getProperty(PName.toStdString());
			if (restype == Kite::RTypes::maxSize) {
				ledit = new QLineEdit(Parent);
				ledit->setStyleSheet("color: orange;");
				ledit->setText(initeVal.as<Kite::KStringID>().str.c_str());
				ledit->setReadOnly(ROnly);
				if (ROnly) {
					ledit->setStyleSheet("color: orange;"
										 "border: none;");
				}
				connect(ledit, &QLineEdit::textChanged, this, &KSTRID::valueChanged);
				connect(ledit, &QLineEdit::textChanged, ledit, &QLineEdit::setToolTip);
				hlayout->addWidget(ledit, 1);
			} else {
				combo = new QComboBox(Parent);
				combo->addItem(initeVal.as<Kite::KStringID>().str.c_str());
				combo->setCurrentText(initeVal.as<Kite::KStringID>().str.c_str());
				combo->installEventFilter(this);
				if (ROnly) {
					combo->setDisabled(true);
				}
				connect(combo, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
						this, &KSTRID::valueChanged);

				hlayout->addWidget(combo, 1);
			}

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}

		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			combo->clear();
			if (restype != Kite::RTypes::maxSize) {
				if (val.as<Kite::KStringID>().str.empty()) {
					combo->addItem("");
					combo->setCurrentText("");
				} else {
					combo->addItem(val.as<Kite::KStringID>().str.c_str());
					combo->setCurrentText(val.as<Kite::KStringID>().str.c_str());
				}
			} else {
				ledit->setText(val.as<Kite::KStringID>().str.c_str());
			}
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);
		void updateResList(Kite::RTypes Type, QStringList &List);

		private slots :
		void valueChanged(const QString &Val) {
			Kite::KAny pval(Kite::KStringID(Val.toStdString()));
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	protected:
		bool eventFilter(QObject *Obj, QEvent *Event) {
			static QStringList resList;
			if (Event->type() == QEvent::MouseButtonPress) {
				auto text = combo->currentText();
				combo->clear();
				auto type = combo->objectName();

				emit(updateResList(restype, resList));
				combo->addItem(""); // empty res
				combo->addItems(resList);
				combo->setCurrentText(text);
			}
			return false;
		}

	private:
		QLineEdit *ledit;
		QComboBox *combo;
		QString pname;
		Kite::RTypes restype;
	};

	// string
	class KSTR : public KProp {
		Q_OBJECT
	public:
		KSTR(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			 bool ROnly = false) :
			KProp(Parent), pname(PName) {
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			auto initeVal = Comp->getProperty(PName.toStdString());
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
			hlayout->addWidget(ledit, 1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}

		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			ledit->setText(val.as<std::string>().c_str());
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(const QString &Val) {
			Kite::KAny pval(Val.toStdString());
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QLineEdit *ledit;
		QString pname;
	};

	// I32
	class KI32 : public KProp {
		Q_OBJECT
	public:
		KI32(Kite::KComponent *Comp, const QString &PName, const QString &Label, QWidget *Parent,
			 bool ROnly = false, int Min = 0, int Max = 0) :
			KProp(Parent), pname(PName) {
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			if (!Label.isEmpty()) {
				auto label = new QLabel(this);
				label->setText(Label);
				label->setStyleSheet("background-color: green; color: white;"
									 "border-top-left-radius: 4px;"
									 "border-bottom-left-radius: 4px;"
									 "border-bottom-right-radius: 0px;"
									 "margin-left: 2px;"
									 "margin-right: 3px;");
				hlayout->addWidget(label);
			}

			auto initeVal = Comp->getProperty(PName.toStdString());
			spin = singleSpin<QSpinBox>(Min, Max);
			spin->setValue(initeVal.asFreeCast<int>());
			spin->setReadOnly(ROnly);
			connect(spin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KI32::valueChanged);
			hlayout->addWidget(spin, 1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			spin->setValue(val.asFreeCast<int>());
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(int Val) {
			Kite::KAny pval(Val, true);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QSpinBox *spin;
		QString pname;
	};

	// F32
	class KF32 : public KProp {
		Q_OBJECT
	public:
		KF32(Kite::KComponent *Comp, const QString &PName, const QString &Label, QWidget *Parent,
			 bool ROnly = false, int Min = 0, int Max = 0) :
			KProp(Parent), pname(PName), slider(nullptr) {
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			if (!Label.isEmpty()) {
				auto label = new QLabel(this);
				label->setText(Label);
				label->setStyleSheet("background-color: green; color: white;"
									 "border-top-left-radius: 4px;"
									 "border-bottom-left-radius: 4px;"
									 "border-bottom-right-radius: 0px;"
									 "margin-left: 2px;"
									 "margin-right: 3px;");
				hlayout->addWidget(label);
			}

			auto initeVal = Comp->getProperty(PName.toStdString());

			// spin
			spin = singleSpin<QDoubleSpinBox>(Min, Max);
			spin->setValue(initeVal.as<float>());
			spin->setReadOnly(ROnly);
			spin->setDecimals(2);
			spin->setSingleStep(0.1);
			connect(spin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KF32::valueChanged);
			hlayout->addWidget(spin, 1);

			// slider
			if (Min != Max) {
				hlayout->addSpacing(5);
				slider = new QSlider();
				slider->setRange(Min * 100, Max * 100);
				slider->setOrientation(Qt::Orientation::Horizontal);
				slider->setValue((initeVal.as<float>() * 100));
				connect(slider, &QSlider::valueChanged, this, &KF32::floatSlider);
				hlayout->addWidget(slider, 1);
			}

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			spin->setValue(val.asFreeCast<float>());
			if (slider != nullptr) {
				slider->setValue(val.asFreeCast<float>() * 100);
			}
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(double Val) {
			if (slider != nullptr) {
				slider->setValue(Val * 100);
			}
			Kite::KAny pval((float)Val, true);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

		void floatSlider(int Val) {
			float tf = Val / 100.0f;
			spin->setValue(tf);
			Kite::KAny pval(tf);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QSlider *slider;
		QDoubleSpinBox *spin;
		QString pname;
	};

	// KVector2I32
	class KV2I32 : public KProp {
		Q_OBJECT
	public:
		KV2I32(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			   bool ROnly = false, int Min = 0, int Max = 0) :
			KProp(Parent), pname(PName) {
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			// X
			auto xlabel = new QLabel(this);
			xlabel->setText("X");
			xlabel->setStyleSheet("QLabel {background-color: green; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			hlayout->addWidget(xlabel);

			auto initeVal = Comp->getProperty(PName.toStdString());
			value = initeVal.as<Kite::KVector2I32>();
			xspin = singleSpin<QSpinBox>(Min, Max);
			xspin->setValue(initeVal.as<Kite::KVector2I32>().x);
			xspin->setReadOnly(ROnly);
			xspin->setSingleStep(1);
			if (Min != Max){
				xspin->setMinimum(Min);
				xspin->setMaximum(Max);
			}
			connect(xspin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KV2I32::valueXChanged);
			hlayout->addWidget(xspin, 1);

			hlayout->addSpacing(5);

			// Y
			auto ylabel = new QLabel(this);
			ylabel->setText("Y");
			ylabel->setStyleSheet("QLabel {background-color: red; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			hlayout->addWidget(ylabel);

			yspin = singleSpin<QSpinBox>(Min, Max);
			yspin->setValue(initeVal.as<Kite::KVector2I32>().y);
			yspin->setReadOnly(ROnly);
			yspin->setSingleStep(1);
			if (Min != Max) {
				yspin->setMinimum(Min);
				yspin->setMaximum(Max);
			}
			connect(yspin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KV2I32::valueYChanged);
			hlayout->addWidget(yspin, 1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
			//hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			value = val.as<Kite::KVector2I32>();
			xspin->setValue(val.as<Kite::KVector2I32>().x);
			yspin->setValue(val.as<Kite::KVector2I32>().y);
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueXChanged(int Val) {
			value.x = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

		void valueYChanged(int Val) {
			value.y = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QSpinBox *xspin;
		QSpinBox *yspin;
		Kite::KVector2I32 value;
		QString pname;
	};

	// KVector2U32
	class KV2U32 : public KProp {
		Q_OBJECT
	public:
		KV2U32(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			   bool ROnly = false, unsigned int Min = 0, unsigned int Max = 0) :
			KProp(Parent), pname(PName) {
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			// X
			auto xlabel = new QLabel(this);
			xlabel->setText("X");
			xlabel->setStyleSheet("QLabel {background-color: green; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			hlayout->addWidget(xlabel);

			auto initeVal = Comp->getProperty(PName.toStdString());
			value = initeVal.as<Kite::KVector2U32>();
			xspin = singleSpin<QSpinBox>(Min, Max);
			xspin->setValue(initeVal.as<Kite::KVector2U32>().x);
			xspin->setReadOnly(ROnly);
			xspin->setSingleStep(1);
			xspin->setMinimum(0);
			connect(xspin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KV2U32::valueXChanged);
			hlayout->addWidget(xspin, 1);

			hlayout->addSpacing(5);

			// Y
			auto ylabel = new QLabel(this);
			ylabel->setText("Y");
			ylabel->setStyleSheet("QLabel {background-color: red; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			hlayout->addWidget(ylabel);

			yspin = singleSpin<QSpinBox>(Min, Max);
			yspin->setValue(initeVal.as<Kite::KVector2U32>().y);
			yspin->setReadOnly(ROnly);
			yspin->setSingleStep(1);
			yspin->setMinimum(0);
			connect(yspin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KV2U32::valueYChanged);
			hlayout->addWidget(yspin, 1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
			//hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			value = val.as<Kite::KVector2U32>();
			xspin->setValue(val.as<Kite::KVector2U32>().x);
			yspin->setValue(val.as<Kite::KVector2U32>().y);
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueXChanged(int Val) {
			value.x = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

		void valueYChanged(int Val) {
			value.y = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QSpinBox *xspin;
		QSpinBox *yspin;
		Kite::KVector2U32 value;
		QString pname;
	};

	// KVector2F32
	class KV2F32 : public KProp {
		Q_OBJECT
	public:
		KV2F32(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			   bool ROnly = false, float Min = 0, float Max = 0) :
			KProp(Parent), pname(PName) {
			auto hlayout = new QHBoxLayout(this);
			hlayout->setMargin(0);
			hlayout->setSpacing(0);

			// X
			auto xlabel = new QLabel(this);
			xlabel->setText("X");
			xlabel->setStyleSheet("QLabel {background-color: green; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			hlayout->addWidget(xlabel);

			auto initeVal = Comp->getProperty(PName.toStdString());
			value = initeVal.as<Kite::KVector2F32>();
			xspin = singleSpin<QDoubleSpinBox>(Min, Max);
			xspin->setValue(initeVal.as<Kite::KVector2F32>().x);
			xspin->setReadOnly(ROnly);
			xspin->setDecimals(2);
			xspin->setSingleStep(0.1);
			connect(xspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KV2F32::valueXChanged);
			hlayout->addWidget(xspin, 1);

			hlayout->addSpacing(5);

			// Y
			auto ylabel = new QLabel(this);
			ylabel->setText("Y");
			ylabel->setStyleSheet("QLabel {background-color: red; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			hlayout->addWidget(ylabel);

			yspin = singleSpin<QDoubleSpinBox>(Min, Max);
			yspin->setValue(initeVal.as<Kite::KVector2F32>().y);
			yspin->setReadOnly(ROnly);
			yspin->setDecimals(2);
			yspin->setSingleStep(0.1);
			connect(yspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KV2F32::valueYChanged);
			hlayout->addWidget(yspin, 1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				hlayout->addSpacing(2);
				hlayout->addWidget(lblROnly);
			}
			//hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			value = val.as<Kite::KVector2F32>();
			xspin->setValue(val.as<Kite::KVector2F32>().x);
			yspin->setValue(val.as<Kite::KVector2F32>().y);
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueXChanged(double Val) {
			value.x = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

		void valueYChanged(double Val) {
			value.y = Val;
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QDoubleSpinBox *xspin;
		QDoubleSpinBox *yspin;
		Kite::KVector2F32 value;
		QString pname;
	};

	// KRectF32
	class KRF32 : public KProp {
		Q_OBJECT
	public:
		KRF32(Kite::KComponent *Comp, const QString &PName, QWidget *Parent,
			   bool ROnly = false) :
			KProp(Parent), pname(PName) {

			auto vlayout = new QVBoxLayout();
			vlayout->setMargin(0);
			vlayout->setSpacing(0);

			// left right
			auto lrlayout = new QHBoxLayout();
			lrlayout->setMargin(0);
			lrlayout->setSpacing(0);

			// bottom top
			auto btlayout = new QHBoxLayout();
			btlayout->setMargin(0);
			btlayout->setSpacing(0);

			// left
			auto leftlbl = new QLabel(this);
			leftlbl->setText("L");
			leftlbl->setStyleSheet("QLabel {background-color: #7e7e7e; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			lrlayout->addWidget(leftlbl);

			auto initeVal = Comp->getProperty(PName.toStdString());
			value = initeVal.as<Kite::KRectF32>();
			leftspin = singleSpin<QDoubleSpinBox>();
			leftspin->setValue(value.left);
			leftspin->setReadOnly(ROnly);
			leftspin->setDecimals(2);
			leftspin->setSingleStep(0.1);
			leftspin->setToolTip("Left");
			connect(leftspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KRF32::valueChanged);
			lrlayout->addWidget(leftspin, 1);
			lrlayout->addSpacing(5);

			// bottom
			auto botlbl = new QLabel(this);
			botlbl->setText("B");
			botlbl->setStyleSheet("QLabel {background-color: #7e7e7e; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			btlayout->addWidget(botlbl);

			botspin = singleSpin<QDoubleSpinBox>();
			botspin->setValue(value.bottom);
			botspin->setReadOnly(ROnly);
			botspin->setDecimals(2);
			botspin->setSingleStep(0.1);
			botspin->setToolTip("Bottom");
			connect(botspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KRF32::valueChanged);
			btlayout->addWidget(botspin, 1);
			btlayout->addSpacing(5);

			// right
			auto rightlbl = new QLabel(this);
			rightlbl->setText("R");
			rightlbl->setStyleSheet("QLabel {background-color: #7e7e7e; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			lrlayout->addWidget(rightlbl);

			rightspin = singleSpin<QDoubleSpinBox>();
			rightspin->setValue(value.left);
			rightspin->setReadOnly(ROnly);
			rightspin->setDecimals(2);
			rightspin->setSingleStep(0.1);
			rightspin->setToolTip("Right");
			connect(rightspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KRF32::valueChanged);
			lrlayout->addWidget(rightspin, 1);

			// top
			auto toplbl = new QLabel();
			toplbl->setText("T");
			toplbl->setStyleSheet("QLabel {background-color: #7e7e7e; color: white;"
								  "border-top-left-radius: 4px;"
								  "border-bottom-left-radius: 4px;"
								  "border-bottom-right-radius: 0px;"
								  "margin-left: 2px;}");
			btlayout->addWidget(toplbl);

			topspin = singleSpin<QDoubleSpinBox>();
			topspin->setValue(value.right);
			topspin->setReadOnly(ROnly);
			topspin->setDecimals(2);
			topspin->setSingleStep(0.1);
			topspin->setToolTip("Top");
			connect(topspin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &KRF32::valueChanged);
			btlayout->addWidget(topspin, 1);

			vlayout->addLayout(lrlayout);
			vlayout->addLayout(btlayout);

			auto mainlayout = new QHBoxLayout(this);
			mainlayout->setSpacing(0);
			mainlayout->setMargin(0);
			mainlayout->addLayout(vlayout, 1);

			if (ROnly) {
				auto lblROnly = new QLabel(this);
				lblROnly->setText("<img src=\":/icons/lock\" height=\"12\" width=\"12\" >");
				lblROnly->setToolTip("Read-Only Property");
				mainlayout->addSpacing(2);
				mainlayout->addWidget(lblROnly);
			}

			//hlayout->addStretch(1);
		}

		void reset(Kite::KComponent *Comp) override {
			auto val = Comp->getProperty(pname.toStdString());
			value = val.as<Kite::KRectF32>();
			topspin->setValue(val.as<Kite::KRectF32>().top);
			botspin->setValue(val.as<Kite::KRectF32>().bottom);
			leftspin->setValue(val.as<Kite::KRectF32>().left);
			rightspin->setValue(val.as<Kite::KRectF32>().right);
		}

signals:
		void propertyEdited(const QString &PName, QVariant &Val);

		private slots :
		void valueChanged(double Val) {
			value.top = topspin->value();
			value.bottom = botspin->value();
			value.left = leftspin->value();
			value.right = rightspin->value();
			Kite::KAny pval(value);
			QVariant v = qVariantFromValue((void *)&pval);
			emit(propertyEdited(pname, v));
		}

	private:
		QDoubleSpinBox *topspin;
		QDoubleSpinBox *botspin;
		QDoubleSpinBox *leftspin;
		QDoubleSpinBox *rightspin;
		Kite::KRectF32 value;
		QString pname;
	};
}

// component view
class ComponentView : public QFrame {
	Q_OBJECT
public:
	ComponentView(Kite::KComponent *Component, QWidget *Parent);

Q_SIGNALS:
	void componentEdited(Kite::KHandle CHandle, const QString &Pname, QVariant &Value);
	void resetSig(Kite::KComponent *Comp);
	void updateResList(Kite::RTypes Type, QStringList &List);
	Kite::KResource *requestRes(const QString &Name);
	Kite::KAny requestPropValue(Kite::CTypes Type, const QString &ComName, const QString &PropName);

public slots:
void reset(Kite::KComponent *Comp);

private slots:
void propChanged(const QString &Pname, QVariant &Value);

private:
	static Kite::KMetaManager *getKMeta() {
		static Kite::KMetaManager kmeta;
		static bool inite = false;
		if (!inite) {
			Kite::registerKiteMeta(&kmeta);
			inite = true;
		}
		return &kmeta;
	}

	void createPOD(Kite::KComponent *Comp, const Kite::KMetaProperty *Meta, QFormLayout *Layout);
	void createEnum(Kite::KComponent *Comp, const Kite::KMetaProperty *PMeta, const Kite::KMetaEnum *EMeta, QFormLayout *Layout);
	Kite::KHandle compHandle;
};
#endif // COMPROPERTY_H