#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <Kite\engine\kengine.h>
#include <time.h>

using namespace Kite;

void koutput(const std::string &Text) {
	printf(Text.c_str());
}

int main() {
	auto sc = new KConfig();

	auto engine = KEngine::createEngine();

	engine->inite(sc, false);

	engine->start();

	engine->shutdown();
	delete engine;
	delete sc;
	_CrtDumpMemoryLeaks();

	return 0;
}