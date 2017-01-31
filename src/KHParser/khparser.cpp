/*
Kite2D Game Framework.
Copyright (C) 2010-2015  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
USA
*/
#include <stdio.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <fstream>
#include <streambuf>
#include <stack>
#include <utility>
#include <cctype>
#include <tinydir/tinydir.h>
#include <hash\mmhash3.h>

std::unordered_map<std::string, std::string> strmap;

#define CLS_ATTRIB 6
#define FUNC_ATTRIB 6
#define VAR_ATTRIB 6
#define COM_ENUM_NAME std::string("CTypes")
#define RES_ENUM_NAME std::string("RTypes")
#define OSTREAM_ENUM_NAME std::string("OStreamTypes")
#define ISTREAM_ENUM_NAME std::string("IStreamTypes")

enum ParsState {
	PS_WORD = 0,	// all names (ex: class, void, myName, ...)
	PS_ENDTOKEN,	// (;)
	PS_MODTOKEN,	// (*&)
	PS_SPLTOKEN,	// (,)
	PS_COLTOKEN,	// (:)
	PS_OTHTOKEN,	// (+-=!)
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
	CT_POD = 1,
	CT_COMPONENT = 2,
	CT_RESOURCE = 4,
	CT_ENTITY = 8,
	CT_SYSTEM = 16,
	CT_ABSTRACT = 32,
	CT_CONTINER = 64,
	CT_ISTREAM = 128,
	CT_OSTREAM = 256,
	CT_SCRIPTABLE = 512,
	CT_OTHER = 1024
};

enum MExportState{
	ES_EXPORT = 0,
	ES_IMPORT,
	ES_NONE
};

enum MOperatorType {
	OT_ADD = 0,
	OT_SUB,
	OT_MUL,
	OT_DIV,
	OT_EQ
};

/*struct MFParam {
	std::string name;
	std::string type;
	std::string defval;
};*/

struct MInfo {
	std::string key;
	std::string info;
};

struct MFunction {
	std::string name;
	std::string customName;
	std::string tokparam;
	bool ista;
	bool inl;
	bool ext;
	bool cons;
	bool vir;
	bool explct;
	unsigned int order;

	MFunction() :
		ista(false), inl(false), ext(false),
		cons(false), vir(false), explct(false),
		order(0)
	{}
};

struct MProperty {
	std::string name;
	MFunction get;
	MFunction set;
	std::string comment;
	std::string type; 
	std::string resType;
	std::string min;
	std::string max;
	std::string show;

	inline bool operator<(const MProperty& Other) {
		return (get.order < Other.get.order);
	}
};

struct MOperator {
	MOperatorType type;
	MFunction fun;
};

struct MVariable {
	std::string name;
	std::string type;
	std::string tokparam;
	bool isStatic;
	bool isConst;
	bool isRes; // is resource pointer

	MVariable() :
		isStatic(false), isConst(false), isRes(false) {}
};

struct MTemplate {
	std::string name;
	std::string types;
};

/*struct MClassBase {
	std::string name;
	MClassBaseAccs accs;

	MClassBase() :
		accs(CA_PRIVATE) {}
};*/

struct MClass {
	std::string name;
	std::string tokparam;
	unsigned int type;
	MExportState exstate;
	MFunction constructure;
	std::string flags;
	std::string templType;
	std::list<MProperty> props; // we should sort properties
	std::vector<MFunction> funcs;
	std::vector<MVariable> vars;
	std::vector<MInfo> infos;
	std::vector<MOperator> opes;
	std::vector<MTemplate> templ;

	MClass() :
		exstate(ES_NONE){}
};

struct MEnumMember {
	std::string name;
	std::string value;
};

struct MEnum {
	std::string name;
	std::string tokparam;
	std::string type;
	std::vector<MEnumMember> members; 
};

unsigned int getHash32(const void *Data, unsigned int Lenght) {
	unsigned int out = 0;
	MurmurHash3_x86_32(Data, Lenght, 13711010, (void *)&out);
	return out;
}

bool isNumber(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

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
	for (size_t i = 0; i < Content.length(); i++) {
		if (Content[i] == Target)
			Content[i] = Replace;
	}
}

// get position of specifi token
unsigned int getTokPos(const std::string &Content, unsigned int Pos, char tok) {
	if (Content.empty())
		return 0;

	if (Pos >= Content.length())
		return 0;

	// ignore space and new line
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == tok) {
			return i;
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
ParsState checkNextRaw(const std::string &Content, unsigned int Pos, bool Colon) {
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

		} else if (Content[i] == '=' || Content[i] == '-' || Content[i] == '+') {
			return PS_OTHTOKEN;

		} else if (Content[i] == ',') {
			return PS_SPLTOKEN;

		} else if ( Content[i] == ';') {
			return PS_ENDTOKEN;

		} else if (Content[i] == '&' || Content[i] == '*' ) {
			return PS_MODTOKEN;

		}else if (Colon && Content[i] == ':'){
			return PS_COLTOKEN;

		} else if (Content[i] == '@') {
			return PS_STRING;

		} else {
			return PS_WORD;
		}
	}

	return PS_END;
}

ParsState checkNext(const std::string &Content, unsigned int Pos) {
	return checkNextRaw(Content, Pos, true);
}

