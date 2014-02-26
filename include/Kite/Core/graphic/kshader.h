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
#ifndef KSHADER_H
#define KSHADER_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/graphic/ktransform.h"
#include <map>
#include <vector>
#include <string>

namespace Kite{
    class KITE_FUNC_EXPORT KShader{
    public:
        KShader();
        ~KShader();

        /// load from file
        void loadFile(const std::string &ShaderFile, KShaderTypes ShaderType);
        void loadFile(const std::string &VertexFile, const std::string &FragmentFile);

        /// load from memory
        void loadMemory(const std::string &ShaderCod, KShaderTypes ShaderType);
        void loadMemory(const std::string &VertexCod, const std::string &FragmentCod);

        /// send a flaot
        void setParam(const std::string &ParamName, F32 x);

        /// send 2x1 vector (vec2 in GLSL)
        void setParam(const std::string &ParamName, F32 x, F32 y);

        /// send 3x1 vector (vec3 in GLSL)
        void setParam(const std::string &ParamName, F32 x, F32 y, F32 z);

        /// send 4x1 vector (vec4 in GLSL)
        void setParam(const std::string &ParamName, F32 x, F32 y, F32 z, F32 w);

        /// send color
        /// a KColor will be transformed
        /// to a normalized vec4(1.0, 0.5, 0.0, 1.0) in the shader
        void setParam(const std::string &ParamName, const KColor& Color);

        /// send 4x4 matrix
        void setParam(const std::string &ParamName, const KTransform& Transform);

        /// send texture
        void setParam(const std::string &ParamName, const KTexture& Texture);

        /// send current texture
        void setParam(const std::string &ParamName, KShaderTextureTypes Texture);

        /// bind the shader
        void bind() const;

        /// unbind the shader
        void unbind() const;

        inline U32 getID() const {return _kprogram;}

    private:
        void readFile(const char *FileName, std::vector<char> &data);
        bool compile(const char *Vertex, const char *Fragment);
        bool createShader(const char *ShaderCod, KShaderTypes ShaderType);
        void fillTextureUnits() const;

        U32 _kprogram;
        std::map<I32, const KTexture *> _ktextureTable;
        I32 _kcurrentTexture;
    };
}

#endif // KSHADER_H
