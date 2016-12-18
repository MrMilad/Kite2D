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
"function hooks.subscribe(Handle, Type, Function)\n"\
"if (hooks.funcs[Type] == nil) then\n"\
"hooks.funcs[Type] = {} end\n"\
"if (hooks.funcs[Type][Handle.index] == nil) then\n"\
"hooks.funcs[Type][Handle.index] = Function\n"\
"else\n"\
"print(\"hooks.subscribe Error: Handle.index: \"..Handle.index..\" Already Exists\")\n"\
"end\n"\
"end\n"\
"function hooks.unsubscribe(Handle)\n"\
"for _, Hook in pairs(hooks.funcs) do\n"\
"if (Hook[Handle.index]) then\n"\
"Hook[Handle.index] = nil\n"\
"end\n"\
"end\n"\
"end\n"\
"function hooks.post(Type, ...)\n"\
"if (hooks.funcs[Type]) then\n"\
"for _,v in pairs(hooks.funcs[Type]) do\n"\
"v(...)\n"\
"end\n"\
"end\n"\
"end\n"\
"function hooks.postDirect( Type, Handle, ... )\n"\
"if (hooks.funcs[Type][Handle.index]) then hooks.funcs[Type][Handle.index](...) end\n"\
"end"

#endif // KLOGICDEF_H