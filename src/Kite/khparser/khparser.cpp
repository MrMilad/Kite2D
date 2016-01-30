#include <stdio.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <streambuf>
#include <stack>
#include <utility>
#include <tinydir/tinydir.h>

std::unordered_map<std::string, std::string> strmap;

#define CLS_ATTRIB 6
#define FUNC_ATTRIB 6
#define VAR_ATTRIB 6

enum ParsState {
	PS_WORD = 0,	// all names (ex: class, void, myName, ...)
	PS_TOKEN,		// all tokens (ex: ,+=-*&")
	PS_STRING,		// string token (ex: @)
	PS_BODY_START,	// all types of body (supported: (, [, {)
	PS_BODY_END,	// all types of body (supported: ), ], })
	PS_END			// end of file
};

enum MClassBaseAccs {
	CA_PUBLIC = 0,
	CA_PROTECTED,
	CA_PRIVATE
};

enum MClassType {
	CT_CLASS = 0,
	CT_STRUCT
};

enum MExportState{
	ES_EXPORT = 0,
	ES_IMPORT,
	ES_NONE
};

struct MFParam {
	std::string name;
	std::string type;
	std::string defval;
};
struct MFunction {
	std::string name;
	std::string ret;
	std::string tokparam;
	bool ista;
	bool inl;
	bool ext;
	bool cons;
	std::vector<MFParam> params;

	MFunction() :
		ista(false), inl(false), ext(false),
		cons(false)
	{}
};

struct MProperty {
	std::string name;
	MFunction get;
	MFunction set;
	std::string comment;
	std::string type;
};

struct MVariable {
	std::string name;
	std::string type;
	std::string tokparam;
	bool cons;
	bool ista;

	MVariable() :
		cons(false), ista(false) 
	{}
};

struct MClassBase {
	std::string name;
	MClassBaseAccs accs;

	MClassBase() :
		accs(CA_PRIVATE) {}
};

struct MClass {
	std::string name;
	std::string tokparam;
	MClassType type;
	MExportState exstate;
	MFunction constructure;
	std::string flags;
	std::vector<MClassBase> bases;
	std::vector<MProperty> props;
	std::vector<MFunction> funcs;
	std::vector<MVariable> vars;

	MClass() :
		exstate(ES_NONE) {}
};

struct MEnum {
	std::string name;
	std::string tokparam;
	std::string type;
	std::vector<std::pair<std::string, std::string>> members;  // pair<name, value>
};

unsigned int tokenCounter(const std::string &Content, const std::string &Token) {
	if (Content.empty()) {
		return 0;
	}

	size_t pos = 0;
	unsigned int count = 0;
	while ((pos = Content.find(Token, pos)) != std::string::npos) {
		pos += Token.length();
		++count;
	}

	return count;
}

// get unique string key
std::string getKey() {
	std::string key("@");
	static unsigned int num = 0;
	key.append(std::to_string(++num));
	return key;
}

// remove anythings after given token
void removeAfter(std::string &Content, const std::string Tok) {
	size_t tpos = 0;
	if ((tpos = Content.find(Tok)) != std::string::npos) {
		Content.erase(tpos, Content.length());
	}
}

// remove given token
void removeTok(std::string &Content, const std::string Tok) {
	size_t tpos = 0;
	while ((tpos = Content.find(Tok)) != std::string::npos) {
		Content.erase(tpos, Tok.length());
	}
}

void replaceTok(std::string &Content, char Target, char Replace) {
	for (auto i = 0; i < Content.length(); i++) {
		if (Content[i] == Target)
			Content[i] = Replace;
	}
}

// get position of specifi token
unsigned int getTokPos(const std::string &Content, unsigned int Pos, char tok, bool Toend = false) {
	if (Content.empty())
		return 0;

	if (Pos >= Content.length())
		return 0;

	// ignore space and new line
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == tok) {
			return i + 1;

		} else if (!Toend){
			std::string::npos;
		}
	}
	return std::string::npos;;
}

// checking specific char token
bool checkTok(const std::string &Content, unsigned int Pos, char tok) {
	if (Content.empty())
		return false;

	if (Pos >= Content.length())
		return false;

	// ignore space and new line
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == tok) {
			return true;

		} else {
			return false;
		}
	}

	return false;
}

// checking next input (ignore space and new line)
ParsState checkNext(const std::string &Content, unsigned int Pos) {
	if (Content.empty())
		return PS_END;

	if (Pos >= Content.length())
		return PS_END;

	// ignore space and new line
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t'){
			continue;

		} else if (Content[i] == '(' || Content[i] == '[' || Content[i] == '{') {
			return PS_BODY_START;

		}else if (Content[i] == ')' || Content[i] == ']' || Content[i] == '}'){
			return PS_BODY_END;

		} else if (Content[i] == '=' || Content[i] == ',' || Content[i] == '*' ||
				   Content[i] == '-' || Content[i] == '+' || Content[i] == '&' ||
				   Content[i] == ':' || Content[i] == ';') {
			return PS_TOKEN;

		} else if (Content[i] == '@') {
			return PS_STRING;

		} else {
			return PS_WORD;
		}
	}

	return PS_END;
}

