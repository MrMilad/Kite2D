#include "executer.h"
#include <Kite/core/kcoredef.h>

WorkerThread::WorkerThread() :
	engine(nullptr), config(nullptr)
{}

WorkerThread::~WorkerThread() {}

void WorkerThread::run() {
	if (engine == nullptr || config == nullptr) {
		return;
	}

	engine->inite(config, false);

	engine->start();

	engine->shutdown();
}

Executer *Executer::instance = nullptr;
Executer::Executer() :
	wthread(nullptr)
{
	instance = this;
}

void Executer::koutCallback(const std::string &Text) {
	if (instance != nullptr) {
		emit(instance->engineOutput(QString(Text.c_str())));
	}
}

void Executer::run(const Kite::KConfig *Config){
	wthread = new WorkerThread;
	engine = Kite::KEngine::createEngine();
	Kite::setEditorPrintCallback(&Executer::koutCallback);

	wthread->setEngine(engine);
	wthread->setConfig(Config);
	connect(wthread, &WorkerThread::finished, this, &Executer::cleanUp);

	wthread->start();

	emit(started());
}

void Executer::unpause() {
	if (engine != nullptr) {
		engine->setPauseFlag(false);
		emit(unpaused());
	}
}

void Executer::pause() {
	if (engine != nullptr) {
		engine->setPauseFlag(true);
		emit(paused());
	}
}

void Executer::stop() {
	if (engine != nullptr) {
		engine->setExitFlag(true);
	}
}

void Executer::cleanUp() {
	delete engine;
	engine = nullptr;

	delete wthread;
	wthread = nullptr;

	emit(stoped());
}