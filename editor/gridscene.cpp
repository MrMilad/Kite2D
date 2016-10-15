#include <gridscene.h>
#include <QGraphicsItem>
#include <Kite/graphic/korthogonalmap.h>
#include <QtGui>
#include <qevent.h>
#include <qgraphicsview.h>
#include <QtWidgets>

GridScene::GridScene(bool Selectable, bool ShowAnchor, QObject *Parent) :
	QGraphicsScene(Parent),
	lcolor(Qt::gray),
	pstyle(Qt::PenStyle::DashLine),
	selectable(Selectable),
	showAnchor(ShowAnchor),
	showGrid(true),
	gview(new QGraphicsView()),
	stamp(1, 1, 1, 1),
	marker(new QGraphicsPixmapItem)
{
	gview->setScene(this);
	gview->setMouseTracking(true);
	marker->setFlag(QGraphicsItem::ItemIsSelectable, false);
}

GridScene::~GridScene() {
	this->clear();
	delete gview;
}

void GridScene::drawForeground(QPainter *painter, const QRectF &rect){
	QGraphicsScene::drawForeground(painter, rect);

	if (showGrid) {
		QPen pen(lcolor);
		pen.setStyle(pstyle);
		painter->setPen(pen);
		painter->drawLines(lines.data(), lines.size());
	}

	// re draw anchor
	if (showAnchor && selectable && stamp.isValid()) {
		painter->setPen(QPen(QColor(Qt::red), 2, Qt::PenStyle::DashLine));
		painter->setBrush(QBrush(QColor(0, 0, 0, 0)));
		auto tpos = stamp.getTileDimension(stamp.getAnchorID());
		painter->drawRect(tpos.left, tpos.bottom, tpos.right - tpos.left, tpos.top - tpos.bottom);
	}
}

void GridScene::reshape(quint32 Width, quint32 Height, quint32 TileWidth, quint32 TileHeight){
	this->clear();
	this->setSceneRect(0, 0, Width + 1, Height + 1);
	slist.clear();
	stamp.resetSize(Width, Height, TileWidth, TileHeight);

	marker = new QGraphicsPixmapItem();
	marker->setZValue(2);

	QPixmap pm(TileWidth, TileHeight);
	pm.fill(QColor(21, 193, 255, 60));
	marker->setPixmap(pm);
	marker->setZValue(1000);
	marker->hide();
	this->addItem(marker);

    lines.clear();
    lines.reserve(stamp.getRowCount() * stamp.getColumnCount());

    for (quint32 i = 0; i <= stamp.getRowCount(); i++){
        QLine line(0, (i * TileHeight), stamp.getColumnCount() * TileWidth, i * TileHeight);
        lines.push_back(line);
    }

    for (quint32 i = 0; i <= stamp.getColumnCount(); i++){
        QLine line(i * TileWidth, 0, i * TileWidth, stamp.getRowCount() * TileHeight);
        lines.push_back(line);
    }

	gview->verticalScrollBar()->setValue(gview->verticalScrollBar()->maximum());
	this->update();
}

void GridScene::selectTile(unsigned int TileID) {
	if (stamp.isValid()) {
		stamp.select(TileID);
		drawSelectedTiles();
		this->update();
	}
}

void GridScene::setSelectable(bool Selectable) {
	if (!Selectable) {
		stamp.resetSelection();
		drawSelectedTiles();
	}

	selectable = Selectable;
}

void GridScene::setShowAnchor(bool Show) {
	showAnchor = Show;
	this->update();
}

void GridScene::setShowGrid(bool Show) {
	showGrid = Show;
	update();
}

void GridScene::drawQueriedTiles(bool ClearOnly) {
	// clear all queried tiles
	for (auto it = qlist.begin(); it != qlist.end(); ++it) {
		removeItem((*it));
		delete (*it);
	}
	qlist.clear();

	if (!stamp.isValid() || !selectable || ClearOnly) {
		return;
	}

	// re draw all queried items
	auto anchDim = stamp.getTileDimension(stamp.getAnchorID());
	auto qtiles = stamp.getQueriedItems();
	for (auto it = qtiles->cbegin(); it != qtiles->cend(); ++it) {
		int left = anchDim.left + (int)(it->col * stamp.getTileWidth());
		int bottom = anchDim.bottom - (int)(it->row * stamp.getTileHeight());

		qlist.push_back(new QGraphicsPixmapItem(marker->pixmap().copy(marker->pixmap().rect())));
		qlist.back()->setZValue(1000);
		qlist.back()->setPos(left, bottom);
		addItem(qlist.back());
	}
}