// get next word (token: sapce or new-line "=+-[]{}())
unsigned int getNextWord(const std::string &Content, unsigned int Pos, std::string &OutTok) {
	OutTok.clear();
	if (Content.empty())
		return 0;

	size_t begin = 0;
	size_t end = 0;
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' ||
			Content[i] == '(' || Content[i] == ')' ||
			Content[i] == '[' || Content[i] == ']' ||
			Content[i] == '{' || Content[i] == '}' ||
			Content[i] == ',' || Content[i] == '=' ||
			Content[i] == '+' || Content[i] == '-' ||
			Content[i] == '&' || Content[i] == '|' ||
			Content[i] == ':' || Content[i] == '*' ||
			Content[i] == '\t' || Content[i] == ';') {
			continue;
		}else{
			begin = i;
			break;
		}
	}

	for (auto i = begin; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' ||
			Content[i] == '(' || Content[i] == ')' ||
			Content[i] == '[' || Content[i] == ']' ||
			Content[i] == '{' || Content[i] == '}' ||
			Content[i] == ',' || Content[i] == '=' ||
			Content[i] == '+' || Content[i] == '-' ||
			Content[i] == '&' || Content[i] == '|' ||
			Content[i] == ':' || Content[i] == '*' ||
			Content[i] == '\t' || Content[i] == ';') {
			end = i;
			break;
		}
	}

	OutTok = Content.substr(begin, (end - begin));
	return end;
}

// get body 
unsigned int getNextBody(const std::string &Content, unsigned int Pos, std::string &OutBody) {
	OutBody.clear();
	if (Content.empty())
		return Pos;

	size_t begin = 0;
	char btok;
	char etok;
	size_t end = 0;
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == '(') {
			begin = i;
			btok = '(';
			etok = ')';
			break;

		} else if (Content[i] == '[') {
			begin = i;
			btok = '[';
			etok = ']';
			break;

		}else if (Content[i] == '{'){
			begin = i;
			btok = '{';
			etok = '}';
			break;

		}else {
			return 0;
		}
	}

	unsigned int deep = 0;
	for (auto i = begin; i < Content.length(); i++) {
		// deep-in
		if (Content[i] == btok) {
			++deep;
			continue;
		}

		// deep-out
		if (Content[i] == etok) {
			--deep;
		}

		// end of body
		if (Content[i] == etok && deep == 0) {
			end = i + 1;
			break;
		}

		// bad structure (missing token)
		if (i >= (Content.length() - 1)) {
			printf("incorrect body structure.\n");
			end = Content.length();
			break;
		}
	}

	OutBody = Content.substr(begin, (end - begin));

	return end;
}

// safe token finder
unsigned int findToken(const std::string &Content, unsigned int Pos, const std::string &Token) {
	size_t pos = 0;
	size_t tpos = Pos;

	if (Content.empty()) {
		return std::string::npos;
	}

	while ((pos = Content.find(Token, tpos)) != std::string::npos) {
		tpos = pos + Token.length();

		// token at the start of file
		if (pos > 0) {

			// check before token
			if (Content[pos - 1] != ' ' && Content[pos - 1] != '\t' && Content[pos - 1] != '\n') {
				continue;
			}
		}

		// check after token
		if (checkNext(Content, tpos) != PS_BODY_START) {
			continue;
		}

		return tpos;
	}

	return std::string::npos;
}

// remove strings ("...")
void collapseStrings(std::string &Content) {
	if (Content.empty())
		return;

	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("\"");
	while ((pos = Content.find("\"", tpos)) != std::string::npos) {
		tpos = pos + tsize;

		// get current line
		size_t line;
		if ((line = Content.find("\n", tpos)) == std::string::npos)
			line = Content.length(); // we are at the end of file

		// string lenght
		size_t len = 0;
		if ((len = Content.find("\"", tpos)) != std::string::npos && len < line) {
			std::string key = getKey();
			strmap.insert({ key, Content.substr(pos + 1, (len + tsize) - (pos + 2)) }); // pos + 1: remove " token from both side
			Content.replace(pos, (len + tsize) - pos, key);
		}// else: (ex: //comm"ent or there is no any quotes in the current line)
	}
}

// remove commnets (//)
void removeFSComments(std::string &Content) {
	if (Content.empty())
		return;

	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("//");
	while ((pos = Content.find("//", tpos)) != std::string::npos) {
		tpos = pos + tsize;

		// get current line
		size_t line = 0; // we are at the begining of the file by default
		if ((line = Content.find("\n", tpos)) != std::string::npos)
			Content.erase(pos, line - pos);
	}
}

// remove commnets (/*)
void removeSSComments(std::string &Content) {
	if (Content.empty())
		return;

	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("/*");
	while ((pos = Content.find("/*", tpos)) != std::string::npos) {
		tpos = pos + tsize;

		// get end cm token (*/)
		size_t ntok = 0;
		if ((ntok = Content.find("*/", tpos)) != std::string::npos) {
			int lines = tokenCounter(Content.substr(pos, (ntok + tsize) - pos), "\n");
			Content.replace(pos, (ntok + tsize) - pos, lines, '\n');
		}
	}
}

void splitBy(const std::string &Content, std::string Tok, std::vector<std::string> &OutList) {
	OutList.clear();
	size_t pos = 0;
	size_t tsize = Tok.length();
	std::string tstr = Content;

	tstr.append(Tok);

	while ((pos = tstr.find(Tok)) != std::string::npos) {
		if (!tstr.substr(0, pos).empty())
			OutList.push_back(tstr.substr(0, pos));
		tstr.erase(0, pos + tsize);
	}
}

// split a list of parameter by token (,)
void splitParamRaw(const std::string &Content, std::vector<std::string> &OutList, bool IgnoreSN) {
	OutList.clear();

	std::string temp = Content;
	if (IgnoreSN) {
		removeTok(temp, " ");
		removeTok(temp, "\n");
		removeTok(temp, "\t");
	}
	removeTok(temp, "(");
	removeTok(temp, ")");
	removeTok(temp, "[");
	removeTok(temp, "]");
	removeTok(temp, "{");
	removeTok(temp, "}");

	if (temp.empty())
		return;

	if (temp[temp.length() - 1] != ',')
		temp.append(",");

	size_t pos = 0;
	size_t tsize = strlen(",");
	while ((pos = temp.find(",")) != std::string::npos) {
		if (!temp.substr(0, pos).empty())
			OutList.push_back(temp.substr(0, pos));
		temp.erase(0, pos + tsize);
	}
}