// get next word (token: sapce or new-line "=+-[]{}())
// TODO: baiad baraie in tabe halat haie mokhtalef dar nazar begirim
// masalan baraie VARIABLE, CLASS, FUNCTIN har kodom ie halat joda
// masalan baraie VARIABLE baiad :: va , ro nadide begire
// baraie CLASS baiad : ro dar nazar begire
// baraie FUNCTION ...
// in halat ha da ghaleb iek enum be tabe dade mishe
// on gzine akhar (bool Colon) ham baiad hazf beshe bejash halat VARIABLE biad
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
			Content[i] == '*' || Content[i] == ':' ||
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
				   Content[i] == '|' || Content[i] == ';' ||
				   Content[i] == '\t' || Content[i] == ':'){
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

// replace a body between Start and End token with Rep
void collapseBody(std::string &Content, char Start, char End, const char *Rep, bool CatchToMap = false) {
	if (Content.empty() || Rep != nullptr) {
		return;
	}

	size_t pos = 0;
	size_t counter = 0;
	size_t start = 0;
	size_t end = 0;
	for (pos; pos < Content.size(); ++pos) {
		// begin of body
		if (Content[pos] == Start) {
			if (counter == 0) {
				start = pos;
			}
			++counter;
			continue;
		}

		// end of body
		if (Content[pos] == End) {
			--counter;
			if (counter == 0) {
				end = pos;

				// catch it to strmap
				if (CatchToMap) {
					strmap.insert({ Content.substr(start, (end - start) + 1), Rep });
				}

				// erase body
				Content.erase(start, (end - start) + 1);

				// add a ";" at the end of function declaration
				Content.insert(start, Rep);
				pos = start;
			}
			continue;
		}
	}
}

void collapseTemplate(std::string &Content) {
	collapseBody(Content, '<', '>', getKey().c_str(), true);
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

void splitBy(const std::string &Content, std::string Tok, std::vector<std::pair<std::string, std::string>> &OutList) {
	OutList.clear();
	size_t pos = 0;
	size_t tsize = Tok.length();
	std::string tstr = Content;

	tstr.append(Tok);

	std::vector<std::string> pairs;

	// splite pairs list
	while ((pos = tstr.find(Tok)) != std::string::npos) {
		if (!tstr.substr(0, pos).empty()) {
			pairs.push_back(tstr.substr(0, pos));
		}
		tstr.erase(0, pos + tsize);
	}

	// splite pairs
	for (auto it = pairs.begin(); it != pairs.end(); ++it) {
		if ((pos = it->find("=")) != std::string::npos) {
			OutList.push_back({ it->substr(0, pos), it->substr(pos, it->size()) });
		} else {
			OutList.push_back({ (*it), "" });
		}
	}
}

// split a list of parameter by token (,)
void splitParamRaw(const std::string &Content, std::vector<std::pair<std::string, std::string>> &OutList, bool IgnoreSN) {
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
	std::vector<std::string> pairs;

	// splite pairs list
	while ((pos = temp.find(",")) != std::string::npos) {
		if (!temp.substr(0, pos).empty()) {
			pairs.push_back(temp.substr(0, pos));
		}
		temp.erase(0, pos + tsize);
	}

	// splite pairs
	for (auto it = pairs.begin(); it != pairs.end(); ++it) {
		if ((pos = it->find("=")) != std::string::npos) {
			OutList.push_back({ it->substr(0, pos), it->substr(pos + 1, it->size()) });
		} else {
			OutList.push_back({ (*it), std::string() });
		}
	}
}

void splitParam(const std::string &Content, std::vector<std::pair<std::string, std::string>> &OutList) {
	splitParamRaw(Content, OutList, true);
}

bool procFunc(const std::string &Content, MFunction &Func, unsigned int Pos) {
	std::string output;
	std::vector<std::pair<std::string, std::string>> params;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// its a generic function processor so we dont check number of token parameters here,
	// and there is no warning for empty parameter list
	Pos = getNextBody(Content, Pos, output);
	Func.tokparam = output;

	// check KP_NAME for custom function names (its optional)
	splitParam(output, params);
	for (auto it = params.begin(); it != params.end(); ++it) {
		if (it->first == "KP_NAME") {
			if (it->second.empty()) {
				printf("error: tag without any value detected ==> function name: %s  TAG: KP_NAME\n", Func.name.c_str());
				return false;
			}
			Func.customName = strmap[it->second];
		}
	}

	// check function attributes 
	if (checkNextRaw(Content, Pos, false) != PS_WORD) {
		printf("error: missing function name/attributes.\n");
		return false;
	}

	std::vector<std::string> vList;
	std::string retType;
	vList.reserve(FUNC_ATTRIB);
	for (auto i = 0; i < FUNC_ATTRIB; i++) {
		if (checkNextRaw(Content, Pos, false) != PS_BODY_START) {
			Pos = getNextWord(Content, Pos, output);

			// pointer or ref (return value)
			/*if (checkNextRaw(Content, Pos, false) == PS_MODTOKEN) {
				if (checkTok(Content, Pos, '*')) {
					retType = " *";
				} else if (checkTok(Content, Pos, '&')) {
					retType = " &";
				}
			}*/

			if (output == "inline") {
				Func.inl = true;
			} else if (output == "static") {
				Func.ista = true;
			} else if (output == "extern") {
				Func.ext = true;
			} else if (output == "virtual") {
				Func.vir = true;
			} else if (output == "explicit") {
				Func.explct = true;
			}

			vList.push_back(output);
		} else {
			break;
		}
	}

	if (vList.empty()) {
		printf("error: could not extract function name. something is wrong.");
		return false;
	}

	Func.name = vList.back();

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
	bool ret = true;
	Funcs.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KM_FUN")) != std::string::npos) {
		MFunction tfun;
		if (procFunc(Content, tfun, pos)) {
			Funcs.push_back(tfun);

		} else {
			ret = false;
		}
	}

	return ret;
}

bool procConstructure(const std::string &Content, MFunction &Func) {
	size_t pos = 0;

	// only 1 constructure supported at this time
	if ((pos = findToken(Content, 0, "KM_CON")) != std::string::npos) {

		if (procFunc(Content, Func, pos)) {
			return true;
		}

	} else {
		// no constructure
		return true;
	}

	return false;
}

bool procProp(const std::vector<MFunction> &AllGet, const std::vector<MFunction> &AllSet
			  , std::list<MProperty> &Output) {
	std::vector<std::pair<std::string, std::string>> param;
	std::unordered_map<std::string, MProperty> map;
	Output.clear();

	// atleast we need a getter
	if (AllGet.empty()) {
		return true;
	}

	// first register all getters
	for (size_t i = 0; i < AllGet.size(); i++) {

		// exteract token parameter
		splitParam(AllGet[i].tokparam, param);

		if (param.empty()) {
			printf("error: missing property info ==> function name: %s\n", AllGet[i].name.c_str());
			return false;

		// at least we need 2 info (name and type)
		} else if (param.size() < 2) {
			printf("error: missing property type ==> function name: %s\n", AllGet[i].name.c_str());
			return false;

		}

		std::string name, type, cm, show, min = "0", max = "0", res;

		for (auto it = param.begin(); it != param.end(); ++it) {
			if (it->first == "KP_NAME") {
				if (it->second.empty()) {
					printf("error: tag without any value detected ==> function name: %s  TAG: KP_NAME\n", AllGet[i].name.c_str());
					return false;
				}
				name = it->second;
			} else if (it->first == "KP_TYPE") {
				if (it->second.empty()) {
					printf("error: tag without any value detected ==> function name: %s  TAG: KP_TYPE\n", AllGet[i].name.c_str());
					return false;
				}
				type = it->second;
			} else if (it->first == "KP_CM") {
				if (it->second.empty()) {
					printf("error: tag without any value detected ==> function name: %s  TAG: KP_CM\n", AllGet[i].name.c_str());
					return false;
				}
				cm = it->second;
			} else if (it->first == "KP_SHOW") {
				if (it->second.empty()) {
					printf("error: tag without any value detected ==> function name: %s  TAG: KP_CM\n", AllGet[i].name.c_str());
					return false;
				}
				show = it->second;
			} else if (it->first == "KP_MIN") {
				if (it->second.empty()) {
					printf("error: tag without any value detected ==> function name: %s  TAG: KP_MIN\n", AllGet[i].name.c_str());
					return false;
				}
				min = it->second;
			} else if (it->first == "KP_MAX") {
				if (it->second.empty()) {
					printf("error: tag without any value detected ==> function name: %s  TAG: KP_MAX\n", AllGet[i].name.c_str());
					return false;
				}
				max = it->second;
			} else if (it->first == "KP_RES") {
				if (it->second.empty()) {
					printf("error: tag without any value detected ==> function name: %s  TAG: KP_RES\n", AllGet[i].name.c_str());
					return false;
				}
				res = it->second;
			} else {
				printf("warning: unsupported tag detected ==> function name: %s\n", AllGet[i].name.c_str());
			}
		}

		// extract string name from string map
		if (name.empty() || (strmap.find(name) == strmap.end())) {
			printf("error: could not extract prperty name from string map ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}
		name = strmap[name];

		// extract comment from string map
		if (!cm.empty()){
			if (strmap.find(cm) == strmap.end()) {
				printf("error: could not extract prperty name from string map ==> function name: %s\n", AllGet[i].name.c_str());
				return false;
			}
			cm = strmap[cm];
		}

		// set maxSize as default for KP_RES (mean there is no resource)
		if (res.empty()) {
			res = RES_ENUM_NAME + "::maxSize";
		}

		// set true as defult for KP_SHOW
		if (show.empty() || show != "false") {
			show = "true";
		}

		// register property to map
		auto prop = map.find(name);
		if (prop != map.end()) {
			printf("error: property overload not supported ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}

		// register getter/name/type/comment
		map.insert({ name, MProperty() });
		map[name].name = name;
		map[name].comment = cm;
		map[name].get = AllGet[i];
		map[name].type = type;
		map[name].min = min;
		map[name].max = max;
		map[name].resType = res;
		map[name].show = show;
	}

	// then register all setters
	for (size_t i = 0; i < AllSet.size(); i++) {

		// exteract token parameter
		splitParam(AllSet[i].tokparam, param);

		if (param.empty()) {
			printf("error: missing property info ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		} else if (param.size() > 1) {
			printf("warning: extera property info (ignored) ==> function name: %s\n", AllGet[i].name.c_str());
		}

		std::string name;

		for (auto it = param.begin(); it != param.end(); ++it) {
			if (it->first == "KP_NAME") {
				name = it->second;
			} else {
				printf("warning: unsupported tag detected ==> function name: %s\n", AllGet[i].name.c_str());
			}
		}

		// extract string name from string map
		if (name.empty() || (strmap.find(name) == strmap.end())) {
			printf("error: could not extract prperty name from string map ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}
		name = strmap[name];

		// register property to map
		auto prop = map.find(name);
		if (prop == map.end()) {
			printf("error: setter-only property not allowed ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}

		// register getter/name/type/comment
		map[name].set = AllSet[i];
	}

	// fill output vector
	for (auto it = map.begin(); it != map.end(); ++it) {
		Output.push_back(it->second);
	}

	Output.sort();

	return true;
}

// parse all property in the content
bool procAllProp(const std::string &Content, std::list<MProperty> &Props) {
	size_t pos = 0;
	std::vector<MFunction> allGet;
	std::vector<MFunction> allSet;
	bool ret = true;
	Props.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KM_PRO_GET")) != std::string::npos) {
		MFunction tfunc;
		tfunc.order = pos;

		if (procFunc(Content, tfunc, pos)) {
			allGet.push_back(tfunc);

		} else {
			ret = false;
		}
	}

	pos = 0;
	while ((pos = findToken(Content, pos, "KM_PRO_SET")) != std::string::npos) {
		MFunction tfunc;
		tfunc.order = pos;

		if (procFunc(Content, tfunc, pos)) {
			allSet.push_back(tfunc);

		} else {
			ret = false;
		}
	}

	if (ret) {
		if (!procProp(allGet, allSet, Props)) {
			ret = false;
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

	std::vector<std::pair<std::string, std::string>> param;
	splitParam(Var.tokparam, param);
	for (auto it = param.begin(); it != param.end(); ++it) {
		if (it->first == "KV_RES") { Var.isRes = true; }
	}

	// check variable
	if (checkNextRaw(Content, Pos, false) != PS_WORD) {
		printf("error: missing variable name/modifiers.\n");
		return false;
	}

	// exteract variable name/attribute/array/size/....
	auto tpos = getTokPos(Content, Pos, ';');
	std::vector<std::string> vList;
	vList.reserve(VAR_ATTRIB);
	for (auto i = 0; i < VAR_ATTRIB; i++) {
		if (Pos < tpos && (checkNext(Content, Pos) != PS_BODY_START)) {
			Pos = getNextWord(Content, Pos, output);

			// is static
			if (output == "static") {
				Var.isStatic = true;
				continue;
			} else if (output == "const") {
				Var.isConst = true;
				continue;
			}

			vList.push_back(output);
		} else {
			break;
		}
	}

	if (vList.empty() || vList.size() < 2) {
		printf("error: could not extract variable name/type. something is wrong.");
		return false;
	}

	// name
	Var.name = vList.back();
	vList.pop_back();
	
	// type
	Var.type = vList.back();

	return true;
}

// parse all VAR tokens
bool procAllVar(const std::string &Content, std::vector<MVariable> &Vars) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Vars.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KM_VAR")) != std::string::npos) {
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

bool procInfo(const std::string &Content, std::vector<MInfo> &AllInfo, unsigned int Pos) {
	std::string output;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// its a generic variable processor so we dont check number of token parameters here,
	// and there is no warning for empty parameter list
	Pos = getNextBody(Content, Pos, output);
	
	std::vector<std::pair<std::string, std::string>> param;
	splitParam(output, param);

	if (!param.empty()) {
		for (auto it = param.begin(); it != param.end(); ++it) {
			MInfo info;
			info.key = it->first;
			info.info = it->second;
			if (strmap.find(it->second) != strmap.end()) {
				info.info = strmap[it->second];
			}
			AllInfo.push_back(info);
		}
	}else{
		printf("missing info parameter.");
	}

	return true;
}

// parse all INFO tokens
bool procAllInfo(const std::string &Content, std::vector<MInfo> &Info) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Info.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KM_INFO")) != std::string::npos) {
		procInfo(Content, Info, pos);
		++count;
	}

	return ret;
}

bool procOper(std::vector<MOperator> &Oprs, const std::vector<MFunction> &Funs) {
	std::vector<std::pair<std::string, std::string>> param;

	for (size_t i = 0; i < Funs.size(); ++i) {
		splitParam(Funs[i].tokparam, param);
		if (param.empty()) {
			printf("missing operator type. oname: %s\n", Funs[i].name.c_str());
			return false;
		} else if (param.size() > 1) {
			printf("extera operator type ignored. oname: %s\n", Funs[i].name.c_str());
		}

		if (param[0].first == "KO_ADD") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_ADD;
			Oprs.push_back(opr);
			continue;
		}

		if (param[0].first == "KO_SUB") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_SUB;
			Oprs.push_back(opr);
			continue;
		}

		if (param[0].first == "KO_MUL") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_MUL;
			Oprs.push_back(opr);
			continue;
		}

		if (param[0].first == "KO_DIV") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_DIV;
			Oprs.push_back(opr);
			continue;
		}

		if (param[0].first == "KO_EQ") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_EQ;
			Oprs.push_back(opr);
			continue;
		}
	}

	return true;
}

// parse all class operators
bool procAllOper(const std::string &Content, std::vector<MOperator> &Oprs) {
	size_t pos = 0;
	unsigned int count = 0;
	std::vector<MFunction> funvec;
	bool ret = true;
	Oprs.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = findToken(Content, pos, "KM_OPE")) != std::string::npos) {
		MFunction tfun;
		if (procFunc(Content, tfun, pos)) {
			funvec.push_back(tfun);

		} else {
			printf("operator token number: %u \n", count);
			ret = false;
		}
		++count;
	}

	ret = procOper(Oprs, funvec);

	return ret;
}

bool procTemplate(const std::string &Content, MTemplate &Temps, unsigned int Pos) {
	std::string output;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	Pos = getNextBody(Content, Pos, output);

	std::vector<std::pair<std::string, std::string>> param;
	splitParam(output, param);

	if (param.size() < 2) {
		printf("error: missing template value.\n");
		return false;
	}

	Temps.name = strmap[param[0].first];

	for (size_t i = 1; i < param.size(); ++i) {
		Temps.types.append(param[i].first);
		if ((i + 1) < param.size()) {
			Temps.types.append(", ");
		}
	}

	return true;
}

bool procAllTemplate(const std::string &Content, MClass &Cls) {
	size_t pos = 0;
	unsigned int count = 0;
	bool ret = true;
	Cls.templ.clear();

	if (Content.empty()) {
		return false;
	}

	if ((pos = findToken(Content, 0, "KM_TEM_PARAM")) != std::string::npos){
		std::string output;
		if (checkNext(Content, pos) != PS_BODY_START) {
			printf("error: missing ().\n");
			return false;
		}

		pos = getNextBody(Content, pos, output);

		std::vector<std::pair<std::string, std::string>> param;
		splitParam(output, param);

		if (param.empty()) {
			printf("error: empty template types list.\n");
			return false;
		}

		for (size_t i = 0; i < param.size(); ++i) {
			Cls.templType.append(param[i].first);
			if ((i + 1) < param.size()) {
				Cls.templType.append(", ");
			}
		}
		
		pos = 0;
		while ((pos = findToken(Content, pos, "KM_TEM_DEF")) != std::string::npos) {
			MTemplate ttempl;
			if (procTemplate(Content, ttempl, pos)) {
				Cls.templ.push_back(ttempl);

			} else {
				printf("template token number: %u \n", count);
				ret = false;
			}
			++count;
		}
	}
	return ret;
}

// parse and extract class information
bool procClass(const std::string &Content, MClass &Cls, unsigned int Pos) {
	size_t pos;
	std::string output;
	//std::vector<std::pair<std::string, std::string>> params;

	// checking token parameter(s)
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// extract token parameter(s)
	pos = getNextBody(Content, Pos, output);
	Cls.tokparam = output;

	// class type
	std::vector<std::pair<std::string,std::string>> tags;
	splitParam(output, tags);
	Cls.type = CT_OTHER;

	for (auto it = tags.begin(); it != tags.end(); ++it) {
		// is abstract
		if (it->first == "ABSTRACT") {
			Cls.type = Cls.type | CT_ABSTRACT;
		}

		// is component
		if (it->first == "COMPONENT") {
			Cls.type = Cls.type | CT_COMPONENT;
		}

		// is system
		if (it->first == "SYSTEM") {
			Cls.type = Cls.type | CT_SYSTEM;
		}

		// is resource
		if (it->first == "RESOURCE") {
			Cls.type = Cls.type | CT_RESOURCE;
		}

		// is istream
		if (it->first == "ISTREAM") {
			Cls.type = Cls.type | CT_ISTREAM;
		}

		// is ostream
		if (it->first == "OSTREAM") {
			Cls.type = Cls.type | CT_OSTREAM;
		}
	}

	// checking type token (class or struct)
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing class/struct type.\n");
		return false;
	}

	pos = getNextWord(Content, pos, output);
	if (output != "class" && output != "struct") {
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

	// class body
	if ((pos = getTokPos(Content, pos, '{')) == std::string::npos) {
		printf("error: missing class\\structure body.\n");
		return false;
	}

	if (checkNext(Content, pos) == PS_BODY_START) {

		// get class body
		std::string cbody;
		getNextBody(Content, pos, cbody);

		// collapse body of inline functions
		collapseBody(cbody, '{', '}', ";");

		if (!procConstructure(cbody, Cls.constructure)) {
			return false;
		}
		
		// parse all properties in the class body
		if (!procAllProp(cbody, Cls.props)) {
			return false;
		}

		// parse all function in the class body
		if (!procAllFunc(cbody, Cls.funcs)) {
			return false;
		}

		// parse all variables in the class body
		if (!procAllVar(cbody, Cls.vars)) {
			return false;
		}

		// parse info in the class body
		if (!procAllInfo(cbody, Cls.infos)) {
			return false;
		}

		// parse operators in the class body
		if (!procAllOper(cbody, Cls.opes)) {
			return false;
		}

		// parse template in the class body
		if (!procAllTemplate(cbody, Cls)) {
			return false;
		}

	// incorrect structure
	} else {
		printf("error: missing class\\structure body.\n");
		return false;
	}

	return true;
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

	while ((pos = findToken(Content, pos, "KM_CLASS")) != std::string::npos) {
		MClass tcls;
		if (procClass(Content, tcls, pos)) {
			Classes.push_back(tcls);

		} else {
			unsigned int line = tokenCounter(Content.substr(0, pos), "\n");
			printf("line number: %u ==> token name: KM_CLASS ==> token number: %u \n", ++line, count);
			ret = false;
		}
		++count;
	}

	return ret;
}

bool procEnumMem(const std::string &Content, std::vector<MEnumMember> &Memers) {
	std::vector<std::pair<std::string, std::string>> param;
	Memers.clear();
	splitParam(Content, param);
	if (param.empty()) {
		printf("error: empty enum.\n");
		return false;
	}

	// member value
	for (size_t i = 0; i < param.size(); ++i) {
		// without value
		if (!param[i].first.empty()) {
			MEnumMember emem;
			emem.name = param[i].first;
			emem.value = param[i].second;
			Memers.push_back(emem);
			continue;
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
		printf("error: incorrect type (%s)\n", output.c_str());
		return false;
	}

	// class token
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing enum class type.\n");
		return false;
	}

	pos = getNextWord(Content, pos, output);
	if (output != "class") {
		printf("error: only enum class (scoped enum) supported.\n");
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
	if (checkNext(Content, pos) == PS_COLTOKEN) {
		pos = getTokPos(Content, pos, ':');
		++pos;

		if (checkNextRaw(Content, pos, false) != PS_WORD) {
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

	if (!procEnumMem(ebody, Enm.members)) {
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

	while ((pos = findToken(Content, pos, "KM_ENUM")) != std::string::npos) {
		MEnum tenu;
		if (procEnum(Content, tenu, pos)) {
			Enums.push_back(tenu);

		} else {
			unsigned int line = tokenCounter(Content.substr(0, pos), "\n");
			printf("line number: %u ==> token name: KM_ENUM ==> token number: %u \n", ++line, count);
			ret = false;
		}
		++count;
	}

	return ret;
}

bool isIgnored(const std::string &Contetnt) {
	if (Contetnt.find("KM_IGNORED") != std::string::npos) {
		return true;
	}
	return false;
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
	collapseTemplate(Content);
	removeTok(Content, "public:");
	removeTok(Content, "private:");
	removeTok(Content, "protected:");

	if (isIgnored(Content)) {
		printf("ignored.\n");
		return false;
	}

	if (!procAllClass(Content, Classes))
		ret = false;

	if (!procAllEnum(Content, Enums))
		ret = false;

	return ret;
}

void createTemplMacro(const MClass &Cls, std::string &Output) {
	std::string upname = Cls.name;
	std::string cParam = Cls.tokparam;
	replaceTok(cParam, '|', ',');
	std::vector<std::pair<std::string,std::string>> ctags;
	splitParam(cParam, ctags);

	// we only support POD's class for template
	if (std::find(ctags.begin(), ctags.end(), std::pair<std::string, std::string>({ "POD", "" })) == ctags.end()) {
		printf("parser only support template for POD's\n");
		return;
	}

	// class body
	Output.append("\n// ----[auto generated: " + Cls.name + " body macro]----\n");
	transform(upname.begin(), upname.end(), upname.begin(), toupper);
	Output.append("#define KMETA_" + upname + "_BODY() \\\n");

	// public section:
	// register functions
	Output.append("public:\\\n");

	// serialize
	Output.append("friend KBaseSerial &operator<<(KBaseSerial &Out, const " + Cls.name + "<" + Cls.templType + "> &Value) {\\\n");
	for (size_t i = 0; i < Cls.vars.size(); ++i) {
		Output.append("Out << Value." + Cls.vars[i].name + ";\\\n");
	}
	Output.append("return Out;}\\\n");
							
	Output.append("friend KBaseSerial &operator>>(KBaseSerial &In, " + Cls.name + "<" + Cls.templType + "> &Value) {\\\n");
	for (size_t i = 0; i < Cls.vars.size(); ++i) {
		Output.append("In >> Value." + Cls.vars[i].name + ";\\\n");
	}
	Output.append("return In;}\\\n");

	// meta registration
	replaceTok(cParam, ',', '|');
	Output.append("static void registerMeta(const std::string &Name, KMetaManager *MMan, lua_State *Lua = nullptr){\\\n"
				  "static KMetaClass instance(Name," + cParam + ", sizeof(" + Cls.name + "<" + Cls.templType + ">));\\\n"
				  "static bool initeMeta = false;\\\n"
				  "if (MMan != nullptr){MMan->setMeta((KMetaBase *)&instance);}\\\n"
				  "if (!initeMeta){\\\n");

	// informations
	for (size_t count = 0; count < Cls.infos.size(); ++count) {
		Output.append("instance.addInfo({\"" + Cls.infos[count].key + "\", \"" + Cls.infos[count].info + "\"});\\\n");
	}

	// properties (meta)
	for (auto it = Cls.props.begin(); it != Cls.props.end(); ++it) {
		std::string prpType("KMP_BOTH");
		if (it->set.name.empty()) {
			prpType = "KMP_GETTER";
		} else {
			prpType = "KMP_BOTH";
		}
		Output.append("instance.addProperty(KMetaProperty(\"" + it->name + "\", \""
					  + it->type + "\", \"" + it->comment + "\", "
					  + prpType + ", " + it->min + ", " + it->max + "));\\\n");
	}

	// functions
	for (size_t count = 0; count < Cls.funcs.size(); count++) {
		std::string ista = "false";
		if (Cls.funcs[count].ista) {
			ista = "true";
		}

		Output.append("instance.addFunction(KMetaFunction(\"" + Cls.funcs[count].name + "\", " + ista + " ));\\\n");
	}
	Output.append("}\\\n");

	// lua binding 
	Output.append("if (Lua != nullptr) { \\\n"
					"LuaIntf::LuaBinding(Lua).beginModule(\"kite\").beginClass<"
					+ Cls.name + "<" + Cls.templType + ">>(Name.c_str())\\\n");

	// constructure
	std::string param;
	if (!Cls.constructure.name.empty()) {
		std::vector<std::pair<std::string, std::string>> cplist;
		splitParam(Cls.constructure.tokparam, cplist);
		if (!cplist.empty()) {
			for (size_t count = 0; count < cplist.size(); count++) {
				param.append("LuaIntf::_opt<" + cplist[count].first + ">");
				if ((count + 1) != cplist.size()) {
					param.append(", ");
				}
			}
		}
		Output.append(".addConstructor(LUA_ARGS(" + param + "))\\\n");
	}

	// properties
	for (auto it = Cls.props.begin(); it != Cls.props.end(); ++it) {
		if (!it->get.name.empty() && !it->set.name.empty()) {
			Output.append(".addProperty(\"" + it->name + "\", &" + Cls.name + "<" + Cls.templType + ">::"
							+ it->get.name + ", &" + Cls.name + "<" + Cls.templType + ">::" + it->set.name + ")\\\n");
		} else if (it->set.name.empty()) {
			Output.append(".addProperty(\"" + it->name + "\", &" + Cls.name + "<" + Cls.templType + ">::"
							+ it->get.name + ")\\\n");
		}
	}

	// functions 
	std::string fista;
	for (size_t count = 0; count < Cls.funcs.size(); count++) {

		if (Cls.funcs[count].ista) {
			fista = "addStaticFunction";
		} else {
			fista = "addFunction";
		}
		Output.append("." + fista + "(\"" + Cls.funcs[count].name + "\", &" + Cls.name + "<" + Cls.templType + ">::"
					  + Cls.funcs[count].name + ")\\\n");
	}

	// operators
	for (size_t count = 0; count < Cls.opes.size(); count++) {
		if (Cls.opes[count].type == OT_ADD) {
			Output.append(".addFunction(\"__add\", &" + Cls.name + "<" + Cls.templType + ">::" + Cls.opes[count].fun.name + ")\\\n");
			continue;
		}

		if (Cls.opes[count].type == OT_SUB) {
			Output.append(".addFunction(\"__sub\", &" + Cls.name + "<" + Cls.templType + ">::" + Cls.opes[count].fun.name + ")\\\n");
			continue;
		}

		if (Cls.opes[count].type == OT_MUL) {
			Output.append(".addFunction(\"__mul\", &" + Cls.name + "<" + Cls.templType + ">::" + Cls.opes[count].fun.name + ")\\\n");
			continue;
		}

		if (Cls.opes[count].type == OT_DIV) {
			Output.append(".addFunction(\"__div\", &" + Cls.name + "<" + Cls.templType + ">::" + Cls.opes[count].fun.name + ")\\\n");
			continue;
		}

		if (Cls.opes[count].type == OT_EQ) {
			Output.append(".addFunction(\"__eq\", &" + Cls.name + "<" + Cls.templType + ">::" + Cls.opes[count].fun.name + ")\\\n");
			continue;
		}
	}

	// variables
	std::vector<std::pair<std::string, std::string>> tpar;
	for (size_t count = 0; count < Cls.vars.size(); count++) {
		splitParam(Cls.vars[count].tokparam, tpar);
		if (!tpar.empty()) {
			if (tpar[0].first == "UNBIND") {
				continue;
			}
		}
		std::string fista;
		if (Cls.vars[count].isStatic) {
			fista = "addStaticVariableRef";
		} else {
			fista = "addVariableRef";
		}
		Output.append("." + fista + "(\"" + Cls.vars[count].name + "\", &" + Cls.name + "<" + Cls.templType + ">::"
						+ Cls.vars[count].name + ")\\\n");
	}

	// end of lua binding
	Output.append(".endClass().endModule();}\\\n");
	Output.append("}\n");
}

void luaPrefabPOD(std::string &Output, const std::string &Name, const std::string &Type) {
	std::string type;
	if (Type == "string") {
		type = "\\\"\" + " + Name + " + \"\\\"";
	} else if (Type == "F32" || Type == "F64" ||
			   Type == "I32" || Type == "U32" ||
			   Type == "I16" || Type == "U16" ||
			   Type == "I8" || Type == "U8") {
		type = "\" + std::to_string(" + Name + ") + \"";
	} else if (Type == "bool") {
		Output.append("if (" + Name + "){\\\n");
		Output.append("code.append(Name + \"." + Name + " = true\\n\");\\\n");
		Output.append("}else{\\\n");
		Output.append("code.append(Name + \"." + Name + " = false\\n\");}\\\n");
	}
	Output.append("code.append(Name + \"." + Name + " = " + type + "\\n\");\\\n");
}

void createMacros(const std::vector<MClass> &Cls, const std::vector<MEnum> &Enms, std::string &Output) {
	Output.clear();

	// enums
	for (size_t i = 0; i < Enms.size(); i++) {
		std::string upname = Enms[i].name;
		std::string eParam = Enms[i].tokparam;
		replaceTok(eParam, ',', '|');

		Output.append("\n// ----[auto generated: " + Enms[i].name + " body macro]----\n");
		transform(upname.begin(), upname.end(), upname.begin(), toupper);
		Output.append("#define KMETA_" + upname + "_BODY() \\\n"
					  "namespace Internal{\\\n"
					  "struct Register" + Enms[i].name + "{\\\n"
					  "static void registerMeta(KMetaManager *MMan = nullptr, lua_State *Lua = nullptr);};}\\\n");


		Output.append("\n// ----[auto generated: " + Enms[i].name + " source macro]----\n");
		Output.append("#define KMETA_" + upname + "_SOURCE() \\\n"
					  //"namespace Internal{\\\n"
					  "void Internal::Register" + Enms[i].name + "::registerMeta(KMetaManager *MMan, lua_State *Lua){\\\n"
					  "static KMetaEnum instance(\"" + Enms[i].name + "\", 0, sizeof(" + Enms[i].name + "));\\\n"
					  "static bool initeMeta = false;\\\n"
					  "if (MMan != nullptr){MMan->setMeta((KMetaBase *)&instance);}\\\n"
					  "if (!initeMeta){\\\n");

		// enum members
		for (size_t count = 0; count < Enms[i].members.size(); ++count) {
			std::string value;
			std::string number = std::to_string(count);
			if (Enms[i].members[count].value.empty()) {
				value = number;
			} else {
				value = Enms[i].members[count].value;
			}
			Output.append("instance.addMember(KMetaEnumMember(\"" + Enms[i].members[count].name + "\", " + value + ", " + number + "));\\\n");
		}
		Output.append("initeMeta = true;}\\\n");

		// lua binding
		Output.append("if (Lua != nullptr){\\\n"
					  "LuaIntf::LuaBinding(Lua).beginModule(\"kite\")\\\n"
					  ".beginModule(\"" + Enms[i].name + "\")\\\n");
		for (size_t count = 0; count < Enms[i].members.size(); ++count) {
			Output.append(".addConstant(\"" + Enms[i].members[count].name + "\", " + Enms[i].name + "::" 
						  + Enms[i].members[count].name + ")\\\n");
		}

		// end of lua binding
		Output.append(".endModule().endModule();}\\\n");

		// end of definition
		Output.append("}\n");
	}

	// class
	for (size_t i = 0; i < Cls.size(); i++) {
		std::string upname = Cls[i].name;
		std::string cParam = Cls[i].tokparam;
		replaceTok(cParam, '|', ',');
		std::vector<std::pair<std::string, std::string>> ctags;
		splitParam(cParam, ctags);

		// is template class (templated class)
		if (!Cls[i].templ.empty()) {
			createTemplMacro(Cls[i], Output);
			continue;
		}

		// is serializable
		bool isPOD = false;
		bool isResource = false;
		bool isComponent = false;
		bool isEntity = false;
		bool isSystem = false;
		bool isContiner = false;
		bool isIStream = false;
		bool isOStream = false;
		bool isScriptable = false;
		bool isAbstract = false;
		bool isSerializer = false;
		for (auto it = ctags.begin(); it != ctags.end(); ++it) {
			if (it->first == "POD") {
				isPOD = true;
			}

			// is scriptable
			if (it->first == "RESOURCE") {
				isResource = true;
			}

			// is editable
			if (it->first == "COMPONENT") {
				isComponent = true;
			}

			// is entity
			if (it->first == "ENTITY") {
				isEntity = true;
			}

			// is system
			if (it->first == "SYSTEM") {
				isSystem = true;
			}

			// is abstract
			if (it->first == "ABSTRACT") {
				isAbstract = true;
			}

			// is continer
			if (it->first == "CONTINER") {
				isContiner = true;
			}

			// is istream
			if (it->first == "ISTREAM") {
				isIStream = true;
			}

			// is ostream
			if (it->first == "OSTREAM") {
				isOStream = true;
			}

			// is scriptable
			if (it->first == "SCRIPTABLE") {
				isScriptable = true;
			}

			// is serializer
			if (it->first == "SERIALIZER") {
				isSerializer = true;
			}
		}

		// class without any flag will ignored
		if (!isPOD && !isEntity && !isResource &&
			!isComponent && !isSystem && !isScriptable &&
			!isContiner && !isIStream && !isOStream &&
			!isSerializer) {
			printf("message: class without any supported flags. %s ignored. \n", Cls[i].name.c_str());
			continue;
		}

		// ctype, rtype
		std::string shortName = Cls[i].name;
		for (auto it = Cls[i].infos.begin(); it != Cls[i].infos.end(); ++it) {
			if (it->key == "KI_NAME") {
				shortName = it->info;
			}
		}

		// linkage state
		std::string exstate;
		if (Cls[i].exstate == ES_NONE) {
			exstate = "KITE_FUNC_EXPORT ";
		}

		// class body
		Output.append("\n// ----[auto generated: " + Cls[i].name + " body macro]----\n");
		transform(upname.begin(), upname.end(), upname.begin(), toupper);
		Output.append("#define KMETA_" + upname + "_BODY() \\\n");
		
		// public section:
		// register functions
		Output.append("public:\\\n");

		// factory methode 
		if (!isAbstract) {
			if (isResource) {
				Output.append(exstate + "static KResource *factory(const std::string &Name);\\\n");
			} else if (isIStream) {
				Output.append(exstate + "static KIStream *factory();\\\n");
			}
		}

		// property setter/getter, getBase, lua cast (only components have properties)
		if (isComponent && !isAbstract) {
			Output.append(exstate + "bool setProperty(const std::string &Name, KAny &Value) override;\\\n" +
						  exstate + "KAny getProperty(const std::string &Name) const override;\\\n");
		}

		// serializable
		if (isPOD || isComponent || isEntity) {
			if (!isAbstract) {
				Output.append("friend KBaseSerial &operator<<(KBaseSerial &Out, const " + Cls[i].name + " &Value) {\\\n"
							  "Value.serial(Out); return Out;}\\\n"
							  "friend KBaseSerial &operator>>(KBaseSerial &In, " + Cls[i].name + " &Value) {\\\n"
							  "Value.deserial(In); return In;}\\\n");

			}
			// base component (polymorphism serial)
			if(isAbstract && isComponent) {
				Output.append(exstate + "virtual void serial(KBaseSerial &Serializer, bool Base = true) const = 0;\\\n");
				Output.append(exstate + "virtual void deserial(KBaseSerial &Serializer, bool Base = true) = 0;\\\n");
				Output.append(exstate + "void baseSerial(KBaseSerial &Serializer, bool Base = true) const;\\\n");
				Output.append(exstate + "void baseDeserial(KBaseSerial &Serializer, bool Base = true);\\\n");

			// others
			} else {
				Output.append(exstate + "void serial(KBaseSerial &Serializer, bool Base = true) const;\\\n");
				Output.append(exstate + "void deserial(KBaseSerial &Serializer, bool Base = true);\\\n");
			}

		}

		// component lua cast and base access
		std::string hash = std::to_string(getHash32(shortName.c_str(), shortName.length()));;
		std::string enumType;
		std::string baseName;
		if (isResource && !isAbstract) {
			enumType = RES_ENUM_NAME;
			baseName = "KResource";
		}else if (isComponent && !isAbstract) {
			enumType = COM_ENUM_NAME;
			baseName = "KComponent";
		} else if (isOStream && !isAbstract) {
			enumType = OSTREAM_ENUM_NAME;
			baseName = "KOStream";
		} else if (isIStream && !isAbstract) {
			enumType = ISTREAM_ENUM_NAME;
			baseName = "KIStream";
		}

		// (base, lua cast, type)
		if ((isIStream || isOStream || isComponent || isResource) && !isAbstract) {
			Output.append("private:\\\n");
			if (isComponent) {
				// resource pointers
				for (auto it = Cls[i].vars.begin(); it != Cls[i].vars.end(); ++it) {
					if (it->isRes) {
						Output.append("std::string " + it->name + "Name;\\\n");
					}
				}

				Output.append("void loadRes() override;\\\n");
			}

			Output.append(exstate + "static " + Cls[i].name + " *to" + shortName + "(" + baseName + " *Base) {\\\n" +
						  "if (Base != nullptr){\\\n"
						  "if (Base->getHashType() == " + hash + ") {\\\n"
						  "return (" + Cls[i].name + " *)Base; }};\\\n" +
						  "return nullptr; }\\\n" +
						  exstate + "inline " + baseName + " *getBase() const override { return (" + baseName + " *) this; }\\\n" +
						  "public:\\\n" +
						  exstate + "inline " + enumType + " getType() const override { return " + enumType + "::" + shortName + "; }\\\n" +
						  exstate + "inline std::string getTypeName() const override { return \"" + shortName + "\"; }\\\n" +
						  exstate + "inline U32 getHashType() const override { return " + hash + "; }\\\n");
			// clone
			if (isIStream) {
				Output.append(exstate + baseName + " *clone() const override { return (" +
							  baseName + " *)new " + Cls[i].name + "(); }\\\n");
			}
		}

		Output.append("public:\\\n" +
					  exstate + "static void registerMeta(KMetaManager *MMan = nullptr, lua_State *Lua = nullptr);\\\n"
					  "virtual const std::string &getClassName() const { static std::string name(\"" + Cls[i].name + "\");\\\n"
					  "return name;}\n");

		// defention
		Output.append("\n// ----[auto generated: " + Cls[i].name + " source macro]----\n");
		Output.append("#define KMETA_" + upname + "_SOURCE()\\\n");

		// factory defention
		if (!isAbstract) {
			if (isResource) {
				Output.append("KResource *" + Cls[i].name + "::factory(const std::string &Name){\\\n"
							  "return new " + Cls[i].name + "(Name);}\\\n");
			} else if (isIStream) {
				Output.append("KIStream *" + Cls[i].name + "::factory(){\\\n"
							  "return new " + Cls[i].name + "();}\\\n");
			}
		}

		// register properties and loadRes
		if (isComponent && !isAbstract) {

			// property setter/getter (only components have properties)
			Output.append("bool " + Cls[i].name + "::setProperty(const std::string &Name, KAny &Value){\\\n");
			for (auto it = Cls[i].props.begin(); it != Cls[i].props.end(); ++it) {
				if (!it->set.name.empty()) {
					Output.append("if (Name == \"" + it->name + "\"){\\\n");
					if (it->resType == "RTypes::maxSize") { // checking resource type
						Output.append("if (Value.is<" + it->type  + ">()){\\\n" +
									  it->set.name + "(Value.as<" + it->type + ">());\\\n");
					} else {
						Output.append("if (Value.is<KResource *>()){\\\n" 
									  "if (Value.as<KResource *>()->getType() == " + it->resType + "){\\\n" +
									  it->set.name + "((" + it->type + " *)Value.as<KResource *>());\\\n"
									  "return true;\\\n"
									  "}else{ KD_FPRINT(\"incorrect resource type. pname: %s\", Name.c_str()); return false;}\\\n");
					}

					Output.append("}else{ KD_FPRINT(\"incorrect property type. pname: %s\", Name.c_str()); return false;}}\\\n");
				}
			}
			Output.append("return false;}\\\n");

			// get
			Output.append("KAny " + Cls[i].name + "::getProperty(const std::string &Name) const {\\\n");
			for (auto it = Cls[i].props.begin(); it != Cls[i].props.end(); ++it) {
				if (!it->get.name.empty()) {
					Output.append("if (Name == \"" + it->name + "\"){\\\n"
								 "return " + it->get.name + "();}\\\n");
				}
			}
			Output.append("return KAny();}\\\n");

			Output.append("void " + Cls[i].name + "::loadRes(){\\\n");
			// resource pointers
			Output.append("if (!getRMan()) { KD_PRINT(\"ResourceManager is null. engine is not initialized\"); return;}\\\n");
			for (auto it = Cls[i].vars.begin(); it != Cls[i].vars.end(); ++it) {
				if (it->isRes) {
					Output.append("if (" + it->name + "Name.empty()){ " + it->name + " = nullptr;}\\\n"
								  "else{ " + it->name + " = (" + it->type + " *)getRMan()->load(" + it->name + "Name);}\\\n");
				}
			}
			Output.append("}\\\n");
		}

		// meta registration
		replaceTok(cParam, ',', '|');
		Output.append("void " + Cls[i].name + "::registerMeta(KMetaManager *MMan, lua_State *Lua){\\\n"
					  "static KMetaClass instance(\"" + Cls[i].name + "\"," + cParam + ", sizeof(" + Cls[i].name + "));\\\n"
					  "static bool initeMeta = false;\\\n"
					  "if (MMan != nullptr){MMan->setMeta((KMetaBase *)&instance);}\\\n"
					  "if (!initeMeta){\\\n");

		// informations
		for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
			Output.append("instance.addInfo({\"" + Cls[i].infos[count].key + "\", \"" + Cls[i].infos[count].info + "\"});\\\n");
		}

		// properties (meta)
		for (auto it = Cls[i].props.begin(); it != Cls[i].props.end(); ++it) {
			std::string prpType("KMP_BOTH");
			if (it->set.name.empty()) {
				prpType = "KMP_GETTER";
			} else {
				prpType = "KMP_BOTH";
			}
			Output.append("instance.addProperty(KMetaProperty(\"" + it->name + "\", "
						  + "\"" + it->type + "\", \"" + it->comment + "\", "
						  + it->show + ", "
						  + prpType + ", " + it->min + ", " + it->max + ", "
						  + it->resType + "));\\\n");
		}

		
		// predefined functions and props
		if ((isOStream || isIStream || isResource || isComponent) && !isAbstract) {
			Output.append("instance.addFunction(KMetaFunction(\"to" + shortName + "\", true));\\\n");
			Output.append("instance.addProperty(KMetaProperty(\"type\", \"" 
						  + enumType + "\", \"type of the object\", false, KMP_GETTER, 0, 0, " + RES_ENUM_NAME + "::maxSize ));\\\n");
			Output.append("instance.addProperty(KMetaProperty(\"typeName\", \"std::string\", \"name of the object type\", false, KMP_GETTER, 0, 0, " 
						  + RES_ENUM_NAME + "::maxSize ));\\\n");
			Output.append("instance.addProperty(KMetaProperty(\"hashType\", \"U32\", \"hash of the object type\", false, KMP_GETTER, 0, 0, "
						  + RES_ENUM_NAME + "::maxSize ));\\\n");
		}

		// functions
		for (size_t count = 0; count < Cls[i].funcs.size(); count++) {
			std::string ista = "false";
			if (Cls[i].funcs[count].ista) {
				ista = "true";
			} 

			std::string fname = Cls[i].funcs[count].name;
			if (!Cls[i].funcs[count].customName.empty()) {
				fname = Cls[i].funcs[count].customName;
			}

			Output.append("instance.addFunction(KMetaFunction(\"" + fname + "\", " + ista + " ));\\\n");
		}
		Output.append("initeMeta = true;}\\\n");

		// lua binding 
		if (isComponent || isScriptable || isEntity || isPOD ||
			isSystem || isIStream || isOStream || isResource || isSerializer) {

			Output.append("if (Lua != nullptr) { \\\n"
						  "LuaIntf::LuaBinding(Lua).beginModule(\"kite\").beginClass<"
						  + Cls[i].name + ">(\"" + shortName + "\")\\\n");

			// constructure
			std::string param;
			if (!Cls[i].constructure.name.empty() && !isAbstract) {
				std::vector<std::pair<std::string, std::string>> cplist;
				splitParam(Cls[i].constructure.tokparam, cplist);
				if (!cplist.empty()) {
					for (size_t count = 0; count < cplist.size(); count++) {
						param.append("LuaIntf::_opt<" + cplist[count].first + ">");
						if ((count + 1) != cplist.size()) {
							param.append(", ");
						}
					}
				}
				Output.append(".addConstructor(LUA_ARGS(" + param + "))\\\n");
			}

			// predefined functions (base, lua cast)
			if ((isIStream || isOStream || isResource || isComponent) && !isAbstract) {
				Output.append(".addProperty(\"hashType\", &" + Cls[i].name + "::getHashType)\\\n");
				Output.append(".addProperty(\"type\", &" + Cls[i].name + "::getType)\\\n");
				Output.append(".addProperty(\"typeName\", &" + Cls[i].name + "::getTypeName)\\\n");
				Output.append(".addProperty(\"base\", &" + Cls[i].name + "::getBase)\\\n");
				Output.append(".addStaticFunction(\"to" + shortName + "\", &" + Cls[i].name + "::to" + shortName + ")\\\n");
			}

			// serializer functions
			if (isPOD || isComponent || isEntity) {
				Output.append(".addFunction(\"serial\", &" + Cls[i].name + "::serial)\\\n");
				Output.append(".addFunction(\"deserial\", &" + Cls[i].name + "::deserial)\\\n");
			}

			// properties
			for (auto it = Cls[i].props.begin(); it != Cls[i].props.end(); ++it) {
				if (!it->get.name.empty() && !it->set.name.empty()) {
					Output.append(".addProperty(\"" + it->name + "\", &" + Cls[i].name + "::"
								  + it->get.name + ", &" + Cls[i].name + "::" + it->set.name + ")\\\n");
				} else if (it->set.name.empty()) {
					Output.append(".addProperty(\"" + it->name + "\", &" + Cls[i].name + "::"
								  + it->get.name + ")\\\n");
				}
			}

			// functions 
			for (size_t count = 0; count < Cls[i].funcs.size(); count++) {
				std::string fista;
				if (Cls[i].funcs[count].ista) {
					fista = "addStaticFunction";
				} else {
					fista = "addFunction";
				}
				std::string fname = Cls[i].funcs[count].name;
				if (!Cls[i].funcs[count].customName.empty()) {
					fname = Cls[i].funcs[count].customName;
				}
				Output.append("." + fista + "(\"" + fname + "\", &" + Cls[i].name + "::" + Cls[i].funcs[count].name + ")\\\n");
			}

			// operators
			for (size_t count = 0; count < Cls[i].opes.size(); count++) {
				if (Cls[i].opes[count].type == OT_ADD) {
					Output.append(".addFunction(\"__add\", &" + Cls[i].name + "::" + Cls[i].opes[count].fun.name + ")\\\n");
					continue;
				}

				if (Cls[i].opes[count].type == OT_SUB) {
					Output.append(".addFunction(\"__sub\", &" + Cls[i].name + "::" + Cls[i].opes[count].fun.name + ")\\\n");
					continue;
				}

				if (Cls[i].opes[count].type == OT_MUL) {
					Output.append(".addFunction(\"__mul\", &" + Cls[i].name + "::" + Cls[i].opes[count].fun.name + ")\\\n");
					continue;
				}

				if (Cls[i].opes[count].type == OT_DIV) {
					Output.append(".addFunction(\"__div\", &" + Cls[i].name + "::" + Cls[i].opes[count].fun.name + ")\\\n");
					continue;
				}

				if (Cls[i].opes[count].type == OT_EQ) {
					Output.append(".addFunction(\"__eq\", &" + Cls[i].name + "::" + Cls[i].opes[count].fun.name + ")\\\n");
					continue;
				}
			}

			// variables (POD's only)
			std::vector<std::pair<std::string, std::string>> tpar;
			if (isPOD) {
				for (size_t count = 0; count < Cls[i].vars.size(); count++) {
					splitParam(Cls[i].vars[count].tokparam, tpar);
					if (!tpar.empty()) {
						if (tpar[0].first == "UNBIND") {
							continue;
						}
					}
					std::string fista;
					if (Cls[i].vars[count].isStatic) {
						fista = "addStaticVariableRef";
					} else {
						fista = "addVariableRef";
					}
					Output.append("." + fista + "(\"" + Cls[i].vars[count].name + "\", &" + Cls[i].name + "::" 
								  + Cls[i].vars[count].name + ")\\\n");
				}
			}

			// end of lua binding
			Output.append(".endClass().endModule();}\\\n");
		}

		// end of rgisterMeta
		if (isPOD || isComponent || isEntity) {
			Output.append("}\\\n");
		} else {
			Output.append("}\n");
		}

		// serial and prefab definition
		if (isPOD || isComponent || isEntity) {
			// serialize
			if (isComponent && isAbstract) {
				Output.append("void " + Cls[i].name + "::baseSerial(KBaseSerial &Serializer, bool Base) const {\\\n");
			} else {
				Output.append("void " + Cls[i].name + "::serial(KBaseSerial &Serializer, bool Base) const {\\\n");
			}

			// KComponent (base)
			if (isComponent && !isAbstract) {
				Output.append("if (Base){\\\n");
				Output.append("KComponent *kc = (KComponent *)(this); kc->baseSerial(Serializer);}\\\n");
			}

			for (size_t vcont = 0; vcont < Cls[i].vars.size(); ++vcont) {
				if (Cls[i].vars[vcont].isRes) { // resource pointers
					Output.append("if (" + Cls[i].vars[vcont].name + "){ \\\n"
								  "Serializer << " + Cls[i].vars[vcont].name + "->getName();\\\n"
								  "}else{ Serializer << std::string();}\\\n" );
				} else {
					Output.append("Serializer << " + Cls[i].vars[vcont].name + ";\\\n");
				}
			}
			Output.append("}\\\n");

			// deserialize
			if (isComponent && isAbstract) {
				Output.append("void " + Cls[i].name + "::baseDeserial(KBaseSerial &Serializer, bool Base) {\\\n");
			} else {
				Output.append("void " + Cls[i].name + "::deserial(KBaseSerial &Serializer, bool Base) {\\\n");
			}

			// KComponent (base)
			if (isComponent && !isAbstract) {
				Output.append("if (Base){\\\n");
				Output.append("KComponent *kc = (KComponent *)(this); kc->baseDeserial(Serializer);}\\\n");
			}

			for (size_t vcont = 0; vcont < Cls[i].vars.size(); ++vcont) {
				if (Cls[i].vars[vcont].isRes) { // resource pointers
					Output.append("Serializer >> " + Cls[i].vars[vcont].name + "Name;\\\n");
				} else {
					Output.append("Serializer >> " + Cls[i].vars[vcont].name + ";\\\n");
				}
			}

			// end of serial()
			Output.append("}\n");
		} 
	}
}

void defEnum(std::string &Output, std::vector<std::string> &Types, const std::string &EnumName) {
	Output.append("enum class " + EnumName + " : U16{\n");

	auto ecounter = 0;
	for (size_t i = 0; i < Types.size(); i++) {
		Output.append(Types[i] + " = " + std::to_string(ecounter) + ",\n");
		++ecounter;
	}
	Output.append("maxSize = " + std::to_string(ecounter) + "};\n");

	Output.append("namespace Internal{\n"
				  "struct Register" + EnumName + "{\n"
				  "static void registerMeta(KMetaManager *MMan = nullptr, lua_State *Lua = nullptr);};}\n");

	Output.append("KITE_FUNC_EXPORT extern const std::string &get" + EnumName + "Name(" + EnumName + " Type);\n"
				  "KITE_FUNC_EXPORT extern " + EnumName + " get" + EnumName + "ByName(const std::string &Name);\n");
}

void implEnum(std::string &Output, std::vector<std::pair<std::string, std::string>> &Types, const std::string &EnumName) {
	// register component enum types
	Output.append("void Internal::Register" + EnumName + "::registerMeta(KMetaManager *MMan, lua_State *Lua){\n"
				  "static KMetaEnum instance(\"" + EnumName + "\", 0, sizeof(" + EnumName + "));\n"
				  "static bool initeMeta = false;\n"
				  "if (MMan != nullptr) { MMan->setMeta((KMetaBase *)&instance); }\n"
				  "if (!initeMeta) {");
	for (size_t i = 0; i < Types.size(); ++i) {
		Output.append("instance.addMember(KMetaEnumMember(\"" + Types[i].second + "\", "
					  + std::to_string(i) + ", " + std::to_string(i) + "));\n");
	}
	Output.append("initeMeta = true;}\n"
				  "if (Lua != nullptr) {\n"
				  "LuaIntf::LuaBinding(Lua).beginModule(\"kite\")\n"
				  ".beginModule(\"" + EnumName + "\")\n");
	for (size_t i = 0; i < Types.size(); ++i) {
		Output.append(".addConstant(\"" + Types[i].second + "\", "
					  + EnumName + "::" + Types[i].second + ")\n");
	}
	Output.append(".endModule().endModule();}}\n");

	// get type name
	Output.append("const std::string &get" + EnumName + "Name(" + EnumName + " Type){\n"
				  "static const std::string names[(U16)" + EnumName + "::maxSize] = {\n");
	for (auto it = Types.begin(); it != Types.end(); ++it) {
		Output.append("\"" + it->second + "\"");
		if (it != Types.end()) Output.append(" ,\n");
	}
	Output.append("};\n return names[(U16)Type];\n}\n");

	// get type by name
	Output.append(EnumName + " get"+ EnumName + "ByName(const std::string &Name){\n");
	for (auto it = Types.begin(); it != Types.end(); ++it) {
		Output.append("if (Name == \"" + it->second + "\") return " + EnumName + "::" + it->second + ";\n");
	}

	Output.append("return " + EnumName + "::maxSize;\n"
				  "}\n");
}

void createEnumTypesHead(std::string &Output, const std::vector<MClass> &Cls) {
	Output.clear();
	// component types (enum)
	Output.append("#ifndef KTYPES_H\n"
				  "#define KTYPES_H\n\n"
				  "// ----[auto generated types]----\n"
				  "#include <string>\n"
				  "#include \"Kite/core/kcoredef.h\"\n"
				  "#include \"Kite/meta/kmetadef.h\"\n\n"
				  "KM_IGNORED\n"
				  "KMETA\n"
				  "namespace Kite{\n");

	
	std::vector<std::string> comlist; // components
	std::vector<std::string> reslist; // resources
	std::vector<std::string> istreamlist;
	std::vector<std::string> ostreamlist;
	for (size_t i = 0; i < Cls.size(); i++) {
		if (Cls[i].type & CT_COMPONENT && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					comlist.push_back(Cls[i].infos[count].info);
				}
			}
			continue;
		}

		if (Cls[i].type & CT_RESOURCE && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					reslist.push_back(Cls[i].infos[count].info);
				}
			}
			continue;
		}

		if (Cls[i].type & CT_ISTREAM && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					istreamlist.push_back(Cls[i].infos[count].info );
				}
			}
			continue;
		}

		if (Cls[i].type & CT_OSTREAM && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					ostreamlist.push_back(Cls[i].infos[count].info );
				}
			}
			continue;
		}
	}

	if (!comlist.empty()) {
		defEnum(Output, comlist, COM_ENUM_NAME);
	}
	if (!reslist.empty()) {
		defEnum(Output, reslist, RES_ENUM_NAME);
	}
	if (!istreamlist.empty()) {
		defEnum(Output, istreamlist, ISTREAM_ENUM_NAME);
	}
	if (!ostreamlist.empty()) {
		defEnum(Output, ostreamlist, OSTREAM_ENUM_NAME);
	}

	Output.append("}\n#endif // KMETATYPES_H");

}

