#include "atlaseditor.h"
#include <Kite/core/kfistream.h>
#include <QtWidgets>
#include "frmnewatlas.h"

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

	auto fromImage = new QAction("Create New Atlas Texture", this);
	importMenu->addAction(fromImage);
	connect(fromImage, &QAction::triggered, this, &AtlasEditor::createNew);
	importMenu->addSeparator();

	auto fromXML = new QAction("Import from Adobe Animate XML (Starling)", this);
	importMenu->addAction(fromXML);
	connect(fromXML, &QAction::triggered, this, &AtlasEditor::importXML);

	auto fromJSON = new QAction("Import from Adobe Animate JSON (non-array)", this);
	fromJSON->setDisabled(true);
	importMenu->addAction(fromJSON);
	//connect(btnLoadImage, &QToolButton::clicked, this, &TextureEditor::loadImage);

	auto btnImport = new QToolButton(this);
	btnImport->setMenu(importMenu);
	btnImport->setToolTip("Create new Atlas");
	btnImport->setIcon(QIcon(":/icons/add"));
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
	wsizes.append(50);

	gview = new QGraphicsView(this);
	gview->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	gscene = new QGraphicsScene(this);
	gscene->installEventFilter(this);
	QBrush backBrush(QImage(":/back/dense"));
	backBrush.setStyle(Qt::BrushStyle::TexturePattern);
	gscene->setBackgroundBrush(backBrush);
	gview->setScene(gscene);
	sheet = gscene->addPixmap(QPixmap());


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

	reload();
}

bool AtlasEditor::saveChanges() {
	return true;
}

void AtlasEditor::reload() {
	if (atlas->getTexture()) {
		Kite::KImage image;
		gscene->setSceneRect(0, 0, image.getWidth(), image.getHeight());
		atlas->getTexture()->getImage(image);
		QImage qimage((const uchar *)image.getPixelsData(), image.getWidth(), image.getHeight(), QImage::Format_RGBA8888);
		sheet->setPixmap(QPixmap::fromImage(qimage));
	} else {
		atlas->setTexture(nullptr);
		atlas->getContiner()->clear();
		sheet->setPixmap(QPixmap(0,0));
		gscene->setSceneRect(0, 0, 0, 0);
		delete marker;
		marker = nullptr;
	}

	// load items
	disconnect(listView, &QListWidget::itemSelectionChanged, this, &AtlasEditor::listItemChange);
	listView->clear();
	auto items = atlas->getContiner();
	auto idcounter = 0;
	for (auto it = items->begin(); it != items->end(); ++it) {
		listView->addItem("ID: " + QString::number(idcounter));
		++idcounter;
	}
	connect(listView, &QListWidget::itemSelectionChanged, this, &AtlasEditor::listItemChange);
}

void AtlasEditor::onRemoveRes(Kite::RTypes Type) {
	if (Type == Kite::RTypes::Texture) {
		reload();
	}
}

void AtlasEditor::createNew() {
	auto newFrm = new FrmNewAtlas();
	newFrm->exec();

	if (newFrm->isOk()) {
		auto imagePatch = newFrm->getImageAddress();
		// try to load texture
		Kite::KTexture *tex = atlas->getTexture();
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
		atlas->setModified(true);
		emit(requestReloadTab((Kite::KResource *)tex));

		// add object to list
		auto aitems = atlas->getContiner();
		aitems->clear();
		aitems->reserve((image.getWidth() / newFrm->getCellWidth()) * (image.getHeight() / newFrm->getCellHeight()));
		for (auto i = 0; i < (image.getHeight() / newFrm->getCellHeight()); ++i) {
			for (auto j = 0; j < (image.getWidth() / newFrm->getCellWidth()); ++j) {
				Kite::KAtlasItem item;
				item.id = aitems->size();
				// calculate and add items to atlas
				item.width = newFrm->getCellWidth();
				item.height = newFrm->getCellHeight();
				item.xpos = j * newFrm->getCellWidth();
				item.ypos = i * newFrm->getCellHeight();
				item.blu = ((float)item.xpos) / (float)image.getWidth();
				item.blv = ((float)item.ypos) / (float)image.getHeight();
				item.tru = ((float)item.xpos + (float)item.width) / (float)image.getWidth();
				item.trv = ((float)item.ypos + (float)item.height) / (float)image.getHeight();
				aitems->push_back(item);
			}
		}

		reload();
	}

	delete newFrm;
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

				// process data
				// try to load texture
				Kite::KTexture *tex = atlas->getTexture();
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
				atlas->setModified(true);
				emit(requestReloadTab((Kite::KResource *)tex));

				// add object to list
				auto idcounter = 0;
				auto aitems = atlas->getContiner();
				aitems->clear();
				aitems->resize(items.size());
				for (auto it = items.begin(); it != items.end(); ++it) {
					// calculate and add items to atlas
					aitems->at(idcounter).id = idcounter;
					aitems->at(idcounter).width = it->width;
					aitems->at(idcounter).height = it->height;
					aitems->at(idcounter).xpos = it->xpos;
					aitems->at(idcounter).ypos = it->ypos;
					aitems->at(idcounter).blu = ((float)it->xpos) / (float)image.getWidth();
					aitems->at(idcounter).blv = ((float)it->ypos) / (float)image.getHeight();
					aitems->at(idcounter).tru = ((float)it->xpos + (float)it->width) / (float)image.getWidth();
					aitems->at(idcounter).trv = ((float)it->ypos + (float)it->height) / (float)image.getHeight();
					++idcounter;

				}
			}
		}
		reload();
		emit(requestReloadRes(Kite::RTypes::TextureGroup));
	}

}

void AtlasEditor::listItemChange() {
	auto index = listView->currentRow();
	auto items = atlas->getContiner();
	if (marker != nullptr) {
		gscene->removeItem(marker);
		delete marker;
	}
	marker = gscene->addRect(items->at(index).xpos, items->at(index).ypos,
				   items->at(index).width, items->at(index).height, QPen(QColor(Qt::red), 2, Qt::PenStyle::DashLine), QBrush(QColor(0, 0, 0, 0)));

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

		auto items = atlas->getContiner();
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