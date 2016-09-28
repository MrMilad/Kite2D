#include "mapeditor.h"
#include "atlaseditor.h"
#include "mapeditorcmd.h"
#include "Kite/core/kfistream.h"
#include "Kite/graphic/katlastexturearray.h"
#include <QtWidgets>
#include <qundostack.h>
#include <frmnewmap.h>
#include "gridscene.h"

MapEditor::MapEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent) :
	TabWidget(Res, KInfo, Parent),
	orthoMap((Kite::KOrthogonalMap *)Res),
	ustack(new QUndoStack(this))
{}

void MapEditor::inite() {
	ustack->setUndoLimit(35);
	auto vlayout = new QVBoxLayout(this);
	vlayout->setMargin(3);
	vlayout->setSpacing(0);

	//auto hlayTools = new QHBoxLayout();
	//hlayTools->setMargin(0);

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
	btnCreate->setPopupMode(QToolButton::InstantPopup);
	toolbar->addWidget(btnCreate);
	toolbar->addSeparator();

	auto actUndo = ustack->createUndoAction(this);
	actUndo->setToolTip("Undo");
	actUndo->setIcon(QIcon(":/icons/undo"));
	actUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	actUndo->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	this->addAction(actUndo);
	toolbar->addAction(actUndo);

	auto actRedo = ustack->createRedoAction(this);
	actRedo->setToolTip("Redo");
	actRedo->setIcon(QIcon(":/icons/redo"));
	actRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
	actRedo->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	this->addAction(actRedo);
	toolbar->addAction(actRedo);
	toolbar->addSeparator();

	// tools
	actStamp = toolbar->addAction(QIcon(":/icons/stamp"), "Stamp Brush Tool");
	actStamp->setToolTip("Stamp Brush Tool (B)");
	actStamp->setCheckable(true);
	actStamp->setChecked(true);
	actStamp->setShortcut(QKeySequence(Qt::Key_B));
	actStamp->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	this->addAction(actStamp);
	connect(actStamp, &QAction::toggled, this, &MapEditor::stampTool);

	actSelect = toolbar->addAction(QIcon(":/icons/select"), "Selection Tool");
	actSelect->setToolTip("Selection Tool (S)");
	actSelect->setCheckable(true);
	actSelect->setShortcut(QKeySequence(Qt::Key_S));
	actSelect->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	this->addAction(actSelect);
	connect(actSelect, &QAction::toggled, this, &MapEditor::selectTool);

	actEraser = toolbar->addAction(QIcon(":/icons/clr"), "Eraser Tool");
	actEraser->setToolTip("Eraser Tool (E)");
	actEraser->setCheckable(true);
	actEraser->setShortcut(QKeySequence(Qt::Key_E));
	actEraser->setShortcutContext(Qt::ShortcutContext::WidgetWithChildrenShortcut);
	this->addAction(actEraser);
	connect(actEraser, &QAction::toggled, this, &MapEditor::eraserTool);
	toolbar->addSeparator();
	
	// stamp options
	auto lblStampOption = new QLabel(this);
	lblStampOption->setText("Stamp Mode: ");
	toolbar->addWidget(lblStampOption);

	cmbStampMode = new QComboBox(this);
	cmbStampMode->addItem("Stack");
	cmbStampMode->addItem("Layer");
	cmbStampMode->setCurrentIndex(1);
	toolbar->addWidget(cmbStampMode);

	auto lblLayer = new QLabel(this);
	lblLayer->setText("Layer: ");
	toolbar->addWidget(lblLayer);

	spnLayer = new QSpinBox(this);
	spnLayer->setMinimum(0);
	spnLayer->setMaximum(1000);
	toolbar->addWidget(spnLayer);
	toolbar->addSeparator();

	// modifiers
	auto btnTint = new QToolButton(this);
	btnTint->setFixedSize(16, 16);
	btnTint->setToolTip("Color Tint");
	btnTint->setStyleSheet("background-color: white;");
	tint = Qt::white;
	connect(btnTint, &QToolButton::clicked, this, &MapEditor::selectTintColor);
	toolbar->addWidget(btnTint);

	auto actFliph = toolbar->addAction(QIcon(":/icons/fliph"), "Flip Horizontal");
	actFliph->setToolTip("Flip Horizontal");

	auto actFlipv = toolbar->addAction(QIcon(":/icons/flipv"), "Flip Vertical");
	actFlipv->setToolTip("Flip Vertical");

	auto actDuplicate = toolbar->addAction(QIcon(":/icons/dup"), "Duplicate");
	actDuplicate->setToolTip("Duplicate Selected Area");
	toolbar->addSeparator();

	auto actGrid = toolbar->addAction(QIcon(":/icons/grid"), "Show/Hide Grid");
	actGrid->setToolTip("Show/Hide Grid");
	actGrid->setCheckable(true);
	actGrid->setChecked(true);

	gscene = new GridScene(false, false);
	gscene->installEventFilter(this);
	gscene->getView()->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	gscene->getView()->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
	connect(actGrid, &QAction::toggled, gscene, &GridScene::setShowGrid);

	QList<int> wsizes;
	auto viewSplit = new QSplitter(this);
	viewSplit->addWidget(gscene->getView());
	wsizes.append(1000);

	// tab
	tileset = new QTabWidget(this);
	tileset->setTabPosition(QTabWidget::TabPosition::South);
	viewSplit->addWidget(tileset);
	wsizes.append(250);
	viewSplit->setSizes(wsizes);

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
	disconnect(tileset, &QTabWidget::currentChanged, this, &MapEditor::tilesetChanged);
	gscene->reshape(orthoMap->getMapWidth() * orthoMap->getTileWidth(), orthoMap->getMapHeight() * orthoMap->getTileHeight(),
					orthoMap->getTileWidth(), orthoMap->getTileHeight());

	totalStackedSize = QString::number(orthoMap->getTotalStackedTiles());

	// reset tileset
	tileset->clear();
	stampList.clear();

	// clear all tabs
	for (auto it = viewPool.begin(); it != viewPool.end(); ++it) {
		(*it)->clear();
		delete (*it);
	}
	viewPool.clear();

	// re-create tabs
	if (orthoMap->getTileset()) {
		auto continer = orthoMap->getTileset()->getContiner();
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

				if (atlas) {
					auto tex = atlas->getTexture();
					if (tex) {
						tscene->reshape(tex->getWidth(),
									   tex->getHeight(),
									   orthoMap->getTileWidth(), orthoMap->getTileHeight());
						Kite::KImage image;
						tex->getImage(image);
						tscene->addPixmap(QPixmap::fromImage(QImage(image.getPixelsData(),
																	image.getWidth(), image.getHeight(), QImage::Format::Format_RGBA8888)));
					}
				}

				viewPool.push_back(tscene);
			}
		}
		if (!viewPool.empty()) {
			viewPool.first()->selectTile(0);
		}

		// resolve map tiles
		std::vector<Kite::KOrthoTile> tlist;
		QHash<unsigned int, QPixmap> pmMap;
		for (size_t i = 0; i < orthoMap->getTotalTiles(); ++i) {
			orthoMap->queryTiles(orthoMap->convertID(i), tlist);

			for (auto it = tlist.begin(); it != tlist.end(); ++it) {
				// tile set is ready
				if (it->textureIndex < viewPool.size()) {
					auto tset = viewPool[it->textureIndex];
					auto mdim = orthoMap->getTileDimension(i);
					auto tdim = tset->getStamp()->getTileDimension(it->atlas.id);

					// search map for pixmap
					if (pmMap.find(it->textureIndex) == pmMap.end()) {
						auto tpmitem = (QGraphicsPixmapItem *)tset->items(Qt::SortOrder::AscendingOrder).first();
						pmMap[it->textureIndex] = tpmitem->pixmap();
					}
					
					auto mainpm = new QGraphicsPixmapItem(pmMap[it->textureIndex]
														  .copy(tdim.left, tdim.bottom, orthoMap->getTileWidth(), orthoMap->getTileHeight()));
					mainpm->setPos(mdim.left, mdim.bottom);
					gscene->addItem(mainpm);

				// there is no tilset with this id
				} else {

				}
			}
		}
	}
	connect(tileset, &QTabWidget::currentChanged, this, &MapEditor::tilesetChanged);
	gscene->update();
}