void createEnumTypesSource(std::string &Output, const std::vector<MClass> &Cls) {
	Output.clear();
	Output.append("\n// ----[auto generated source file]----\n"
				  "#include \"Kite/meta/kmetaenum.h\"\n"
				  "#include \"Kite/meta/kmetamanager.h\"\n"
				  "#include <luaintf/LuaIntf.h>\n"
				  "#include \"KiteMeta/ktypes.khgen.h\"\n"
				  "namespace Kite {\n");

	// create component list
	std::vector<std::pair<std::string, std::string>> comlist;
	std::vector<std::pair<std::string, std::string>> reslist;
	std::vector<std::pair<std::string, std::string>> istreamlist;
	std::vector<std::pair<std::string, std::string>> ostreamlist;
	for (size_t i = 0; i < Cls.size(); i++) {
		if (Cls[i].type & CT_COMPONENT && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					comlist.push_back({ Cls[i].name , Cls[i].infos[count].info });
				}
			}
			continue;
		}

		if (Cls[i].type & CT_RESOURCE && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					reslist.push_back({ Cls[i].name ,Cls[i].infos[count].info });
				}
			}
			continue;
		}

		if (Cls[i].type & CT_ISTREAM && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					istreamlist.push_back({ Cls[i].name ,Cls[i].infos[count].info });
				}
			}
			continue;
		}

		if (Cls[i].type & CT_OSTREAM && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					ostreamlist.push_back({ Cls[i].name ,Cls[i].infos[count].info });
				}
			}
			continue;
		}
	}

	if (!comlist.empty()) {
		implEnum(Output, comlist, COM_ENUM_NAME);
	}

	if (!reslist.empty()) {
		implEnum(Output, reslist, RES_ENUM_NAME);
	}

	if (!istreamlist.empty()) {
		implEnum(Output, istreamlist, ISTREAM_ENUM_NAME);
	}

	if (!ostreamlist.empty()) {
		implEnum(Output, ostreamlist, OSTREAM_ENUM_NAME);
	}

	Output.append("}\n");
}

