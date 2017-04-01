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
#ifndef KENGINEDEF_H
#define KENGINEDEF_H

#include "Kite/core/kcoredef.h"

#ifdef KITE_DEV_DEBUG
#define SUB0_DEBUG \
"	 -- check component state\n"\
"    if (Node.isOnActiveHierarchy == false) then\n"\
"        print(\"addListener(): Error: node id: \"..Node.id..\"inactive node\")\n"\
"		 return\n"\
"    end\n"

#define SUB1_DEBUG \
"	 -- check function\n"\
"    if (ty[Node.id] ~= nil) then\n"\
"        print(\"addListener(): Warning: node.id: \"..Node.id..\" function handle replaced\")\n"\
"    end\n"
#else
#define SUB0_DEBUG 
#define SUB1_DEBUG 
#endif // KITE_DEV_DEBUG



#define KLUA_EVENTS \
"events = {}\n"\
"events.types = {}\n"\
"function events.addListener(self, Node, Type, Function)\n"\
SUB0_DEBUG\
"	 -- create types table\n"\
"    local ty = events.types[Type]\n"\
"    if (ty == nil) then\n"\
"        events.types[Type] = {}\n"\
"        ty = events.types[Type]\n"\
"        setmetatable(ty, { __mode = 'v' })\n"\
"    end\n"\
SUB1_DEBUG\
"	 -- add function to type table\n"\
"    ty[Node.id] = Function\n"\
"end\n"\
"function events.removeListener(self, Node, Type)\n"\
SUB0_DEBUG\
"    local ty = events.types[Type]\n"\
"    if (ty) then\n"\
"		 ty[Node.id] = nil\n"\
"    end\n"\
"end\n"\
"function events.trigger(self, Type, Data)\n"\
"    local ty = events.types[Type]\n"\
"    if (ty) then\n"\
"        for _, v in pairs(ty) do\n"\
"			 v(Type, Data)\n"\
"        end\n"\
"    end\n"\
"end\n"\
"function events.triggerNodes(self, Nodes, Type,  Data)\n"\
"    local ty = events.types[Type]\n"\
"    if (ty) then\n"\
"        for _, v in pairs(Nodes) do\n"\
"			 local fun = ty[v.id]\n"\
"			 if (fun) then\n"\
"				 fun(Type, Data)\n"\
"			 end\n"\
"        end\n"\
"    end\n"\
"end\n"\
"function events.postFrom(self, Node, Type, Data)\n"\
SUB0_DEBUG\
"    local nt = nodes[Node.id]\n"\
"	 local counter = 0\n"\
"    if (nt) then\n"\
"		 local ty = nt[Type]\n"\
"		 if (ty) then\n"\
"			 for _, v in pairs(ty) do\n"\
"				 v(Type, Data)\n"\
"				 counter = counter + 1\n"\
"			 end\n"\
"		 end\n"\
"    end\n"\
"	 return counter\n"\
"end\n"

#endif // KENGINEDEF_H