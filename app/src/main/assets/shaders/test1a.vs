attribute vec3 aPosition;
attribute vec2 aTexture;

varying vec2 vTexture;

uniform mat4 MVP;
void main()
{

    //gl_Position = vec4(aPosition,1);
   gl_Position =  MVP * vec4(aPosition,1.0);
    //gl_Position = vec4(aPosition,1);
    vTexture = aTexture;
}