void createHead(std::string &Output) {
	Output.clear();

	Output.append("\n// ----[auto generated header file]----\n");
	Output.append("#ifndef KMETA_H\n"
				  "#define KMETA_H\n\n"
				  "#include \"Kite/core/kcoredef.h\"\n"
				  "#include \"Kite/ecs/ksystem.h\"\n"
				  "#include \"Kite/meta/kmetadef.h\"\n\n"
				  "#include <vector>\n"
				  "#include <memory>\n"
				  "KMETA\n"
				  "namespace Kite{\n"
				  "class KMetaManager;\n"
				  "class KScene;\n"
				  "class KResourceManager;\n"
				  "KITE_FUNC_EXPORT extern void registerKiteMeta(KMetaManager *MMan = nullptr, lua_State *Lua = nullptr);\n"
				  "KITE_FUNC_EXPORT extern void registerCTypes(KScene *Scene);\n"
				  "KITE_FUNC_EXPORT extern void registerRTypes(KResourceManager *RMan);\n"
				  "KITE_FUNC_EXPORT extern void createSystems(std::vector<std::unique_ptr<KSystem>> &Systems);\n");
	Output.append("}\n#endif // KITEMETA_H");
}

void createSource(const std::vector<std::string> &Files, const std::vector<MClass> &Cls,
				  const std::vector<MEnum> &Ens, std::string &Output) {


	Output.clear();
	Output.append("\n// ----[auto generated source file]----\n");

	Output.append("#include \"Kite/meta/kmetaenum.h\"\n"
				  "#include \"Kite/meta/kmetapod.h\"\n"
				  "#include \"KiteMeta/kmeta.khgen.h\"\n"
				  "#include \"KiteMeta/ktypes.khgen.h\"\n");

	// add headers
	for (size_t i = 0; i < Files.size(); i++) {
		Output.append("#include \"" + Files[i] + "\"\n");
	}

	Output.append("namespace Kite{\n"
				  "void registerKiteMeta(KMetaManager *MMan, lua_State *Lua){\n"
				  "Internal::registerMetaPOD(MMan);\n");

	// register classes
	for (size_t i = 0; i < Cls.size(); i++) {
		if (!Cls[i].templ.empty() && !Cls[i].templType.empty()) {
			for (size_t count = 0; count < Cls[i].templ.size(); ++count) {
				Output.append(Cls[i].name + "<" + Cls[i].templ[count].types + ">::registerMeta(\"" + Cls[i].templ[count].name + "\", MMan, Lua);\n");
			}
		} else {
			Output.append(Cls[i].name + "::registerMeta(MMan, Lua);\n");
		}
	}

	// register enums
	for (size_t i = 0; i < Ens.size(); i++) {
		Output.append("Internal::Register" + Ens[i].name + "::registerMeta(MMan, Lua);\n");
	}
	Output.append("Internal::Register" + COM_ENUM_NAME + "::registerMeta(MMan, Lua);\n");
	Output.append("Internal::Register" + RES_ENUM_NAME + "::registerMeta(MMan, Lua);\n");
	Output.append("Internal::Register" + ISTREAM_ENUM_NAME + "::registerMeta(MMan, Lua);\n");
	Output.append("Internal::Register" + OSTREAM_ENUM_NAME + "::registerMeta(MMan, Lua);\n");

	// end of meta
	Output.append("}\n");

	// res/components list
	std::vector<std::pair<std::string, std::string>> comList; // <name, cname>
	std::vector<std::pair<std::string, std::string>> resList; // <name, cname>
	std::vector<std::pair<std::string, std::string>> istreamList; // <name, cname>
	for (size_t i = 0; i < Cls.size(); i++) {

		if (Cls[i].type & CT_COMPONENT && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					comList.push_back({ Cls[i].name , Cls[i].infos[count].info });
					break;
				}
			}
		}

		if (Cls[i].type & CT_RESOURCE && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					resList.push_back({ Cls[i].name , Cls[i].infos[count].info });
					break;
				}
			}
		}

		if (Cls[i].type & CT_ISTREAM && !(Cls[i].type & CT_ABSTRACT)) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "KI_NAME") {
					istreamList.push_back({ Cls[i].name , Cls[i].infos[count].info });
					break;
				}
			}
		}
	}

	// register component types
	Output.append("void registerCTypes(KScene *Scene){\n");

	for (auto it = comList.begin(); it != comList.end(); ++it){
		Output.append("Scene->registerComponent<" + it->first + ">(" + COM_ENUM_NAME + "::" + it->second + ");\n");
	}

	// end of ctypes
	Output.append("}\n");

	// register resource types
	Output.append("void registerRTypes(KResourceManager *RMan){\n");

	// resource factory
	for (auto it = resList.begin(); it != resList.end(); ++it){
		Output.append("RMan->registerResource(" + RES_ENUM_NAME + "::" + it->second + ", " + it->first + "::factory);\n");
	}

	// stream factory
	for (auto it = istreamList.begin(); it != istreamList.end(); ++it) {
		Output.append("RMan->registerIStream(" + ISTREAM_ENUM_NAME + "::" + it->second + ", " + it->first + "::factory);\n");
	}

	// end of rtypes
	Output.append("}\n");

	// systems
	Output.append("void createSystems(std::vector<std::unique_ptr<KSystem>> &Systems){\n");
	Output.append("Systems.clear();\n");
	for (size_t i = 0; i < Cls.size(); i++) {
		if (Cls[i].type & CT_SYSTEM && !(Cls[i].type & CT_ABSTRACT)) {
			Output.append("Systems.push_back(std::unique_ptr<KSystem>(new " + Cls[i].name + "));\n");
		}
	}
	Output.append("}\n");

	// end of namespace
	Output.append("}\n");

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

