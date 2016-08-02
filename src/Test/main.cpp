#include <Kite\engine\kengine.h>
#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>
#include <Kite/graphic/kimage.h>

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

	//auto pre = rman.create("KTexture", "texure");
	//pre->saveStream(&stream, "e:\\tex.tex");
	auto tex = rman.load("KFIStream", "KTexture", "e:\\tex.tex");
	tex->saveStream(&stream, "f:\\test.tex");
	//tex->inite();

	engine->start();

	engine->shutdown();
	delete engine;
	delete sc;

	return 0;
}