attribute vec3 aPosition;
attribute vec2 aTexture;

varying vec2 vTexture;

uniform mat4 MVP;
uniform vec3 MaterialAmbient;
uniform vec3 LightAmbient;

varying vec4 FinalColor;

void main()
{

    vec3 ambient = MaterialAmbient * LightAmbient;
    FinalColor = vec4(ambient, 1.0);
    gl_Position =  MVP * vec4(aPosition,1.0);

    vTexture = aTexture;
}