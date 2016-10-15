#include "mapeditor.h"
#include "atlaseditor.h"
#include "Kite/core/kfistream.h"
#include "Kite/graphic/katlastexturearray.h"
#include "Kite/graphic/kgraphicdef.h"
#include <QtWidgets>
#include <qundostack.h>
#include <frmnewmap.h>
#include <qpixmap.h>
#include "gridscene.h"

MapEditor::MapEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent) :
	TabWidget(Res, KInfo, Parent),
	orthoMap((Kite::KOrthogonalMap *)Res),
	ustack(new QUndoStack(this))
{}

void MapEditor::inite() {
	ustack->setUndoLimit(50);
	auto vlayout = new QVBoxLayout(this);
	vlayout->setMargin(3);
	vlayout->setSpacing(0);

	auto toolbar = new QToolBar(this);
	toolbar->setStyleSheet("QToolBar {spacing: 3px; border: 0px ; height: 32px}");
	toolbar->setMovable(false);
	toolbar->setIconSize(QSize(16, 16));
	vlayout->addWidget(toolbar);
	vlayout->addSpacing(3);

	auto createMenu = new QMenu(this);
	auto createNew = new QAction("Create New Map", this);
	createMenu->addAction(createNew);
	connect(createNew, &QAction::triggered, this, &MapEditor::createNew);
	createMenu->addSeparator();

	auto importTmx = new QAction("Import from TMX", this);
	importTmx->setDisabled(true);
	createMenu->addAction(importTmx);

	auto btnCreate = new QToolButton(this);
	btnCreate->setMenu(createMenu);
	btnCreate->setToolTip("Create New Map ...");
	btnCreate->setIcon(QIcon(":/icons/add"));
	btnCreate->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
	toolbar->addWidget(btnCreate);
	toolbar->addSeparator();

	auto actUndo = ustack->createUndoAction(this);
	actUndo->setToolTip("Undo");
	actUndo->setIcon(QIcon(":/icons/undo"));
	actUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	this->addAction(actUndo);
	toolbar->addAction(actUndo);

	auto actRedo = ustack->createRedoAction(this);
	actRedo->setToolTip("Redo");
	actRedo->setIcon(QIcon(":/icons/redo"));
	actRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
	this->addAction(actRedo);
	toolbar->addAction(actRedo);
	toolbar->addSeparator();

	// tools
	actStamp = toolbar->addAction(QIcon(":/icons/stamp"), "Stamp Brush Tool");
	actStamp->setToolTip("Stamp Brush Tool (B)");
	actStamp->setCheckable(true);
	actStamp->setChecked(true);
	actStamp->setShortcut(QKeySequence(Qt::Key_B));
	this->addAction(actStamp);
	connect(actStamp, &QAction::toggled, this, &MapEditor::stampTool);

	actEraser = toolbar->addAction(QIcon(":/icons/clr"), "Eraser Tool");
	actEraser->setToolTip("Eraser Tool (E)");
	actEraser->setCheckable(true);
	actEraser->setShortcut(QKeySequence(Qt::Key_E));
	this->addAction(actEraser);
	connect(actEraser, &QAction::toggled, this, &MapEditor::eraserTool);
	toolbar->addSeparator();

	actSelect = toolbar->addAction(QIcon(":/icons/select"), "Selection Tool");
	actSelect->setToolTip("Selection Tool (S)");
	actSelect->setCheckable(true);
	actSelect->setShortcut(QKeySequence(Qt::Key_S));
	this->addAction(actSelect);
	connect(actSelect, &QAction::toggled, this, &MapEditor::selectTool);

	// modifiers
	auto actBlend = toolbar->addAction(QIcon(":/icons/cblend"), "Color Blend Tool");
	actBlend->setToolTip("Set Blending Color (C)");
	actBlend->setShortcut(QKeySequence(Qt::Key_C));
	this->addAction(actBlend);
	connect(actBlend, &QAction::triggered, this, &MapEditor::selectBlend);

	btnBlend = new QToolButton(this);
	btnBlend->setFixedSize(16, 16);
	btnBlend->setToolTip("Blend Color Picker");
	btnBlend->setStyleSheet("background-color: white;");
	blend = Qt::white;
	connect(btnBlend, &QToolButton::clicked, this, &MapEditor::blendColorPicker);
	toolbar->addWidget(btnBlend);

	auto actFliph = toolbar->addAction(QIcon(":/icons/fliph"), "Flip Horizontal");
	actFliph->setObjectName("fliph");
	actFliph->setToolTip("Flip Horizontal (CTRL + F)");
	actFliph->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
	this->addAction(actFliph);
	connect(actFliph, &QAction::triggered, this, &MapEditor::selectFlip);

	auto actFlipv = toolbar->addAction(QIcon(":/icons/flipv"), "Flip Vertical");
	actFlipv->setObjectName("flipv");
	actFlipv->setToolTip("Flip Vertical (F)");
	actFlipv->setShortcut(QKeySequence(Qt::Key_F));
	this->addAction(actFlipv);
	connect(actFlipv, &QAction::triggered, this, &MapEditor::selectFlip);

	auto actDuplicate = toolbar->addAction(QIcon(":/icons/dup"), "Duplicate");
	actDuplicate->setToolTip("Duplicate Selected Area (D)");
	actDuplicate->setShortcut(QKeySequence(Qt::Key_D));
	this->addAction(actDuplicate);
	toolbar->addSeparator();
	connect(actDuplicate, &QAction::triggered, this, &MapEditor::selectDuplicate);

	auto actAddLayer = toolbar->addAction(QIcon(":/icons/addl"), "Add New Layer");
	actAddLayer->setToolTip("Add New Layer (N)");
	actAddLayer->setShortcut(QKeySequence(Qt::Key_N));
	this->addAction(actAddLayer);
	connect(actAddLayer, &QAction::triggered, this, &MapEditor::addLayer);

	auto actRemLayer = toolbar->addAction(QIcon(":/icons/reml"), "Remove Last Layer");
	actRemLayer->setToolTip("Remove Last Layer");
	actRemLayer->setShortcut(QKeySequence(Qt::Key_Delete));
	connect(actRemLayer, &QAction::triggered, this, &MapEditor::removeLayer);

	auto actClearLayer = toolbar->addAction(QIcon(":/icons/clayer"), "Clear Current Layer");
	actClearLayer->setToolTip("Clear Current Layer");
	//actClearLayer->setShortcut(QKeySequence(Qt::Key_Delete));
	//actClearLayer->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	connect(actClearLayer, &QAction::triggered, this, &MapEditor::clearLayer);
	toolbar->addSeparator();

	auto actUpLayer = toolbar->addAction(QIcon(":/icons/uarrow"), "Lower Layer");
	actUpLayer->setToolTip("Lower Layer");
	//actUpLayer->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	//actUpLayer->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	connect(actUpLayer, &QAction::triggered, this, &MapEditor::lowerLayer);

	auto actDownLayer = toolbar->addAction(QIcon(":/icons/darrow"), "Raise Layer");
	actDownLayer->setToolTip("Raise Layer");
	//actDownLayer->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
	//actDownLayer->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	connect(actDownLayer, &QAction::triggered, this, &MapEditor::raiseLayer);

	toolbar->addSeparator();
	auto actGrid = toolbar->addAction(QIcon(":/icons/grid"), "Show/Hide Grid");
	actGrid->setToolTip("Show/Hide Grid");
	actGrid->setCheckable(true);
	actGrid->setChecked(true);

	gscene = new GridScene(false, false);
	gscene->installEventFilter(this);
	gscene->getView()->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	gscene->getView()->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
	connect(gscene, &GridScene::stampChanged, this, &MapEditor::manageSceneSelection);
	connect(actGrid, &QAction::toggled, gscene, &GridScene::setShowGrid);

	QList<int> wsizes;
	auto viewSplit = new QSplitter(this);
	viewSplit->addWidget(gscene->getView());
	wsizes.append(1000);

	auto hsplitter = new QSplitter(this);
	hsplitter->setOrientation(Qt::Orientation::Vertical);
	viewSplit->addWidget(hsplitter);
	wsizes.append(250);
	viewSplit->setSizes(wsizes);

	// layer list
	layerTable = new QTableWidget(this);
	layerTable->setColumnCount(1);
	layerTable->setShowGrid(false);
	layerTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Layers"));
	layerTable->horizontalHeader()->setStretchLastSection(true);
	layerTable->addAction(actRemLayer);
	layerTable->setAlternatingRowColors(true);
	layerTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	layerTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
	connect(layerTable, &QTableWidget::itemChanged, this, &MapEditor::layerItemChanged);
	hsplitter->addWidget(layerTable);
	
	// tileset
	tileset = new QTabWidget(this);
	tileset->setTabPosition(QTabWidget::TabPosition::South);
	hsplitter->addWidget(tileset);

	vlayout->addWidget(viewSplit, 1);
	vlayout->addSpacing(3);

	auto botFrame = new QFrame(this);
	botFrame->setStyleSheet("QFrame { background-color: rgb(30,30,30); }");
	auto botHLayout = new QHBoxLayout(botFrame);
	botHLayout->setMargin(3);

	lblWarning = new QLabel(this);
	lblWarning->setText("<font color = \"orange\">Tile ID:  Layers:  X:  Y:  Used Tiles: </font>");
	lblWarning->setFixedSize(512, 16);
	botHLayout->addWidget(lblWarning);
	botHLayout->addStretch(1);
	vlayout->addWidget(botFrame);

	reload();
}

