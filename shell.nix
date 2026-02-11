{ pkgs ? import <nixpkgs> { } }:

#THIS WORKS, DO NOT TOUCH IT

pkgs.mkShell
{
  packages = with pkgs;
    [
    glfw
    glew
    cglm
    #libGL#actually, do I really need to include this or is this included with glfw?? //this doesn't seem to contain glGenBuffers???
    #gl3w#freeglut not needed i think, just use GLFW + libGL
    #glew
    #cglm
    ];
}

