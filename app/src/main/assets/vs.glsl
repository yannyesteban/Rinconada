attribute vec3 vPosition;
attribute vec3 v_color;
uniform mat4 MVP;
varying vec3 f_color;
void main()
{
    //gl_Position = vec4(vPosition,1);
    //float y =  vPosition.y/2;
    gl_Position =  MVP * vec4(vPosition,1.0);
   // gl_Position =  MVP * vec4(vPosition.x, y ,vPosition.z ,1.0);
    f_color = v_color;
}