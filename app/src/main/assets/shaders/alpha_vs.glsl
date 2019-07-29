attribute vec3 aPosition;
attribute vec3 aColor;

varying vec3 vColor;
uniform mat4 MVP;
void main()
{
    vColor = aColor;
    gl_Position =  MVP*vec4(aPosition,1.0);


}