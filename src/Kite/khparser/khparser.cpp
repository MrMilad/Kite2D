#include <stdio.h>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <streambuf>
#include <tinydir/tinydir.h>

std::unordered_map<std::string, std::string> strmap;

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

struct MProperty {
	std::string name;
	std::string get;
	std::string set;
	std::string comment;
	std::string type;
};

struct MFParam {
	std::string name;
	std::string type;
	std::string defval;
};
struct MFunction {
	std::string name;
	std::string ret;
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

struct MClassBase {
	std::string name;
	MClassBaseAccs accs;
};

struct MClass {
	std::string name;
	MClassType type;
	MExportState exstate;
	MFunction constructure;
	std::string flags;
	std::vector<MClassBase> bases;
	std::vector<MProperty> props;
	std::vector<MFunction> funcs;
};

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
			return i + 1;

		} else {
			return 0;
		}
	}
	return 0;
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

// checking previous input (ignore space and new line)
ParsState checkPrev(const std::string &Content, unsigned int Pos) {
	if (Content.empty())
		return PS_END;

	if (Pos >= Content.length())
		return PS_END;

	// ignore space and new line
	for (auto i = Pos; i > 0; i--) {
		if (Content[i] == ' ' || Content[i] == '\n' || Content[i] == '\t') {
			continue;

		} else if (Content[i] == '(' || Content[i] == '[' || Content[i] == '{') {
			return PS_BODY_START;

		} else if (Content[i] == ')' || Content[i] == ']' || Content[i] == '}') {
			return PS_BODY_END;

		} else if (Content[i] == '=' || Content[i] == ',' || Content[i] == '*' ||
				   Content[i] == '-' || Content[i] == '+' || Content[i] == '&' ||
				   Content[i] == ':') {
			return PS_TOKEN;

		} else if (Content[i] == '@') {
			return PS_STRING;

		} else {
			return PS_WORD;
		}
	}

	return PS_END;
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
				   Content[i] == ':') {
			return PS_TOKEN;

		} else if (Content[i] == '@') {
			return PS_STRING;

		} else {
			return PS_WORD;
		}
	}

	return PS_END;
}

// get previous word (token: sapce or new-line "=+-[]{}())
unsigned int getPrevWord(const std::string &Content, unsigned int Pos, std::string &OutTok) {
	OutTok.clear();
	if (Content.empty())
		return 0;

	size_t begin = 0;
	size_t end = 0;
	for (auto i = Pos; i > 0; i--) {
		if (Content[i] == ' ' || Content[i] == '\n' ||
			Content[i] == '(' || Content[i] == ')' ||
			Content[i] == '[' || Content[i] == ']' ||
			Content[i] == '{' || Content[i] == '}' ||
			Content[i] == ',' || Content[i] == '=' ||
			Content[i] == '+' || Content[i] == '-' ||
			Content[i] == '&' || Content[i] == '|' ||
			Content[i] == ':' || Content[i] == '*' ||
			Content[i] == '\t') {
			continue;
		} else {
			begin = i;
			break;
		}
	}

	for (auto i = begin; i > 0; i--) {
		if (Content[i] == ' ' || Content[i] == '\n' ||
			Content[i] == '(' || Content[i] == ')' ||
			Content[i] == '[' || Content[i] == ']' ||
			Content[i] == '{' || Content[i] == '}' ||
			Content[i] == ',' || Content[i] == '=' ||
			Content[i] == '+' || Content[i] == '-' ||
			Content[i] == '&' || Content[i] == '|' ||
			Content[i] == ':' || Content[i] == '*' ||
			Content[i] == '\t') {
			end = i;
			break;
		}
	}

	OutTok = Content.substr(end, ((++begin) - (++end)));
	return --end;
}

