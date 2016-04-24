#include <Kite\engine\kengine.h>

using namespace Kite;
int main() {
	KFIStream fstream;
	fstream.open("E:\\loader.txt", KIOTypes::KRT_BIN);
	std::string code;

	code.resize(fstream.getSize());
	fstream.read(&code[0], fstream.getSize());
	fstream.close();

	KScript loader("Loader");
	loader.setCode(code);

	KFOStream fostream;
	if (fostream.open("E:\\kc.kres", KIOTypes::KRT_BIN)) {
		if (fostream.isOpen()) {
			Kite::KScene scene("asdssa");
			scene.saveStream(&fostream);
		}
	}

	KEngine engine;

	engine.inite(loader, true);

	engine.start();

	engine.shutdown();
	return 0;
}