bool MapEditor::saveChanges() {
	return true;
}

void MapEditor::reload() {
	disconnect(layerTable, &QTableWidget::itemChanged, this, &MapEditor::layerItemChanged);
	disconnect(tileset, &QTabWidget::currentChanged, this, &MapEditor::tilesetChanged);

	gscene->reshape(orthoMap->getMapWidth() * orthoMap->getTileWidth(), orthoMap->getMapHeight() * orthoMap->getTileHeight(),
					orthoMap->getTileWidth(), orthoMap->getTileHeight());

	// reset tileset
	tileset->clear();
	markerList.clear();

	// clear all tabs
	for (auto it = viewPool.begin(); it != viewPool.end(); ++it) {
		(*it)->clear();
		delete (*it);
	}
	viewPool.clear();
	tilesetPixmap.clear();
	layerList.clear();
	layerTable->clear();
	layerTable->setRowCount(0);
	layerTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Layers"));

	// re-create tabs
	if (orthoMap->getTileset()) {
		auto continer = orthoMap->getTileset()->getContiner();
		tilesetPixmap.reserve(continer->size());
		for (auto it = continer->begin(); it != continer->end(); ++it) {
			if ((*it)) {
				auto tscene = new GridScene(true, true);
				tscene->setPenStyle(Qt::PenStyle::SolidLine);
				tscene->setLineColor(Qt::white);
				tscene->getView()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
				tileset->addTab(tscene->getView(), (*it)->getName().c_str());
				connect(tscene, &GridScene::stampChanged, this, &MapEditor::recreateMarker);

				// create stamp
				Kite::KAtlasTexture *atlas;
				emit(atlas = (Kite::KAtlasTexture *)requestRes((*it)->getName().c_str()));

				// inite stamp image
				if (atlas) {
					auto tex = atlas->getTexture();
					if (tex) {
						Kite::KImage image;
						tex->getImage(image);
						QImage qimage(image.getPixelsData(), image.getWidth(), image.getHeight(), QImage::Format::Format_RGBA8888);

						// add pixmap to oue vector
						tilesetPixmap.push_back(QPixmap::fromImage(qimage));

						// redraw tileset
						tscene->reshape(tex->getWidth(), tex->getHeight(), orthoMap->getTileWidth(), orthoMap->getTileHeight());
						tscene->addPixmap(tilesetPixmap.back());
					}
				}

				viewPool.push_back(tscene);
			}
		}
		if (!viewPool.empty()) {
			viewPool.first()->selectTile(0);
		}

		// resolve map tiles
		std::vector<Kite::KOrthoLayer> tlist;
		for (size_t i = 0; i < orthoMap->getTilesCount(); ++i) {
			orthoMap->getTileLayers(orthoMap->convertID(i), tlist);

			for (auto it = tlist.begin(); it != tlist.end(); ++it) {
				// tile set is ready
				if (it->textureID < tilesetPixmap.size()) {
					auto mdim = orthoMap->getTileDimension(i);
					auto mainpm = new QGraphicsPixmapItem();
					mainpm->setPos(mdim.left, mdim.bottom);
					initePixmap(mainpm, tilesetPixmap[it->textureID], (*it));

					// set pixmap item pointer
					orthoMap->setScenePtr(orthoMap->convertID(i), it->layerIndex, (void *)mainpm);

					for (; layerList.size() <= it->layerIndex;) {
						auto layerGroup = new QGraphicsItemGroup();
						layerList.push_back(layerGroup);
						layerGroup->setZValue(layerList.size() - 1);
						gscene->addItem(layerGroup);
						gscene->addItem(layerList.back());

						auto name = orthoMap->getMapLayerName(layerList.size() - 1);
						auto titem = new QTableWidgetItem(name.c_str());
						titem->setCheckState(Qt::CheckState::Checked);
						layerTable->insertRow(layerTable->rowCount());
						layerTable->setItem(layerTable->rowCount() - 1, 0, titem);
						layerTable->setCurrentItem(titem);
					}
					layerList.at(it->layerIndex)->addToGroup(mainpm);

				// there is no tilset with this id
				} else {

				}
			}
		}
	}
	connect(layerTable, &QTableWidget::itemChanged, this, &MapEditor::layerItemChanged);
	connect(tileset, &QTabWidget::currentChanged, this, &MapEditor::tilesetChanged);
	gscene->update();
}

