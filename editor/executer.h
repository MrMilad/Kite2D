#ifndef EXECUTER_H
#define EXECUTER_H

#include "Kite/engine/kengine.h"
#include <qstring.h>
#include <string>
#include <qthread.h>

class WorkerThread : public QThread {
	Q_OBJECT
public:
	WorkerThread();
	~WorkerThread();
	void run() Q_DECL_OVERRIDE;

	inline void setEngine(Kite::KEngine *Engine) { engine = Engine; }
	inline void setConfig(const Kite::KConfig *Config) { config = Config; }

private:
	Kite::KEngine *engine;
	const Kite::KConfig *config;
};

class Executer : public QObject{
	Q_OBJECT
public:
    Executer();

	void run(const Kite::KConfig *Config);
	void unpause();
	void pause();
	void stop();

signals:
	void started();
	void paused();
	void unpaused();
	void stoped();
	void engineOutput(const QString &Text);

private slots:
	void cleanUp();

private:
	static void koutCallback(const std::string &Text);
	static Executer *instance;
	WorkerThread *wthread;
	Kite::KEngine *engine;
};

#endif // EXECUTER_H