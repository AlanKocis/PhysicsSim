#include <h/Shader.h>


Shader::Shader(const char *vertex_source, const char *fragment_source)
{
    loadShaderProgram(vertex_source, fragment_source);
}

Shader::Shader()
{
    ID = 0;
}

void Shader::loadShaderProgram(const char* vertex_source, const char* fragment_source)
{

    if (vertex_source == NULL || fragment_source == NULL)
    {
        printf("error in Shader.c :: argument was null");
        return;
    }


    char bytes[256] = ROOT_DIR;
    strcat(bytes, "shaders/");
    strcat(bytes, vertex_source);

    FILE* vertex_file;
    
    if (fopen_s(&vertex_file, bytes, "r") != 0)
    {
        printf("error in Shader.c :: couldn't open vertex_source :: %s\n", bytes);
        return;
    }

    char bytes2[256] = ROOT_DIR;
    strcat(bytes2, "shaders/");
    strcat(bytes2, fragment_source);

    FILE* fragment_file;
    if (fopen_s(&fragment_file, bytes2, "r") != 0)
    {
        printf("error in Shader.c :: couldn't open fragment_source :: %s\n", bytes2);
        fclose(vertex_file);
        return;
    }

    char* vertex_str = _readfile(vertex_file);
    if (vertex_str == NULL)
    {
        printf("error in Shader.c :: couldn't read vertex_source\n");
        fclose(vertex_file);
        fclose(fragment_file);
        return;
    }

    char* fragment_str = _readfile(fragment_file);
    if (fragment_str == NULL)
    {
        printf("error in Shader.c :: couldn't read fragment_source\n");
        fclose(vertex_file);
        fclose(fragment_file);
        return;
    }


    int success;
    char errorLog[512]; 

    GLuint64 vertex_shader_object = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_object, 1, &vertex_str, NULL);
    glCompileShader(vertex_shader_object);
    glGetShaderiv(vertex_shader_object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader_object, 512, NULL, errorLog);
        printf("error in Shader.c :: couldn't compile vertex shader :: %s\n", errorLog);
        return;
    }

    GLuint64 fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_object, 1, &fragment_str, NULL);
    glCompileShader(fragment_shader_object);
    glGetShaderiv(fragment_shader_object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader_object, 512, NULL, errorLog);
        printf("error in Shader.c :: couldn't compile fragment shader :: %s\n", errorLog);
        return;
    }

    // all shaders loaded successfully

    GLuint newID = glCreateProgram();

    glAttachShader(newID, vertex_shader_object);
    glAttachShader(newID, fragment_shader_object);
    glLinkProgram(newID);
    glGetShaderiv(newID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(newID, 512, NULL, errorLog);
        printf("error in Shader.c :: couldn't link shader program :: %s\n", errorLog);
    }

    glDeleteShader(vertex_shader_object);
    glDeleteShader(fragment_shader_object);

    fclose(vertex_file);
    fclose(fragment_file);
    this->ID = newID;

    printf("Compiled Shader Program :: %s :: %s\n", bytes, bytes2);
}

GLuint &Shader::getID()
{
    return this->ID;
}

char* _readfile(FILE* f)
{
    // f invalid? fseek() fail?
    if (f == NULL || fseek(f, 0, SEEK_END)) {
        return NULL;
    }

    long length = ftell(f);
    rewind(f);
    // Did ftell() fail?  Is the length too long?
    if (length == -1 || (unsigned long)length >= SIZE_MAX) {
        return NULL;
    }

    // Convert from long to size_t
    size_t ulength = (size_t)length;
    char* buffer = (char *)malloc(ulength + 1);

    // Allocation failed? Read incomplete?
    //if (buffer == NULL || fread(buffer, 1, ulength, f) != ulength) {
    //    free(buffer);
    //    return NULL;
    //}

    if (buffer == NULL)
    {
        free(buffer);
        return NULL;
    }

    memset(buffer, '\0', ulength + 1);
    int res = fread(buffer, 1, ulength, f);
      

    buffer[ulength] = '\0'; // Now buffer points to a string

    return buffer;
}