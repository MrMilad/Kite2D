
// ----[auto generated: KMessage body macro]----
#define KMETA_KMESSAGE_BODY() \
private:\
public:\
static void registerMeta(lua_State *Lua = nullptr);\
const std::string &getClassName() const { static std::string name("KMessage");\
return name;}\

// ----[auto generated: KMessage source macro]----
#define KMETA_KMESSAGE_SOURCE()\
void KMessage::registerMeta(lua_State *Lua){\
static bool inite = true;\
static KMetaClass instance("KMessage",(SCRIPTABLE), sizeof(KMessage));\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
instance.addProperty(new KMetaProperty("type", "std::string", "message type", KMP_BOTH));\
instance.addProperty(new KMetaProperty("getConsume", "bool", "consume state of message", KMP_GETTER));\
instance.addProperty(new KMetaProperty("getHash", "U32", "32 bit hash code of message type", KMP_GETTER));\
instance.addProperty(new KMetaProperty("getSize", "U32", "size of message", KMP_GETTER));\
instance.addProperty(new KMetaProperty("getUse", "U32", "use counter", KMP_GETTER));\
KMetaFunction *fun;\
fun = new KMetaFunction("consume", "void", false);\
instance.addFunction(fun);\
fun = new KMetaFunction("use", "void", false);\
instance.addFunction(fun);\
fun = new KMetaFunction("setDataTable", "void", false);\
fun->paramsType.push_back("std::string");\
instance.addFunction(fun);\
const KMetaObject *minfo = KMetaManager::getMeta("KMessage");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)) { \
LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<KMessage>("KMessage")\
.addConstructor(LUA_ARGS())\
.addProperty("type", &KMessage::getType, &KMessage::setType)\
.addProperty("getConsume", &KMessage::getConsume)\
.addProperty("getHash", &KMessage::getHash)\
.addProperty("getSize", &KMessage::getSize)\
.addProperty("getUse", &KMessage::getUse)\
.addFunction("consume", &KMessage::consume)\
.addFunction("use", &KMessage::use)\
.addFunction("setDataTable", &KMessage::setDataTable)\
.endClass().endModule();}\
} inite = false;}
