#include <Kite\engine\kengine.h>
#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>
#include <Kite/graphic/kimage.h>
#include <Kite/graphic/kcameracom.h>
#include <Kite/graphic/kgraphicdef.h>
#include <Kite/graphic/kquadcom.h>
#include <Kite/graphic/krendercom.h>
#include <Kite/memory/kpoolstorage.h>

using namespace Kite;

void koutput(const std::string &Text) {
	printf(Text.c_str());
}

int main() {


	KFOStream stream;
	KFIStream istream;

	auto v = sizeof(Internal::KGLVertex);
	auto c = sizeof(KVector2F32);

	auto sc = new KConfig();
	/*sc->window.width = 800;
	sc->window.height = 600;
	sc->dictionary = "C:\\Users\\_M1L4D_\\Desktop\\kite2d\\ff\\dict.kdict";
	sc->startUpScene = "iii.sce";*/

	auto engine = KEngine::createEngine();
	engine->inite(sc, false);

	auto rman = engine->getResourceManager();
	auto eman = engine->getSceneManager()->getActiveScene()->getEManager();
	auto ent2 = eman->createEntity("ent2");

	ent2->addComponent("Logic", "l1");
	ent2->addComponent("Logic", "l2");
	ent2->addComponent("Logic", "l3");

	ent2->removeComponent("Logic", "l2");
	ent2->removeComponent("Logic", "l1");

	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;
	return 0;
}