void splitParam(const std::string &Content, std::vector<std::string> &OutList) {
	splitParamRaw(Content, OutList, true);
}

bool procFunc(const std::string &Content, MFunction &Func, unsigned int Pos) {
	std::string output;
	std::vector<std::string> params;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// its a generic function processor so we dont check number of token parameters here,
	// and there is no warning for empty parameter list
	Pos = getNextBody(Content, Pos, output);
	Func.tokparam = output;

	// check function attributes 
	if (checkNext(Content, Pos) != PS_WORD) {
		printf("error: missing function name/attributes.\n");
		return false;
	}

	unsigned int rpos[2];
	rpos[0] = Pos;
	for (auto i = 0; i < FUNC_ATTRIB; i++) {
		if (checkNext(Content, Pos) != PS_BODY_START) {
			rpos[1] = rpos[0]; // catch last 2 position for extreact return type
			rpos[0] = Pos;
			Pos = getNextWord(Content, Pos, output);

			if (output == "inline") {
				Func.inl = true;
			} else if (output == "static") {
				Func.ista = true;
			} else if (output == "extern") {
				Func.ext = true;
			}
		} else {
			// name
			Func.name = output;

			// return type
			getNextWord(Content, rpos[1], output);

			// without return type (constructures)
			if (Func.name != output) {
				Func.ret = output;
			}

			break;
		}
	}

	if (Func.name.empty()) {
		printf("error: could not extract function name. something is wrong.");
		return false;
	}

	// get function parameter list
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing () ==> function name: %s", Func.name);
		return false;
	}

	Pos = getNextBody(Content, Pos, output);
	splitParamRaw(output, params, false);

	std::vector<std::string> pinfo;
	if (!params.empty()) {
		for (auto i = 0; i < params.size(); i++) {

			MFParam tpar;

			// default value
			splitBy(params[i], "=", pinfo);
			if (pinfo.size() > 1) {
				removeTok(pinfo[1], " ");
				removeTok(pinfo[1], "\n");
				removeTok(pinfo[1], "\t");
				removeAfter(params[i], "=");
				tpar.defval = pinfo[1]; // default value
			}

			// name and type
			splitBy(params[i], " ", pinfo);
			if (pinfo.size() < 2) {
				printf("error: missing parameter name ==> function name: %s\n", Func.name.c_str());
				return false;
			}


			// ex: char param
			if (pinfo.size() == 2) {
				removeTok(pinfo[0], "*");
				removeTok(pinfo[0], "&");
				tpar.type = pinfo[0]; // type
				removeTok(pinfo[1], "*");
				removeTok(pinfo[1], "&");
				tpar.name = pinfo[1]; // name
			}

			// ex: const char param
			if (pinfo.size() == 3) {
				removeTok(pinfo[1], "*");
				removeTok(pinfo[1], "&");
				tpar.type = pinfo[1]; // type
				removeTok(pinfo[2], "*");
				removeTok(pinfo[2], "&");
				tpar.name = pinfo[2]; // name
			}
			
			Func.params.push_back(tpar);
		}
	}

	// check const
	if (checkNext(Content, Pos) == PS_WORD) {
		for (auto i = 0; i < FUNC_ATTRIB; i++) {
			if (checkNext(Content, Pos) == PS_WORD) {
				Pos = getNextWord(Content, Pos, output);

				if (output == "const") {
					Func.cons = true;
				}
			} else {
				break;
			}
		}
	}
	
	return true;
}

// parse all property in the content
bool procAllFunc(std::string &Content, std::vector<MFunction> &Funcs) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Funcs.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_FUNCTION")) != std::string::npos) {
		MFunction tfun;
		if (procFunc(Content, tfun, pos)) {
			Funcs.push_back(tfun);

		} else {
			ret = false;
			printf("Function token number: %u\n", count);
		}
		++count;
	}

	return ret;
}

bool procConstructure(const std::string &Content, MFunction &Func) {
	size_t pos = 0;

	// only 1 constructure supported at this time
	if ((pos = findToken(Content, 0, "KMETA_CONSTRUCTURE")) != std::string::npos) {

		if (procFunc(Content, Func, pos)) {
			return true;
		}

	} else {
		// default contructure
		return true;
	}

	return false;
}

bool procProp(const std::vector<MFunction> &Allprop, std::vector<MProperty> &Output) {
	std::vector<std::string> param;
	std::unordered_map<std::string, std::pair<MProperty, bool>> map;
	Output.clear();

	if (Allprop.empty()) {
		return true;
	}

	for (auto i = 0; i < Allprop.size(); i++) {

		// exteract token parameter
		splitParam(Allprop[i].tokparam, param);
		
		if (param.empty()) {
			printf("error: missing property info ==> function name: %s", Allprop[i].name);
			return false;
		} else if (param.size() > 2) {
			printf("warning: extera property info (ignored) ==> function name: %s", Allprop[i].name);
		}


		// extract string name from string map
		auto pname = strmap.find(param[0]);
		if (pname == strmap.end()) {
			printf("error: could not extract prperty name from string map ==> function name: %s", Allprop[i].name);
			return false;
		}

		auto prop = map.find(pname->second);
		auto *pptr = &map[pname->second];
		pptr->first.name = pname->second;

		// new property
		if (prop == map.end()) {
			pptr->second = false;

		// registered property
		} else {

			// check overload
			if (prop->second.second) {
				printf("error: property overload not supported ==> property name: %s", pname->second);
				return false;
			}

			prop->second.second = true;
		}

		// have comment
		if (param.size() > 1) {
			pname = strmap.find(param[1]);
			if (pname == strmap.end()) {
				printf("error: could not extract prperty comment from string map ==> function name: %s", Allprop[i].name);
				return false;
			}
			pptr->first.comment = pname->second;
		}

		// check function type (setter or getter)
		// getter
		if (Allprop[i].params.empty()) {
			pptr->first.get = Allprop[i];
			pptr->first.type = Allprop[i].ret;

			// setter
		} else if (Allprop[i].params.size() == 1) {
			pptr->first.set = Allprop[i];
			pptr->first.type = Allprop[i].params[0].type;

			// more than 1 parameter (not supported)
		} else {
			printf("error: properties with more than 1 parameter not supported ==> function name: %s", Allprop[i].name);
			return false;
		}
		
	}

	// type match - iterate over all registered properties
	for (auto it = map.begin(); it != map.end(); ++it) {
		if (it->second.first.get.ret != it->second.first.set.params[0].type) {
			printf("error: setter/getter type mismatch ==> property name: %s", it->second.first.name);
			return false;
		}

		if (!it->second.second) {
			printf("error: missing setter/getter ==> property name: %s", it->second.first.name);
			return false;
		}

		Output.push_back(it->second.first);
	}

	return true;
}

