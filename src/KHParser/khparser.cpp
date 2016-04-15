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
#include <fstream>
#include <streambuf>
#include <stack>
#include <utility>
#include <cctype>
#include <tinydir/tinydir.h>

std::unordered_map<std::string, std::string> strmap;

#define CLS_ATTRIB 6
#define FUNC_ATTRIB 6
#define VAR_ATTRIB 6

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
	CT_OTHER = 0,
	CT_COMP,
	CT_SYS,
	CT_POD,
	CT_RES
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
	OT_DIV
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
	std::string tokparam;
	bool ista;
	bool inl;
	bool ext;
	bool cons;
	bool vir;
	bool explct;

	MFunction() :
		ista(false), inl(false), ext(false),
		cons(false), vir(false), explct(false)
	{}
};

struct MProperty {
	std::string name;
	MFunction get;
	MFunction set;
	std::string comment;
	std::string type;
	std::string min;
	std::string max;
};

struct MOperator {
	MOperatorType type;
	MFunction fun;
};

struct MVariable {
	std::string name;
	std::string tokparam;

	MVariable() {}
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
	MClassType type;
	MExportState exstate;
	MFunction constructure;
	std::string flags;
	std::string templType;
	std::vector<MProperty> props;
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
		if (!temp.substr(0, pos).empty()) {
			OutList.push_back(temp.substr(0, pos));
		}
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
			  , std::vector<MProperty> &Output) {
	std::vector<std::string> param;
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

		// extract string name from string map
		auto pname = strmap.find(param[0]);
		if (pname == strmap.end()) {
			printf("error: could not extract prperty name from string map ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}

		// check for comment
		std::string comment;
		if (param.size() >= 3) {
			auto pcomm = strmap.find(param[2]);
			if (pcomm == strmap.end()) {
				printf("warning: could not extract prperty comment from string map ==> function name: %s\n", AllGet[i].name.c_str());
			} else {
				comment = pcomm->second;
			}
		}

		// check for min/max
		std::string minv("0");
		std::string maxv("0");
		if (param.size() >= 4) {
			minv = param[3];
			if (param.size() >= 5) {
				maxv = param[4];
			}
		}

		// register property to map
		auto prop = map.find(pname->second);
		if (prop != map.end()) {
			printf("error: property overload not supported ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}

		// register getter/name/type/comment
		map.insert({ pname->second, MProperty() });
		map[pname->second].name = pname->second;
		map[pname->second].comment = comment;
		map[pname->second].get = AllGet[i];
		map[pname->second].type = param[1];
		map[pname->second].min = minv;
		map[pname->second].max = maxv;
	}

	// first register all setters
	for (size_t i = 0; i < AllSet.size(); i++) {

		// exteract token parameter
		splitParam(AllSet[i].tokparam, param);

		if (param.empty()) {
printf("error: missing property info ==> function name: %s\n", AllGet[i].name.c_str());
return false;
		} else if (param.size() > 1) {
			printf("warning: extera property info (ignored) ==> function name: %s\n", AllGet[i].name.c_str());
		}

		// extract string name from string map
		auto pname = strmap.find(param[0]);
		if (pname == strmap.end()) {
			printf("error: could not extract prperty name from string map ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}

		// register property to map
		auto prop = map.find(pname->second);
		if (prop == map.end()) {
			printf("error: setter-only property not allowed ==> function name: %s\n", AllGet[i].name.c_str());
			return false;
		}

		// register getter/name/type/comment
		map[pname->second].set = AllSet[i];
	}

	// fill output vector
	for (auto it = map.begin(); it != map.end(); ++it) {
		Output.push_back(it->second);
	}

	return true;
}

// parse all property in the content
bool procAllProp(const std::string &Content, std::vector<MProperty> &Props) {
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

		if (procFunc(Content, tfunc, pos)) {
			allGet.push_back(tfunc);

		} else {
			ret = false;
		}
	}

	pos = 0;
	while ((pos = findToken(Content, pos, "KM_PRO_SET")) != std::string::npos) {
		MFunction tfunc;

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

bool procInfo(const std::string &Content, MInfo &Info, unsigned int Pos) {
	std::string output;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// its a generic variable processor so we dont check number of token parameters here,
	// and there is no warning for empty parameter list
	Pos = getNextBody(Content, Pos, output);
	
	std::vector<std::string> param;
	splitParam(output, param);

	if (param.size() >= 2) {
		Info.key = strmap[param[0]];
		Info.info = strmap[param[1]];
		return true;
	} else {
		printf("missing info parameter.");
	}

	return false;
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
		MInfo tinfo;
		if (procInfo(Content, tinfo, pos)) {
			Info.push_back(tinfo);

		} else {
			printf("info token number: %u \n", count);
			ret = false;
		}
		++count;
	}

	return ret;
}

bool procOper(std::vector<MOperator> &Oprs, const std::vector<MFunction> &Funs) {
	std::vector<std::string> param;

	for (size_t i = 0; i < Funs.size(); ++i) {
		splitParam(Funs[i].tokparam, param);
		if (param.empty()) {
			printf("missing operator type. oname: %s\n", Funs[i].name.c_str());
			return false;
		} else if (param.size() > 1) {
			printf("extera operator type ignored. oname: %s\n", Funs[i].name.c_str());
		}

		if (param[0] == "KO_ADD") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_ADD;
			Oprs.push_back(opr);
			continue;
		}

		if (param[0] == "KO_SUB") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_SUB;
			Oprs.push_back(opr);
			continue;
		}

		if (param[0] == "KO_MUL") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_MUL;
			Oprs.push_back(opr);
			continue;
		}

		if (param[0] == "KO_DIV") {
			MOperator opr;
			opr.fun = Funs[i];
			opr.type = OT_DIV;
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

	std::vector<std::string> param;
	splitParam(output, param);

	if (param.size() < 2) {
		printf("error: missing template value.\n");
		return false;
	}

	Temps.name = strmap[param[0]];

	for (size_t i = 1; i < param.size(); ++i) {
		Temps.types.append(param[i]);
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

		std::vector<std::string> param;
		splitParam(output, param);

		if (param.empty()) {
			printf("error: empty template types list.\n");
			return false;
		}

		for (size_t i = 0; i < param.size(); ++i) {
			Cls.templType.append(param[i]);
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
	std::vector<std::string> params;

	// checking token parameter(s)
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing ().\n");
		return false;
	}

	// extract token parameter(s)
	pos = getNextBody(Content, Pos, output);
	Cls.tokparam = output;

	// is component
	std::vector<std::string> tags;
	splitParam(output, tags);
	Cls.type = CT_OTHER;
	if (std::find(tags.begin(), tags.end(), "COMPONENT") != tags.end()) {
		if (std::find(tags.begin(), tags.end(), "ABSTRACT") == tags.end()) {
			Cls.type = CT_COMP;
		}
	}

	// is system
	if (std::find(tags.begin(), tags.end(), "SYSTEM") != tags.end()) {
		if (std::find(tags.begin(), tags.end(), "ABSTRACT") == tags.end()) {
			Cls.type = CT_SYS;
		}
	}

	// is resource
	if (std::find(tags.begin(), tags.end(), "RESOURCE") != tags.end()) {
		if (std::find(tags.begin(), tags.end(), "ABSTRACT") == tags.end()) {
			Cls.type = CT_RES;
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
	std::vector<std::string> param;
	std::vector<std::string> defval;
	Memers.clear();
	splitParam(Content, param);
	if (param.empty()) {
		printf("error: empty enum.\n");
		return false;
	}

	// member value
	for (size_t i = 0; i < param.size(); i++) {
		splitBy(param[i], "=", defval);

		// without value
		if (defval.size() == 1) {
			MEnumMember emem;
			emem.name = param[i];
			Memers.push_back(emem);
			continue;

		// with value
		} else if (defval.size() == 2) {
			removeAfter(param[i], "=");
			MEnumMember emem;
			emem.name = param[i];
			emem.value = defval[1];
			Memers.push_back(emem);

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
	std::vector<std::string> ctags;
	splitParam(cParam, ctags);

	// we only support POD's class for template
	if (std::find(ctags.begin(), ctags.end(), "POD") == ctags.end()) {
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
					"static bool inite = true;\\\n"
					"static KMetaClass instance(Name," + cParam + ", sizeof(" + Cls.name + "<" + Cls.templType + ">));\\\n"
					"if (inite) {\\\n"
					"if (MMan != nullptr){\\\n"
					"MMan->setMeta((KMetaBase *)&instance);\\\n");

	// informations
	for (size_t count = 0; count < Cls.infos.size(); ++count) {
		Output.append("instance.addInfo({\"" + Cls.infos[count].key + "\", \"" + Cls.infos[count].info + "\"});\\\n");
	}

	// properties (meta)
	for (size_t count = 0; count < Cls.props.size(); count++) {
		std::string prpType("KMP_BOTH");
		if (Cls.props[count].set.name.empty()) {
			prpType = "KMP_GETTER";
		} else {
			prpType = "KMP_BOTH";
		}
		Output.append("instance.addProperty(KMetaProperty(\"" + Cls.props[count].name + "\", \""
						+ Cls.props[count].type + "\", \"" + Cls.props[count].comment + "\", " + prpType + "));\\\n");
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
					"LuaIntf::LuaBinding(Lua).beginModule(\"Kite\").beginClass<"
					+ Cls.name + "<" + Cls.templType + ">>(Name.c_str())\\\n");

	// constructure
	std::string param;
	if (!Cls.constructure.name.empty()) {
		std::vector<std::string> cplist;
		splitParam(Cls.constructure.tokparam, cplist);
		if (!cplist.empty()) {
			for (size_t count = 0; count < cplist.size(); count++) {
				param.append("LuaIntf::_opt<" + cplist[count] + ">");
				if ((count + 1) != cplist.size()) {
					param.append(", ");
				}
			}
		}
		Output.append(".addConstructor(LUA_ARGS(" + param + "))\\\n");
	}

	// properties
	for (size_t count = 0; count < Cls.props.size(); count++) {
		if (!Cls.props[count].get.name.empty() && !Cls.props[count].set.name.empty()) {
			Output.append(".addProperty(\"" + Cls.props[count].name + "\", &" + Cls.name + "<" + Cls.templType + ">::"
							+ Cls.props[count].get.name + ", &" + Cls.name + "<" + Cls.templType + ">::" + Cls.props[count].set.name + ")\\\n");
		} else if (Cls.props[count].set.name.empty()) {
			Output.append(".addProperty(\"" + Cls.props[count].name + "\", &" + Cls.name + "<" + Cls.templType + ">::"
							+ Cls.props[count].get.name + ")\\\n");
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
	}

	// end of lua binding
	Output.append(".endClass().endModule();}\\\n");
	Output.append("} inite = false;}\n");
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
					  "static bool inite = true;\\\n"
					  "static KMetaEnum instance(\"" + Enms[i].name + "\", 0, sizeof(" + Enms[i].name + "));\\\n"
					  "if (inite) {\\\n"
					  "if (MMan != nullptr){\\\n"
					  "MMan->setMeta((KMetaBase *)&instance);\\\n");

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
		Output.append("}\\\n");

		// lua binding
		Output.append("if (Lua != nullptr){\\\n"
					  "LuaIntf::LuaBinding(Lua).beginModule(\"Kite\")\\\n"
					  ".beginModule(\"" + Enms[i].name + "\")\\\n");
		for (size_t count = 0; count < Enms[i].members.size(); ++count) {
			Output.append(".addConstant(\"" + Enms[i].members[count].name + "\", " + Enms[i].name + "::" 
						  + Enms[i].members[count].name + ")\\\n");
		}

		// end of lua binding
		Output.append(".endModule().endModule();}\\\n");

		// end of definition
		Output.append("} inite = false;}\n");
	}

	// class
	for (size_t i = 0; i < Cls.size(); i++) {
		std::string upname = Cls[i].name;
		std::string cParam = Cls[i].tokparam;
		replaceTok(cParam, '|', ',');
		std::vector<std::string> ctags;
		splitParam(cParam, ctags);

		// is template class (templated class)
		if (!Cls[i].templ.empty()) {
			createTemplMacro(Cls[i], Output);
			continue;
		}

		// is serializable
		bool isPOD = false;
		if (std::find(ctags.begin(), ctags.end(), "POD") != ctags.end()) {
			isPOD = true;
		}

		// is scriptable
		bool isResource = false;
		if (std::find(ctags.begin(), ctags.end(), "RESOURCE") != ctags.end()) {
			isResource = true;
		}

		// is editable
		bool isComponent = false;
		if (std::find(ctags.begin(), ctags.end(), "COMPONENT") != ctags.end()) {
			isComponent = true;
		}

		// is entity
		bool isEntity = false;
		if (std::find(ctags.begin(), ctags.end(), "ENTITY") != ctags.end()) {
			isEntity = true;
		}

		// is system
		bool isSystem = false;
		if (std::find(ctags.begin(), ctags.end(), "SYSTEM") != ctags.end()) {
			isSystem = true;
		}

		// is abstract
		bool isAbstract = false;
		if (std::find(ctags.begin(), ctags.end(), "ABSTRACT") != ctags.end()) {
			isAbstract = true;
		}

		// is continer
		bool isContiner = false;
		if (std::find(ctags.begin(), ctags.end(), "CONTINER") != ctags.end()) {
			isContiner = true;
		}

		// is scriptable
		bool isScriptable = false;
		if (std::find(ctags.begin(), ctags.end(), "SCRIPTABLE") != ctags.end()) {
			isScriptable = true;
		}

		// class without any flag will ignored
		if (!isPOD && !isEntity && !isResource && !isComponent && !isSystem && !isScriptable && !isContiner) {
			printf("message: class without any supported flags. %s ignored. \n", Cls[i].name.c_str());
			continue;
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

		// factory methode (only resources have factory methode)
		if (isResource && !isAbstract) {
			Output.append(exstate + "static KResource *factory(const std::string &Name);\\\n");
		}

		// property setter/getter (only components have properties)
		if (isComponent && !isAbstract) {
			Output.append(exstate + "bool setProperty(const std::string &Name, KAny &Value) override;\\\n" +
						  exstate + "KAny getProperty(const std::string &Name) override;\\\n");
		}

		// serializable
		if (isPOD || isComponent || isEntity) {
			if (!isAbstract) {
				Output.append("friend KBaseSerial &operator<<(KBaseSerial &Out, const " + Cls[i].name + " &Value) {\\\n"
							  "Value.serial(Out); return Out;}\\\n"
							  "friend KBaseSerial &operator>>(KBaseSerial &In, " + Cls[i].name + " &Value) {\\\n"
							  "Value.deserial(In); return In;}\\\n");
			}
			Output.append(exstate + "void serial(KBaseSerial &Serializer) const;\\\n");
			Output.append(exstate + "void deserial(KBaseSerial &Serializer);\\\n");
		}

		Output.append(exstate + "static void registerMeta(KMetaManager *MMan = nullptr, lua_State *Lua = nullptr);\\\n"
					  "const std::string &getClassName() const { static std::string name(\"" + Cls[i].name + "\");\\\n"
					  "return name;}\n");

		// defention
		Output.append("\n// ----[auto generated: " + Cls[i].name + " source macro]----\n");
		Output.append("#define KMETA_" + upname + "_SOURCE()\\\n");

		// factory defention
		if (isResource && !isAbstract) {
			Output.append("KResource *" + Cls[i].name + "::factory(const std::string &Name){\\\n"
						  "return new " + Cls[i].name + "(Name);}\\\n");
		}

		// register properties
		if (isComponent && !isAbstract) {

			// property setter/getter (only components have properties)
			Output.append("bool " + Cls[i].name + "::setProperty(const std::string &Name, KAny &Value){\\\n");
			for (size_t count = 0; count < Cls[i].props.size(); ++count) {
				if (!Cls[i].props[count].set.name.empty()) {
					Output.append("if (Name == \"" + Cls[i].props[count].name + "\"){\\\n" +
								  "if (Value.is<" + Cls[i].props[count].type + ">()){\\\n" +
								  Cls[i].props[count].set.name + "(Value.as<" + Cls[i].props[count].type + ">());\\\n"
								  "return true;\\\n"
								  "}else{ KD_FPRINT(\"incorrect property type. pname: %s\", Name.c_str()); return false;}}\\\n");
				}
			}
			Output.append("return false;}\\\n");

			// get
			Output.append("KAny " + Cls[i].name + "::getProperty(const std::string &Name){\\\n");
			for (size_t count = 0; count < Cls[i].props.size(); ++count) {
				if (!Cls[i].props[count].get.name.empty()) {
					Output.append("if (Name == \"" + Cls[i].props[count].name + "\"){\\\n"
								 "return " + Cls[i].props[count].get.name + "();}\\\n");
				}
			}
			Output.append("return KAny(nullptr);}\\\n");
		}

		// meta registration
		replaceTok(cParam, ',', '|');
		Output.append("void " + Cls[i].name + "::registerMeta(KMetaManager *MMan, lua_State *Lua){\\\n"
					  "static bool inite = true;\\\n"
					  "static KMetaClass instance(\"" + Cls[i].name + "\"," + cParam + ", sizeof(" + Cls[i].name + "));\\\n"
					  "if (inite) {\\\n"
					  "if (MMan != nullptr){\\\n"
					  "MMan->setMeta((KMetaBase *)&instance);\\\n");

		// informations
		for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
			Output.append("instance.addInfo({\"" + Cls[i].infos[count].key + "\", \"" + Cls[i].infos[count].info + "\"});\\\n");
		}

		// properties (meta)
		for (size_t count = 0; count < Cls[i].props.size(); count++) {
			std::string prpType("KMP_BOTH");
			if (Cls[i].props[count].set.name.empty()) {
				prpType = "KMP_GETTER";
			} else {
				prpType = "KMP_BOTH";
			}
			Output.append("instance.addProperty(KMetaProperty(\"" + Cls[i].props[count].name + "\", \""
						  + Cls[i].props[count].type + "\", \"" + Cls[i].props[count].comment + "\", "
						  + prpType + ", " + Cls[i].props[count].min + ", " + Cls[i].props[count].max + "));\\\n");
		}

		// functions
		for (size_t count = 0; count < Cls[i].funcs.size(); count++) {
			std::string ista = "false";
			if (Cls[i].funcs[count].ista) {
				ista = "true";
			} 

			Output.append("instance.addFunction(KMetaFunction(\"" + Cls[i].funcs[count].name + "\", " + ista + " ));\\\n");
		}
		Output.append("}\\\n");

		// lua binding 
		if (isComponent || isScriptable || isEntity || isPOD || isSystem) {
			Output.append("if (Lua != nullptr) { \\\n"
						  "LuaIntf::LuaBinding(Lua).beginModule(\"Kite\").beginClass<"
						  + Cls[i].name + ">(\"" + Cls[i].name + "\")\\\n");

			// constructure
			std::string param;
			if (!Cls[i].constructure.name.empty() && !isAbstract) {
				std::vector<std::string> cplist;
				splitParam(Cls[i].constructure.tokparam, cplist);
				if (!cplist.empty()) {
					for (size_t count = 0; count < cplist.size(); count++) {
						param.append("LuaIntf::_opt<" + cplist[count] + ">");
						if ((count + 1) != cplist.size()) {
							param.append(", ");
						}
					}
				}
				Output.append(".addConstructor(LUA_ARGS(" + param + "))\\\n");
			}

			// properties
			for (size_t count = 0; count < Cls[i].props.size(); count++) {
				if (!Cls[i].props[count].get.name.empty() && !Cls[i].props[count].set.name.empty()) {
					Output.append(".addProperty(\"" + Cls[i].props[count].name + "\", &" + Cls[i].name + "::"
								  + Cls[i].props[count].get.name + ", &" + Cls[i].name + "::" + Cls[i].props[count].set.name + ")\\\n");
				} else if (Cls[i].props[count].set.name.empty()) {
					Output.append(".addProperty(\"" + Cls[i].props[count].name + "\", &" + Cls[i].name + "::"
								  + Cls[i].props[count].get.name + ")\\\n");
				}
			}

			// functions 
			std::string fista;
			for (size_t count = 0; count < Cls[i].funcs.size(); count++) {

				if (Cls[i].funcs[count].ista) {
					fista = "addStaticFunction";
				} else {
					fista = "addFunction";
				}
				Output.append("." + fista + "(\"" + Cls[i].funcs[count].name + "\", &" + Cls[i].name + "::" + Cls[i].funcs[count].name + ")\\\n");
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
			}

			// end of lua binding
			Output.append(".endClass().endModule();}\\\n");
		}

		// register components
		if (isComponent && !isAbstract) {
			//Output.append("if (EMan != nullptr) { \\\n");
		}

		// end of rgisterMeta
		if (isPOD || isComponent || isEntity) {
			Output.append("} inite = false;}\\\n");
		} else {
			Output.append("} inite = false;}\n");
		}

		// serial definition
		if (isPOD || isComponent || isEntity) {
			// serialize
			Output.append("void " + Cls[i].name + "::serial(KBaseSerial &Serializer) const {\\\n");

			// KComponent (base)
			if (isComponent && !isAbstract) {
				Output.append("KComponent *kc = (KComponent *)(this); kc->serial(Serializer);\\\n");
			}

			for (size_t vcont = 0; vcont < Cls[i].vars.size(); ++vcont) {
				Output.append("Serializer << " + Cls[i].vars[vcont].name + ";\\\n");
			}
			Output.append("}\\\n");

			// deserialize
			Output.append("void " + Cls[i].name + "::deserial(KBaseSerial &Serializer) {\\\n");

			// KComponent (base)
			if (isComponent && !isAbstract) {
				Output.append("KComponent *kc = (KComponent *)(this); kc->deserial(Serializer);\\\n");
			}

			for (size_t vcont = 0; vcont < Cls[i].vars.size(); ++vcont) {
				Output.append("Serializer >> " + Cls[i].vars[vcont].name + ";\\\n");
			}

			// end of serial()
			Output.append("}\n");
		} 
	}
}

void createHead(std::string &Output) {
	Output.clear();

	Output.append("\n// ----[auto generated header file]----\n");
	Output.append("#ifndef KITEMETA_H\n"
				  "#define KITEMETA_H\n\n"
				  "#include \"Kite/core/kcoredef.h\"\n"
				  "#include \"Kite/core/ksystem.h\"\n"
				  "#include \"Kite/core/kentitymanager.h\"\n"
				  "#include \"Kite/meta/kmetadef.h\"\n\n"
				  "#include \"Kite/meta/kmetamanager.h\"\n"
				  "#include <vector>\n"
				  "#include <memory>\n"
				  "KM_IGNORED\n"
				  "KMETA\n"
				  "namespace Kite{\n"
				  "KITE_FUNC_EXPORT extern void registerKiteMeta(KMetaManager *MMan = nullptr, lua_State *Lua = nullptr);\n"
				  "KITE_FUNC_EXPORT extern void registerCTypes(KEntityManager &EMan);\n"
				  "KITE_FUNC_EXPORT extern void registerRTypes(KResourceManager &RMan);\n"
				  "KITE_FUNC_EXPORT extern void createSystems(std::vector<std::unique_ptr<KSystem>> &Systems);\n"
				  "}\n"
				  "#endif // KITEMETA_H");
}

void createSource(const std::vector<std::string> &Files, const std::vector<MClass> &Cls,
				  const std::vector<MEnum> &Ens, std::string &Output) {


	Output.clear();
	Output.append("\n// ----[auto generated source file]----\n");

	// POD header file
	Output.append("#include \"Kite/meta/kmetapod.h\"\n");
	Output.append("#include \"KiteMeta/kmeta.khgen.h\"\n");

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

	// end of meta
	Output.append("}\n");

	// register component types
	Output.append("void registerCTypes(KEntityManager &EMan){\n");

	for (size_t i = 0; i < Cls.size(); i++) {
		if (Cls[i].type == CT_COMP) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "CType") {
					Output.append("EMan.registerComponent<" + Cls[i].name + ">(\"" + Cls[i].infos[count].info + "\");\n");
					break;
				}
			}
		}
	}

	// end of ctypes
	Output.append("}\n");

	// register resource types
	Output.append("void registerRTypes(KResourceManager &RMan){\n");

	for (size_t i = 0; i < Cls.size(); i++) {
		if (Cls[i].type == CT_RES) {
			for (size_t count = 0; count < Cls[i].infos.size(); ++count) {
				if (Cls[i].infos[count].key == "RType") {
					Output.append("RMan.registerResource(\"" + Cls[i].infos[count].info + "\", " + Cls[i].name + "::factory);\n");
					break;
				}
			}
		}
	}

	// end of rtypes
	Output.append("}\n");

	// systems
	Output.append("void createSystems(std::vector<std::unique_ptr<KSystem>> &Systems){\n");
	Output.append("Systems.clear();\n");
	for (size_t i = 0; i < Cls.size(); i++) {
		if (Cls[i].type == CT_SYS) {
			Output.append("Systems.push_back(std::unique_ptr<KSystem>(new " + Cls[i].name + "));\n");
		}
	}

	// end of namespace
	Output.append("}}\n");

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

	// create register function
	std::string header;
	std::string source;
	if (!cls.empty() || !ens.empty()) {
		createHead(header);
		createSource(hadrs, cls, ens, source);
	}

	std::ofstream out(outadr + "/kmeta.khgen.h");
	out << header;
	out.close();

	out.open(outadr + "/kmeta.khgen.cpp");
	out << source;
	out.close();

	printf("\n---------[ Kite header parser log ]---------\n%u\tfile(s) parsed\n%u\tdirectory searched\n%u\tfile skipped\n%u\tfile generated\n",
		   fcounter, dcounter, skipped, gcounter);

	return 0;
}