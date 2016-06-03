#include <Kite\engine\kengine.h>
#include <time.h>

using namespace Kite;

void koutput(const std::string &Text) {
	printf(Text.c_str());
}

int main() {
	auto sc = new KConfig();
	sc->dictionary = "C:\\Users\\_M1L4D_\\Desktop\\proj1\\dict.kdict";
	sc->startUpScene = "scene1";

	auto engine = KEngine::createEngine();

	engine->inite(sc, false);

	engine->start();

	engine->shutdown();

	delete engine;
	delete sc;
	return 0;
}