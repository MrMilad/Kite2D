#include <Kite\engine\kengine.h>

using namespace Kite;
int main() {
	KFIStream fstream;
	fstream.open("E:\\loader.txt", KIOTypes::KRT_BIN);

	KScript loader("Loader");
	loader.loadStream(fstream);

	KEngine engine;

	engine.inite(loader, true);

	engine.start();

	engine.shutdown();
	return 0;
}