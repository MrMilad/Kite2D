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
	std::vector<KComponent *> list;
	auto engine = KEngine::createEngine();

	engine->inite(sc, false);

	auto ent = engine->getSceneManager()->getActiveScene()->getEManager()->createEntity("test");
	auto c0 = ent->addComponent("Logic", "0")->getHandle();
	auto c1 = ent->addComponent("Logic", "1")->getHandle();
	auto c2 = ent->addComponent("Logic", "2")->getHandle();
	auto c3 = ent->addComponent("Logic", "3")->getHandle();
	auto c4 = ent->addComponent("Logic", "4")->getHandle();

	ent->getScriptComponents(list);
	ent->reorderScriptComponent(c1, 3);
	ent->getScriptComponents(list);

	engine->start();

	engine->shutdown();
	delete engine;
	delete sc;
	_CrtDumpMemoryLeaks();

	return 0;
}