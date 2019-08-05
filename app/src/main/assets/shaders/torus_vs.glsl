attribute vec3 aPosition;


uniform mat4 MVP;
//uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;
//uniform vec3 MaterialAmbient;
//uniform vec3 LightAmbient;


void main()
{


    if(2==2){
        gl_Position = MVP * vec4(aPosition,1.0);
    }else{
        gl_Position = vec4(aPosition,1.0);
    }


}