void GridScene::drawSelectedTiles() {
	// clear all selected tiles
	for (auto it = slist.begin(); it != slist.end(); ++it) {
		removeItem((*it));
		delete (*it);
	}
	slist.clear();

	if (!stamp.isValid() || !selectable) {
		return;
	}

	// re draw all selected items
	auto stiles = stamp.getSelectedItems();
	auto anchDim = stamp.getTileDimension(stamp.getAnchorID());
	for (auto it = stiles->cbegin(); it != stiles->cend(); ++it) {
		int left = anchDim.left + (int)(it->col * stamp.getTileWidth());
		int bottom = anchDim.bottom - (int)(it->row * stamp.getTileHeight());

		slist.push_back(new QGraphicsPixmapItem(marker->pixmap().copy(marker->pixmap().rect())));
		slist.back()->setZValue(1000);
		slist.back()->setPos(left, bottom);
		addItem(slist.back());
	}
}

bool GridScene::event(QEvent *Event) {
	static bool isSelection = true;
	static bool isPressed = false;
	static int lastID = -1;
	static Kite::KRectF32 area;;
	if (Event->type() == QEvent::GraphicsSceneMousePress && selectable){

		// select tile
		if (QGuiApplication::mouseButtons() != Qt::MouseButton::MidButton ) {

			// retrive tile under cursor
			QPoint origin = gview->mapFromGlobal(QCursor::pos());
			QPointF pos = gview->mapToScene(origin);

			unsigned int tid = 0;
			bool validTile;

			// selection/deselction state
			if ((validTile = stamp.getTileID(Kite::KVector2F32(pos.x(), pos.y()), tid))) {
				isSelection = !stamp.isSelected(tid);
			}

			// check CTRL
			if (QGuiApplication::keyboardModifiers() != Qt::KeyboardModifier::ControlModifier) {
				isSelection = true;
				stamp.resetSelection();
				if (validTile) {
					stamp.setAnchorID(tid);
				}
				drawSelectedTiles();
				update();
			}

			// save rect for first under cursor tile
			area.bottom = area.top = pos.y();
			area.left = area.right = pos.x();

			// draw first under cursor tile
			stamp.query(area, !isSelection);
			drawQueriedTiles();

			isPressed = true;

		// change anchor point
		} else if (QGuiApplication::mouseButtons() == Qt::MouseButton::MidButton && showAnchor) {
			QPoint origin = gview->mapFromGlobal(QCursor::pos());
			QPointF pos = gview->mapToScene(origin);
			Kite::U32 tileID;
			if (stamp.getTileID(Kite::KVector2F32(pos.x(), pos.y()), tileID)) {
				stamp.setAnchorID(tileID);
				update();
				emit(stampChanged(stamp.getSelectedItems()));
			}
		}

	} else if (Event->type() == QEvent::GraphicsSceneMouseRelease && selectable && isPressed) {
		QPoint origin = gview->mapFromGlobal(QCursor::pos());
		QPointF pos = gview->mapToScene(origin);
		isPressed = false;

		// draw selected area
		area.top = pos.y();
		area.right = pos.x();
		Kite::KRectF32 tempArea = area;
		tempArea.correctSides();
		if (isSelection) {
			stamp.querySelect(tempArea);
		} else {
			stamp.queryDeselect(tempArea);
		}
		drawSelectedTiles();
		drawQueriedTiles(true); // clear quried tiles
		emit(stampChanged(stamp.getSelectedItems()));

	}else if (Event->type() == QEvent::GraphicsSceneMouseMove) {
		QPoint origin = gview->mapFromGlobal(QCursor::pos());
		QPointF pos = gview->mapToScene(origin);
		Kite::U32 tileID;
		if (stamp.getTileID(Kite::KVector2F32(pos.x(), pos.y()), tileID)) {
			if (lastID != tileID) {

				// draw marker
				auto tpos = stamp.getTileDimension(tileID);
				marker->setPos(tpos.left, tpos.bottom);
				marker->show();

				// draw queried area
				if (isPressed) {
					area.top = pos.y();
					area.right = pos.x();
					Kite::KRectF32 tempArea = area;
					tempArea.correctSides();
					stamp.query(tempArea, !isSelection);
					drawQueriedTiles();
				}

				lastID = tileID;
			}
		} else {
			lastID = -1;
			marker->hide();
		}
	} else if (Event->type() == QEvent::GraphicsSceneWheel) {
		this->update();
	} else if (Event->type() == QEvent::Leave) {
		lastID = -1;
		marker->hide();
	}
	return QGraphicsScene::event(Event);
}
