#include <Kite\window\kwindow.h>
#include <Kite\input\kkeyboard.h>
#include <Kite\input\kmouse.h>
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
	}
	printf("dsdsd");
	return 0;
}