// get next word (token: sapce or new-line "=+-[]{}())
unsigned int getNextWord(const std::string &Content, unsigned int Pos, std::string &OutTok) {
	OutTok.clear();
	if (Content.empty())
		return 0;

	size_t begin = 0;
	size_t end = 0;
	for (auto i = Pos; i < Content.length(); i++) {
		if (Content[i] == ' ' || Content[i] == '\n'||
			Content[i] == '(' || Content[i] == ')' ||
			Content[i] == '[' || Content[i] == ']' ||
			Content[i] == '{' || Content[i] == '}' ||
			Content[i] == ',' || Content[i] == '=' ||
			Content[i] == '+' || Content[i] == '-' ||
			Content[i] == '&' || Content[i] == '|' ||
			Content[i] == ':' || Content[i] == '*' ||
			Content[i] == '\t') {
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
			Content[i] == '\t') {
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
			strmap.insert({ key, Content.substr(pos, (len + tsize) - pos) });
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
		if ((ntok = Content.find("*/", tpos)) != std::string::npos)
			Content.erase(pos, (ntok + tsize) - pos);
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

bool procConstructure(std::string &Content, MFunction &Func) {
	size_t pos = 0;
	std::string output;
	std::vector<std::string> params;
	if ((pos = Content.find("KMETA_CONSTRUCTURE", 0)) == std::string::npos) {
		return true;
	}
	pos += strlen("KMETA_CONSTRUCTURE");

	// check constructure overload
	if (Content.find("KMETA_CONSTRUCTURE", pos) != std::string::npos) {
		printf("error: constructure overload not supported.\n");
		return false;
	}

	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: incorrect constructure token usage.\n");
		return false;
	}

	pos = getNextWord(Content, pos, output);
	Func.name = output;

	// parameter list
	if (checkNext(Content, pos) != PS_BODY_START) {
		printf("error: missing constructure parameter list.\n");
		return false;
	}

	getNextBody(Content, pos, output);
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
				printf("error: missing parameter name ==> constructure name: %s\n", Func.name.c_str());
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

	return true;
}

bool procFunc(std::string &Content, MFunction &Funcs, unsigned int Pos) {
	std::string output;
	std::vector<std::string> params;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: incorrect function token structure.\n");
		return false;
	}

	Pos = getNextBody(Content, Pos, output);
	splitParam(output, params);

	if (params.empty()) {
		printf("error: missing function token parameter.\n");
		return false;
	}

	if (params.size() > 1) {
		printf("warning: extra function token parameter (ignored) ==> function name: %s\n", params[0].c_str());
	}

	// function name
	Funcs.name = params[0];

	// check function
	size_t fpos = 0;
	size_t rpos = 0;
	if ((fpos = Content.find(std::string(Funcs.name + "("), 0)) == std::string::npos) {
		printf("error: missing function ==> function name: %s\n", Funcs.name.c_str());
		return false;
	}
	rpos = fpos - 1;
	fpos += Funcs.name.length();

	// check function overload
	if (Content.find(std::string(Funcs.name + "("), fpos) != std::string::npos) {
		printf("error: function overload not supported ==> function name: %s\n", Funcs.name.c_str());
		return false;
	}

	// get return type
	rpos = getPrevWord(Content, rpos, Funcs.ret);

	// check for static/inline/extern - check 5 word before function name 
	for (auto i = 0; i < 5; i++) {
		if (checkPrev(Content, rpos) == PS_WORD) {
			rpos = getPrevWord(Content, rpos, output);
			if (output == "static") {
				Funcs.ista = true;
			}else if (output == "inline") {
				Funcs.inl = true;
			} else if (output == "extern") {
				Funcs.ext = true;
			}
		} else {
			break;
		}
	}

	// get function parameter list
	if (checkNext(Content, fpos) != PS_BODY_START) {
		printf("error: incorrect function structure ==> function name: %s\n", Funcs.name.c_str());
		return false;
	}

	fpos = getNextBody(Content, fpos, output);
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
				printf("error: missing parameter name ==> function name: %s\n", Funcs.name.c_str());
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
			
			Funcs.params.push_back(tpar);
		}
	}

	// check const
	if (checkNext(Content, fpos) == PS_WORD) {
		for (auto i = 0; i < 3; i++) {
			fpos = getNextWord(Content, fpos, output);

			if (output == "const") {
				Funcs.cons = true;
			}

			if (checkNext(Content, fpos) != PS_WORD)
				break;
		}
	}

	return true;
}

// parse all property in the content
bool procAllFunc(std::string &Content, std::vector<MFunction> &Funcs) {
	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("KMETA_FUNCTION");
	unsigned int count = 0;
	bool ret = true;
	Funcs.clear();

	if (Content.empty()) {
		return false;
	}
	while ((pos = Content.find("KMETA_FUNCTION", tpos)) != std::string::npos) {
		tpos = pos + tsize;
		MFunction tfnc;

		if (procFunc(Content, tfnc, tpos)) {
			Funcs.push_back(tfnc);

		} else {
			ret = false;
			printf("function token number: %u\n", count);
		}

		++count;
	}

	return ret;
}

