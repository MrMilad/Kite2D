#include <Kite\core\kcore.h>
#include <Kite\window\kwindow.h>
#include <Kite\input\kkeyboard.h>
#include <Kite\input\kmouse.h>
#include <Kite\input\kinputsys.h>
#include <Kite\input\kinputcom.h>
#include <Kite\logic\klogicsys.h>
#include <Kite\logic\klogiccom.h>
#include <Kite\math\ktransformsys.h>
#include <Kite\math\ktransformcom.h>
#include <Kite/serialization/kbinaryserial.h>
#include <cstdio>
#include <luaintf\LuaIntf.h>
#include <Kite/core/kvariant.h>
#include <kmeta.khgen.h>

using namespace Kite;
int main() {
	lua_State *vm = luaL_newstate();
	static const luaL_Reg lualibs[] =
	{
		{ "base", luaopen_base },
		{ NULL, NULL }
	};

	const luaL_Reg *lib = lualibs;
	for (; lib->func != NULL; lib++) {
		lib->func(vm);
		lua_settop(vm, 0);
	}

	KCFStorage<KTransformCom> storage;
	auto trc1 = storage.add(KTransformCom());
	auto trc2 = storage.add(KTransformCom());
	auto trc3 = storage.add(KTransformCom());
	storage.remove(trc2);
	auto trc4 = storage.add(KTransformCom());
	auto trc5 = storage.add(KTransformCom());
	auto trc6 = storage.add(KTransformCom());
	storage.remove(trc5);

	KBinarySerial bserial;
	//bserial << storage;

	storage.clear();

	//bserial >> storage;

	KWindowState state;
	state.title = "salam";
	state.width = 800;
	state.height = 600;
	state.xpos = 100;
	state.ypos = 100;

	KGLWindow win(state);
	win.open();

	KKeyboard::initeKeyboard();
	KMouse::initeMouse();

	KMetaManager mman;
	KEntityManager eman;
	KEntityManager eman2;
	KResourceManager rman;
	std::vector<std::unique_ptr<KSystem>> sys;

	registerKiteMeta(&mman, vm);
	registerCTypes(eman);
	createSystems(sys);

	for (auto it = sys.begin(); it != sys.end(); ++it) {
		(*it)->inite((void *)vm);
	}

	auto scrip = rman.load<KScript, KFIStream>("e:\\lt.txt", false);
	auto scrip2 = rman.load<KScript, KFIStream>("e:\\lt2.txt", false);


	for (U32 i = 0; i < 10; ++i) {
		auto ent = eman.createEntity(std::string("ent" + std::to_string(i)));
		auto comp = eman.getEntity(ent)->addComponent("Logic", "logcomp");
		auto compptr = (KLogicCom *)eman.getEntity(ent)->getComponent("Logic", comp);
		compptr->setScript("e:\\lt.txt");
	}
	/*auto ent1 = eman.createEntity("MyEnt1");
	auto ent2 = eman.createEntity("MyEnt2");
	auto ent3 = eman.createEntity("MyEnt3");
	auto ent4 = eman.createEntity("MyEnt4");
	auto ent5 = eman.createEntity("MyEnt5");
	auto ent6 = eman.createEntity("MyEnt6");

	eman.getEntity(ent1)->removeChild(ent3);
	eman.getEntity(ent1)->removeChild(ent1);
	eman.getEntity(ent1)->addChild(ent2);
	eman.getEntity(ent1)->addChild(ent3);
	eman.getEntity(ent1)->addChild(ent1);
	eman.getEntity(ent1)->addChild(ent3);
	eman.getEntity(ent1)->addChild(ent4);

	//eman.removeEntity(ent1);

	eman.getEntity(ent2)->addChild(ent4);
	eman.getEntity(ent2)->addChild(ent5);
	eman.getEntity(ent2)->addChild(ent6);

	eman.getEntity(ent1)->addComponent("Transform");
	eman.getEntity(ent2)->addComponent("Input");
	eman.getEntity(ent1)->addComponent("Logic", "ent1comp");
	eman.getEntity(ent2)->addComponent("Logic", "lcomp");
	eman.getEntity(ent2)->addComponent("Logic", "lcomp2");
	eman.getEntity(ent3)->addComponent("Logic", "ent3comp");
	eman.getEntity(ent4)->addComponent("Logic", "ent4comp");
	eman.getEntity(ent5)->addComponent("Logic", "ent5comp");

	eman.removeEntityByName("MyEnt");

	KLogicCom *lcomp0 = (KLogicCom *)eman.getEntityByName("MyEnt1")->getComponentByName("Logic", "ent1comp");
	lcomp0->setScript("e:\\lt.txt");

	KLogicCom *lcomp = (KLogicCom *)eman.getEntityByName("MyEnt2")->getComponentByName("Logic", "lcomp");
	lcomp->setScript("e:\\lt.txt");

	KLogicCom *lcomp2 = (KLogicCom *)eman.getEntityByName("MyEnt2")->getComponentByName("Logic", "lcomp2");
	lcomp2->setScript("e:\\lt.txt");

	KLogicCom *lcomp3 = (KLogicCom *)eman.getEntityByName("MyEnt3")->getComponentByName("Logic", "ent3comp");
	lcomp3->setScript("e:\\lt.txt");

	KLogicCom *lcomp4 = (KLogicCom *)eman.getEntityByName("MyEnt4")->getComponentByName("Logic", "ent4comp");
	lcomp4->setScript("e:\\lt.txt");

	KLogicCom *lcomp5 = (KLogicCom *)eman.getEntityByName("MyEnt5")->getComponentByName("Logic", "ent5comp");
	lcomp5->setScript("e:\\lt.txt");
	
	bserial << eman;
	//eman.removeEntity("MyEnt");

	for (auto it = eman.beginComponent<KLogicCom>("Logic");
	it != eman.endComponent<KLogicCom>("Logic"); ++it) {
		printf("comp name: %s\n", (*it).getName().c_str());
	}*/

	KMessage msg;
	msg.setType("TEST_TYPE");
	
	while (win.update()) {
		if (KMouse::isAnyKeyDown() && KKeyboard::isButtonPressed(KKCode::U)) {
			printf("e pressed\n");
		}

		if (KKeyboard::isButtonPressed(KKCode::S)) {
			printf("S pressed\n");
		}

		if (KMouse::isButtonPressed(KMButton::MIDDLE)) {
			KVector2I32 pos = KMouse::getWheelValue();
			printf("lmouse pressed xpos: %i  y pos: %i\n", pos.x, pos.y);
		}


		if (KKeyboard::getKeyboardState()[(U32)KKCode::U] && KKeyboard::isModifierPressed(KKModifier::LCTRL)) {
			printf("u + lctrl pressed\n");
		}

		for (auto it = sys.begin(); it != sys.end(); ++it) {
			(*it)->update(0, eman, rman);
		}

		//eman.getEntity(ent2)->onMessage(msg, KMessageScopeTypes::KMS_SELF);
	}
	return 0;
}