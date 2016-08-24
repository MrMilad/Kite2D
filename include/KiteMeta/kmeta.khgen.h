
// ----[auto generated header file]----
#ifndef KMETA_H
#define KMETA_H

#include "Kite/core/kcoredef.h"
#include "Kite/core/ksystem.h"
#include "Kite/meta/kmetadef.h"

#include <vector>
#include <memory>
KMETA
namespace Kite{
class KMetaManager;
class KEntityManager;
class KResourceManager;
KITE_FUNC_EXPORT extern void registerKiteMeta(KMetaManager *MMan = nullptr, lua_State *Lua = nullptr);
KITE_FUNC_EXPORT extern void registerCTypes(KEntityManager *EMan);
KITE_FUNC_EXPORT extern void registerRTypes(KResourceManager *RMan);
KITE_FUNC_EXPORT extern void createSystems(std::vector<std::unique_ptr<KSystem>> &Systems);
}
#endif // KITEMETA_H