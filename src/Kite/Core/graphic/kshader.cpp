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
#include "Kite/graphic/kshader.h"
#include "Kite/graphic/kgraphicutil.h"
#include "src/Kite/graphic/glcall.h"
#include <fstream>

namespace Kite{

    KShader::KShader():
        _kprogram(0),
        _kcurrentTexture(-1)
    {}

    KShader::~KShader(){
        // destroy program object
        if (_kprogram)
            DGL_CALL(glDeleteObjectARB(_kprogram));
    }


    void KShader::loadFile(const std::string &ShaderFile, KShaderTypes ShaderType){
        if (!ShaderFile.empty()){
            std::vector<char> data;

            // read file content
            readFile(ShaderFile.c_str(), data);

            // compile
            if (ShaderType == KS_VERTEX)
                compile(&data[0], NULL);
            else if (ShaderType == KS_FRAGMENT)
                compile(NULL, &data[0]);
        }
    }

    void KShader::loadFile(const std::string &VertexFile, const std::string &FragmentFile){
        if (!VertexFile.empty() && !FragmentFile.empty()){
            std::vector<char> vertexSh;
            std::vector<char> fragmentSh;

            // read file content
            readFile(VertexFile.c_str(), vertexSh);
            readFile(FragmentFile.c_str(), fragmentSh);

            // compile
            compile(&vertexSh[0], &fragmentSh[0]);
        }
    }

    void KShader::loadMemory(const std::string &ShaderCod, KShaderTypes ShaderType){
        if (!ShaderCod.empty()){
            if (ShaderType == KS_VERTEX)
                compile(ShaderCod.c_str(), NULL);
            else if (ShaderType == KS_FRAGMENT)
                compile(NULL, ShaderCod.c_str());
        }
    }

    void KShader::loadMemory(const std::string &VertexCod, const std::string &FragmentCod){
        if (!VertexCod.empty() && !FragmentCod.empty()){
            compile(VertexCod.c_str(), FragmentCod.c_str());
        }
    }

    void KShader::setParam(const std::string &ParamName, F32 x){
        if (_kprogram){

            // enable program
            GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
            DGL_CALL(glUseProgramObjectARB(_kprogram));

            // get parameter location and assign it new values
            GLint location = glGetUniformLocationARB(_kprogram, ParamName.c_str());
            if (location != -1){
                DGL_CALL(glUniform1fARB(location, x));
            }else{
                KDEBUG_PRINT("requested parameter not found in shader");
            }

            // disable program
            DGL_CALL(glUseProgramObjectARB(program));
        }
    }

    void KShader::setParam(const std::string &ParamName, F32 x, F32 y){
        if (_kprogram){

            // enable program
            GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
            DGL_CALL(glUseProgramObjectARB(_kprogram));

            // get parameter location and assign it new values
            GLint location = glGetUniformLocationARB(_kprogram, ParamName.c_str());
            if (location != -1){
                DGL_CALL(glUniform2fARB(location, x, y));
            }else{
                KDEBUG_PRINT("requested parameter not found in shader");
            }

            // disable program
            DGL_CALL(glUseProgramObjectARB(program));
        }
    }

    void KShader::setParam(const std::string &ParamName, F32 x, F32 y, F32 z){
        if (_kprogram){

            // enable program
            GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
            DGL_CALL(glUseProgramObjectARB(_kprogram));

            // get parameter location and assign it new values
            GLint location = glGetUniformLocationARB(_kprogram, ParamName.c_str());
            if (location != -1){
                DGL_CALL(glUniform3fARB(location, x, y, z));
            }else{
                KDEBUG_PRINT("requested parameter not found in shader");
            }

            // disable program
            DGL_CALL(glUseProgramObjectARB(program));
        }
    }

    void KShader::setParam(const std::string &ParamName, F32 x, F32 y, F32 z, F32 w){
        if (_kprogram){

            // enable program
            GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
            DGL_CALL(glUseProgramObjectARB(_kprogram));

            // get parameter location and assign it new values
            GLint location = glGetUniformLocationARB(_kprogram, ParamName.c_str());
            if (location != -1){
                DGL_CALL(glUniform4fARB(location, x, y, z, w));
            }else{
                KDEBUG_PRINT("requested parameter not found in shader");
            }

            // disable program
            DGL_CALL(glUseProgramObjectARB(program));
        }
    }

    void KShader::setParam(const std::string &ParamName, const KColor& Color){
        setParam(ParamName, Color.r / 255.f, Color.g / 255.f, Color.b / 255.f, Color.a / 255.f);
    }

