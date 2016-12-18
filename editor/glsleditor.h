#ifndef GLSLEDITOR_H
#define GLSLEDITOR_H

#include "tabwidget.h"
#include "codeeditor.h"
#include "completer.h"
#include "glslhlight.h"
#include <Kite/graphic/kshader.h>

class QUndoStack;

class GLSLEditor: public TabWidget{
public:
    GLSLEditor(Kite::KResource *Res, Kite::KIStream *Stream, QUndoStack *UStack, QWidget *Parent = nullptr);
    ~GLSLEditor();

	void inite() override;
    void saveChanges() override;
	void reload() override;

    static TabWidget *factory(Kite::KResource *Res, Kite::KIStream *Stream, QUndoStack *UStack, KiteInfo *KInfo, QWidget *Parent = nullptr) {
        return new GLSLEditor(Res, Stream, UStack, Parent);
    }

private:
	void initeModel();
    Kite::KShader *shader;
	CodeEditor *editor;
    GLSLHLight *hlight;
	static Completer *completer;
	QUndoStack *ustack;
};

#endif // GLSLEDITOR_H
