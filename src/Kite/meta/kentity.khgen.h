
// ----[auto generated: KEntity body macro]----
#define KMETA_KENTITY_BODY() \
private:\
public:\
static void registerMeta(lua_State *Lua = nullptr);\
const std::string &getClassName() const { static std::string name("KEntity");\
return name;}\

// ----[auto generated: KEntity source macro]----
#define KMETA_KENTITY_SOURCE()\
void KEntity::registerMeta(lua_State *Lua){\
static bool inite = true;\
static KMetaClass instance("KEntity",(SCRIPTABLE| SERIALIZABLE), sizeof(KEntity));\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
instance.addBase(new KMetaBase("KObject", KMB_PUBLIC));\
instance.addProperty(new KMetaProperty("ID", "U32", "entity unique ID", KMP_GETTER));\
KMetaFunction *fun;\
fun = new KMetaFunction("addComponentByType", "void", false);\
fun->paramsType.push_back("KComponentTypes");\
instance.addFunction(fun);\
fun = new KMetaFunction("addComponent", "void", false);\
fun->paramsType.push_back("KComponent");\
instance.addFunction(fun);\
fun = new KMetaFunction("removeComponentByType", "void", false);\
fun->paramsType.push_back("KComponentTypes");\
instance.addFunction(fun);\
fun = new KMetaFunction("removeComponentByName", "void", false);\
fun->paramsType.push_back("char");\
instance.addFunction(fun);\
fun = new KMetaFunction("removeComponent", "void", false);\
fun->paramsType.push_back("KComponent");\
instance.addFunction(fun);\
fun = new KMetaFunction("getComponentByType", "KComponent", false);\
fun->paramsType.push_back("KComponentTypes");\
instance.addFunction(fun);\
fun = new KMetaFunction("getComponentByName", "KComponent", false);\
fun->paramsType.push_back("std::string");\
instance.addFunction(fun);\
fun = new KMetaFunction("hasComponentByType", "bool", false);\
fun->paramsType.push_back("KComponentTypes");\
instance.addFunction(fun);\
fun = new KMetaFunction("hasComponentByName", "bool", false);\
fun->paramsType.push_back("std::string");\
instance.addFunction(fun);\
const KMetaObject *minfo = KMetaManager::getMeta("KEntity");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)) { \
LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<KEntity>("KEntity")\
.addConstructor(LUA_ARGS(LuaIntf::_def<U32, 0>))\
.addProperty("ID", &KEntity::getID)\
.addFunction("addComponentByType", &KEntity::addComponentByType)\
.addFunction("addComponent", &KEntity::addComponent)\
.addFunction("removeComponentByType", &KEntity::removeComponentByType)\
.addFunction("removeComponentByName", &KEntity::removeComponentByName)\
.addFunction("removeComponent", &KEntity::removeComponent)\
.addFunction("getComponentByType", &KEntity::getComponentByType)\
.addFunction("getComponentByName", &KEntity::getComponentByName)\
.addFunction("hasComponentByType", &KEntity::hasComponentByType)\
.addFunction("hasComponentByName", &KEntity::hasComponentByName)\
.endClass().endModule();}\
} inite = false;}
