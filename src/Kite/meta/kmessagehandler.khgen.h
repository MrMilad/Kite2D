
// ----[auto generated: KMessageHandler body macro]----
#define KMETA_KMESSAGEHANDLER_BODY() \
private:\
public:\
static void registerMeta(lua_State *Lua = nullptr);\
const std::string &getClassName() const { static std::string name("KMessageHandler");\
return name;}\

// ----[auto generated: KMessageHandler source macro]----
#define KMETA_KMESSAGEHANDLER_SOURCE()\
void KMessageHandler::registerMeta(lua_State *Lua){\
static bool inite = true;\
static KMetaClass instance("KMessageHandler",(SCRIPTABLE), sizeof(KMessageHandler));\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
instance.addProperty(new KMetaProperty("ID", "U32", "Message handler owner ID (Entity or Component ID)", KMP_GETTER));\
instance.addProperty(new KMetaProperty("Enable", "bool", "enable/disable recieving message", KMP_BOTH));\
const KMetaObject *minfo = KMetaManager::getMeta("KMessageHandler");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)) { \
LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<KMessageHandler>("KMessageHandler")\
.addProperty("ID", &KMessageHandler::getMsgHandlerID)\
.addProperty("Enable", &KMessageHandler::getEnableRecieveMsg, &KMessageHandler::setEnableRecieveMsg)\
.endClass().endModule();}\
} inite = false;}