void MapEditor::onRemoveRes(Kite::RTypes Type) {
	//if (Type == Kite::RTypes::TextureGroup || Type == Kite::RTypes::AtlasTexture || Type == Kite::RTypes::Texture) {
		//reload(); // reloading entire (big) map is very expensive
	//}
}

void MapEditor::tilesetChanged(int Index) {
	recreateMarker(viewPool[Index]->getStamp()->getSelectedItems());
}

void MapEditor::createNew() {
	QStringList tlist;
	QStringList tsetList;
	QList<const Kite::KResource *> tsetResList;
	tlist.push_back("Orthogonal");

	// atlas textures
	emit(requestResList(Kite::RTypes::TextureGroup, tsetResList));
	for (auto it = tsetResList.begin(); it != tsetResList.end(); ++it) {
		tsetList.push_back((*it)->getName().c_str());
	}

	auto newMapFrm = new frmNewMap(tlist, orthoMap->getMapWidth(), orthoMap->getMapHeight(),
								   orthoMap->getTileWidth(), orthoMap->getTileHeight(), tsetList, this);
	newMapFrm->exec();
	if (newMapFrm->isOk()) {
		Kite::KAtlasTextureArray *tset;
		emit(tset = (Kite::KAtlasTextureArray *)requestRes(newMapFrm->getTileSet()));
		orthoMap->setTileset(tset);
		orthoMap->create(newMapFrm->getWidth(), newMapFrm->getHeight(), newMapFrm->getTWidth(), newMapFrm->getTHeight());
		orthoMap->setModified(true);
		ustack->clear();
		reload();
	}
	delete newMapFrm;
}

