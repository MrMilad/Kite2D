#include <Kite/ecs/kecs.h>
/*#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>
#include <Kite/graphic/katlastexturearray.h>
#include <Kite/graphic/kimage.h>
#include <Kite/graphic/kcameracom.h>
#include <Kite/graphic/kgraphicdef.h>
#include <Kite/graphic/kquadcom.h>
#include <Kite/graphic/krendercom.h>
#include <Kite/memory/kpoolstorage.h>
#include <Kite/graphic/korthogonalmap.h>
#include <src/Kite/graphic/glcall.h>*/

using namespace Kite;

int main() {
	KNode root("my node");
	auto ch1 = root.addNewChild();
	auto ch2 = root.addNewChild()->addNewChild();
	root.addNewChild()->addNewChild()->addNewChild();
	root.addNewChild();

	root.removeChild(ch2);
	root.removeChild(ch1);

	return 0;
}