// parse all property in the content
bool procAllProp(const std::string &Content, std::vector<MProperty> &Props) {
	size_t pos = 0;
	std::vector<MFunction> allprop;
	unsigned int count = 0;
	bool ret = true;
	Props.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_PROPERTY")) != std::string::npos) {
		MFunction tfunc;

		if (procFunc(Content, tfunc, pos)) {
			allprop.push_back(tfunc);

		} else {
			ret = false;
			printf("property token number: %u\n", count);
		}
		++count;
	}

	if (ret) {
		if (!procProp(allprop, Props)) {
			ret = false;
			printf("property token number: %u\n", count);
		}
	}

	return ret;
}

bool procVar(const std::string &Content, MVariable &Var, unsigned int Pos) {
	std::string output;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// its a generic variable processor so we dont check number of token parameters here,
	// and there is no warning for empty parameter list
	Pos = getNextBody(Content, Pos, output);
	Var.tokparam = output;

	// check variable attributes 
	if (checkNext(Content, Pos) != PS_WORD) {
		printf("error: missing variable name/attributes.\n");
		return false;
	}

	unsigned int rpos[2];
	rpos[0] = Pos;
	auto tpos = getTokPos(Content, Pos, ';', true);
	--tpos;
	for (auto i = 0; i < VAR_ATTRIB; i++) {
		if (Pos < tpos) {
			rpos[1] = rpos[0]; // catch last 2 position for extreact return type
			rpos[0] = Pos;
			Pos = getNextWord(Content, Pos, output);

			if (output == "static") {
				Var.ista = true;
			} else if (output == "const") {
				Var.cons = true;
			}
		} else {
			// name
			Var.name = output;

			//  type
			getNextWord(Content, rpos[1], output);
			Var.type = output;

			break;
		}
	}

	if (Var.name.empty() || Var.type.empty()) {
		printf("error: could not extract variable name/type. something is wrong.");
		return false;
	}

	return true;
}

// parse all CLASS tokens
bool procAllVar(const std::string &Content, std::vector<MVariable> &Vars) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Vars.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_VARIABLE")) != std::string::npos) {
		MVariable tvar;
		if (procVar(Content, tvar, pos)) {
			Vars.push_back(tvar);

		} else {
			printf("variable token number: %u \n", count);
			ret = false;
		}
		++count;
	}

	return ret;
}

// parse and extract class information
bool procClass(const std::string &Content, MClass &Cls, unsigned int Pos) {
	size_t pos;
	std::string cbody;
	std::string output;
	std::vector<std::string> params;

	// checking token parameter(s)
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// extract token parameter(s)
	pos = getNextBody(Content, Pos, output);
	Cls.tokparam = output;

	// checking type token (class or struct)
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing class/struct type.\n");
		return false;
	}

	pos = getNextWord(Content, pos, output);
	if (output == "class") {
		Cls.type = CT_CLASS;

	} else if (output == "struct") {
		Cls.type = CT_STRUCT;

	} else {
		printf("error: incorrect type (%s).\n", output.c_str());
		return false;
	}

	// checking expoert/import macro and name of class
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing export/import or class name.\n");
		return false;
	}

	// search CLS_ATTRIB word for class name and its attributes
	for (auto i = 0; i < CLS_ATTRIB; i++){
		if (checkNext(Content, pos) == PS_WORD) {
			pos = getNextWord(Content, pos, output);

			if (output == "KITE_FUNC_EXPORT") {
				Cls.exstate = ES_EXPORT;
			} else if (output == "KITE_FUNC_IMPORT") {
				Cls.exstate = ES_IMPORT;
			}// else ES_NONE by default

		} else {
			Cls.name = output;
			break;
		}
	}

	// checking base classes
	// with base(s)
	if (checkNext(Content, pos) == PS_TOKEN) {
		if (!checkTok(Content, pos, ':')) {
			printf("error: missing \":\" token.\n");
			return false;
		}
		pos = getTokPos(Content, pos, ':');

		if (checkNext(Content, pos) != PS_WORD) {
			printf("error: missing base(es).\n");
			return false;
		}

		while (checkNext(Content, pos) == PS_WORD || checkNext(Content, pos) == PS_TOKEN) {
			pos = getNextWord(Content, pos, output);
			MClassBase base;
			if (output == "public") {
				base.accs = CA_PUBLIC;
				pos = getNextWord(Content, pos, output);
				base.name = output;
				Cls.bases.push_back(base);

			} else if (output == "private") {
				base.accs = CA_PRIVATE;
				pos = getNextWord(Content, pos, output);
				base.name = output;
				Cls.bases.push_back(base);

			} else if (output == "protected") {
				base.accs = CA_PROTECTED;
				pos = getNextWord(Content, pos, output);
				base.name = output;
				Cls.bases.push_back(base);

			} else {
				base.accs = CA_PRIVATE;
				base.name = output;
				Cls.bases.push_back(base);
			}
		}
	} 

	// without base(s)
	size_t cpos = 0;
	if (checkNext(Content, pos) == PS_BODY_START) {

		// get class body
		getNextBody(Content, pos, cbody);

		if (!procConstructure(cbody, Cls.constructure)) {
			return false;
		}

		// default contructure
		if (Cls.constructure.name.empty()) {
			Cls.constructure.name = Cls.name;
		}
		
		// parse all properties in the class body
		if (!procAllProp(cbody, Cls.props)) {
			return false;
		}

		// parse all function in the class body
		if (!procAllFunc(cbody, Cls.funcs)) {
			return false;
		}

		if (!procAllVar(cbody, Cls.vars)) {
			return false;
		}

	// incorrect structure
	} else {
		printf("error: missing class\structure body.\n");
		return false;
	}
}

