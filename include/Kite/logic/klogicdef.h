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
#ifndef KLOGICDEF_H
#define KLOGICDEF_H

#include "Kite/core/kcoredef.h"

#define KLUA_HOOK \
"hooks = {}\n"\
"hooks.funcs = {}\n"\
"function hooks.add(Name, Handle, Function)\n"\
"if (hooks.funcs[Name] == nil) then\n"\
"print(\"Hook \"..Name..\" Does Not Exist\")\n"\
"else\n"\
"if (hooks.funcs[Name][Handle.index] == nil) then\n"\
"hooks.funcs[Name][Handle.index] = Function\n"\
"else\n"\
"print(\"hooks.add Error: Handle.index: \"..Handle.index..\" Already Exists\")\n"\
"end\n"\
"end\n"\
"end\n"\
"function hooks.remove(Handle)\n"\
"for _, Hook in pairs(hooks.funcs) do\n"\
"if (Hook[Handle.index]) then\n"\
"Hook[Handle.index] = nil\n"\
"end\n"\
"end\n"\
"end\n"\
"function hooks.call(Name, ...)\n"\
"for  k=1, #hooks.funcs[Name] do\n"\
"if (hooks.funcs[Name][k]) then hooks.funcs[Name][k](...)\n"\
"end\n"\
"end\n"\
"end\n"\
"function hooks.callDirect( Name, Handle, ... )\n"\
"if (hooks.funcs[Name][Handle.index]) then hooks.funcs[Name][Handle.index](...) end\n"\
"end"

#endif // KLOGICDEF_H