void createStaticName(std::vector<MClass> Cls, std::vector<MEnum> Enm, std::string &Output) {
	Output.clear();

	// headers
	Output.append("\n// ----[auto generated header file]----\n");
	Output.append("#include \"Kite/meta/kmetadef.h\"\n");
	Output.append("#include \"Kite/core/kcoredef.h\"\n\n");

	Output.append("namespace Kite{\n");
	Output.append("template<typename T>\n");
	Output.append("struct getMetaName;\n\n");

	Output.append("#define KMETA_REG_NAME(TYPE)\\\n");
	Output.append("template<>\\\n");
	Output.append("struct getMetaName<TYPE> {\\\n");
	Output.append("static const char *name;};\\\n");
	Output.append("const char *getMetaName<TYPE>::name = #TYPE\n\n");

	// pod types
	Output.append("KMETA_REG_NAME(I8);\n");
	Output.append("KMETA_REG_NAME(I16);\n");
	Output.append("KMETA_REG_NAME(I32);\n");
	Output.append("KMETA_REG_NAME(I64);\n");
	Output.append("KMETA_REG_NAME(U8);\n");
	Output.append("KMETA_REG_NAME(U16);\n");
	Output.append("KMETA_REG_NAME(U32);\n");
	Output.append("KMETA_REG_NAME(U64);\n");
	Output.append("KMETA_REG_NAME(F32);\n");
	Output.append("KMETA_REG_NAME(F64);\n");
	Output.append("KMETA_REG_NAME(bool);\n");

	for (size_t i = 0; i < Cls.size(); i++) {
		Output.append("KMETA_REG_NAME(" + Cls[i].name + ");\n");
	}

	for (size_t i = 0; i < Cls.size(); i++) {
		Output.append("KMETA_REG_NAME(" + Enm[i].name + ");\n");
	}

	Output.append("}\n");
}

