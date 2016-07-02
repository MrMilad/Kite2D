#include <Kite\engine\kengine.h>
#include <time.h>

using namespace Kite;

void koutput(const std::string &Text) {
	printf(Text.c_str());
}

int main() {
	KFOStream stream;
	KFIStream istream;

	auto sc = new KConfig();
	auto engine = KEngine::createEngine();

	engine->inite(sc, false);

	//engine->getSceneManager()->getActiveScene()->loadStream(&istream, "e:\\sc.txt");

	auto ent1 = engine->getSceneManager()->getActiveScene()->getEManager()->createEntity("test1")->getHandle();
	auto pent2 = engine->getSceneManager()->getActiveScene()->getEManager()->createEntity("test2");
	pent2->addChild(ent1);
	auto ent2 = pent2->getHandle();

	auto ent3 = engine->getSceneManager()->getActiveScene()->getEManager()->createEntity("test3")->getHandle();
	auto ent4 = engine->getSceneManager()->getActiveScene()->getEManager()->createEntity("test4")->getHandle();
	auto pent5 = engine->getSceneManager()->getActiveScene()->getEManager()->createEntity("test5");
	pent5->addChild(ent4);
	auto ent5 = pent5->getHandle();

	auto ent6 = engine->getSceneManager()->getActiveScene()->getEManager()->createEntity("test6");
	ent6->addChild(ent3);
	ent6->addChild(ent2);
	ent6->addChild(ent5);

	auto c0 = ent6->addComponent("Logic", "0")->getHandle();
	auto c1 = ent6->addComponent("Logic", "1")->getHandle();
	
	engine->getSceneManager()->getActiveScene()->getEManager()->savePrefab(ent6->getHandle(), &stream, "e:\\prefab.txt");
	//engine->getSceneManager()->getActiveScene()->saveStream(&stream, "e:\\sc.txt");

	engine->start();

	engine->shutdown();
	delete engine;
	delete sc;

	return 0;
}