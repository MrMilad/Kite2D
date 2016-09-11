#include "textureeditor.h"
#include <qboxlayout.h>
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qevent.h>
#include <qscrollarea.h>
#include <qfiledialog.h>
#include <qcolordialog.h>

TextureEditor::TextureEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent):
	TabWidget(Res, KInfo, Parent),
	texture((Kite::KTexture *)Res)
{}

void TextureEditor::inite() {
	Kite::KImage image;
	texture->getImage(image);

	auto vlayout = new QVBoxLayout(this);
	vlayout->setMargin(3);
	vlayout->setSpacing(0);

	auto hlayTools = new QHBoxLayout();
	hlayTools->setMargin(0);

	auto btnLoadImage = new QToolButton(this);
	btnLoadImage->setText("Browse Image");
	btnLoadImage->setToolTip("Create Texture From Image");
	hlayTools->addWidget(btnLoadImage);
	connect(btnLoadImage, &QToolButton::clicked, this, &TextureEditor::loadImage);
	
	hlayTools->addSpacing(5);
	auto sep1 = new QLabel(this);
	sep1->setText("|");
	sep1->setStyleSheet("color: yellow");
	hlayTools->addWidget(sep1);
	hlayTools->addSpacing(5);

	auto btnSelectColor = new QToolButton(this);
	btnSelectColor->setText("Select Color");
	btnSelectColor->setToolTip("Create Texture With Color");
	hlayTools->addWidget(btnSelectColor);
	hlayTools->addSpacing(3);
	connect(btnSelectColor, &QToolButton::clicked, this, &TextureEditor::createColor);

	auto lblx = new QLabel(this);
	lblx->setText("Width: ");
	hlayTools->addWidget(lblx);
	spinX = new QSpinBox(this);
	spinX->setMinimum(0);
	spinX->setMaximum(10000);
	spinX->setValue(image.getSize().x);
	spinX->setDisabled(true);
	connect(spinX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &TextureEditor::resizeWidth);
	hlayTools->addWidget(spinX);
	hlayTools->addSpacing(3);

	auto lbly = new QLabel(this);
	lbly->setText("Height: ");
	hlayTools->addWidget(lbly);
	spinY = new QSpinBox(this);
	spinY->setMinimum(0);
	spinY->setMaximum(10000);
	spinY->setValue(image.getSize().y);
	spinY->setDisabled(true);
	connect(spinY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			this, &TextureEditor::resizeHeight);
	hlayTools->addWidget(spinY);

	hlayTools->addSpacing(5);
	auto sep2 = new QLabel(this);
	sep2->setText("|");
	sep2->setStyleSheet("color: yellow");
	hlayTools->addWidget(sep2);
	hlayTools->addSpacing(5);

	auto lblFilter = new QLabel(this);
	lblFilter->setText("Filter: ");
	hlayTools->addWidget(lblFilter);
	cmbFilter = new QComboBox(this);
	cmbFilter->addItem("Nearest");
	cmbFilter->addItem("Linear");
	cmbFilter->setCurrentIndex((int)texture->getFilter());
	hlayTools->addWidget(cmbFilter);
	hlayTools->addSpacing(5);
	
	auto lblWrap = new QLabel(this);
	lblWrap->setText("Wrap: ");
	hlayTools->addWidget(lblWrap);
	cmbWrap = new QComboBox(this);
	cmbWrap->addItem("Repeat");
	cmbWrap->addItem("Mirrored Repeat");
	cmbWrap->addItem("Clamp to Edge");
	cmbWrap->setCurrentIndex((int)texture->getWrap());
	hlayTools->addWidget(cmbWrap);

	hlayTools->addStretch(1);
	vlayout->addLayout(hlayTools);
	vlayout->addSpacing(5);

	lblImageView = new QLabel(this);
	lblImageView->setStyleSheet("QLabel { background-image: url(:/back/dense);\n"
								"background-repeat: repeat-xy;}");
	lblImageView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	lblImageView->setScaledContents(true);
	lblImageView->setMouseTracking(true);
	lblImageView->installEventFilter(this);

	QImage qimage((const uchar *)image.getPixelsData(), image.getSize().x, image.getSize().y, QImage::Format_RGBA8888);
	lblImageView->setPixmap(QPixmap().fromImage(qimage));
	lblImageView->resize(lblImageView->sizeHint());
	
	auto scrollArea = new QScrollArea(this);
	scrollArea->setWidget(lblImageView);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	scrollArea->setMouseTracking(true);
	scrollArea->installEventFilter(this);
	vlayout->addWidget(scrollArea);

	auto botFrame = new QFrame(this);
	botFrame->setStyleSheet("QFrame { background-color: rgb(30,30,30); }");
	auto botHLayout = new QHBoxLayout(botFrame);
	botHLayout->setMargin(3);

	lblPos = new QLabel(botFrame);
	lblPos->setText("x: <font color = \"orange\">0</font> y: <font color = \"orange\">0</font>");
	botHLayout->addWidget(lblPos);
	botHLayout->addStretch(1);
	vlayout->addWidget(botFrame);
	
	//vlayout->addStretch(1);
}

