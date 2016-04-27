#include <qobject.h>
#include <qvariant.h>
#include "Kite/core/kany.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/math/kmathstructs.h"

class ComProperty : public QObject {
	Q_OBJECT
public:
	ComProperty(QObject *Parrent = nullptr);

	inline void setCName(QString CName) { cname = CName; }
	inline QString getCName() const { return cname; }

	inline void setCType(QString CType) { ctype = CType; }
	inline QString getCType() const { return ctype; }

	inline void setPName(QString PName) { pname = PName; }
	inline QString getPName() const { return pname; }

	inline void setPType(QString PType) { ptype = PType; }
	inline QString getPType() const { return ptype; }

	inline void setEntity(Kite::KHandle EHandle) { entity = EHandle; }
	inline Kite::KHandle getEntity() const { return entity; }

	virtual Kite::KAny getValue() = 0;

Q_SIGNALS:
	void finishEdit();

private:
	QString cname;
	QString ctype;
	QString pname;
	QString ptype;
	Kite::KHandle entity;
};

class KV2F32 : public ComProperty {
	Q_OBJECT
public:
	KV2F32(QObject *Parrent = nullptr) :
		ComProperty(Parrent) {}

	Kite::KAny getValue() override {
		return Kite::KAny(value);
	}

public slots:
void editedX(const QString & text) {
	value.x = text.toFloat();
}
void editedY(const QString & text) {
	value.y = text.toFloat();
}

private:
	Kite::KVector2F32 value;
};