// parse all CLASS tokens
bool procAllClass(const std::string &Content, std::vector<MClass> &Classes) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Classes.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_CLASS")) != std::string::npos) {
		MClass tcls;
		if (procClass(Content, tcls, pos)) {
			Classes.push_back(tcls);

		} else {
			unsigned int line = tokenCounter(Content.substr(0, pos), "\n");
			printf("line number: %u ==> token name: KMETA_CLASS ==> token number: %u \n", ++line, count);
			ret = false;
		}
		++count;
	}

	return ret;
}

bool procEnumMem(const std::string &Content, MEnum &Enm) {
	std::vector<std::string> param;
	std::vector<std::string> defval;
	Enm.members.clear();
	splitParam(Content, param);
	if (param.empty()) {
		printf("error: empty enum.\n");
		return false;
	}

	// member value
	for (auto i = 0; i < param.size(); i++) {
		splitBy(param[i], "=", defval);

		// without value
		if (defval.size() == 1) {
			Enm.members.push_back({ param[i], "" });
			continue;

		// with value
		} else if (defval.size() == 2) {
			removeAfter(param[i], "=");
			Enm.members.push_back({ param[i], defval[1] });

		// extra or incorrect state
		} else {
			printf("error: incorrect enum member defention.\n");
			return false;
		}
	}

	return true;
}

bool procEnum(const std::string &Content, MEnum &Enm, unsigned int Pos) {
	size_t pos;
	std::string ebody;
	std::string output;
	if (Content.empty()) {
		return false;
	}

	// checking token parameter(s)
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// extract token parameter(s)
	pos = getNextBody(Content, Pos, output);
	Enm.tokparam = output;

	// checking type token (class or struct)
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing enum type.\n");
		return false;
	}

	// enum token
	pos = getNextWord(Content, pos, output);
	if (output != "enum") {
		printf("error: incorrect type (%s)\n", output);
		return false;
	}

	// class token
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing enum class type.\n");
		return false;
	}

	pos = getNextWord(Content, pos, output);
	if (output != "class") {
		printf("error: only enum class (scoped enum) supported.\n", output);
		return false;
	}

	// name
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing enum name.\n");
		return false;
	}
	pos = getNextWord(Content, pos, output);
	Enm.name = output;

	// type
	if (checkNext(Content, pos) == PS_TOKEN) {
		pos = getTokPos(Content, pos, ':');

		if (checkNext(Content, pos) != PS_WORD) {
			printf("error: missing enum type.\n");
			return false;
		}

		pos = getNextWord(Content, pos, output);
		Enm.type = output;
	} else {
		// int by default
		Enm.type = "I32";
	}
	
	// parse enum memebers
	if (checkNext(Content, pos) != PS_BODY_START) {
		printf("error: incorrect enum defention.\n");
		return false;
	}
	getNextBody(Content, pos, ebody);

	if (!procEnumMem(ebody, Enm)) {
		return false;
	}

	return true;
}

bool procAllEnum(const std::string &Content, std::vector<MEnum> &Enums) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Enums.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KMETA_ENUM")) != std::string::npos) {
		MEnum tenu;
		if (procEnum(Content, tenu, pos)) {
			Enums.push_back(tenu);

		} else {
			unsigned int line = tokenCounter(Content.substr(0, pos), "\n");
			printf("line number: %u ==> token name: KMETA_ENUM ==> token number: %u \n", ++line, count);
			ret = false;
		}
		++count;
	}

	return ret;
}

// main parser
bool parse(std::string &Content, std::vector<MClass> &Classes, std::vector<MEnum> &Enums) {
	Classes.clear();
	Enums.clear();
	if (Content.empty()) {
		printf("empty file.\n");
		return false;
	}

	bool ret = true;

	collapseStrings(Content);
	removeFSComments(Content);
	removeSSComments(Content);
	removeTok(Content, "public:");
	removeTok(Content, "private:");
	removeTok(Content, "protected:");

	if (!procAllClass(Content, Classes))
		ret = false;

	if (!procAllEnum(Content, Enums))
		ret = false;

	return ret;
}