void MapEditor::initeMarkerPixmap(QPixmap *Pixmap, MarkerItem &Marker) {
	auto pix = Pixmap->copy(Marker.stamp.atlas.xpos, Marker.stamp.atlas.ypos, Marker.stamp.atlas.width, Marker.stamp.atlas.height);
	if (Marker.stamp.atlas.getFlipH()) {
		pix = pix.transformed(QTransform().scale(-1, 1));
	}

	if (Marker.stamp.atlas.getFlipV()) {
		pix = pix.transformed(QTransform().scale(1, -1));
	}

	//blend
	if (Marker.stamp.blend != Kite::KColor(Kite::Colors::WHITE) || Marker.stamp.blend.getA() < 255) {
		QColor col(Marker.stamp.blend.getR(), Marker.stamp.blend.getG(),
				   Marker.stamp.blend.getB(), Marker.stamp.blend.getA());

		auto image = pix.toImage();
		auto alpha = image.alphaChannel();
		for (int x = 0; x != image.width(); ++x) {
			for (int y(0); y != image.height(); ++y) {
				if (qAlpha(image.pixel(x, y)) == 0) continue; // transparrent pixels

				QColor icol(image.pixel(x, y));
				icol.setRed(BLEND_Multiply(icol.red(), col.red()));
				icol.setBlue(BLEND_Multiply(icol.blue(), col.blue()));
				icol.setGreen(BLEND_Multiply(icol.green(), col.green()));
				image.setPixel(x, y, icol.rgb());
			}
		}
		image.setAlphaChannel(alpha);
		Marker.gitem->setPixmap(QPixmap::fromImage(image));
	} else {
		Marker.gitem->setPixmap(pix);
	}
}

