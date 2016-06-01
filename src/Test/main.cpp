#include <Kite\engine\kengine.h>
#include <time.h>

using namespace Kite;
int main() {
	auto sc = new KConfig();
	auto engine = KEngine::createEngine();

	engine->inite(sc, false);

	engine->getWindow()->setShowCursor(false);
	//engine->getWindow()->setShowCursor(true);

	engine->start();

	engine->shutdown();

	delete engine;
	delete sc;

	sc = new KConfig();
	engine = KEngine::createEngine();

	engine->inite(sc, false);

	engine->getWindow()->setShowCursor(false);
	//engine->getWindow()->setShowCursor(true);

	engine->start();

	engine->shutdown();

	delete engine;
	delete sc;

	return 0;
}