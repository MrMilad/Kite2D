/* 
 Kite2D Game Framework.
 Copyright (C) 2010-2014  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or 
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef KGLRENDER_H
#define KGLRENDER_H

#include "Kite/system/ksystemdef.h"
#include "Kite/system/knoncopyable.h"
#include "Kite/graphic/kgraphictypes.h"
#include <cstring>

namespace Kite{
    class KITE_FUNC_EXPORT KGLRender : KNonCopyable{
    public:
        KGLRender();
        virtual ~KGLRender();

        virtual void pushStates(); // re-call initialize.
        virtual void popStates();

        virtual bool initialize() = 0;
        virtual std::string getRendererName() = 0;
        virtual KRendererFlags getRendererFlag() = 0;

    protected:
        static bool init(); // init glew
    };
}

#endif // KGLRENDER_H