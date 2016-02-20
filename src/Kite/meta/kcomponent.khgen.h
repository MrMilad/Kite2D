
// ----[auto generated: KComponent body macro]----
#define KMETA_KCOMPONENT_BODY() \
private:\
static KObject *_createNew(KBaseStorage &Allocator);\
inline void _prpKComponentgetname(KRefVariant Value) { Value.copyByVal(getName()); }\
inline void _prpKComponentgettype(KRefVariant Value) { Value.copyByVal(getType()); }\
inline void _prpKComponentsetneedUpdate(KRefVariant Value){ setNeedUpdate(Value.getValue<bool>()); }\
inline void _prpKComponentgetneedUpdate(KRefVariant Value) { Value.copyByVal(getNeedUpdate()); }\
inline void _prpKComponentgetownerID(KRefVariant Value) { Value.copyByVal(getOwnerID()); }\
public:\
static void registerMeta(lua_State *Lua = nullptr);\
const std::string &getClassName() const { static std::string name("KComponent");\
return name;}\
friend KBaseSerial &operator<<(KBaseSerial &Out, KComponent &Value) {\
Value.serial(Out, KST_SERIALIZE); return Out;}\
friend KBaseSerial &operator>>(KBaseSerial &In, KComponent &Value) {\
Value.serial(In, KST_DESERIALIZE); return In;}\
protected:\
void serial(KBaseSerial &Serializer, KSerialStateTypes State);

// ----[auto generated: KComponent source macro]----
#define KMETA_KCOMPONENT_SOURCE()\
KObject *KComponent::_createNew(KBaseStorage &Allocator){\
return allocateNew<KComponent>(Allocator);}\
void KComponent::registerMeta(lua_State *Lua){\
static bool inite = true;\
static KMetaClass instance("KComponent",(COMPONENT), sizeof(KComponent));\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
KMetaManager::setFactory("KComponent", KComponent::_createNew);\
instance.addBase(new KMetaBase("KMessageHandler", KMB_PUBLIC));\
instance.addProperty(new KMetaProperty("name", "std::string", "component name", KMP_GETTER));\
prpMap.insert({ "_prpKComponentgetname", (void (KComponent::*)(KRefVariant)) &KComponent::_prpKComponentgetname});\
instance.addProperty(new KMetaProperty("type", "KComponentTypes", "component type", KMP_GETTER));\
prpMap.insert({ "_prpKComponentgettype", (void (KComponent::*)(KRefVariant)) &KComponent::_prpKComponentgettype});\
instance.addProperty(new KMetaProperty("needUpdate", "bool", "need update", KMP_BOTH));\
prpMap.insert({ "_prpKComponentgetneedUpdate", (void (KComponent::*)(KRefVariant)) &KComponent::_prpKComponentgetneedUpdate});\
prpMap.insert({ "_prpKComponentsetneedUpdate", (void (KComponent::*)(KRefVariant)) &KComponent::_prpKComponentsetneedUpdate});\
instance.addProperty(new KMetaProperty("ownerID", "U32", "owner(entity) id", KMP_GETTER));\
prpMap.insert({ "_prpKComponentgetownerID", (void (KComponent::*)(KRefVariant)) &KComponent::_prpKComponentgetownerID});\
KMetaFunction *fun;\
fun = new KMetaFunction("isDependOn", "bool", false);\
fun->paramsType.push_back("KComponentTypes");\
instance.addFunction(fun);\
const KMetaObject *minfo = KMetaManager::getMeta("KComponent");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)) { \
LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<KComponent>("KComponent")\
.addProperty("name", &KComponent::getName)\
.addProperty("type", &KComponent::getType)\
.addProperty("needUpdate", &KComponent::getNeedUpdate, &KComponent::setNeedUpdate)\
.addProperty("ownerID", &KComponent::getOwnerID)\
.addFunction("isDependOn", &KComponent::isDependOn)\
.endClass().endModule();}\
} inite = false;}\
void KComponent::serial(KBaseSerial &Serializer, KSerialStateTypes State){\
if (State == KST_SERIALIZE) {\
KMessageHandler *bptr0 = (KMessageHandler *)this; Serializer << bptr0;\
Serializer << _kname;\
Serializer << _ktype;\
Serializer << _kownerID;\
Serializer << _kneedup;\
Serializer << _kdependency;\
} else if (State == KST_DESERIALIZE) {\
KMessageHandler *bptr0 = (KMessageHandler *)this; Serializer >> bptr0;\
Serializer >> _kname;\
Serializer >> _ktype;\
Serializer >> _kownerID;\
Serializer >> _kneedup;\
Serializer >> _kdependency;\
}}
