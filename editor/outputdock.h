#ifndef OUTPUTDOCK_H
#define OUTPUTDOCK_H

#include <qdockwidget.h>
#include <qplaintextedit.h>
#include <qaction.h>
#include <qlabel.h>
#include <qframe.h>

class OutputDock : public QDockWidget {
	Q_OBJECT

public:
	explicit OutputDock(QWidget *parent = 0);
	~OutputDock();
	
	inline auto getEditor() { return textEdit; }

public slots:
	void autoShow();
	void autoHide();

private slots:
	void toggleWordWrap();

private:
	void setupTEdit();
	void setupActions();
	void setupHTools();

	bool wasHide;
	QPlainTextEdit *textEdit;
	QAction *clearAll;
	QAction *wordWrap;
	QFrame *htools;
	QLabel *hlabel;
};

#endif // OUTPUTDOCK_H