void createMacros(const std::vector<MClass> &Cls, const std::vector<MEnum> &Enms, std::string &Output) {
	Output.clear();

	// enums
	for (auto i = 0; i < Enms.size(); i++) {
		std::string upname = Enms[i].name;

		// declaration
		Output.append("\n// ----[auto generated: " + Enms[i].name + " body macro]----\n");
		transform(upname.begin(), upname.end(), upname.begin(), toupper);
		Output.append("#define KMETA_" + upname + "_BODY() \\\n"
					  "namespace Internal{\\\n"
					  "KITE_FUNC_EXPORT extern void registerMeta_" + Enms[i].name + "(lua_State *Lua = nullptr);}\n");

		// definition
		Output.append("\n// ----[auto generated: " + Enms[i].name + " source macro]----\n");
		Output.append("#define KMETA_" + upname + "_SOURCE() \\\n"
					  "namespace Internal{\\\n"
					  "void registerMeta_" + Enms[i].name + "(lua_State *Lua){\\\n"
					  "static bool inite = true;\\\n"
					  "static KMetaEnum instance(\"" + Enms[i].name + "\"," + Enms[i].tokparam + ", sizeof(" + Enms[i].type + "), "
					  "typeid(" + Enms[i].type + "));\\\n"
					  "if (inite) {\\\n"
					  "KMetaManager::setMeta(std::type_index(typeid(" + Enms[i].name + ")), (KMeta *)&instance);\\\n");

		// enum members
		Output.append("KMetaEnumMember *mem = 0;\\\n");
		for (auto count = 0; count < Enms[i].members.size(); count++) {
			std::string vc = std::to_string(count);
			if (!Enms[i].members[count].second.empty())
				vc = Enms[i].members[count].second;

			Output.append("mem = new KMetaEnumMember(\"" + Enms[i].members[count].first + "\", " + vc + ", "
						  + std::to_string(count) + ");\\\n");

			Output.append("instance.addMember(mem);\\\n");
		}

		// lua binding
		// enum members
		Output.append("if (Lua != nullptr){\\\n"
			"LuaIntf::LuaBinding(Lua).beginModule(\"Kite\")\\\n"
					  ".beginModule(\"" + Enms[i].name + "\")\\\n");
		for (auto count = 0; count < Enms[i].members.size(); count++) {

			Output.append(".addConstant(\"" + Enms[i].members[count].first + "\", " + Enms[i].name + "::" 
						  + Enms[i].members[count].first + ")\\\n");
		}

		// end of lua binding
		Output.append(".endModule().endModule();}\\\n");

		// end of definition
		Output.append("} inite = false;}}\n");
	}

	// class
	for (auto i = 0; i < Cls.size(); i++) {
		std::string upname = Cls[i].name;

		// declaration
		Output.append("\n// ----[auto generated: " + Cls[i].name + " body macro]----\n");
		transform(upname.begin(), upname.end(), upname.begin(), toupper);
		Output.append("#define KMETA_" + upname + "_BODY() \\\n"
					  "private:\\\n");
		
		// define properties
		for (auto count = 0; count < Cls[i].props.size(); count++) {
			Output.append("inline void _prp" + Cls[i].name + "set" + Cls[i].props[count].name  + "(KRefVariant Value){ " +
						  Cls[i].props[count].set.name + "(Value.getValue<" + Cls[i].props[count].type + ">()); }\\\n");

			Output.append("inline void _prp" + Cls[i].name + "get" + Cls[i].props[count].name + "(KRefVariant Value) { " +
						  "Value.copyByVal(" + Cls[i].props[count].get.name + "()); }\\\n");
		}

		// register functions
		Output.append("public:\\\n"
					  "KITE_FUNC_EXPORT static void registerMeta(lua_State *Lua = nullptr, U32 Flag = 0);\\\n"
					  "friend KBytesArray &operator<<(KBytesArray &Out, " + Cls[i].name + "& Value) {\\\n"
					  "Value.serial(Out, KST_SERIALIZE); return Out;}\\\n"
					  "friend KBytesArray &operator>>(KBytesArray &In, " + Cls[i].name + "& Value) {\\\n"
					  "Value.serial(In, KST_DESERIALIZE); return In;}\\\n"
					  "friend KBytesArray &operator<<(KBytesArray &Out, " + Cls[i].name + "* Value) {\\\n"
					  "Value->serial(Out, KST_SERIALIZE); return Out;}\\\n"
					  "friend KBytesArray &operator>>(KBytesArray &In, " + Cls[i].name + "* Value) {\\\n"
					  "Value->serial(In, KST_DESERIALIZE); return In;}\\\n"
					  "private:\\\n"
					  "KITE_FUNC_EXPORT void serial(KBytesArray &Serializer, KSerialStateTypes State);\n");

		// registerMeta()
		Output.append("\n// ----[auto generated: " + Cls[i].name + " source macro]----\n");
		Output.append("#define KMETA_" + upname + "_SOURCE()\\\n"
					"void " + Cls[i].name + "::registerMeta(lua_State *Lua, U32 Flag){\\\n"
					  "static bool inite = true;\\\n"
					  "static KMetaClass instance(\"" + Cls[i].name + "\"," + Cls[i].tokparam + ", sizeof(" + Cls[i].name + "));\\\n"
					  "if (inite) {\\\n"
					  "KMetaManager::setMeta(std::type_index(typeid(" + Cls[i].name + ")), (KMeta *)&instance);\\\n");

		// bases
		for (auto count = 0; count < Cls[i].bases.size(); count++) {
			std::string acc;
			if (Cls[i].bases[count].accs == CA_PUBLIC) { acc = "KMB_PUBLIC"; }
			if (Cls[i].bases[count].accs == CA_PRIVATE) { acc = "KMB_PRIVATE"; }
			if (Cls[i].bases[count].accs == CA_PROTECTED) { acc = "KMB_PROTECTED"; }
			Output.append("instance.addBase(new KMetaBase(\"" + Cls[i].bases[count].name + "\", "
						  + acc + ", typeid(" + Cls[i].bases[count].name + ")));\\\n");
		}

		// properties
		for (auto count = 0; count < Cls[i].props.size(); count++) {
			Output.append("instance.addProperty(new KMetaProperty(\"" + Cls[i].props[count].name + "\", \""
						   + Cls[i].props[count].comment + "\", KMP_BOTH, typeid(" + Cls[i].props[count].type + ")));\\\n");

			// insert to property map
			Output.append("prpMap.insert({ \"_prp" + Cls[i].name + "get" + Cls[i].props[count].name 
						  + "\", (void (KProperty::*)(KRefVariant)) &" + Cls[i].name + "::" 
						  + "_prp" + Cls[i].name + "get" + Cls[i].props[count].name+ "});\\\n");

			Output.append("prpMap.insert({ \"_prp" + Cls[i].name + "set" + Cls[i].props[count].name
						  + "\", (void (KProperty::*)(KRefVariant)) &" + Cls[i].name + "::"
						  + "_prp" + Cls[i].name + "set" + Cls[i].props[count].name + "});\\\n");
		}

		// functions
		Output.append("KMetaFunction *fun;\\\n");
		for (auto count = 0; count < Cls[i].funcs.size(); count++) {
			std::string ista = "false";
			if (Cls[i].funcs[count].ista) {
				ista = "true";
			} 
			Output.append("fun = new KMetaFunction(\"" + Cls[i].funcs[count].name + "\", " + ista + ", "
						  + "typeid(" + Cls[i].funcs[count].ret + "));\\\n");

			// function parameters
			for (auto pcount = 0; pcount < Cls[i].funcs[count].params.size(); pcount++) {
				Output.append("fun->paramsType.push_back(typeid(" + Cls[i].funcs[count].params[pcount].type + "));\\\n");
			}
			Output.append("instance.addFunction(fun);\\\n");
		}

		// lua binding 
		Output.append("if (Lua != nullptr && (Flag & SCRIPTABLE)) { \\\n"
					  "LuaIntf::LuaBinding(Lua).beginModule(\"Kite\").beginClass<"
					  + Cls[i].name + ">(\"" + Cls[i].name + "\")\\\n");

		// constructure
		std::string param;
		std::string tok;
		if (!Cls[i].constructure.params.empty()) {
			for (auto count = 0; count < Cls[i].constructure.params.size(); count++) {
				if (!Cls[i].constructure.params[count].defval.empty()) {
					param.append(tok + "LuaIntf::_def<" + Cls[i].constructure.params[count].type + ", " + Cls[i].constructure.params[count].defval + ">");
				} else {
					param.append(tok + "LuaIntf::_opt<" + Cls[i].constructure.params[count].type + ">");
				}
				tok = ",";
			}
		}
		Output.append(".addConstructor(LUA_ARGS(" + param + "))\\\n");

		// properties
		for (auto count = 0; count < Cls[i].props.size(); count++) {
			Output.append(".addProperty(\"" + Cls[i].props[count].name + "\", &" + Cls[i].name + "::"
						  + Cls[i].props[count].get.name + ", &" + Cls[i].name + "::" + Cls[i].props[count].set.name + ")\\\n");
		}

		// functions 
		param = "";
		tok = "";
		std::string fista;
		for (auto count = 0; count < Cls[i].funcs.size(); count++) {
			
			if (Cls[i].funcs[count].ista) {
				fista = "addStaticFunction";
			} else {
				fista = "addFunction";
			}
			if (Cls[i].funcs[count].params.empty()) {
				Output.append("." + fista + "(\"" + Cls[i].funcs[count].name + "\", &" + Cls[i].name + "::" + Cls[i].funcs[count].name + ")\\\n");
			} else {
				for (auto pcount = 0; pcount < Cls[i].funcs[count].params.size(); pcount++) {
					if (Cls[i].funcs[count].params[pcount].defval.empty()) {
						param.append(tok + Cls[i].funcs[count].params[pcount].type);
					} else {
						param.append(tok + "LuaIntf::_def<" + Cls[i].funcs[count].params[pcount].type + ", " + Cls[i].funcs[count].params[pcount].defval + ">");
					}
					tok = ",";
					Output.append("." + fista + "(\"" + Cls[i].funcs[count].name + "\", &" + Cls[i].name + "::" + Cls[i].funcs[count].name
								  + ", LUA_ARGS(" + param + "))\\\n");
				}
			}
		}

		// variables
		for (auto count = 0; count < Cls[i].vars.size(); count++) {
			std::string func;
			if (Cls[i].vars[count].ista) {
				func = ".addStaticVariable";
			} else {
				func = ".addVariable";
			}

			std::vector<std::string> vparam;
			splitParam(Cls[i].vars[count].tokparam, vparam);

			for (auto iter = 0; iter < vparam.size(); iter++) {
				if (vparam[iter] == "SCRIPTABLE") {
					std::string write;
					if (Cls[i].vars[count].cons) {
						write = "false";
					} else {
						write = "true";
					}

					Output.append(func + "(\"" + Cls[i].vars[count].name + "\", &" + Cls[i].name + "::" 
								  + Cls[i].vars[count].name + ", " + write + ")");
					break;
				}
			}
		}

		// end of lua binding
		Output.append(".endClass().endModule();}\\\n");

		// end of rgisterMeta
		Output.append("} inite = false;}\\\n");

		// serial definition
		Output.append("void " + Cls[i].name + "::serial(KBytesArray &Serializer, KSerialStateTypes State){\\\n");

		// serialize base class(es)
		std::string ser;
		std::string dser;
		for (auto bcount = 0; bcount < Cls[i].bases.size(); bcount++) {
			ser.append(Cls[i].bases[bcount].name + " *bptr" + std::to_string(bcount)
					   + " = (" + Cls[i].bases[bcount].name + " *)this; Serializer << bptr" + std::to_string(bcount) + ";\\\n");
			dser.append(Cls[i].bases[bcount].name + " *bptr" + std::to_string(bcount)
						+ " = (" + Cls[i].bases[bcount].name + " *)this; Serializer >> bptr" + std::to_string(bcount) + ";\\\n");
		}

		// serialize
		Output.append("if (State == KST_SERIALIZE) {\\\n");
		if (!Cls[i].bases.empty()) {
			Output.append(ser);
		}

		// variables
		for (auto vcont = 0; vcont < Cls[i].vars.size(); vcont++) {
			Output.append("Serializer << " + Cls[i].vars[vcont].name + ";\\\n");
		}

		// deserialize
		Output.append("} else if (State == KST_DESERIALIZE) {\\\n");
		Output.append(dser);

		// variables
		for (auto vcont = 0; vcont < Cls[i].vars.size(); vcont++) {
			Output.append("Serializer >> " + Cls[i].vars[vcont].name + ";\\\n");
		}

		// end of serial()
		Output.append("}}\n");
	}
}

