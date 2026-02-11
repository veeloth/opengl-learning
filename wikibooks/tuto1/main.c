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

int init_resources()
  {
  GLint compile_ok;
  GLint link_ok = GL_FALSE;
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char* vs_source = load_shader("vertex.glsl");
  const char* fs_source = load_shader("fragment.glsl");
  const char* attribute_name = "coord2d";

  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok != GL_TRUE)
    {
    printf("couldn't compile vertex shader\n");
    return 1;
    }

  glShaderSource(fs, 1, &fs_source, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok)
    {
    printf("couldn't compile fragment shader\n");
    return 1;
    }

  program = __glewCreateProgram();
  __glewAttachShader(program, vs);
  __glewAttachShader(program, fs);
  __glewLinkProgram(program);
  __glewGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok)
    {
    printf("couldn't link program\n");
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

void init()
  {
  GLenum glew_status;
  if (!glfwInit())
    {
    printf("couldn't initialize glfw\n");
    exit(1);
    }
  printf("%s\n", glGetString(GL_VERSION));

  if (!(window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL)))
    {
    glfwTerminate();
    exit(1);
    }

  glfwMakeContextCurrent(window);
  
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