void MapEditor::onRemoveRes(Kite::RTypes Type) {
	//if (Type == Kite::RTypes::TextureGroup || Type == Kite::RTypes::AtlasTexture || Type == Kite::RTypes::Texture) {
		//reload(); // reloading entire (big) map is very expensive
	//}
}

void MapEditor::tilesetChanged(int Index) {
	recreateMarker(viewPool[Index]->getStamp());
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

void MapEditor::recreateMarker(const Kite::KOrthoTileStamp *Stamp) {
	// clear stamplist
	for (auto it = stampList.begin(); it != stampList.end(); ++it) {
		gscene->removeItem((*it));
		delete (*it);
	}
	stampList.clear();

	auto slist = Stamp->getSelectedItems();
	auto atlasArray = orthoMap->getTileset();
	Kite::KImage image;
	atlasArray->getContiner()->at(tileset->currentIndex())->getTexture()->getImage(image);

	QImage qimage(image.getPixelsData(), image.getWidth(), image.getHeight(), QImage::Format::Format_RGBA8888);

	for (auto it = slist->cbegin(); it != slist->cend(); ++it) {
		stampList.push_back(new QGraphicsPixmapItem(QPixmap::fromImage(
			qimage.copy(it->atlas.xpos, it->atlas.ypos, it->atlas.width, it->atlas.height))));
		stampList.back()->setZValue(1);
		stampList.back()->setOpacity(0.5f);
		stampList.back()->hide();
		gscene->addItem(stampList.back());
	}
}

void MapEditor::redrawMarker(unsigned int AnchorID, bool OutOfView) {
	if (OutOfView) {
		for (auto i = 0; i < stampList.size(); ++i) {
				stampList[i]->hide();
		}
		return;
	}

	auto anchDim = orthoMap->getTileDimension(AnchorID);
	auto currTileset = viewPool[tileset->currentIndex()];
	auto currStamp = currTileset->getStamp()->getSelectedItems();

	int bottom = 0;
	int left = 0;
	for (auto i = 0; i < stampList.size(); ++i) {
		left = anchDim.left + (int)(currStamp->at(i).col * orthoMap->getTileWidth());
		bottom = anchDim.bottom - (int)(currStamp->at(i).row * orthoMap->getTileHeight());
		stampList[i]->setPos(left, bottom);

		// culling 
		if (left >= orthoMap->getTotalWidthPixelSize() || bottom >= orthoMap->getTotalHeightPixelSize()) {
			stampList[i]->hide();
		} else {
			stampList[i]->show();
		}
	}
}

void MapEditor::selectTintColor() {
	auto btnColor = (QToolButton *)sender();
	auto tcolor = QColorDialog::getColor(tint, this, "Color Picker", QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
	if (tcolor.isValid()) {
		tint = tcolor;
		btnColor->setStyleSheet("QToolButton { background-color: rgb("
								+ QString::number(tint.red()) + ", "
								+ QString::number(tint.green()) + ", "
								+ QString::number(tint.blue()) + "); }");
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

	} else {
		gscene->setSelectable(false);
	}
}

void MapEditor::eraserTool(bool Check) {
	if (Check) {
		if (actStamp->isChecked()) actStamp->setChecked(false);
		if (actSelect->isChecked()) actSelect->setChecked(false);
	}
}

bool MapEditor::eventFilter(QObject *Obj, QEvent *Event) {
	static int lastID = -1;
	static bool pushing = false;
	static bool erasing = false;
	if (Event->type() == QEvent::GraphicsSceneMouseMove) {
		QPoint origin = gscene->getView()->mapFromGlobal(QCursor::pos());
		QPointF pos = gscene->getView()->mapToScene(origin);
		Kite::U32 tileID;
		if (orthoMap->getTileID(Kite::KVector2F32(pos.x(), pos.y()), tileID)) {
			if (lastID != tileID) {
				auto dim = orthoMap->getTileDimension(orthoMap->convertID(tileID));
				lblWarning->setText("<font color = \"orange\">Tile ID: </font>" + QString::number(orthoMap->convertID(tileID)) +
									"<font color = \"orange\"> Layers: </font>" + QString::number(orthoMap->getTileSize(orthoMap->convertID(tileID))) +
									"<font color = \"orange\"> X: </font>" + QString::number(dim.left) +
									"<font color = \"orange\"> Y: </font>" + QString::number(dim.bottom) +
									"<font color = \"orange\"> Used Tiles: </font>" + totalStackedSize);

				// actions
				if (actStamp->isChecked()) redrawMarker(tileID, false);

				// push
				if (pushing) {
					ustack->push(new PushStampCMD(gscene ,orthoMap, tileset->currentIndex(), tileID,
											   *viewPool.at(tileset->currentIndex())->getStamp()->getSelectedItems(), tileset->currentIndex()));
				}

				// erase
				if (erasing) {
					if (orthoMap->getTileSize(orthoMap->convertID(tileID)) > 0) {
						ustack->push(new EraserCMD(gscene, orthoMap, tileID));
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

				// push
				if (actStamp->isChecked() && !viewPool.at(tileset->currentIndex())->getStamp()->getSelectedItems()->empty()){
					ustack->push(new PushStampCMD(gscene, orthoMap, tileset->currentIndex(), tileID,
												  *viewPool.at(tileset->currentIndex())->getStamp()->getSelectedItems(), tileset->currentIndex()));
					pushing = true;
				}

				// erase
				if (actEraser->isChecked()) {
					if (orthoMap->getTileSize(orthoMap->convertID(tileID)) > 0) {
						ustack->push(new EraserCMD(gscene, orthoMap, tileID));
					}
					erasing = true;
				}
			}
		}
	} else if (Event->type() == QEvent::GraphicsSceneMouseRelease) {
		pushing = false;
		erasing = false;
	}
	return false;
}