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
#include <cstdio>
#include <luaintf\LuaIntf.h>

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

	KEntityManager eman;
	eman.registerComponent<KTransformCom>(KComTypes::KCT_TRANSFORM);
	eman.registerComponent<KInputCom>(KComTypes::KCT_INPUT);
	eman.registerComponent<KLogicCom>(KComTypes::KCT_LOGIC);

	KResourceManager rman;

	auto scrip = rman.load<KScript, KFIStream>("e:\\lt.txt", false);
	auto scrip2 = rman.load<KScript, KFIStream>("e:\\lt2.txt", false);

	KTransformSys trsys;
	trsys.inite(nullptr);

	KInputSys insys;
	insys.inite(nullptr);

	KLogicSys lsys;
	lsys.inite((void *)vm);


	auto ent1 = eman.createEntity("MyEnt1");
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

	eman.removeEntity(ent1);

	eman.getEntity(ent2)->addChild(ent4);
	eman.getEntity(ent2)->addChild(ent5);
	eman.getEntity(ent2)->addChild(ent6);

	eman.getEntity(ent1)->addComponent(KComTypes::KCT_TRANSFORM);
	eman.getEntity(ent2)->addComponent(KComTypes::KCT_INPUT);
	eman.getEntity(ent2)->addComponent(KComTypes::KCT_LOGIC, "lcomp");
	eman.getEntity(ent2)->addComponent(KComTypes::KCT_LOGIC, "lcomp2");

	eman.removeEntity("MyEnt");

	KLogicCom *lcomp = (KLogicCom *)eman.getEntity("MyEnt2")->getComponent(KComTypes::KCT_LOGIC, "lcomp");
	lcomp->setScript("e:\\lt.txt");

	KLogicCom *lcomp2 = (KLogicCom *)eman.getEntity("MyEnt2")->getComponent(KComTypes::KCT_LOGIC, "lcomp2");
	lcomp2->setScript("e:\\lt2.txt");
	
	//eman.removeEntity("MyEnt");

	for (auto it = eman.beginComponent<KLogicCom>(KComTypes::KCT_LOGIC);
	it != eman.endComponent<KLogicCom>(KComTypes::KCT_LOGIC); ++it) {
		printf("comp name: %s\n", (*it).getName().c_str());
	}

	while (win.update()) {
		if (KMouse::isAnyKeyDown() && KKeyboard::isButtonPressed(KCT_E)) {
			printf("e pressed\n");
		}

		if (KKeyboard::isButtonPressed(KCT_S)) {
			printf("S pressed\n");
		}

		if (KMouse::isButtonPressed(KMouseButtonTypes::KMB_MIDDLE)) {
			KVector2I32 pos = KMouse::getWheelValue();
			printf("lmouse pressed xpos: %i  y pos: %i\n", pos.x, pos.y);
		}


		if (KKeyboard::getKeyboardState()[KCT_U] && KKeyboard::isModifierPressed(KKeyModifierTypes::KMT_LCTRL)) {
			printf("u + lctrl pressed\n");
		}

		trsys.update(0, eman, rman);
		insys.update(0, eman, rman);
		lsys.update(0, eman, rman);
	}
	return 0;
}