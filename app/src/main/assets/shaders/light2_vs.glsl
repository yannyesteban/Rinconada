attribute vec3 aPosition;
attribute vec3 aColor;
attribute vec2 aTexture;
attribute vec3 aNormal;

varying vec3 vColor;
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
    if(2==2){
        // Calculate normal, eye coord and light vector
        vec3 nNormal = normalize(NormalMatrix * aNormal);
        vec3 eyeCoord = vec3(ModelViewMatrix * vec4(aPosition, 1.0));
        vec3 nLight = normalize (LightPosition - eyeCoord);

        // Calculate cosine Normal and light vector
        float cosAngle = max(0.0, dot(nNormal, nLight));
        vec3 diffuse = MaterialDiffuse * LightDiffuse;

        vec3 color = cosAngle * diffuse;
        FinalColor = vec4(color, 1);
        //FinalColor = vec4(diffuse,1);
    }else{
        FinalColor = vec4(1.0,0.0,1.0,1.0);
    }

    gl_Position = MVP * vec4(aPosition,1.0);
    vColor = aColor;
    vTexture = aTexture;
}