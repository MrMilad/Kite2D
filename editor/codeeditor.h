#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <qstandarditemmodel.h>
#include <Kite/core/kresource.h>
#include <kite/logic/kscript.h>
#include "completer.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class CodeEditor : public QPlainTextEdit{
    Q_OBJECT

public:
    CodeEditor(Completer *Comp, QWidget *parent = 0);
	
    void lineNumberAreaPaintEvent(QPaintEvent *Event);
    int lineNumberAreaWidth();
	inline void setStartNumber(unsigned int Number) { snumber = Number; }

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
	void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
	void insertCompletion(const QString &completion);

private:
	unsigned int snumber;
	QString textUnderCursor() const;
    QWidget *lineNumberArea;
	Completer *comp;
};


class LineNumberArea : public QWidget{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};


#endif