void MapEditor::recreateMarker(const std::vector<Kite::KTileStamp> *Stamp) {
	// clear 
	for (auto it = markerList.begin(); it != markerList.end(); ++it) {
		gscene->removeItem(it->gitem);
		delete (it->gitem);
	}
	markerList.clear();

	for (auto it = Stamp->cbegin(); it != Stamp->cend(); ++it) {
		short tindex = tileset->currentIndex();

		// index lower than 0 mean current texture index
		if (it->textureIndex >= 0) tindex = it->textureIndex;

		MarkerItem mitem;
		mitem.gitem = new QGraphicsPixmapItem();
		mitem.stamp = (*it);
		mitem.blend = it->blend;
		initeMarkerPixmap(&tilesetPixmap[tindex], mitem);

		mitem.gitem->setZValue(1000);
		mitem.gitem->setOpacity(0.5f);
		mitem.gitem->hide();

		gscene->addItem(mitem.gitem);
		markerList.push_back(mitem);
	}
}

void MapEditor::redrawMarker(unsigned int AnchorID, bool OutOfView) {
	if (OutOfView) {
		for (auto i = 0; i < markerList.size(); ++i) {
			markerList[i].gitem->hide();
		}
		return;
	}

	auto anchDim = orthoMap->getTileDimension(AnchorID);

	for (auto it = markerList.begin(); it != markerList.end(); ++it) {
		int left = anchDim.left + (int)(it->stamp.col * orthoMap->getTileWidth());
		int bottom = anchDim.bottom - (int)(it->stamp.row * orthoMap->getTileHeight());
		it->gitem->setPos(left, bottom);

		// culling 
		if (left >= orthoMap->getMapWidthPixel() || bottom >= orthoMap->getMapHeightPixel()) {
			it->gitem->hide();
		} else {
			it->gitem->show();
		}
	}
}

