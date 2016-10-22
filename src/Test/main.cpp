#include <Kite\engine\kengine.h>
#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>
#include <Kite/graphic/katlastexturearray.h>
#include <Kite/graphic/kimage.h>
#include <Kite/graphic/kcameracom.h>
#include <Kite/graphic/kgraphicdef.h>
#include <Kite/graphic/kquadcom.h>
#include <Kite/graphic/krendercom.h>
#include <Kite/memory/kpoolstorage.h>
#include <Kite/graphic/korthogonalmap.h>
#include <src/Kite/graphic/glcall.h>

using namespace Kite;

int main() {
	auto temp2 = sizeof(KOrthoNode);

	KBinarySerial bserial;
	KFOStream stream;
	KFIStream istream;
	auto sc = new KConfig();

	sc->window.xpos = 10;
	sc->window.ypos = 10;
	sc->window.width = 800;
	sc->window.height = 600;
	sc->window.fullscreen = false;
	sc->render.camDepth = false;
	sc->render.zSorting = false;
	sc->render.culling = true;
	//sc->window.resizable = true;
	sc->dictionary = "C:\\Users\\_M1L4D_\\Desktop\\K2D\\examples\\Orthogonal Tiled Map\\dict.kdict";
	sc->startUpScene = "map.sce";

	auto engine = KEngine::createEngine();
	engine->inite(sc, false);

	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;
	return 0;
}