void createHead(std::string &Output) {
	Output.clear();

	Output.append("#ifndef KITEMETA_H\n"
				  "#define KITEMETA_H\n\n"
				  "#include \"Kite/Core/system/ksystemdef.h\"\n"
				  "#include \"Kite/Core/utility/kmetadef.h\"\n\n"
				  "KMETA\n"
				  "namespace Kite{\n"
				  "\n// ----[auto generated register function]----\n"
				  "KITE_FUNC_EXPORT extern void registerKiteMeta(lua_State *Lua = nullptr, U32 Flag = 0);}\n"
				  "#endif // KITEMETA_H");
}

void createSource(const std::vector<std::string> &Files, const std::vector<MClass> &Cls,
				  const std::vector<MEnum> &Ens, std::string &Output) {


	Output.clear();
	Output.append("\n// ----[auto generated source file]----\n");

	// add headers
	for (auto i = 0; i < Files.size(); i++) {
		Output.append("#include \"" + Files[i] + "\"\n");
	}

	Output.append("namespace Kite{\n"
				  "void registerKiteMeta(lua_State *Lua, U32 Flag){\n");

	// register classes
	for (auto i = 0; i < Cls.size(); i++) {
		Output.append(Cls[i].name + "::registerMeta(Lua, Flag);\n");
	}

	// register enums
	for (auto i = 0; i < Ens.size(); i++) {
		Output.append("Internal::registerMeta_" + Ens[i].name + "(Lua);\n");
	}

	Output.append("}}");

}

