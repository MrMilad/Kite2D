#ifndef GLSLEDITOR_H
#define GLSLEDITOR_H

#include "tabwidget.h"
#include "codeeditor.h"
#include "completer.h"
#include "glslhlight.h"
#include <Kite/graphic/kshader.h>

class GLSLEditor: public TabWidget{
public:
    GLSLEditor(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr);
    ~GLSLEditor();

	void inite() override;
    bool saveChanges() override;

    static TabWidget *factory(Kite::KResource *Res, KiteInfo *KInfo, QWidget *Parent = nullptr) {
        return new GLSLEditor(Res, KInfo, Parent);
    }

private:
	void initeModel();
    Kite::KShader *shader;
	CodeEditor *editor;
    GLSLHLight *hlight;
	static Completer *completer;
};

#endif // GLSLEDITOR_H
