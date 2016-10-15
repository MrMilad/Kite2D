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
	sc->render.culling = false;
	//sc->window.resizable = true;
	sc->dictionary = "C:\\Users\\_M1L4D_\\Desktop\\ff\\dict.kdict";
	sc->startUpScene = "fr.sce";

	auto engine = KEngine::createEngine();
	engine->inite(sc, false);
	auto eman = engine->getEntityManager();
	auto ent = eman->getEntityByName("cam");
	auto cam = (KCameraCom *)ent->getComponent(CTypes::Camera);
	cam->addTransformRatio(KCameraTransformRatio(KVector2F32(2.f, 1), 1, 1));
	//engine->getWindow()->toggleFullscreen();

	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;
	return 0;
}