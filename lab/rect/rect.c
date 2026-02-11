#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

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

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.4f, 0.4f, 1.0f);
    glRectf(-0.75f,0.75f, 0.75f, -0.75f);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
    }

  glfwTerminate();
  return 0;
  }
