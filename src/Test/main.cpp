#include <Kite\engine\kengine.h>
#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>

using namespace Kite;

void koutput(const std::string &Text) {
	printf(Text.c_str());
}

int main() {
	KFOStream stream;
	KFIStream istream;
	KResourceManager rman;
	registerRTypes(&rman);

	auto sc = new KConfig();
	auto engine = KEngine::createEngine();

	engine->inite(sc, false);

	//auto tex = (KTexture *)rman.create("KTexture", "text");
	auto tex = rman.load("KFIStream", "KTexture",  "C:\\Users\\_M1L4D_\\Desktop\\hh\\resources\\qwe");
	tex->inite();

	engine->start();

	engine->shutdown();
	delete engine;
	delete sc;

	return 0;
}