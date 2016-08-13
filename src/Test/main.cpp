#include <Kite\engine\kengine.h>
#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>
#include <Kite/graphic/kimage.h>
#include <Kite/memory/kpoolstorage.h>

using namespace Kite;

void koutput(const std::string &Text) {
	printf(Text.c_str());
}

int main() {


	int a = 0;
	KFOStream stream;
	KFIStream istream;
	KResourceManager rman;
	registerRTypes(&rman);

	auto sc = new KConfig();
	auto engine = KEngine::createEngine();

	engine->inite(sc, false);

	//engine->getSceneManager()->loadScene("e:\\test.sce");
	//auto eman = engine->getSceneManager()->getScene("e:\\test.sce")->getEManager();
	auto eman = engine->getSceneManager()->getActiveScene()->getEManager();
	eman->createLayer("lay1");
	eman->createLayer("lay2");

	for (auto i = 0; i < 250; ++i) {
		eman->createEntity();
	}

	auto ent1 = eman->createEntity("ent1")->getHandle();
	auto ent2 = eman->createEntity("ent2")->getHandle();
	eman->addEntityToLayer(ent1, "");
	eman->addEntityToLayer(ent1, "lay1");
	eman->addEntityToLayer(ent2, "lay1");
	eman->addEntityToLayer(ent1, "lay2");

	eman->removeLayer("lay1");

	engine->getSceneManager()->getActiveScene()->saveStream(&stream, "e:\\test.sce");

	auto scene = (KScene *)rman.load("KFIStream", "KScene", "e:\\test.sce");

	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;
	return 0;
}