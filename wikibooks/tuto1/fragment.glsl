#version 120
void main(void) {
  gl_FragColor[0] = 1.;
  gl_FragColor[1] = gl_FragCoord.x/512;
  gl_FragColor[2] = gl_FragCoord.y/512;
}
