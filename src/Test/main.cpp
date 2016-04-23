#include <Kite\engine\kengine.h>

using namespace Kite;
int main() {
	KFIStream fstream;
	fstream.open("E:\\loader.txt", KIOTypes::KRT_BIN);
	std::string code;

	code.resize(fstream.getSize());
	fstream.read(&code[0], fstream.getSize());

	KScript loader("Loader");
	loader.setCode(code);

	KEntityManager eman;
	eman.createEntity("1");
	eman.createEntity("2");
	eman.createEntity("3");
	eman.createEntity("4");
	eman.createEntity("5");
	eman.createEntity("6");
	eman.createEntity("7");
	eman.createEntity("8");

	eman.getEntityByName("2")->addChild(eman.getEntityByName("3")->getHandle());

	eman.removeEntityByName("2");
	eman.postWork();

	KEngine engine;

	engine.inite(loader, true);

	engine.start();

	engine.shutdown();
	return 0;
}