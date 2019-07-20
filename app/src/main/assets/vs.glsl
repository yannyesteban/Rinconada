attribute vec4 vPosition;
attribute vec3 v_color;
varying vec3 f_color;
void main()
{
    gl_Position = vPosition;
    f_color = v_color;
}