bool TextureEditor::saveChanges() {
	Kite::TextureFilter filter = (Kite::TextureFilter)cmbFilter->currentIndex();
	Kite::TextureWrap wrap = (Kite::TextureWrap)cmbWrap->currentIndex();

	// save image pixels
	Kite::KImage image;
	image.create(lblImageView->pixmap()->size().width(), lblImageView->pixmap()->size().height(),
				 lblImageView->pixmap()->toImage().convertToFormat(QImage::Format_RGBA8888).bits());

	texture->create(image, filter, wrap);
	texture->setModified(true);
	return true;
}

void TextureEditor::reload() {
	Kite::KImage image;
	texture->getImage(image);

	spinX->setValue(image.getSize().x);
	spinX->setDisabled(true);

	spinY->setValue(image.getSize().y);
	spinY->setDisabled(true);

	cmbFilter->setCurrentIndex((int)texture->getFilter());
	cmbWrap->setCurrentIndex((int)texture->getWrap());

	QImage qimage((const uchar *)image.getPixelsData(), image.getSize().x, image.getSize().y, QImage::Format_RGBA8888);
	lblImageView->setPixmap(QPixmap().fromImage(qimage));
	lblImageView->resize(lblImageView->sizeHint());
}

bool TextureEditor::eventFilter(QObject *Obj, QEvent *Event) {
	if (Event->type() == QEvent::MouseMove) {
		auto mevent = (QMouseEvent *)Event;
		lblPos->setText("x: <font color = \"orange\">" + QString::number(mevent->pos().x()) 
						+ "</font> y: <font color = \"orange\">" + QString::number(mevent->pos().y()) + "</font>");
	}
	return false;
}

void TextureEditor::loadImage() {
	QString fileName = QFileDialog::getOpenFileName(this, "Open Image File", "", "Image Files (*.png *.bmp *.tga)");

	if (!fileName.isEmpty()) {
		QPixmap image(fileName);
		spinX->setDisabled(true);
		spinY->setDisabled(true);
		spinX->setValue(image.size().width());
		spinY->setValue(image.size().height());
		lblImageView->setPixmap(image);
		lblImageView->resize(lblImageView->sizeHint());
		texture->setModified(true);
	}
}

void TextureEditor::createColor() {
	QPixmap pmap(spinX->value(), spinY->value());
	color = QColorDialog::getColor();
	if (color.isValid()) {
		pmap.fill(color);
		spinX->setDisabled(false);
		spinY->setDisabled(false);
		lblImageView->setPixmap(pmap);
		lblImageView->resize(lblImageView->sizeHint());
		texture->setModified(true);
	}
}

void TextureEditor::resizeWidth(int Width) {
	QPixmap pmap(Width, spinY->value());
	pmap.fill(color);
	lblImageView->setPixmap(pmap);
	lblImageView->resize(lblImageView->sizeHint());
	texture->setModified(true);
}

void TextureEditor::resizeHeight(int Height) {
	QPixmap pmap(spinX->value(), Height);
	pmap.fill(color);
	lblImageView->setPixmap(pmap);
	lblImageView->resize(lblImageView->sizeHint());
	texture->setModified(true);
}