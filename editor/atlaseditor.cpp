#include "atlaseditor.h"
#include <Kite/core/kfistream.h>
#include <QtWidgets>

AtlasEditor::AtlasEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent):
	TabWidget(Res, KInfo, Parent),
	atlas((Kite::KAtlasTexture *)Res),
	marker(nullptr),
	gscene(nullptr)
{}

AtlasEditor::~AtlasEditor() {}

void AtlasEditor::inite() {
	auto vlayout = new QVBoxLayout(this);
	vlayout->setMargin(3);
	vlayout->setSpacing(0);

	auto hlayTools = new QHBoxLayout();
	hlayTools->setMargin(0);

	auto importMenu = new QMenu(this);

	auto fromXML = new QAction("Adobe Animate XML (Starling)", this);
	importMenu->addAction(fromXML);
	connect(fromXML, &QAction::triggered, this, &AtlasEditor::importXML);

	//auto fromJSON = new QAction("Adobe Animate JSON (non-array)", this);
	//importMenu->addAction(fromJSON);
	//connect(btnLoadImage, &QToolButton::clicked, this, &TextureEditor::loadImage);

	auto btnImport = new QToolButton(this);
	btnImport->setMenu(importMenu);
	btnImport->setText("Import Atlas From ...  ");
	btnImport->setPopupMode(QToolButton::InstantPopup);
	hlayTools->addWidget(btnImport);
	hlayTools->addStretch(1);

	vlayout->addLayout(hlayTools);
	vlayout->addSpacing(5);
	
	// view
	auto viewSplit = new QSplitter(this);
	QList<int> wsizes;

	listView = new QListWidget(this);
	listView->setAlternatingRowColors(true);
	connect(listView, &QListWidget::itemSelectionChanged, this, &AtlasEditor::listItemChange);
	viewSplit->addWidget(listView);
	wsizes.append(150);

	gview = new QGraphicsView(this);
	gview->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	gscene = new QGraphicsScene(this);
	gscene->installEventFilter(this);
	QBrush backBrush(QImage(":/back/dense"));
	backBrush.setStyle(Qt::BrushStyle::TexturePattern);
	gscene->setBackgroundBrush(backBrush);
	gview->setScene(gscene);
	sheet = gscene->addPixmap(QPixmap());
	auto tex = atlas->getTexture();
	if (tex != nullptr) {
		Kite::KImage image;
		tex->getImage(image);
		QImage qimage((const uchar *)image.getPixelsData(), image.getSize().x, image.getSize().y, QImage::Format_RGBA8888);
		sheet->setPixmap(QPixmap::fromImage(qimage));
	}
	viewSplit->addWidget(gview);
	viewSplit->setStretchFactor(1, 1);
	viewSplit->setCollapsible(1, false);
	wsizes.append(1000);

	viewSplit->setSizes(wsizes);
	vlayout->addWidget(viewSplit, 1);
	vlayout->addSpacing(3);

	auto botFrame = new QFrame(this);
	botFrame->setStyleSheet("QFrame { background-color: rgb(30,30,30); }");
	auto botHLayout = new QHBoxLayout(botFrame);
	botHLayout->setMargin(3);

	lblPos = new QLabel(botFrame);
	botHLayout->addWidget(lblPos);
	botHLayout->addStretch(1);
	vlayout->addWidget(botFrame);

	// load items
	auto items = atlas->getItemContiner();
	auto idcounter = 0;
	for (auto it = items->begin(); it != items->end(); ++it) {
		auto item = new QListWidgetItem(listView);
		auto label = new QLabel(listView);
		label->setText("<font color = \"orange\">ID: </font>" + QString::number(idcounter)
					   + "<font color = \"yellow\"> Width: </font>" + QString::number(it->width)
					   + "<font color = \"yellow\"> Height: </font>" + QString::number(it->height)
					   + "<font color = \"yellow\"> X: </font>" + QString::number(it->xpos)
					   + "<font color = \"yellow\"> Y: </font>" + QString::number(it->ypos));
		listView->addItem(item);
		listView->setItemWidget(item, label);
		++idcounter;
	}
}

bool AtlasEditor::saveChanges() {
	return true;
}

void AtlasEditor::reload() {
	// TODO
}

