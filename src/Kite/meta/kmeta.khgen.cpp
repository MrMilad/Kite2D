
// ----[auto generated source file]----
#include "Kite/Core/meta/kmetapod.h"
#include "graphic/kgraphicstructs.h"
#include "gameplay/kcomponent.h"
#include "gameplay/kentity.h"
#include "gameplay/kmessage.h"
#include "gameplay/kmessagehandler.h"
#include "gameplay/kmessenger.h"
#include "kmeta.khgen.h"
namespace Kite{
void registerKiteMeta(lua_State *Lua){
Internal::registerMetaPOD();
KColor::registerMeta(Lua);
KComponent::registerMeta(Lua);
KEntity::registerMeta(Lua);
KMessage::registerMeta(Lua);
KMessageHandler::registerMeta(Lua);
KMessenger::registerMeta(Lua);
Internal::registerMeta_myEnum(Lua);
Internal::registerMeta_myEnum2(Lua);
}}