
// ----[auto generated: KMessenger body macro]----
#define KMETA_KMESSENGER_BODY() \
private:\
public:\
static void registerMeta(lua_State *Lua = nullptr);\
const std::string &getClassName() const { static std::string name("KMessenger");\
return name;}\

// ----[auto generated: KMessenger source macro]----
#define KMETA_KMESSENGER_SOURCE()\
void KMessenger::registerMeta(lua_State *Lua){\
static bool inite = true;\
static KMetaClass instance("KMessenger",(SCRIPTABLE), sizeof(KMessenger));\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
instance.addBase(new KMetaBase("KNonCopyable", KMB_PRIVATE));\
KMetaFunction *fun;\
fun = new KMetaFunction("publicSubscribe", "bool", false);\
fun->paramsType.push_back("KMessageHandler");\
instance.addFunction(fun);\
fun = new KMetaFunction("typeSubscribe", "bool", false);\
fun->paramsType.push_back("KMessageHandler");\
fun->paramsType.push_back("std::string");\
instance.addFunction(fun);\
fun = new KMetaFunction("publicUnsubscribe", "void", false);\
fun->paramsType.push_back("KMessageHandler");\
instance.addFunction(fun);\
fun = new KMetaFunction("typeUnsubscribe", "void", false);\
fun->paramsType.push_back("KMessageHandler");\
fun->paramsType.push_back("std::string");\
instance.addFunction(fun);\
fun = new KMetaFunction("send", "U32", false);\
fun->paramsType.push_back("KMessage");\
fun->paramsType.push_back("KMessageScopeTypes");\
instance.addFunction(fun);\
fun = new KMetaFunction("publish", "U32", false);\
fun->paramsType.push_back("KMessage");\
fun->paramsType.push_back("KMessageScopeTypes");\
instance.addFunction(fun);\
fun = new KMetaFunction("sendToQueue", "void", false);\
fun->paramsType.push_back("KMessage");\
fun->paramsType.push_back("KMessageScopeTypes");\
fun->paramsType.push_back("F32");\
instance.addFunction(fun);\
fun = new KMetaFunction("publishToQueue", "void", false);\
fun->paramsType.push_back("KMessage");\
fun->paramsType.push_back("KMessageScopeTypes");\
fun->paramsType.push_back("F32");\
instance.addFunction(fun);\
fun = new KMetaFunction("getPoolSize", "U32", false);\
instance.addFunction(fun);\
fun = new KMetaFunction("getUsedPoolSize", "U32", false);\
instance.addFunction(fun);\
fun = new KMetaFunction("discardQueue", "void", false);\
instance.addFunction(fun);\
fun = new KMetaFunction("reset", "void", false);\
instance.addFunction(fun);\
const KMetaObject *minfo = KMetaManager::getMeta("KMessenger");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)) { \
LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<KMessenger>("KMessenger")\
.addFunction("publicSubscribe", &KMessenger::publicSubscribe)\
.addFunction("typeSubscribe", &KMessenger::typeSubscribe)\
.addFunction("publicUnsubscribe", &KMessenger::publicUnsubscribe)\
.addFunction("typeUnsubscribe", &KMessenger::typeUnsubscribe)\
.addFunction("send", &KMessenger::send)\
.addFunction("publish", &KMessenger::publish)\
.addFunction("sendToQueue", &KMessenger::sendToQueue)\
.addFunction("publishToQueue", &KMessenger::publishToQueue)\
.addFunction("getPoolSize", &KMessenger::getPoolSize)\
.addFunction("getUsedPoolSize", &KMessenger::getUsedPoolSize)\
.addFunction("discardQueue", &KMessenger::discardQueue)\
.addFunction("reset", &KMessenger::reset)\
.endClass().endModule();}\
} inite = false;}
