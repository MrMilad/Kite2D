#include <Kite\engine\kengine.h>
#include <time.h>

using namespace Kite;
int main() {

	/* 
	srand(time(NULL));
	auto num = rand() % 99999999 + 10000000;
	*/

	KMetaManager mman;
	KMetaManager mman2;
	registerKiteMeta(&mman);
	registerKiteMeta(&mman2);

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
		Kite::KScene scene("asdssa");
		Kite::registerCTypes(scene.getEManager());
		auto ent1 = scene.getEManager()->createEntity("ent1");
		scene.getEManager()->createEntity("ent2");
		scene.getEManager()->createEntity("ent3");
		scene.getEManager()->createEntity("ent4");
		scene.getEManager()->createEntity("ent5");
		scene.getEManager()->getEntity(ent1)->addComponent("Logic", "Log1");
		scene.getEManager()->getEntity(ent1)->addComponent("Transform", "Tr");
		scene.getEManager()->getEntity(ent1)->addComponent("Input", "Inp");
		scene.saveStream(&fostream);
	}
	fostream.close();

	if (fostream.open("E:\\script.kres", KIOTypes::KRT_BIN)) {
		loader.saveStream(&fostream,0, "12345678");
	}
	fostream.close();

	Kite::KScene scene("");
	if (fstream.open("C:/Users/_M1L4D_/Desktop/kc/resources/sen2.kres", KIOTypes::KRT_BIN)) {
		Kite::registerCTypes(scene.getEManager());
		scene.loadStream(&fstream);
	}

	KEngine engine;

	engine.inite(loader, true);

	engine.start();

	engine.shutdown();
	return 0;
}