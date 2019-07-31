attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexture;

varying vec2 vTexture;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
//uniform vec3 MaterialAmbient;
//uniform vec3 LightAmbient;



uniform vec3 MaterialDiffuse;
uniform vec3 LightDiffuse;
uniform vec3 LightPosition;

varying vec4 FinalColor;

void main()
{

    // Calculate normal, eye coord and light vector
    vec3 nNormal = normalize(NormalMatrix * aNormal);
    vec3 eyeCoord = vec3( ModelViewMatrix * vec4(aPosition, 1.0) );
    vec3 nLight = normalize (LightPosition - eyeCoord);

    // Calculate cosine Normal and light vector
    float cosAngle = max( 0.0, dot( nNormal, nLight ));
    vec3 diffuse = MaterialDiffuse * LightDiffuse;
    FinalColor = vec4(cosAngle * diffuse, 1);
    gl_Position = MVP * vec4(aPosition,1.0);

    vTexture = aTexture;
}