#include "objectscene.h"
#include <Kite/core/kresource.h>
#include <Kite/core/kentity.h>
#include <Kite/core/kscene.h>
#include <Kite/core/kcomponent.h>
#include <Kite/graphic/kquadcom.h>
#include <Kite/graphic/korthomapcom.h>
#include <Kite/graphic/kgraphicdef.h>
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

void ObjectScene::syncQuad(Kite::KQuadCom *Quad) {
	auto pixItem = (QGraphicsPixmapItem *)Quad->getSceneItem();

	Kite::KRectF32 brect;
	Quad->getBoundingRect(brect);

	if (!Quad->getAtlasTextureArray().str.empty()) {
		Kite::KAtlasTextureArray *tarray = nullptr;
		emit(tarray = (Kite::KAtlasTextureArray *)requestResource(Quad->getAtlasTextureArray().str.c_str()));

		if (tarray) {
			auto atex = tarray->getItem(Quad->getTextureArrayIndex());
			if (atex) {
				Kite::KTexture *tex = atex->getTexture();
				if (tex) {
					Kite::KImage image;
					tex->getImage(image);
					QImage qimage(image.getPixelsData(), image.getWidth(), image.getHeight(), QImage::Format::Format_RGBA8888);
					auto procimgae = qimage.copy(Quad->getAtlasItem().xpos, Quad->getAtlasItem().ypos,
																	  Quad->getAtlasItem().width, Quad->getAtlasItem().height)
														  .mirrored(Quad->getAtlasItem().getFlipH(), !Quad->getAtlasItem().getFlipV())
														  .scaled(Quad->getWidth(), Quad->getHeight());

					//blend
					if (Quad->getBlendColor() != Kite::KColor(Kite::Colors::WHITE)) {
						QColor col(Quad->getBlendColor().getR(), Quad->getBlendColor().getG(),
								   Quad->getBlendColor().getB(), Quad->getBlendColor().getA());

						auto alpha = procimgae.alphaChannel();
						for (int x = 0; x != procimgae.width(); ++x) {
							for (int y(0); y != procimgae.height(); ++y) {
								if (qAlpha(procimgae.pixel(x, y)) == 0) continue; // transparrent pixels

								QColor icol(procimgae.pixel(x, y));
								icol.setRed(BLEND_Multiply(icol.red(), col.red()));
								icol.setBlue(BLEND_Multiply(icol.blue(), col.blue()));
								icol.setGreen(BLEND_Multiply(icol.green(), col.green()));
								procimgae.setPixel(x, y, icol.rgb());
							}
						}
						procimgae.setAlphaChannel(alpha);
					}

					pixItem->setPixmap(QPixmap::fromImage(procimgae));
					pixItem->setOpacity(Quad->getBlendColor().getGLA());
					return;
				}
			}
		}
	}
	
	QPixmap pm(Quad->getWidth(), Quad->getHeight());
	pm.fill(QColor(Quad->getBlendColor().getR(), Quad->getBlendColor().getG(), Quad->getBlendColor().getB(), Quad->getBlendColor().getA()));
	pixItem->setPixmap(pm);
	pixItem->setOpacity(Quad->getBlendColor().getGLA());
}

void ObjectScene::syncTransform(Kite::KTransformCom *Trans) {
	auto rect = (QGraphicsPixmapItem *)Trans->getSceneItem();

	Trans->computeMatrix();
	QTransform itTransf(Trans->getMatrix().at(0), Trans->getMatrix().at(3), Trans->getMatrix().at(6),
						Trans->getMatrix().at(1), Trans->getMatrix().at(4), Trans->getMatrix().at(7),
						Trans->getMatrix().at(2), Trans->getMatrix().at(5), Trans->getMatrix().at(8));

	rect->setTransform(itTransf);
}

void ObjectScene::resSelect(Kite::KResource *Scene) {
	if (currScene == (Kite::KScene *)Scene) return;

	if (Scene->getType() != Kite::RTypes::Scene) return;

	currScene = (Kite::KScene *)Scene;
	clear();
	for (auto it = layers.begin(); it != layers.end(); ++it) {
		(*it) = nullptr;
	}

	// quads
	auto quadContiner = currScene->getEManager()->getComponentStorage<Kite::KQuadCom>(Kite::CTypes::Quad);
	for (auto it = quadContiner->begin(); it != quadContiner->end(); ++it) {
		auto ent = currScene->getEManager()->getEntity(it->getOwnerHandle());
		auto tr = (Kite::KTransformCom *)ent->getComponent(Kite::CTypes::Transform);

		// check layer
		if (layers.at(ent->getLayer()) == nullptr) {
			layers[ent->getLayer()] = new QGraphicsItemGroup();
			addItem(layers[ent->getLayer()]);
		}

		auto rect = new QGraphicsPixmapItem(layers[ent->getLayer()]);
		tr->setSceneItem(rect);
		it->setSceneItem(rect);

		syncQuad(&(*it));
		syncTransform(tr);
	}

	// tile maps
	/*auto tmapContiner = currScene->getEManager()->getComponentStorage<Kite::KOrthoMapCom>(Kite::CTypes::OrthogonalMapView);
	for (auto it = tmapContiner->begin(); it != tmapContiner->end(); ++it) {
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
		rect->setRotation(tr->getRotation());
		rect->setTransform(QTransform::fromScale(tr->getScale().x, tr->getScale().y), true);
		rect->setPos(tr->getPosition().x, tr->getPosition().y);
	}*/
}

void ObjectScene::componentEdited(Kite::KEntity *Entity, Kite::KComponent *Component, const QString &PName) {
	if (Component->getType() == Kite::CTypes::Transform) {
		auto tr = static_cast<Kite::KTransformCom *>(Component);
		syncTransform(tr);
	}

	if (Component->getType() == Kite::CTypes::Quad) {
		auto qcom = static_cast<Kite::KQuadCom *>(Component);
		syncQuad(qcom);
	}
}