void AtlasEditor::importXML() {
	QString fileName = QFileDialog::getOpenFileName(this, "Import XML Sprite-sheet Data", "", "XML File (*.xml)");
	QString imagePatch;
	QVector<Kite::KAtlasItem> items;

	if (!fileName.isEmpty()) {
		QFile file(fileName);
		QFileInfo finfo(file);
		if (file.open(QIODevice::ReadOnly)) {
			
			// XML parsing
			QXmlStreamReader xml(&file);
			if (xml.readNextStartElement()) {

				// read image patch
				if (xml.name() == "TextureAtlas") {
					imagePatch = finfo.path() + "/";
					imagePatch += xml.attributes().value("imagePath").toString();
				}

				// read sub textures
				while (!xml.atEnd()) {
					xml.readNext();

					// SubTexture
					if (xml.isStartElement() && xml.name() == "SubTexture") {
						items.push_back(Kite::KAtlasItem());
						items.back().width = xml.attributes().value("width").toUInt();
						items.back().height = xml.attributes().value("height").toUInt();
						items.back().xpos = xml.attributes().value("x").toUInt();
						items.back().ypos = xml.attributes().value("y").toUInt();
					}
				}

				file.close();

				// cleanup 
				listView->clear();

				// process data
				// try to load texture
				auto tex = atlas->getTexture();
				if (tex == nullptr) {
					emit(tex = (Kite::KTexture *)requestAddRes(Kite::RTypes::Texture, QString(atlas->getName().c_str()) + ".tex"));

					if (tex == nullptr) {
						return; // proper message from resource manager displayed.
					}
				} 

				// loading image and set it to texture
				Kite::KImage image;
				Kite::KFIStream istream;
				if (!image.loadStream(istream, imagePatch.toStdString())) {
					QMessageBox msg;
					msg.setWindowTitle("Message");
					msg.setText("cant load AtlasTexture Image.\nImage Address: " + imagePatch);
					msg.exec();
					return;
				}
				tex->create(image, tex->getFilter(), tex->getWrap());
				tex->setModified(true);
				atlas->setTexture(tex);
				emit(requestReloadTab((Kite::KResource *)tex));

				// load to scene
				QImage qimage((const uchar *)image.getPixelsData(), image.getSize().x, image.getSize().y, QImage::Format_RGBA8888);
				sheet->setPixmap(QPixmap::fromImage(qimage));
				gscene->setSceneRect(0, 0, image.getSize().x, image.getSize().y);
				atlas->setModified(true);

				// add object to list
				auto idcounter = 0;
				auto aitems = atlas->getItemContiner();
				aitems->clear();
				aitems->resize(items.size());
				for (auto it = items.begin(); it != items.end(); ++it) {
					auto item = new QListWidgetItem;
					auto label = new QLabel();
					label->setText("<font color = \"orange\">ID: </font>" + QString::number(idcounter)
									+ "<font color = \"yellow\"> Width: </font>" + QString::number(it->width)
									+ "<font color = \"yellow\"> Height: </font>" + QString::number(it->height)
									+ "<font color = \"yellow\"> X: </font>" + QString::number(it->xpos)
									+ "<font color = \"yellow\"> Y: </font>" + QString::number(it->ypos));
					listView->addItem(item);
					listView->setItemWidget(item, label);

					// calculate and add items to atlas
					aitems->at(idcounter).width = it->width;
					aitems->at(idcounter).height = it->height;
					aitems->at(idcounter).xpos = it->xpos;
					aitems->at(idcounter).ypos = it->ypos;
					aitems->at(idcounter).blu = ((float)it->xpos) / (float)image.getSize().x;
					aitems->at(idcounter).blv = ((float)it->ypos) / (float)image.getSize().y;
					aitems->at(idcounter).tru = ((float)it->xpos + (float)it->width) / (float)image.getSize().x;
					aitems->at(idcounter).trv = ((float)it->ypos + (float)it->height) / (float)image.getSize().y;
					++idcounter;

				}
			}
		}
	}
}

void AtlasEditor::listItemChange() {
	auto index = listView->currentRow();
	auto items = atlas->getItemContiner();
	if (marker != nullptr) {
		gscene->removeItem(marker);
		delete marker;
	}
	marker = gscene->addRect(items->at(index).xpos, items->at(index).ypos,
				   items->at(index).width, items->at(index).height, QPen(QColor(Qt::red), 2), QBrush(QColor(0, 0, 0, 0)));

	lblPos->setText("<font color = \"orange\">ID: </font>" + QString::number(index)
					+ "<font color = \"orange\"> X: </font>" + QString::number(items->at(index).xpos)
					+ "<font color = \"orange\"> Y: </font>" + QString::number(items->at(index).ypos)
					+ "<font color = \"orange\"> Width: </font>" + QString::number(items->at(index).width)
					+ "<font color = \"orange\"> Height: </font>" + QString::number(items->at(index).height));
}

bool AtlasEditor::eventFilter(QObject *Obj, QEvent *Event) {
	if (Event->type() == QEvent::GraphicsSceneMousePress) {
		QPoint origin = gview->mapFromGlobal(QCursor::pos());
		QPointF pos = gview->mapToScene(origin);

		auto items = atlas->getItemContiner();
		auto index = 0;
		bool found = false;
		for (auto it = items->begin(); it != items->end(); ++it) {
			if (pos.x() > it->xpos && pos.y() > it->ypos 
				&& pos.x() < (it->xpos + it->width) && pos.y() < (it->ypos + it->height)) {
				found = true;
				break;
			}

			++index;
		}

		if (found) {
			listView->setCurrentRow(index);
		}
	}
	return false;
}