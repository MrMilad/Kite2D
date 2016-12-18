#ifndef OBJECTSCENE_H
#define OBJECTSCENE_H
#include <QGraphicsScene>
#include <qgraphicsitem.h>
#include <qvector.h>
#include <qstring.h>

namespace Kite {
	class KEntity;
	class KComponent;
	class KScene;
	class KResource;
	class KEntityManager;
	class KQuadCom;
	class KTransformCom;
}
class ObjectScene : public QGraphicsScene {
	Q_OBJECT

public:
	ObjectScene(QObject * parent = Q_NULLPTR);
	~ObjectScene();

public slots:
	void resSelect(Kite::KResource *Scene);
	//void resDelete(Kite::KResource *Scene);
	//void objectSelected(Kite::KEntityManager *EMan, Kite::KEntity *Entity, bool isPrefab);
	//void objectDelete(Kite::KEntityManager *EMan, Kite::KEntity *Entity);
	//void componentAdded(Kite::KEntity *Entity, Kite::KComponent *Component);
	//void componentDelete(Kite::KEntity *Entity, Kite::KComponent *Component);
	void componentEdited(Kite::KEntity *Entity, Kite::KComponent *Component, const QString &PName);
	//void entityLayerChanged(Kite::KEntity *Entity);
	//void entityZOrderChanged(Kite::KEntity *Entity);*/

signals:
	Kite::KResource *requestResource(const QString &Name);

protected:
	void drawForeground(QPainter *painter, const QRectF &rect) override;

private:
	void syncQuad(Kite::KQuadCom *Quad);
	void syncTransform(Kite::KTransformCom *Trans);

	Kite::KScene *currScene;
	QVector<QGraphicsItemGroup *> layers;
};
#endif // OBJECTSCENE_H