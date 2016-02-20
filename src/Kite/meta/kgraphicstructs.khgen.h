
// ----[auto generated: myEnum body macro]----
#define KMETA_MYENUM_BODY() \
namespace Internal{\
KITE_FUNC_EXPORT extern void registerMeta_myEnum(lua_State *Lua = nullptr);}

// ----[auto generated: myEnum source macro]----
#define KMETA_MYENUM_SOURCE() \
namespace Internal{\
void registerMeta_myEnum(lua_State *Lua){\
static bool inite = true;\
static KMetaEnum instance("myEnum",(), sizeof(U32), "U32");\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
KMetaEnumMember *mem = 0;\
mem = new KMetaEnumMember("EN1_MEM1", 10, 0);\
instance.addMember(mem);\
mem = new KMetaEnumMember("EN1_MEM2", 22, 1);\
instance.addMember(mem);\
mem = new KMetaEnumMember("EN1_MEM3", 54, 2);\
instance.addMember(mem);\
const KMetaObject *minfo = KMetaManager::getMeta("myEnum");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)){\
LuaIntf::LuaBinding(Lua).beginModule("Kite")\
.beginModule("myEnum")\
.addConstant("EN1_MEM1", myEnum::EN1_MEM1)\
.addConstant("EN1_MEM2", myEnum::EN1_MEM2)\
.addConstant("EN1_MEM3", myEnum::EN1_MEM3)\
.endModule().endModule();}\
} inite = false;}}

// ----[auto generated: myEnum2 body macro]----
#define KMETA_MYENUM2_BODY() \
namespace Internal{\
KITE_FUNC_EXPORT extern void registerMeta_myEnum2(lua_State *Lua = nullptr);}

// ----[auto generated: myEnum2 source macro]----
#define KMETA_MYENUM2_SOURCE() \
namespace Internal{\
void registerMeta_myEnum2(lua_State *Lua){\
static bool inite = true;\
static KMetaEnum instance("myEnum2",(), sizeof(I32), "I32");\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
KMetaEnumMember *mem = 0;\
mem = new KMetaEnumMember("EN2_MEM1", 0, 0);\
instance.addMember(mem);\
mem = new KMetaEnumMember("EN2_MEM2", 1, 1);\
instance.addMember(mem);\
mem = new KMetaEnumMember("EN2_MEM3", 2, 2);\
instance.addMember(mem);\
const KMetaObject *minfo = KMetaManager::getMeta("myEnum2");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)){\
LuaIntf::LuaBinding(Lua).beginModule("Kite")\
.beginModule("myEnum2")\
.addConstant("EN2_MEM1", myEnum2::EN2_MEM1)\
.addConstant("EN2_MEM2", myEnum2::EN2_MEM2)\
.addConstant("EN2_MEM3", myEnum2::EN2_MEM3)\
.endModule().endModule();}\
} inite = false;}}

// ----[auto generated: KColor body macro]----
#define KMETA_KCOLOR_BODY() \
private:\
KITE_FUNC_EXPORT static KObject *_createNew(KBaseStorage &Allocator);\
public:\
KITE_FUNC_EXPORT static void registerMeta(lua_State *Lua = nullptr);\
const std::string &getClassName() const { static std::string name("KColor");\
return name;}\
friend KBaseSerial &operator<<(KBaseSerial &Out, KColor &Value) {\
Value.serial(Out, KST_SERIALIZE); return Out;}\
friend KBaseSerial &operator>>(KBaseSerial &In, KColor &Value) {\
Value.serial(In, KST_DESERIALIZE); return In;}\
protected:\
KITE_FUNC_EXPORT void serial(KBaseSerial &Serializer, KSerialStateTypes State);

// ----[auto generated: KColor source macro]----
#define KMETA_KCOLOR_SOURCE()\
KObject *KColor::_createNew(KBaseStorage &Allocator){\
return allocateNew<KColor>(Allocator);}\
void KColor::registerMeta(lua_State *Lua){\
static bool inite = true;\
static KMetaClass instance("KColor",(POD), sizeof(KColor));\
if (inite) {\
KMetaManager::setMeta((KMetaObject *)&instance);\
KMetaManager::setFactory("KColor", KColor::_createNew);\
instance.addProperty(new KMetaProperty("b", "U8", "b component", KMP_BOTH));\
instance.addProperty(new KMetaProperty("r", "U8", "r component", KMP_BOTH));\
instance.addProperty(new KMetaProperty("g", "U8", "g component", KMP_BOTH));\
instance.addProperty(new KMetaProperty("a", "U8", "a component", KMP_BOTH));\
const KMetaObject *minfo = KMetaManager::getMeta("KColor");\
if (Lua != nullptr && (minfo->getFlag() & SCRIPTABLE)) { \
LuaIntf::LuaBinding(Lua).beginModule("Kite").beginClass<KColor>("KColor")\
.addConstructor(LUA_ARGS(LuaIntf::_def<U8, 255>,LuaIntf::_def<U8, 255>,LuaIntf::_def<U8, 255>,LuaIntf::_def<U8, 255>))\
.addProperty("b", &KColor::getB, &KColor::setB)\
.addProperty("r", &KColor::getR, &KColor::setR)\
.addProperty("g", &KColor::getG, &KColor::setG)\
.addProperty("a", &KColor::getA, &KColor::setA)\
.endClass().endModule();}\
} inite = false;}\
void KColor::serial(KBaseSerial &Serializer, KSerialStateTypes State){\
if (State == KST_SERIALIZE) {\
Serializer << r;\
Serializer << g;\
Serializer << b;\
Serializer << a;\
} else if (State == KST_DESERIALIZE) {\
Serializer >> r;\
Serializer >> g;\
Serializer >> b;\
Serializer >> a;\
}}
