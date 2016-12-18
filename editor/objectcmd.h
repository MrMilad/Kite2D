#ifndef OBJECTCMD_H
#define OBJECTCMD_H

#include <qundostack.h>
#include <Kite/core/kprefab.h>

class AddRootCMD : public QUndoCommand {
public:
	AddRootCMD(const Kite::KHandle &Handle) :
		handle(Handle)
	{
		setText("Add Object Root");
	}

	virtual void undo() override {

	}

	virtual void redo() override {

	}

private:
	Kite::KPrefab copy;
};

#endif // OBJECTCMD_H