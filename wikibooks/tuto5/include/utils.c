#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//utility n stuff
char* load_shader(const char* name)
  {
  FILE* fp;
  long size = 0;
  char* shader;

  //read for size
  if(!(fp = fopen(name, "rb")))
    return "";

  fseek(fp, 0, SEEK_END);
  size = ftell(fp)+1;
  fclose(fp);

  //read actual file
  fp = fopen(name, "r");
  shader = memset(malloc(size), '\0', size);
  if (!fread(shader, 1, size-1, fp))
    return "";

  fclose(fp);

  return shader;
  }


//gl related wrappers / code organization
void print_log(GLuint object)
  {
  GLint log_length = 0;
  if (glIsShader(object))
    {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }
  else if (glIsProgram(object))
    {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }
  else
    {
    fprintf(stderr, "printlog: not a shader or a program\n");
    return;
    }

  char* log = malloc(log_length);

  if (glIsShader(object))
    glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
      glGetProgramInfoLog(object, log_length, NULL, log);

  fprintf(stderr, "%s\n", log);
  free(log);
  }


GLuint create_shader(const char* filename, GLenum type)
  {
  void* source_mem = load_shader(filename);
  const GLchar* source = source_mem;

  if (!source)
    {
    fprintf(stderr, "error opening %s\n", filename);
    return 0;
    }

  GLuint res = glCreateShader(type);
  glShaderSource(res, 1, &source, NULL);
  free(source_mem);

  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {
    fprintf(stderr, "an error ocurred while compiling %s: \n", filename);
    print_log(res);
    glDeleteShader(res);
    return 0;
    }

  return res;
  }

int link_shader_program(unsigned int program)
  {
  int status;
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  return 0;
  }