bool procFile(const std::string &FAddress, const std::string &OAdress, std::vector<MClass> &Cls, std::vector<MEnum> &Ens) {
	bool ret = true;
	std::string macro;
	std::vector<MClass> lcls;
	std::vector<MEnum> lens;
	std::ifstream in(FAddress);
	std::string content((std::istreambuf_iterator<char>(in)),
					std::istreambuf_iterator<char>());

	if (!parse(content, lcls, lens))
		return false;

	createMacros(lcls, lens, macro);

	if (macro.empty())
		return false;

	unsigned int pos = 0;
	if ((pos = FAddress.rfind("/")) == std::string::npos) {
		printf("cant fix output file address");
		return false;
	}

	unsigned int epos = 0;
	if ((epos = FAddress.rfind(".")) == std::string::npos) {
		printf("cant retrieve file extension");
		return false;
	}


	std::ofstream out(OAdress + "/" + FAddress.substr(pos, (epos - pos)) + ".khgen.h");
	out << macro;
	out.close();

	Cls.insert(Cls.end(), lcls.begin(), lcls.end());
	Ens.insert(Ens.end(), lens.begin(), lens.end());

	return true;
}

int main(int argc, char* argv[]) {

	if (argc != 3) {
		printf("missing input parameter\nusage: khparser.exe [includeDirectory] [outputDirectory]");
		return 0;
	}

	std::string baseadr(argv[1]);
	std::string outadr(argv[2]);
	std::vector<std::string> hadrs;
	std::vector<MClass> cls;
	std::vector<MEnum> ens;
	std::stack<std::string> dirstack;
	tinydir_dir dir;
	tinydir_open(&dir, argv[1]);

	unsigned int fcounter = 0;
	unsigned int dcounter = 0;
	unsigned int gcounter = 0;
	while (true) {
		if (dir.has_next) {
			tinydir_file file;
			tinydir_readfile(&dir, &file);

			if (file.is_dir) {
				if (std::string(file.name) != "." && std::string(file.name) != "..") {
					dirstack.push(file.path);
					++dcounter;
				}

			} else {
				printf("parse file: %s\n", file.path);
				if (procFile(std::string(file.path), outadr, cls, ens)) {
					hadrs.push_back(std::string(file.path).substr(baseadr.length() + 1)); // + 1 for "/"
					++gcounter;
				}
				++fcounter;
			}

			tinydir_next(&dir);
		} else {
			if (!dirstack.empty()) {
				std::string top = dirstack.top();
				dirstack.pop();
				tinydir_open(&dir, top.c_str());
			} else {
				break;
			}
		}

	}

	tinydir_close(&dir);

	// create register function
	std::string header;
	std::string source;
	if (!cls.empty() || !ens.empty()) {
		createHead(header);
		createSource(hadrs, cls, ens, source);
	}

	std::ofstream out(outadr + "/kitemeta.khgen.h");
	out << header;
	out.close();

	out.open(outadr + "/kitemeta.khgen.cpp");
	out << source;
	out.close();

	printf("\n---------[ Kite header parser log ]---------\n%u file(s) parsed\n%u directory searched\n%u file generated\n",
		   fcounter, dcounter, gcounter);
}