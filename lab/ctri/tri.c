//#include "GL/glext.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
//#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLfloat triangle_vertices[] =
  {
  0.0, 0.8,
 -0.8,-0.8,
  0.8,-0.8,
  };

GLint attribute_coord2d;
GLuint program;

char* load_shader(const char* name)
  {
  FILE* fp;
  long size = 0;
  char* shader;

  //read for size
  if(!(fp = fopen(name, "rb")))
    {
    return "";
    }
  fseek(fp, 0, SEEK_END);
  size = ftell(fp)+1;
  fclose(fp);

  //read actual file
  fp = fopen(name, "r");
  shader = memset(malloc(size), '\0', size);
  fread(shader, 1, size-1, fp);
  fclose(fp);

  return shader;
  }

int init_resources()
  {
  GLint compile_ok;
  GLint link_ok = GL_FALSE;

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  const char* vs_source = load_shader("vertex.glsl");
  printf("%s\n", vs_source);
  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok != GL_TRUE)
    {
    printf("couldn't compile vertex shader\n");
    return 1;
    }

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fs_source =
    "#version 120\n"
    "void main(void) {  "
    "  gl_FragColor[0] = 0.0;  "
    "  gl_FragColor[1] = 0.0;  "
    "  gl_FragColor[2] = 1.0;  "
    "}";
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

  const char* attribute_name = "coord2d";
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
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
    {
    printf("glew init error: %s\n", glewGetErrorString(glew_status));
    exit(-1);
    }
  if (init_resources())
    {
    printf("couldn't initialize resources\n");
    exit(-1);
    }
  }

void terminate()
  {
  free_resources();
  }

void draw()
  {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1.0f, 0.4f, 0.4f, 1.0f);
  //glRectf(-0.75f,0.75f, 0.75f, -0.75f);

  __glewUseProgram(program);
  __glewEnableVertexAttribArray(attribute_coord2d);

  __glewVertexAttribPointer(
    attribute_coord2d,
    2,
    GL_FLOAT,
    GL_FALSE,
    0,
    triangle_vertices
    );

  glDrawArrays(GL_TRIANGLES, 0, 3);

  __glewDisableVertexAttribArray(attribute_coord2d);
  }

int main(void)
  {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
    {
    glfwTerminate();
    return -1;
    }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  printf("%s\n", glGetString(GL_VERSION));

  init();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
    /* Render here */
    draw();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
    }

  glfwTerminate();
  return 0;
  }
