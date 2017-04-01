#include <Kite/ecs/kecs.h>
#include <Kite/core/kcore.h>
#include <Kite/math/ktransformcom.h>
#include <Kite/logic/klogic.h>
#include <Kite/engine/kengine.h>
/*#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>
#include <Kite/graphic/katlastexturearray.h>
#include <Kite/graphic/kimage.h>
#include <Kite/graphic/kcameracom.h>
#include <Kite/graphic/kgraphicdef.h>
#include <Kite/graphic/kquadcom.h>
#include <Kite/graphic/krendercom.h>
#include <Kite/graphic/korthogonalmap.h>
#include <src/Kite/graphic/glcall.h>*/

using namespace Kite;

void run(KEngine *engine) {
	KFOStream fs;

	auto rman = engine->getResourceManager();
	rman->registerName("script2", "e:\\lua2.txt", Resource::SCRIPT, InStream::FILEINSTREAM);
	rman->registerName("script", "e:\\lua.txt", Resource::SCRIPT, InStream::FILEINSTREAM);
	rman->registerName("hir", "e:\\hir.txt", Resource::NODE, InStream::FILEINSTREAM);
	//rman->registerName("script2", "e:\\lua2.txt", Resource::SCRIPT, InStream::FILEINSTREAM);

	auto root = new KNode("my node");
	auto logic = (KLogicCom *)root->addChild()->addComponent(Component::LOGIC, "myLogic");
	logic->setScript(rman->load("script"));

	auto child = root->addChild();
	child->setName("child");
	//logic = (KLogicCom *)child->addComponent(Component::LOGIC, "myLogic");
	//logic->setScript(rman->load("script2"));
	//root->saveStream(KFOStream(), "e:\\hir.txt");

	KSharedResource hierarchy(root);
	engine->setActiveHierarchy(hierarchy);
}

int main() {
	//KMetaManager mman;
	//KResourceManager rman;
	KConfig conf;
	conf.window.width = 800;
	conf.window.height = 600;
	conf.window.swapInterval = 0;
	conf.window.fixFPS = true; 
	conf.window.maxFPS = 1000;	// ignored if fixFPS disabled
	KEngine *engine = nullptr;
	try {
		engine = new KEngine(conf);
		auto logic = (KLogicSys *)engine->getSystem(System::LOGICSYSTEM);
		logic->setUpdatePerFrame(true);
	}
	catch (const std::string &Msg) {
		printf("systems creation failed: msg: %s", Msg.c_str());
	}
	
	run(engine);
	engine->start();

	delete engine;
	return 0;
}