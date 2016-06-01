#include "executer.h"

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

Executer::Executer() :
	wthread(nullptr)
{}

void Executer::run(const Kite::KConfig *Config){
	wthread = new WorkerThread;
	engine = Kite::KEngine::createEngine();

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