bool procProp(std::string &Content, MProperty &Prp, unsigned int Pos) {
	std::string output;
	std::vector<std::string> params;
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: incorrect property structure.\n");
		return false;
	}

	Pos = getNextBody(Content, Pos, output);
	splitParam(output, params);

	// KMETA_PROPERTY(NAME, SET, GET, CMT)
	if (params.size() < 4) {
		printf("error: missing property token parameter\n");
		return false;
	}

	if (params.size() > 4) {
		printf("warning: extra property token parameter (ignored) ==> property name: %s\n", params[0].c_str());
	}

	Prp.name = params[0];
	Prp.set = params[1];
	Prp.get = params[2];
	Prp.comment = params[3];

	// check set and get functions
	size_t spos = 0;
	if ((spos = Content.find(std::string(Prp.set + "("), 0)) == std::string::npos) {
		printf("error: missing property setter ==> property name: %s\n", Prp.name.c_str());
		return false;
	}
	spos += Prp.set.length();

	// check setter overload
	if (Content.find(std::string(Prp.set + "("), spos) != std::string::npos) {
		printf("error: property overload not supported ==> property name: %s\n", Prp.name.c_str());
		return false;
	}

	size_t gpos = 0;
	if ((gpos = Content.find(std::string(Prp.get + "()"), 0)) == std::string::npos) {
		printf("error: missing property getter ==> property name: %s\n", Prp.name.c_str());
		return false;
	}

	gpos += Prp.get.length();

	// check getter overload
	if (Content.find(std::string(Prp.get + "()"), gpos) != std::string::npos) {
		printf("error: property overload not supported ==> property name: %s\n", Prp.name.c_str());
		return false;
	}

	// extract property type from setter function
	if (checkNext(Content, spos) != PS_BODY_START) {
		printf("error: incorrect property setter ==> property name: %s\n", Prp.name.c_str());
		return false;
	}

	spos = getNextBody(Content, spos, output);
	splitParam(output, params);

	// we dont support properties with more than 1 parameter
	if (params.size() > 1) {
		printf("error: property with more than 1 parameter not supported ==> property name: %s\n", Prp.name.c_str());
		return false;
	}

	replaceTok(output, ' ', ',');
	splitParam(output, params);

	if (params.empty()) {
		printf("error: missing property setter parameter ==> property name: %s\n", Prp.name.c_str());
		return false;
	}

	// extract property type from getter function
	gpos -= Prp.get.length();
	gpos = getPrevWord(Content, --gpos, output);

	// check peroperty type match
	if (params[0] != output) {
		printf("error: property type mismatch ==> property name: %s\n", Prp.name.c_str());
		return false;
	}

	Prp.type = params[0];

	return true;
}

// parse all property in the content
bool procAllProp(std::string &Content, std::vector<MProperty> &Props) {
	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("KMETA_PROPERTY");
	unsigned int count = 0;
	bool ret = true;
	Props.clear();

	if (Content.empty()) {
		return false;
	}
	while ((pos = Content.find("KMETA_PROPERTY", tpos)) != std::string::npos) {
		tpos = pos + tsize;
		MProperty tcls;

		if (procProp(Content, tcls, tpos)) {
			Props.push_back(tcls);

		} else {
			ret = false;
			printf("prperty token number: %u\n", count);
		}

		++count;
	}

	return ret;
}

