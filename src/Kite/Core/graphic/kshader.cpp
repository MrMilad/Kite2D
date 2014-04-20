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
#include "Kite/core/graphic/kshader.h"
#include "Kite/core/graphic/kgraphicutil.h"
#include "src/Kite/core/graphic/glcall.h"
#include <fstream>

namespace Kite{
    U32 KShader::_klastProgId = 0;
    KShader::KShader():
        _kprogId(0),
        _kcurrentTexture(-1)
    {}

    KShader::~KShader(){
        // destroy program object
        DGL_CALL(glDeleteObjectARB(_kprogId));
        if (_klastProgId == _kprogId){
            _klastProgId = 0;
        }
    }

    bool KShader::loadFile(const std::string &VertexFile, const std::string &FragmentFile){
        // first make sure that we can use shaders
//        if(!isShaderAvailable()){
//            KDEBUG_PRINT("shader is not available.")
//            return false;
//        }

        if (!VertexFile.empty() && !FragmentFile.empty()){
            std::vector<char> vertexSh;
            std::vector<char> fragmentSh;

            // read file content
            _readFile(VertexFile.c_str(), vertexSh);
            _readFile(FragmentFile.c_str(), fragmentSh);

            if (vertexSh.empty() || fragmentSh.empty()){
                KDEBUG_PRINT("empty shader file.")
                return false;
            }

            // destroy the shader if it was already created
            if (_kprogId)
                DGL_CALL(glDeleteObjectARB(_kprogId));

            // create the new program
            _kprogId = glCreateProgramObjectARB();

            // create vertex shader object
            if (!_createShader(&vertexSh[0], KS_VERTEX))
                return false;

            // create fragment shader object
            if (!_createShader(&fragmentSh[0], KS_FRAGMENT))
                return false;

            return true;
        }
        return false;
    }

    bool KShader::loadMemory(const std::string &VertexCod, const std::string &FragmentCod){
        // first make sure that we can use shaders
//        if(!isShaderAvailable()){
//            KDEBUG_PRINT("shader is not available.")
//            return false;
//        }

        if (!VertexCod.empty() && !FragmentCod.empty()){
            // destroy the shader if it was already created
            if (_kprogId)
                DGL_CALL(glDeleteObjectARB(_kprogId));

            // create the new program
            _kprogId = glCreateProgramObjectARB();

            // create vertex shader object
            if (!_createShader(VertexCod.c_str(), KS_VERTEX))
                return false;

            // create fragment shader object
            if (!_createShader(FragmentCod.c_str(), KS_FRAGMENT))
                return false;

            return true;
        }
        return false;
    }

    void KShader::bindAttribute(U16 Index, const std::string Name){
        DGL_CALL(glBindAttribLocation(_kprogId, (GLuint)Index, Name.c_str()));
    }

    bool KShader::link(){
        if (!_kprogId)
            return false;

        // link the program
        DGL_CALL(glLinkProgramARB(_kprogId));

        // check the link log
        GLint isLinked;
        DGL_CALL(glGetProgramiv(_kprogId, GL_LINK_STATUS, (int *)&isLinked));
        //DGL_CALL(glGetObjectParameterivARB(_kprogram, GL_OBJECT_LINK_STATUS_ARB, &success)); // old version
        if (isLinked == GL_FALSE){
            KDEBUG_PRINT("Failed to link shader program.");
            DGL_CALL(glDeleteObjectARB(_kprogId));
            _kprogId = 0;
            return false;
        }

        return true;
    }

    I16 KShader::getUniformLocation(const std::string &ParamName) const{
        if (_kprogId){
            GLint location = glGetUniformLocationARB(_kprogId, ParamName.c_str());
            return (I16)location;
        }
        return -1;
    }

    void KShader::setParam(Kite::I16 Location, F32 x) const{
        if (_kprogId && Location >= 0){
            // get parameter location and assign it new values
            DGL_CALL(glUniform1fARB(Location, x));
        }
    }

    void KShader::setParam(Kite::I16 Location, F32 x, F32 y) const{
        if (_kprogId && Location >= 0){
            DGL_CALL(glUniform2fARB(Location, x, y));
        }
    }

    void KShader::setParam(Kite::I16 Location, F32 x, F32 y, F32 z) const{
        if (_kprogId && Location >= 0){
            DGL_CALL(glUniform3fARB(Location, x, y, z));
        }
    }

    void KShader::setParam(Kite::I16 Location, F32 x, F32 y, F32 z, F32 w) const{
        if (_kprogId && Location >= 0){
            DGL_CALL(glUniform4fARB(Location, x, y, z, w));
        }
    }

    void KShader::setParam(Kite::I16 Location, const KColor& Color) const{
        setParam(Location, Color.r / 255.f, Color.g / 255.f, Color.b / 255.f, Color.a / 255.f);
    }

