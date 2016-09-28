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
	auto temp = sizeof(KOrthoTileNode);
	auto temp2 = sizeof(KRootTileMap);

	KFOStream stream;
	KFIStream istream;
	auto sc = new KConfig();

	KAtlasTextureArray at("sfdf");
	at.saveStream(stream, "e:\\ff", false);

	KOrthogonalMap orthoMap("ggg");
	orthoMap.create(4, 4, 10, 10);

	U32 out;
	orthoMap.getTileID(KVector2F32(33, 0), out);
	out = orthoMap.convertID(out);

	sc->window.width = 800;
	sc->window.height = 600;
	//sc->dictionary = "C:\\Users\\_M1L4D_\\Desktop\\ff\\dict.kdict";
	//sc->startUpScene = "sss.sce";

	auto engine = KEngine::createEngine();
	engine->inite(sc, false);

	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;
	return 0;
}