attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexture;


varying vec3 vColor;
uniform mat4 MVP;
//uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;
//uniform vec3 MaterialAmbient;
//uniform vec3 LightAmbient;


void main()
{
    vColor = aNormal;

    if(2==2){
        gl_Position = MVP * vec4(aPosition,1.0);
    }else{

        vec3 x = aNormal*vec3(aTexture,1.0);
        gl_Position = vec4(aPosition,1.0);
    }


}