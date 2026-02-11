#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLfloat triangle_vertices[] =
  {
  0.0,-0.8,
  0.8, 0.8,
 -0.8, 0.8,
  };

GLint attribute_coord2d;
GLuint program;
GLFWwindow* window;


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
  else if (__glewIsProgram(object))
    {
    __glewGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    }
  else
    {
    fprintf(stderr, "printlog: not a shader or a program\n");
    return;
    }

  char* log = malloc(log_length);

  if (glIsShader(object))
    __glewGetShaderInfoLog(object, log_length, NULL, log);
  else if (__glewIsProgram(object))
      __glewGetProgramInfoLog(object, log_length, NULL, log);

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
    __glewDeleteShader(res);
    return 0;
    }

  return res;
  }


int init_resources()
  {
  GLint compile_ok;
  GLint link_ok = GL_FALSE;
  GLuint vs;
  GLuint fs;
  if (!(vs = create_shader("vertex.glsl", GL_VERTEX_SHADER)))
    return 0;
  if (!(fs = create_shader("fragment.glsl", GL_FRAGMENT_SHADER)))
    return 0;
  const char* attribute_name = "coord2d";

  program = __glewCreateProgram();
  __glewAttachShader(program, vs);
  __glewAttachShader(program, fs);
  __glewLinkProgram(program);
  __glewGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok)
    {
    printf("couldn't link program\n");
    print_log(program);
    return 1;
    }

  attribute_coord2d = __glewGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1)
    {
    printf("couldn't bind attribute\n");
    return 1;
    }
  return 0;
  }

void free_resources()
  {
  }


//main flow declarations
void init()
  {
  GLenum glew_status;
  if (!glfwInit())
    {
    printf("couldn't initialize glfw\n");
    exit(1);
    }

  if (!(window = glfwCreateWindow(1366, 768, "Hello World", NULL, NULL)))
    {
    glfwTerminate();
    exit(1);
    }

  glfwMakeContextCurrent(window);
  printf("%s\n", glGetString(GL_VERSION));//print version
  
  if ((glew_status = glewInit()))
    {
    printf("glew init error: %s\n", glewGetErrorString(glew_status));
    exit(1);
    }

  if (init_resources())
    {
    printf("couldn't initialize resources\n");
    exit(1);
    }
  }


void terminate()
  {
  free_resources();
  glfwTerminate();
  }


void draw()
  {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.f, 0.f, 0.f, 1.0f);

  __glewUseProgram(program);
  __glewEnableVertexAttribArray(attribute_coord2d);
  __glewVertexAttribPointer
      (
      attribute_coord2d,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      triangle_vertices
      );

  glDrawArrays(GL_TRIANGLES, 0, 3);

  __glewDisableVertexAttribArray(attribute_coord2d);

  glfwSwapBuffers(window);
  }


void update()
  {
  glfwPollEvents();
  }



//
int main(void)
  {
  init();

  while (!glfwWindowShouldClose(window))
    {
    draw();
    update();
    }

  terminate();
  return 0;
  }
