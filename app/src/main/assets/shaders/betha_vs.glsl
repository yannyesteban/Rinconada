attribute vec3 aPosition;
attribute vec2 aTexture;

varying vec2 vTexture;

uniform mat4 MVP;
void main()
{

    gl_Position =  MVP * vec4(aPosition,1.0);

    vTexture = aTexture;
}