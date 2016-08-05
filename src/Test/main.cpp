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

	KAny any(GLPrimitive::LINES);
	//int f = (int)GLPrimitive::LINES;
	int f = any.asFreeCast<int>();

	engine->inite(sc, false);

	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;

	return 0;
}