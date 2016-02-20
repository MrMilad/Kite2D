
// ----[auto generated: KComManager body macro]----
#define KMETA_KCOMMANAGER_BODY() \
private:\
public:\
KITE_FUNC_EXPORT static void registerMeta(lua_State *Lua = nullptr, U32 Flag = 0);\
friend KBytesArray &operator<<(KBytesArray &Out, KComManager& Value) {\
Value.serial(Out, KST_SERIALIZE); return Out;}\
friend KBytesArray &operator>>(KBytesArray &In, KComManager& Value) {\
Value.serial(In, KST_DESERIALIZE); return In;}\
friend KBytesArray &operator<<(KBytesArray &Out, KComManager* Value) {\
Value->serial(Out, KST_SERIALIZE); return Out;}\
friend KBytesArray &operator>>(KBytesArray &In, KComManager* Value) {\
Value->serial(In, KST_DESERIALIZE); return In;}\
private:\
KITE_FUNC_EXPORT void serial(KBytesArray &Serializer, KSerialStateTypes State);

// ----[auto generated: KComManager source macro]----
#define KMETA_KCOMMANAGER_SOURCE()\
void KComManager::registerMeta(lua_State *Lua, U32 Flag){\
static bool inite = true;\
static KMetaClass instance("KComManager",(), sizeof(KComManager));\
if (inite) {\
KMetaManager::setMeta(std::type_index(typeid(KComManager)), (KMeta *)&instance);\
KMetaFunction *fun;\
if (Lua != nullptr && (Flag & SCRIPTABLE)) { \
LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<KComManager>("KComManager")\
.addConstructor(LUA_ARGS())\
.endClass().endModule();}\
} inite = false;}\
void KComManager::serial(KBytesArray &Serializer, KSerialStateTypes State){\
if (State == KST_SERIALIZE) {\
} else if (State == KST_DESERIALIZE) {\
}}
