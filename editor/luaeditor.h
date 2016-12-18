#ifndef LUAEDITOR_H
#define LUAEDITOR_H

#include "tabwidget.h"
#include "codeeditor.h"
#include "completer.h"

class LuaHLight;
class LuaEditor : public TabWidget{
public:
    LuaEditor(Kite::KResource *Res, Kite::KIStream *Stream, KiteInfo *KInfo, QWidget *Parent = nullptr);
	~LuaEditor();

	void inite() override;
	void saveChanges() override;
	void reload() override;

	static TabWidget *factory(Kite::KResource *Res, Kite::KIStream *Stream, KiteInfo *KInfo, QWidget *Parent = nullptr) {
		return new LuaEditor(Res, Stream, KInfo, Parent);
	}

private:
	void initeModel();
	CodeEditor *editor;
	Kite::KScript *script;
	LuaHLight *hlight;
	KiteInfo *kinfo;
	static Completer *completer;
};

#endif // LUAEDITOR_H
