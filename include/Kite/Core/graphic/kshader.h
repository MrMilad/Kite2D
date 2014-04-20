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
#include "Kite/Core/system/knoncopyable.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/ktexture.h"
#include "Kite/Core/math/ktransform.h"
#include <map>
#include <vector>
#include <string>

namespace Kite{
    class KITE_FUNC_EXPORT KShader : KNonCopyable{
    public:
        KShader();
        ~KShader();

        /// load from file
        //void loadFile(const std::string &ShaderFile, KShaderTypes ShaderType);
        bool loadFile(const std::string &VertexFile, const std::string &FragmentFile);

        /// load from memory
        //void loadMemory(const std::string &ShaderCod, KShaderTypes ShaderType);
        bool loadMemory(const std::string &VertexCod, const std::string &FragmentCod);

        /// bind attribute
        void bindAttribute(U16 Index, const std::string Name);

        /// link the shader program
        bool link();

        /// get location of the uniform in the shader
        /// return -1 if failed
        I16 getUniformLocation(const std::string &ParamName) const;

        /// pass a flaot
        void setParam(I16 Location, F32 x) const;

        /// pass 2x1 vector (vec2 in GLSL)
        void setParam(I16 Location, F32 x, F32 y) const;

        /// pass 3x1 vector (vec3 in GLSL)
        void setParam(I16 Location, F32 x, F32 y, F32 z) const;

        /// pass 4x1 vector (vec4 in GLSL)
        void setParam(I16 Location, F32 x, F32 y, F32 z, F32 w) const;

        /// pass color
        /// a KColor will be transformed
        /// to a normalized vec4(1.0, 0.5, 0.0, 1.0) in the shader
        void setParam(I16 Location, const KColor& Color) const;

        /// pass 4x4 matrix
        void setParam(I16 Location, const KTransform& Transform) const;

        /// pass texture
        void setParam(I16 Location, const KTexture& Texture);

        /// pass current texture
        //void setParam(I16 Location, KShaderTextureTypes Texture);

        /// bind the shader
        void bind() const;

        void unbind();

        /// unbind currently shader
        static void unbindShader();

        inline U32 getID() const {return _kprogId;}
        static const std::string getShaderVersion();

    private:
        void _readFile(const char *FileName, std::vector<char> &data);
        bool _createShader(const char *ShaderCod, KShaderTypes ShaderType);
        void _fillTextureUnits() const;

        U32 _kprogId;
        std::map<I32, const KTexture *> _ktextureTable;
        I32 _kcurrentTexture;
        static U32 _klastProgId;
    };
}

#endif // KSHADER_H