void MapEditor::blendColorPicker() {
	auto tcolor = QColorDialog::getColor(blend, this, "Color Picker", QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
	if (tcolor.isValid()) {
		blend = tcolor;
		btnBlend->setStyleSheet("QToolButton { background-color: rgb("
								+ QString::number(blend.red()) + ", "
								+ QString::number(blend.green()) + ", "
								+ QString::number(blend.blue()) + "); }");
	}
}

void MapEditor::selectBlend() {
	if (actSelect->isChecked()
		&& !gscene->getStamp()->getSelectedItems()->empty()
		&& layerTable->currentRow() >= 0) {
		std::vector<Kite::KOrthoLayer> layerList;
		orthoMap->getTilesLayer(orthoMap->convertID(gscene->getStamp()->getAnchorID()), layerTable->currentRow(), *gscene->getStamp()->getSelectedItems(), layerList);
		if (!layerList.empty()) {
			ustack->push(new BlendColorCMD(gscene, orthoMap, &tilesetPixmap,
										   gscene->getStamp()->getAnchorID(), layerTable->currentRow(),
										   blend, *gscene->getStamp()->getSelectedItems()));
		}
	}
}

void MapEditor::selectFlip() {
	auto senderObj = (QAction *)sender();
	if (actSelect->isChecked() 
		&& !gscene->getStamp()->getSelectedItems()->empty()
		&& layerTable->currentRow() >= 0) 
	{
		std::vector<Kite::KOrthoLayer> tempList;
		orthoMap->getTilesLayer(orthoMap->convertID(gscene->getStamp()->getAnchorID()),
								layerTable->currentRow(), *gscene->getStamp()->getSelectedItems(), tempList);
		if (!tempList.empty()) {
			if (senderObj->objectName() == "fliph") {
				ustack->push(new FlipCMD(gscene, orthoMap, &tilesetPixmap,
										  gscene->getStamp()->getAnchorID(), layerTable->currentRow(),
										  *gscene->getStamp()->getSelectedItems(), true));
			} else {
				ustack->push(new FlipCMD(gscene, orthoMap, &tilesetPixmap,
										 gscene->getStamp()->getAnchorID(), layerTable->currentRow(),
										 *gscene->getStamp()->getSelectedItems(), false));
			}
		}
	}
}

void MapEditor::selectDuplicate() {
	if (!gscene->getStamp()->getSelectedItems()->empty()
		&& layerTable->currentRow() >= 0) {

		auto slist = gscene->getStamp()->getSelectedItems();
		static std::vector<Kite::KTileStamp> markerList;
		markerList.clear();
		Kite::KOrthoLayer layer;
		for (auto it = slist->cbegin(); it != slist->cend(); ++it) {
			if (orthoMap->getTileLayer(orthoMap->convertID(it->atlas.id), layerTable->currentRow(), layer)) {
				markerList.push_back((*it));
				markerList.back().atlas = layer.atlas;
				markerList.back().textureIndex = layer.textureID;
				markerList.back().blend = layer.blend;
			}
		}

		if (!markerList.empty()) {
			recreateMarker(&markerList);
			actStamp->setChecked(true);
		}
	}
}

void MapEditor::stampTool(bool Check) {
	if (Check) {
		if (actSelect->isChecked()) actSelect->setChecked(false);
		if (actEraser->isChecked()) actEraser->setChecked(false);
		
	} else {
		redrawMarker(0, true);
	}
}

void MapEditor::selectTool(bool check) {
	if (check) {
		if (actStamp->isChecked()) actStamp->setChecked(false);
		if (actEraser->isChecked()) actEraser->setChecked(false);
		gscene->setSelectable(true);
		gscene->setShowAnchor(true);
	} else {
		gscene->setSelectable(false);
		gscene->setShowAnchor(false);
	}
}

void MapEditor::eraserTool(bool Check) {
	if (Check) {
		if (actStamp->isChecked()) actStamp->setChecked(false);
		if (actSelect->isChecked()) actSelect->setChecked(false);
	}
}

void MapEditor::addLayer() {
	ustack->push(new AddLayerCMD(gscene, orthoMap, layerTable, &layerList));
}

void MapEditor::removeLayer() {
	if (layerTable->rowCount() > 0) {
		ustack->push(new RemoveLayerCMD(gscene, orthoMap, &tilesetPixmap, &layerList, layerTable));
	}
}

void MapEditor::clearLayer() {
	if (layerTable->currentRow() >= 0) {
		ustack->push(new ClearLayerCMD(gscene, orthoMap, &tilesetPixmap, &layerList, layerTable->currentRow()));
	}
}

void MapEditor::lowerLayer() {
	if (layerTable->currentRow() > 0) {
		ustack->push(new LowerLayerCMD(gscene, orthoMap, &layerList, layerTable, layerTable->currentRow()));
	}
}

void MapEditor::raiseLayer() {
	if (layerTable->currentRow() < (layerTable->rowCount() - 1)) {
		ustack->push(new RaiseLayerCMD(gscene, orthoMap, &layerList, layerTable, layerTable->currentRow()));
	}
}

void MapEditor::layerItemChanged(QTableWidgetItem * Item) {
	if (Item->checkState() == Qt::CheckState::Unchecked) {
		layerList.at(layerTable->row(Item))->setVisible(false);

	} else if (Item->checkState() == Qt::CheckState::Checked) {
		layerList.at(layerTable->row(Item))->setVisible(true);
	}

	if (layerTable->currentRow() >= 0) {
		orthoMap->setMapLayerName(layerTable->currentRow(), layerTable->currentItem()->text().toStdString());
	}
}

void MapEditor::manageSceneSelection(const std::vector<Kite::KTileStamp> *Stamp) {
	if (actStamp->isChecked()) {
		selectDuplicate();
		gscene->setSelectable(false);
	}


	if (actEraser->isChecked()) {
		if (layerTable->currentRow() >= 0
			&& !gscene->getStamp()->getSelectedItems()->empty()) {

			std::vector<Kite::KOrthoLayer> tempList;
			orthoMap->getTilesLayer(orthoMap->convertID(gscene->getStamp()->getAnchorID()),
									layerTable->currentRow(), *gscene->getStamp()->getSelectedItems(), tempList);

			if (!tempList.empty()) {
				ustack->push(new EraserCMD(gscene, orthoMap, &tilesetPixmap,
										   &layerList, gscene->getStamp()->getAnchorID(),
										   layerTable->currentRow(), *gscene->getStamp()->getSelectedItems()));
			}
		}
		gscene->setSelectable(false);
	}
}

bool MapEditor::eventFilter(QObject *Obj, QEvent *Event) {
	static int lastID = -1;
	static bool stamping = false;
	static bool erasing = false;
	static std::vector<Kite::KOrthoLayer> tileList;
	if (Event->type() == QEvent::GraphicsSceneMouseMove) {
		QPoint origin = gscene->getView()->mapFromGlobal(QCursor::pos());
		QPointF pos = gscene->getView()->mapToScene(origin);
		Kite::U32 tileID;
		if (orthoMap->getTileID(Kite::KVector2F32(pos.x(), pos.y()), tileID)) {
			if (lastID != tileID) {
				auto dim = orthoMap->getTileDimension(orthoMap->convertID(tileID));
				// tile info
				// layer
				QString layer("empty");
				if (orthoMap->getTileLayerSize(orthoMap->convertID(tileID)) > 0) layer = QString::number(orthoMap->getTileLayerSize(orthoMap->convertID(tileID)));
				lblWarning->setText("<font color = \"orange\">Tile ID: </font>" + QString::number(orthoMap->convertID(tileID)) +
									"<font color = \"orange\"> Layers: </font>" + layer +
									"<font color = \"orange\"> X: </font>" + QString::number(dim.left) +
									"<font color = \"orange\"> Y: </font>" + QString::number(dim.bottom));
									//"<font color = \"orange\"> Used Layers: </font>" + QString::number(orthoMap->getTotalUsedLayers()));
	
				orthoMap->getTileLayers(orthoMap->convertID(tileID), tileList);

				// blend color
				/*if (spnLayer->value() < tileList.size()) {
					auto tileColor = &tileList[spnLayer->value()].blend;
					blend = QColor(tileColor->getR(), tileColor->getG(), tileColor->getB(), tileColor->getA());

				} else {
					blend = Qt::white;
				}
				btnBlend->setStyleSheet("QToolButton { background-color: rgb("
										+ QString::number(blend.red()) + ", "
										+ QString::number(blend.green()) + ", "
										+ QString::number(blend.blue()) + "); }");*/

				// actions
				if (actStamp->isChecked()) redrawMarker(tileID, false);

				// stamp
				if (stamping) {
						ustack->push(new PushStampLayerCMD(gscene, orthoMap, &tilesetPixmap, &layerList, tileID,
														   tileset->currentIndex(), layerTable->currentRow(), markerList));
				}

				// erase
				if (erasing) {
					Kite::KOrthoLayer ltemp;
					if (orthoMap->getTileLayer(orthoMap->convertID(tileID), layerTable->currentRow(), ltemp)) {
						std::vector<Kite::KTileStamp> slist(1, gscene->getStamp()->getStamp(tileID));
						ustack->push(new EraserCMD(gscene, orthoMap, &tilesetPixmap,
												   &layerList, gscene->getStamp()->getAnchorID(), layerTable->currentRow(), slist));
					}
				}
			}

			lastID = tileID;
		} else {

			lastID = -1;
			// actions
			if (actStamp->isChecked()) redrawMarker(0, true);
		}
	} else if (Event->type() == QEvent::Leave) {
		lastID = -1;

		// actions
		if (actStamp->isChecked()) redrawMarker(0, true);

	} else if (Event->type() == QEvent::GraphicsSceneMousePress) {
		if (QGuiApplication::mouseButtons() == Qt::LeftButton) {
			QPoint origin = gscene->getView()->mapFromGlobal(QCursor::pos());
			QPointF pos = gscene->getView()->mapToScene(origin);
			Kite::U32 tileID;
			if (orthoMap->getTileID(Kite::KVector2F32(pos.x(), pos.y()), tileID)) {

				// stamp
				if (actStamp->isChecked() 
					&& !viewPool.at(tileset->currentIndex())->getStamp()->getSelectedItems()->empty()
					&& layerTable->currentRow() >= 0){
					ustack->push(new PushStampLayerCMD(gscene, orthoMap, &tilesetPixmap, &layerList, tileID,
														 tileset->currentIndex(), layerTable->currentRow(),
														markerList));
					stamping = true;
				}

				// erase
				if (actEraser->isChecked()) {
					Kite::KOrthoLayer ltemp;
					std::vector<Kite::KTileStamp> slist(1, gscene->getStamp()->getStamp(tileID));
					if (orthoMap->getTileLayer(orthoMap->convertID(tileID), layerTable->currentRow(), ltemp)) {
						ustack->push(new EraserCMD(gscene, orthoMap, &tilesetPixmap,
												   &layerList, gscene->getStamp()->getAnchorID(), layerTable->currentRow(), slist));
					}
					erasing = true;
				}

			}

		// selecting
		} else if (QGuiApplication::mouseButtons() == Qt::RightButton) {
			if (actStamp->isChecked() || actEraser->isChecked()) {
				gscene->setSelectable(true);
			}
		}
	} else if (Event->type() == QEvent::GraphicsSceneMouseRelease) {
		stamping = false;
		erasing = false;
	}
	return false;
}