    void KShader::setParam(const std::string &ParamName, const KTransform& Transform){
        if (_kprogram){

            // enable program
            GLhandleARB program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
            DGL_CALL(glUseProgramObjectARB(_kprogram));

            // get parameter location and assign it new values
            GLint location = glGetUniformLocationARB(_kprogram, ParamName.c_str());
            if (location != -1){
                DGL_CALL(glUniformMatrix4fvARB(location, 1, GL_FALSE, Transform.getMatrix()));
            }else{
                KDEBUG_PRINT("requested parameter not found in shader");
            }

            // disable program
            DGL_CALL(glUseProgramObjectARB(program));
        }
    }

    void KShader::setParam(const std::string &ParamName, const KTexture& Texture){
        if (_kprogram){

            // Find the location of the variable in the shader
            int location = glGetUniformLocationARB(_kprogram, ParamName.c_str());
            if (location == -1){
                KDEBUG_PRINT("Texture not found in shader");
                return;
            }

            // store the location -> texture mapping
            std::map<I32, const KTexture *>::iterator it = _ktextureTable.find(location);
            if (it == _ktextureTable.end()){
                // new entry, make sure there are enough texture units
                static const GLint maxUnits = getMaximumTextureUnit();
                if (_ktextureTable.size() + 1 >= static_cast<std::size_t>(maxUnits)){
                    KDEBUG_PRINT("all available texture units are used");
                    return;
                }

                _ktextureTable[location] = &Texture;
            }
            else{
                // location already used, just replace the texture
                it->second = &Texture;
            }
        }
    }

    void KShader::setParam(const std::string &ParamName, KShaderTextureTypes Texture){
        if (_kprogram){

            // find the location of the variable in the shader
            _kcurrentTexture = glGetUniformLocationARB(_kprogram, ParamName.c_str());
            if (_kcurrentTexture == -1)
                KDEBUG_PRINT("Texture not found in shader");
        }
    }

    void KShader::bind() const{
        if (_kprogram){
            // enable the program
            DGL_CALL(glUseProgramObjectARB(_kprogram));

            // fill the texture units for sending them to shader
            fillTextureUnits();

            // bind the current texture
            if (_kcurrentTexture != -1)
                DGL_CALL(glUniform1iARB(_kcurrentTexture, 0));
        }
    }

    void KShader::unbind() const{
        // disable the program
        DGL_CALL(glUseProgramObjectARB(0));
    }

    void KShader::readFile(const char *FileName, std::vector<char> &data){

        // open file in binary mode
        std::ifstream reader(FileName, std::ios_base::binary);
        KDEBUG_ASSERT_T(reader);

        // calculate size of file
        reader.seekg(0, std::ios_base::end);
        std::streamsize size = reader.tellg();
        KDEBUG_ASSERT_T(size > 0);

        // reading file
        reader.seekg(0, std::ios_base::beg);
        data.resize(size);
        reader.read(&data[0], size);

        // append '\0' at the end of cod
        data.push_back('\0');
    }

    bool KShader::compile(const char *Vertex, const char *Fragment){
        // first make sure that we can use shaders
        if(!isShaderAvailable()){
            KDEBUG_PRINT("shader is not available.")
            return false;
        }

        // destroy the shader if it was already created
        if (_kprogram)
            DGL_CALL(glDeleteObjectARB(_kprogram));

        // create the program
        _kprogram = glCreateProgramObjectARB();

        // create vertex shader object
        if (Vertex)
            if (!createShader(Vertex, KS_VERTEX))
                return false;

        // create fragment shader object
        if (Fragment)
            if (!createShader(Fragment, KS_FRAGMENT))
                return false;

        // link the program
        DGL_CALL(glLinkProgramARB(_kprogram));

        // check the link log
        GLint success;
        DGL_CALL(glGetObjectParameterivARB(_kprogram, GL_OBJECT_LINK_STATUS_ARB, &success));
        if (success == GL_FALSE){
            KDEBUG_PRINT("Failed to compile vertex shader.");
            DGL_CALL(glDeleteObjectARB(_kprogram));
            _kprogram = 0;
            return false;
        }

        return true;
    }

    bool KShader::createShader(const char *ShaderCod, KShaderTypes ShaderType){

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
                KDEBUG_PRINT("Failed to compile vertex shader.");

                // cleanup
                DGL_CALL(glDeleteObjectARB(shader));
                DGL_CALL(glDeleteObjectARB(_kprogram));
                _kprogram = 0;
                return false;
            }

            // attach the shader to the program, and delete it (not needed anymore)
            DGL_CALL(glAttachObjectARB(_kprogram, shader));
            DGL_CALL(glDeleteObjectARB(shader));
            return true;
        }
        return false;
    }

    void KShader::fillTextureUnits() const{
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
}
