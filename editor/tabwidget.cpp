#include "tabwidget.h"
#include <qevent.h>
#include <qmessagebox.h>

TabWidget::TabWidget(Kite::KResource *Res, Kite::KIStream *Stream, QWidget *Parent) :
	QFrame(Parent),
	res(Res),
	stream(Stream)
{}

TabWidget::~TabWidget(){
	delete res;
	delete stream;
}