int main(int argc, char* argv[]) {

	if (argc < 4) {
		printf("missing input parameter\nusage: khparser.exe -i [includeDirectory] -o [outputDirectory]");
		return 0;
	}

	std::string baseadr;
	std::string outadr;
	for (int i = 0; i < argc; i++) {
		if (std::string(argv[i]) == "-i" && i < (argc - 1)) {
			baseadr = argv[i + 1];
		} else if (std::string(argv[i]) == "-o" && i < (argc - 1)) {
			outadr = argv[i + 1];
		}
	}

	std::vector<std::string> hadrs;
	std::vector<MClass> cls;
	std::vector<MEnum> ens;
	std::stack<std::string> dirstack;
	tinydir_dir dir;
	tinydir_open(&dir, baseadr.c_str());

	unsigned int fcounter = 0;
	unsigned int dcounter = 0;
	unsigned int gcounter = 0;
	unsigned int skipped = 0;
	while (true) {
		if (dir.has_next) {
			tinydir_file file;
			tinydir_readfile(&dir, &file);

			if (file.is_dir) {
				if (std::string(file.name) != "." && std::string(file.name) != "..") {
					dirstack.push(file.path);
					++dcounter;
				}

			} else if ((std::string(file.name).find(".khgen.") != std::string::npos) ||
					   (std::string(file.name).find(".cpp") != std::string::npos)||
					   (std::string(file.path).find("removed") != std::string::npos)) {
				printf("skipped file: %s\n", file.path);
				++skipped;

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


	std::string header;
	std::string source;

	// create enums (types)
	if (!cls.empty()) {
		createEnumTypesHead(header, cls);
		createEnumTypesSource(source, cls);
	}

	std::ofstream out(outadr + "/ktypes.khgen.h");
	out << header;
	out.close();
	header.clear();

	out.open(outadr + "/ktypes.khgen.cpp");
	out << source;
	out.close();
	source.clear();

	// create register function
	if (!cls.empty() || !ens.empty()) {
		createHead(header);
		createSource(hadrs, cls, ens, source);
	}

	out.open(outadr + "/kmeta.khgen.h");
	out << header;
	out.close();

	out.open(outadr + "/kmeta.khgen.cpp");
	out << source;
	out.close();

	printf("\n---------[ Kite header parser log ]---------\n%u\tfile(s) parsed\n%u\tdirectory searched\n%u\tfile skipped\n%u\tfile generated\n",
		   fcounter, dcounter, skipped, gcounter);

	return 0;
}