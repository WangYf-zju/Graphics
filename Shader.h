/* This file references from 
 * https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_m.h
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_CORE>

class Shader
{
public:
    unsigned int ID;
    QOpenGLFunctions_3_3_Core * f;
    std::string vertexCode;
    std::string fragmentCode;

    Shader(const char* vertexPath, const char* fragmentPath) {
        // retrieve the vertex/fragment source code from filePath
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
    }
    Shader * initialize(QOpenGLFunctions_3_3_Core * f)
    {
        this->f = f;
        return this;
    }
    // constructor generates the modelShader on the fly
    Shader * compile ()
    {
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // compile shaders
        unsigned int vertex, fragment;
        // vertex modelShader
        vertex = f->glCreateShader(GL_VERTEX_SHADER);
        f->glShaderSource(vertex, 1, &vShaderCode, NULL);
        f->glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = f->glCreateShader(GL_FRAGMENT_SHADER);
        f->glShaderSource(fragment, 1, &fShaderCode, NULL);
        f->glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // modelShader Program
        ID = f->glCreateProgram();
        f->glAttachShader(ID, vertex);
        f->glAttachShader(ID, fragment);
        f->glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        f->glDeleteShader(vertex);
        f->glDeleteShader(fragment);
        return this;
    }
    // activate the modelShader
    Shader * use()
    {
        f->glUseProgram(ID);
        return this;
    }
    // utility uniform functions
    Shader * setBool(const std::string &name, bool value)
    {
        f->glUniform1i(f->glGetUniformLocation(ID, name.c_str()), (int)value);
        return this;
    }
    Shader * setInt(const std::string &name, int value)
    {
        f->glUniform1i(f->glGetUniformLocation(ID, name.c_str()), value);
        return this;
    }
    Shader * setFloat(const std::string &name, float value)
    {
        f->glUniform1f(f->glGetUniformLocation(ID, name.c_str()), value);
        return this;
    }
    Shader * setVec2(const std::string &name, const glm::vec2 &value)
    {
        f->glUniform2fv(f->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        return this;
    }
    Shader * setVec2(const std::string &name, float x, float y)
    {
        f->glUniform2f(f->glGetUniformLocation(ID, name.c_str()), x, y);
        return this;
    }
    Shader * setVec3(const std::string &name, const glm::vec3 &value)
    {
        f->glUniform3fv(f->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        return this;
    }
    Shader * setVec3(const std::string &name, float x, float y, float z)
    {
        f->glUniform3f(f->glGetUniformLocation(ID, name.c_str()), x, y, z);
        return this;
    }
    Shader * setVec4(const std::string &name, const glm::vec4 &value)
    {
        f->glUniform4fv(f->glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        return this;
    }
    Shader * setVec4(const std::string &name, float x, float y, float z, float w)
    {
        f->glUniform4f(f->glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        return this;
    }
    Shader * setMat2(const std::string &name, const glm::mat2 &mat)
    {
        f->glUniformMatrix2fv(f->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        return this;
    }
    Shader * setMat3(const std::string &name, const glm::mat3 &mat)
    {
        f->glUniformMatrix3fv(f->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        return this;
    }
    Shader * setMat4(const std::string &name, const glm::mat4 &mat)
    {
        f->glUniformMatrix4fv(f->glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        return this;
    }

private:
    // utility function for checking modelShader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type) const
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            f->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                f->glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            f->glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                f->glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif