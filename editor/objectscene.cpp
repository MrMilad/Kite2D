#include "objectscene.h"
#include <Kite/core/kresource.h>
#include <Kite/core/kentity.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kcomponent.h>
#include <Kite/graphic/kquadcom.h>
#include <qpainter.h>

ObjectScene::ObjectScene(QObject * parent):
	QGraphicsScene(parent) ,
	currScene(nullptr),
	layers(KENTITY_LAYER_SIZE)
{}

ObjectScene::~ObjectScene() {}

void ObjectScene::drawForeground(QPainter *painter, const QRectF &rect) {
	QGraphicsScene::drawForeground(painter, rect);

	painter->setPen(Qt::green);
	painter->drawLine(0, 0, 150, 0);

	painter->setPen(Qt::red);
	painter->drawLine(0, 0, 0, 150);
}

void ObjectScene::resSelect(Kite::KResource *Scene) {
	if (currScene == (Kite::KScene *)Scene) return;

	if (Scene->getType() != Kite::RTypes::Scene) return;

	currScene = (Kite::KScene *)Scene;
	clear();
	for (auto it = layers.begin(); it != layers.end(); ++it) {
		(*it) = nullptr;
	}

	auto quadContiner = currScene->getEManager()->getComponentStorage<Kite::KQuadCom>(Kite::CTypes::Quad);
	for (auto it = quadContiner->begin(); it != quadContiner->end(); ++it) {
		auto ent = currScene->getEManager()->getEntity(it->getOwnerHandle());
		auto tr = (Kite::KTransformCom *)ent->getComponent(Kite::CTypes::Transform);

		// check layer
		if (layers.at(ent->getLayer()) == nullptr) {
			layers[ent->getLayer()] = new QGraphicsItemGroup();
			addItem(layers[ent->getLayer()]);
		}

		Kite::KRectF32 brect;
		it->getBoundingRect(brect);
		auto rect = new QGraphicsRectItem(layers[ent->getLayer()]);
		rect->setRect(QRect(brect.left, brect.bottom, brect.right - brect.left, brect.top - brect.bottom));
		rect->setTransformOriginPoint(tr->getCenter().x, tr->getCenter().y);
		rect->setPos(tr->getPosition().x, tr->getPosition().y);
		rect->setRotation(tr->getRotation());
		rect->setScale(tr->getScale().x);
	}
}
