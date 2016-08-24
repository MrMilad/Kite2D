
// ----[auto generated source file]----
#include "Kite/meta/kmetaenum.h"
#include "Kite/meta/kmetapod.h"
#include "KiteMeta/kmeta.khgen.h"
#include "KiteMeta/ktypes.khgen.h"
#include "Kite/window/kglwindow.h"
#include "Kite/window/kwindowstructs.h"
#include "Kite/serialization/kbaseserial.h"
#include "Kite/serialization/kbinaryserial.h"
#include "Kite/math/kmathstructs.h"
#include "Kite/math/kmathtypes.h"
#include "Kite/math/kmatrix3.h"
#include "Kite/math/ktransform.h"
#include "Kite/math/ktransformcom.h"
#include "Kite/math/ktransformsys.h"
#include "Kite/logic/klogiccom.h"
#include "Kite/logic/klogicinstancecom.h"
#include "Kite/logic/klogicsys.h"
#include "Kite/logic/kscript.h"
#include "Kite/input/kinputcom.h"
#include "Kite/input/kinputsys.h"
#include "Kite/input/kinputtypes.h"
#include "Kite/input/kkeyboard.h"
#include "Kite/input/kmouse.h"
#include "Kite/graphic/katlastexture.h"
#include "Kite/graphic/kcameracom.h"
#include "Kite/graphic/kgraphicstructs.h"
#include "Kite/graphic/kgraphictypes.h"
#include "Kite/graphic/kquadcom.h"
#include "Kite/graphic/krendercom.h"
#include "Kite/graphic/krendersys.h"
#include "Kite/graphic/kshader.h"
#include "Kite/graphic/kshaderprogram.h"
#include "Kite/graphic/ktexture.h"
#include "Kite/engine/kengine.h"
#include "Kite/core/kcomponent.h"
#include "Kite/core/kcorestructs.h"
#include "Kite/core/kcoretypes.h"
#include "Kite/core/kentity.h"
#include "Kite/core/kentitymanager.h"
#include "Kite/core/kfistream.h"
#include "Kite/core/kfostream.h"
#include "Kite/core/kistream.h"
#include "Kite/core/klistener.h"
#include "Kite/core/kmessage.h"
#include "Kite/core/kmessenger.h"
#include "Kite/core/kostream.h"
#include "Kite/core/kprefab.h"
#include "Kite/core/kresource.h"
#include "Kite/core/kresourcemanager.h"
#include "Kite/core/kscene.h"
#include "Kite/core/kscenemanager.h"
#include "Kite/core/ksystem.h"
namespace Kite{
void registerKiteMeta(KMetaManager *MMan, lua_State *Lua){
Internal::registerMetaPOD(MMan);
KGLWindow::registerMeta(MMan, Lua);
KWindowState::registerMeta(MMan, Lua);
KEnumDisplay::registerMeta(MMan, Lua);
KBaseSerial::registerMeta(MMan, Lua);
KBinarySerial::registerMeta(MMan, Lua);
KVector2<F32>::registerMeta("KVector2F32", MMan, Lua);
KVector2<I32>::registerMeta("KVector2I32", MMan, Lua);
KVector2<U32>::registerMeta("KVector2U32", MMan, Lua);
KRect<F32>::registerMeta("KRectF32", MMan, Lua);
KRect<I32>::registerMeta("KRectI32", MMan, Lua);
KRect<U32>::registerMeta("KRectU32", MMan, Lua);
KRect2<F32>::registerMeta("KRect2F32", MMan, Lua);
KRect2<I32>::registerMeta("KRect2I32", MMan, Lua);
KRect2<U32>::registerMeta("KRect2U32", MMan, Lua);
KMatrix3::registerMeta(MMan, Lua);
KTransform::registerMeta(MMan, Lua);
KTransformCom::registerMeta(MMan, Lua);
KTransformSys::registerMeta(MMan, Lua);
KLogicCom::registerMeta(MMan, Lua);
KLogicInstanceCom::registerMeta(MMan, Lua);
KLogicSys::registerMeta(MMan, Lua);
KScript::registerMeta(MMan, Lua);
KInputCom::registerMeta(MMan, Lua);
KInputSys::registerMeta(MMan, Lua);
KKeyboard::registerMeta(MMan, Lua);
KMouse::registerMeta(MMan, Lua);
KAtlasTexture::registerMeta(MMan, Lua);
KCameraCom::registerMeta(MMan, Lua);
KColor::registerMeta(MMan, Lua);
KVertex::registerMeta(MMan, Lua);
KAtlasItem::registerMeta(MMan, Lua);
KQuadCom::registerMeta(MMan, Lua);
KRenderCom::registerMeta(MMan, Lua);
KRenderSys::registerMeta(MMan, Lua);
KShader::registerMeta(MMan, Lua);
KShaderProgram::registerMeta(MMan, Lua);
KTexture::registerMeta(MMan, Lua);
KConfig::registerMeta(MMan, Lua);
KEngine::registerMeta(MMan, Lua);
KComponent::registerMeta(MMan, Lua);
KHandle::registerMeta(MMan, Lua);
KStringID::registerMeta(MMan, Lua);
KLayerInfo::registerMeta(MMan, Lua);
KEntity::registerMeta(MMan, Lua);
KEntityManager::registerMeta(MMan, Lua);
KFIStream::registerMeta(MMan, Lua);
KFOStream::registerMeta(MMan, Lua);
KIStream::registerMeta(MMan, Lua);
KListener::registerMeta(MMan, Lua);
KMessage::registerMeta(MMan, Lua);
KMessenger::registerMeta(MMan, Lua);
KOStream::registerMeta(MMan, Lua);
KPrefab::registerMeta(MMan, Lua);
KResource::registerMeta(MMan, Lua);
KResourceManager::registerMeta(MMan, Lua);
KScene::registerMeta(MMan, Lua);
KSceneManager::registerMeta(MMan, Lua);
KSystem::registerMeta(MMan, Lua);
Internal::RegisterOrientation::registerMeta(MMan, Lua);
Internal::RegisterInterpolation::registerMeta(MMan, Lua);
Internal::RegisterKeyCode::registerMeta(MMan, Lua);
Internal::RegisterKeyModifier::registerMeta(MMan, Lua);
Internal::RegisterMouseButton::registerMeta(MMan, Lua);
Internal::RegisterJoystcikID::registerMeta(MMan, Lua);
Internal::RegisterTextureEnv::registerMeta(MMan, Lua);
Internal::RegisterVBufferType::registerMeta(MMan, Lua);
Internal::RegisterBufferTarget::registerMeta(MMan, Lua);
Internal::RegisterMapAccess::registerMeta(MMan, Lua);
Internal::RegisterGLPrimitive::registerMeta(MMan, Lua);
Internal::RegisterColors::registerMeta(MMan, Lua);
Internal::RegisterTextureFilter::registerMeta(MMan, Lua);
Internal::RegisterTextureWrap::registerMeta(MMan, Lua);
Internal::RegisterShaderType::registerMeta(MMan, Lua);
Internal::RegisterAttributeCount::registerMeta(MMan, Lua);
Internal::RegisterAttributeType::registerMeta(MMan, Lua);
Internal::RegisterBuiltinShaderType::registerMeta(MMan, Lua);
Internal::RegisterAtlasFile::registerMeta(MMan, Lua);
Internal::RegisterIOMode::registerMeta(MMan, Lua);
Internal::RegisterPrimitive::registerMeta(MMan, Lua);
Internal::RegisterRecieveTypes::registerMeta(MMan, Lua);
Internal::RegisterMessageScope::registerMeta(MMan, Lua);
Internal::RegisterKCTypes::registerMeta(MMan, Lua);
}
void registerCTypes(KEntityManager *EMan){
EMan->registerComponent<KTransformCom>(KCTypes::Transform);
EMan->registerComponent<KLogicCom>(KCTypes::Logic);
EMan->registerComponent<KLogicInstanceCom>(KCTypes::LogicInstance);
EMan->registerComponent<KInputCom>(KCTypes::Input);
EMan->registerComponent<KCameraCom>(KCTypes::Camera);
EMan->registerComponent<KQuadCom>(KCTypes::Quad);
EMan->registerComponent<KRenderCom>(KCTypes::Render);
}
void registerRTypes(KResourceManager *RMan){
RMan->registerResource("KScript", KScript::factory, false);
RMan->registerResource("KAtlasTexture", KAtlasTexture::factory, false);
RMan->registerResource("KShader", KShader::factory, false);
RMan->registerResource("KShaderProgram", KShaderProgram::factory, false);
RMan->registerResource("KTexture", KTexture::factory, false);
RMan->registerIStream("KFIStream", KFIStream::factory);
RMan->registerResource("KPrefab", KPrefab::factory, false);
RMan->registerResource("KScene", KScene::factory, false);
}
void createSystems(std::vector<std::unique_ptr<KSystem>> &Systems){
Systems.clear();
Systems.push_back(std::unique_ptr<KSystem>(new KTransformSys));
Systems.push_back(std::unique_ptr<KSystem>(new KLogicSys));
Systems.push_back(std::unique_ptr<KSystem>(new KInputSys));
Systems.push_back(std::unique_ptr<KSystem>(new KRenderSys));
}
}
