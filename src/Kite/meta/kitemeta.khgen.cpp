
// ----[auto generated source file]----
#include "graphic/kgraphicstructs.h"
#include "gameplay/kcommanager.h"
#include "gameplay/kmessagehandler.h"
#include "gameplay/kmessenger.h"
namespace Kite{
void registerKiteMeta(lua_State *Lua, U32 Flag){
KColor::registerMeta(Lua, Flag);
KComManager::registerMeta(Lua, Flag);
KMessageHandler::registerMeta(Lua, Flag);
KMessenger::registerMeta(Lua, Flag);
Internal::registerMeta_myEnum(Lua);
Internal::registerMeta_myEnum2(Lua);
}}