// parse and extract class information
bool procClass(std::string &Content, MClass &Cls, unsigned int Pos) {
	size_t pos;
	std::string cbody;
	std::string output;
	std::vector<std::string> params;

	// checking flags
	if (checkNext(Content, Pos) != PS_BODY_START) {
		printf("error: missing class/struct flags.\n");
		return false;
	}

	pos = getNextBody(Content, Pos, output);
	splitParam(output, params);

	if (params.empty()) {
		printf("error: empty class flag list.\n");
		return false;
	}

	Cls.flags = params[0];

	// checking type token (class or struct)
	if (checkNext(Content, pos) != PS_WORD) {
		printf("error: missing type token.\n");
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

	pos = getNextWord(Content, pos, output);
	if (output == "KITE_FUNC_EXPORT") {
		Cls.exstate = ES_EXPORT;

		// get class name
		if (checkNext(Content, pos) != PS_WORD) {
			printf("error: missing class name.\n");
			return false;
		}
		pos = getNextWord(Content, pos, output);
		Cls.name = output;

	} else if (output == "KITE_FUNC_IMPORT") {
		Cls.exstate = ES_IMPORT;

		// get class name
		if (checkNext(Content, pos) != PS_WORD) {
			printf("error: missing class name.\n");
			return false;
		}
		pos = getNextWord(Content, pos, output);
		Cls.name = output;

	} else {
		Cls.exstate = ES_NONE;
		Cls.name = output;
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
			printf("error: missing base class.\n");
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

	// incorrect structure
	} else {
		printf("error: incorrect class structure.\n");
		return false;
	}
}

// parse all CLASS tokens
bool procAllClass(std::string &Content, std::vector<MClass> &Classes) {
	size_t pos = 0;
	size_t tpos = 0;
	size_t tsize = strlen("KMETA_CLASS");
	unsigned int count = 0;
	bool ret = true;
	Classes.clear();

	if (Content.empty()) {
		return false;
	}

	while ((pos = Content.find("KMETA_CLASS", tpos)) != std::string::npos) {
		tpos = pos + tsize;
		MClass tcls;
		if (procClass(Content, tcls, tpos)) {
			Classes.push_back(tcls);

		} else {
			printf("class token number: %u\n", count);
			ret = false;
		}
		++count;
	}

	return ret;
}

// remove unnecessary things (comments and strings)
bool parse(std::string &Content, std::vector<MClass> &Classes) {
	Classes.clear();
	if (Content.empty()) {
		printf("empty file.\n");
		return false;
	}

	collapseStrings(Content);
	removeFSComments(Content);
	removeSSComments(Content);
	removeTok(Content, "public:");
	removeTok(Content, "private:");
	removeTok(Content, "protected:");
	if (!procAllClass(Content, Classes))
		return false;

	return true;
}

void createMacros(const std::vector<MClass> &Cls, std::string &Output) {
	Output.clear();

	for (auto i = 0; i < Cls.size(); i++) {
		std::string upname = Cls[i].name;

		// declaration
		Output.append("\n// ----[auto generated: " + Cls[i].name + " body macro]----\n");
		transform(upname.begin(), upname.end(), upname.begin(), toupper);
		Output.append("#define KMETA_" + upname + "_BODY() \\\n"
					  "private:\\\n");
		
		// define properties
		for (auto count = 0; count < Cls[i].props.size(); count++) {
			Output.append("inline void _prpset" + Cls[i].props[count].name + Cls[i].name + "(KRefVariant Value){ " +
						  Cls[i].props[count].set + "(Value.getValue<" + Cls[i].props[count].type + ">()); }\\\n");
		}

		// register functions
		Output.append("static void registerMeta(lua_State *Lua = nullptr, U32 Flag = 0);\\\n"
						"void serial(KBytesArray &Serializer, KSerialStateTypes State);\\\n"
						"friend KBytesArray &operator<<(KBytesArray &Out, " + Cls[i].name + "& Value) {\\\n"
						"Value.serial(Out, KST_SERIALIZE); return Out;}\\\n"
						"friend KBytesArray &operator>>(KBytesArray &In, " + Cls[i].name + "& Value) {\\\n"
						"Value.serial(In, KST_DESERIALIZE); return In;}\\\n"
						"friend KBytesArray &operator<<(KBytesArray &Out, " + Cls[i].name + "* Value) {\\\n"
						"Value.serial(Out, KST_SERIALIZE); return Out;}\\\n"
						"friend KBytesArray &operator>>(KBytesArray &In, " + Cls[i].name + "* Value) {\\\n"
						"Value.serial(In, KST_DESERIALIZE); return In;}\n");

		// declaration
		Output.append("\n// ----[auto generated: " + Cls[i].name + "::registerMeta() defention]----\n");
		Output.append("#define KMETA_" + upname + "_START() \\\n"
					  "void " + Cls[i].name + "::registerMeta(lua_State *Lua, U32 Flag){\\\n"
					  "static bool inite = true;\\\n"
					  "static KMetaClass instance(\"" + Cls[i].name + "\"," + Cls[i].flags + ", sizeof(" + Cls[i].name + "));\\\n"
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
			auto found = strmap.find(Cls[i].props[count].comment);
			Output.append("instance.addProperty(new KMetaProperty(\"" + Cls[i].props[count].name + "\", "
						   + found->second + ", KMP_BOTH, typeid(" + Cls[i].props[count].type + ")));\\\n");
		}

		// functions
		Output.append("KMetaFunction *fun;\\\n");
		for (auto count = 0; count < Cls[i].funcs.size(); count++) {
			Output.append("fun = new KMetaFunction(\"" + Cls[i].funcs[count].name + "\", "
						  + "typeid(" + Cls[i].funcs[count].ret + "));\\\n");

			// function parameters
			for (auto pcount = 0; pcount < Cls[i].funcs[count].params.size(); pcount++) {
				Output.append("fun->paramsType.push_back(typeid(" + Cls[i].funcs[count].params[pcount].type + "));\\\n");
			}
			Output.append("instance.addFunction(fun);\\\n");
		}

		// lua binding 
		Output.append("if (Lua != nullptr && (Flag & KMFLAG_SCRIPTABLE)) { \\\n"
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
						  + Cls[i].props[count].get + ", &" + Cls[i].name + "::" + Cls[i].props[count].set + ")\\\n");
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

		// end of lua binding
		Output.append(".endClass().endModule();}\\\n");

		// end of rgisterMeta
		Output.append("} inite = false;}\\\n");

		// serial defention
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

		if (!Cls[i].bases.empty()) {
			Output.append("if (State == KST_SERIALIZE) {\\\n" + ser + "} else if (State == KST_DESERIALIZE) {\\\n" + dser + "}");
		}
					  
	}
}

int main(void) {
	std::string macro;
	std::ifstream t("F:\\KEngineLab\\Repo\\include\\Kite\\Core\\graphic\\kgraphicstructs.h");
	std::string str((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());

	std::vector<MClass> cls;
	parse(str, cls);
	createMacros(cls, macro);
	
	system("pause");
}