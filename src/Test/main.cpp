#include <Kite\core\kcore.h>
#include <Kite\window\kwindow.h>
#include <Kite\input\kkeyboard.h>
#include <Kite\input\kmouse.h>
#include <Kite\math\ktransformsys.h>
#include <Kite\math\ktransformcom.h>
#include <cstdio>

using namespace Kite;
int main() {
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
	eman.registerType<KTransformCom>();

	KTransformSys trsys;
	trsys.inite();

	auto ent = eman.createEntity("MyEnt");
	eman.createEntity("MyEnt3");
	eman.createEntity("MyEnt4");
	eman.createEntity("MyEnt5");
	eman.createEntity("MyEnt6");


	eman.createComponent<KTransformCom>("MyEnt3");
	eman.createComponent<KTransformCom>("MyEnt2");
	eman.createComponent<KTransformCom>("MyEnt");
	eman.createComponent<KTransformCom>("MyEnt5");
	eman.createComponent<KTransformCom>("MyEnt5");
	eman.createComponent<KTransformCom>("MyEnt6");


	eman.removeComponent<KTransformCom>(ent);
	eman.removeComponent<KTransformCom>("MyEnt2");
	//eman.removeComponent<KTransformCom>("MyEnt3");
	eman.removeComponent<KTransformCom>("MyEnt5");
	eman.removeComponent<KTransformCom>("MyEnt6");

	//eman.removeEntity("MyEnt");

	auto ptr = eman.getComponent<KTransformCom>("MyEnt3");

	while (win.update()) {
		if (KKeyboard::isButtonPressed(KCT_E)) {
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

		trsys.update(0, eman);
	}
	printf("dsdsd");
	return 0;
}