    void KShader::setParam(Kite::I16 Location, const KTransform& Transform) const{
        if (_kprogId && Location >= 0){
            DGL_CALL(glUniformMatrix4fvARB(Location, 1, GL_FALSE, Transform.getMatrix()));
        }
    }

    void KShader::setParam(Kite::I16 Location, const KTexture& Texture){
        if (_kprogId && Location >= 0){
            // store the location -> texture mapping
            std::map<I32, const KTexture *>::iterator it = _ktextureTable.find(Location);
            if (it == _ktextureTable.end()){
                // new entry, make sure there are enough texture units
                static const GLint maxUnits = getMaximumTextureUnit();
                if (_ktextureTable.size() + 1 >= static_cast<std::size_t>(maxUnits)){
                    KDEBUG_PRINT("all available texture units are used");
                    return;
                }

                _ktextureTable[Location] = &Texture;
            }
            else{
                // location already used, just replace the texture
                it->second = &Texture;
            }
        }
    }

//    void KShader::setParam(Kite::I16 Location, KShaderTextureTypes Texture){
//        if (_kprogram && Location >= 0){
//            KDEBUG_TEMP(if (Location == -1))
//                KDEBUG_PRINT("Texture not found in shader");
//        }
//    }

    void KShader::bind() const{
        if (_klastProgId != _kprogId){
            // enable the program
            DGL_CALL(glUseProgramObjectARB(_kprogId));
            _klastProgId = _kprogId;

            // fill the texture units for sending them to shader
            _fillTextureUnits();

            // bind the current texture
            if (_kcurrentTexture != -1)
                DGL_CALL(glUniform1iARB(_kcurrentTexture, 0));
        }
    }

    void KShader::unbind(){
        // disable the program
        if (_klastProgId == _kprogId){
            DGL_CALL(glUseProgramObjectARB(0));
            _klastProgId = 0;
        }
    }

    void KShader::unbindShader(){
        // disable the program
        DGL_CALL(glUseProgramObjectARB(0));
        _klastProgId = 0;
    }

    void KShader::_readFile(const char *FileName, std::vector<char> &data){

        // open file in binary mode
        std::ifstream reader(FileName, std::ios_base::binary);
        KDEBUG_ASSERT_T(reader);

        // calculate size of file
        reader.seekg(0, std::ios_base::end);
		std::streamsize size = reader.tellg();
        KDEBUG_ASSERT_T(size > 0);

        // reading file
        reader.seekg(0, std::ios_base::beg);
        data.resize((U32)size);
        reader.read(&data[0], size);

        // append '\0' at the end of cod
        data.push_back('\0');
    }

    bool KShader::_createShader(const char *ShaderCod, KShaderTypes ShaderType){

        if (ShaderCod){

            // create and compile the shader
            GLhandleARB shader;
            switch (ShaderType){
            case KS_VERTEX:
                shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
                break;
            case KS_FRAGMENT:
                shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
                break;
            default:
                return false;
            }

            DGL_CALL(glShaderSourceARB(shader, 1, &ShaderCod, NULL));
            DGL_CALL(glCompileShaderARB(shader));

            // check the compile log
            GLint success;
            DGL_CALL(glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
            if (success == GL_FALSE){
                if  (ShaderType == KS_VERTEX){
                    KDEBUG_PRINT("Failed to compile vertex shader.");
                }else{
                    KDEBUG_PRINT("Failed to compile fragment shader.");
                }

                // cleanup
                DGL_CALL(glDeleteObjectARB(shader));
                DGL_CALL(glDeleteObjectARB(_kprogId));
                _kprogId = 0;
                return false;
            }

            // attach the shader to the program, and delete it (not needed anymore)
            DGL_CALL(glAttachObjectARB(_kprogId, shader));
            DGL_CALL(glDeleteObjectARB(shader));
            return true;
        }
        KDEBUG_PRINT("invalid pointer");
        return false;
    }

    void KShader::_fillTextureUnits() const{
        // iterate our texture table and setup texture units
        std::map<I32, const KTexture *>::const_iterator it = _ktextureTable.begin();
        for (std::size_t i = 0; i < _ktextureTable.size(); ++i){
            GLint index = static_cast<GLsizei>(i + 1);
            DGL_CALL(glUniform1iARB(it->first, index));
            DGL_CALL(glActiveTextureARB(GL_TEXTURE0_ARB + index));
            it->second->bind();
            ++it;
        }

        // finally we activate texture unit 0 again
        DGL_CALL(glActiveTextureARB(GL_TEXTURE0_ARB));
    }

    const std::string KShader::getShaderVersion(){
        std::string ver;
        ver.append((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        return ver;
    }
}
