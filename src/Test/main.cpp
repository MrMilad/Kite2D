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

int main() {
	KFOStream stream;
	KFIStream istream;
	auto sc = new KConfig();

	sc->window.width = 800;
	sc->window.height = 600;
	sc->dictionary = "C:\\Users\\_M1L4D_\\Desktop\\ff\\dict.kdict";
	sc->startUpScene = "sss.sce";

	auto engine = KEngine::createEngine();
	engine->inite(sc, false);
	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;
	return 0;
}