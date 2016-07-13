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


	auto c0 = (KLogicCom *) ent6->addComponent("Logic", "0");
	c0->setScript("myScript");
	auto c1 = ent6->addComponent("Logic", "1")->getHandle();
	auto c2 = (KInputCom *)ent6->addComponent("Input", "incom");
	c2->setEnableMouse(false);
	c2->setEnableKeyboard(false);
	
	KPrefab pre("mypre");
	//pre.loadStream(&istream, "E:\\prefab.pre");
	//auto hndl = engine->getSceneManager()->getActiveScene()->getEManager()->loadPrefab(&pre);

	engine->getSceneManager()->getActiveScene()->getEManager()->copyPrefab(ent6->getHandle(), &pre, true);
	pre.saveStream(&stream, "E:\\prefab.pre");

	engine->start();

	engine->shutdown();
	delete engine;
	delete sc;

	return 0;
}