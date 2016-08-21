#include <Kite\engine\kengine.h>
#include <Kite/graphic/kshaderprogram.h>
#include <Kite/graphic/ktexture.h>
#include <time.h>
#include <Kite/graphic/kimage.h>
#include <Kite/graphic/kcameracom.h>
#include <Kite/graphic/kgraphicdef.h>
#include <Kite/graphic/kquadcom.h>
#include <Kite/graphic/krendercom.h>
#include <Kite/memory/kpoolstorage.h>

using namespace Kite;

void koutput(const std::string &Text) {
	printf(Text.c_str());
}

int main() {


	int a = 0;

	auto b = --a;
	KFOStream stream;
	KFIStream istream;

	auto v = sizeof(Internal::KGLVertex);
	auto c = sizeof(KVector2F32);

	auto sc = new KConfig();
	sc->window.width = 800;
	sc->window.height = 600;
	sc->dictionary = "C:\\Users\\_M1L4D_\\Desktop\\ff\\dict.kdict";
	sc->startUpScene = "iii.sce";

	auto engine = KEngine::createEngine();
	engine->inite(sc, false);

	/*auto rman = engine->getResourceManager();
	auto eman = engine->getSceneManager()->getActiveScene()->getEManager();
	auto ent2 = eman->createEntity("ent2");


	auto vert = (KShader *)rman->load("KFIStream", "KShader", "F:\\KEngineLab\\Lab\\develop\\shader\\vert.vert");
	auto frag = (KShader *)rman->load("KFIStream", "KShader", "F:\\KEngineLab\\Lab\\develop\\shader\\frag.frag");
	auto shprog = (KShaderProgram *)rman->createAndRegist("KShaderProgram", "shprog");
	shprog->setShader(vert, ShaderType::VERTEX);
	shprog->setShader(frag, ShaderType::FRAGMENT);

	auto eman = engine->getSceneManager()->getActiveScene()->getEManager();

	auto cam = (KCameraCom *)eman->createEntity("ent1")->addComponent("Camera");
	cam->setClearColor(KColor(10, 30, 50, 255));
	cam->setSize(KVector2U32(800, 600));
	//cam->rotate(10);
	//cam->setPosition(KVector2F32(-10, -100));
	//cam->setCenter(KVector2F32(400, 300));

	//auto pt = KTransform::transformPoint(cam->getMatrix(), KVector2F32(1.0f, 1.0f));

	auto ent2 = eman->createEntity("ent2"); 
	auto quad = (KQuadCom *)ent2->addComponent("Quad");
	quad->setWidth(100);
	quad->setHeight(100);
	quad->setColor(KColor::fromName(Colors::CADETBLUE));
	//quad->setBorder(2);
	quad->setBorderColor(KColor::fromName(Colors::RED));

	auto render = (KRenderCom *)ent2->getComponentByName("Render", "");
	render->setShader(shprog->getNameID());

	auto tr = (KTransformCom *)ent2->getComponentByName("Transform", "");
	//tr->setRotation(45);
	//tr->setPosition(KVector2F32(100, 100));*/

	engine->start();
	engine->shutdown();
	delete engine;
	delete sc;
	return 0;
}