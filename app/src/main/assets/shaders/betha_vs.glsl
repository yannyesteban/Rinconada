attribute vec3 aPosition;
attribute vec2 aTexture;
attribute vec3 aNormal;
varying vec2 vTexture;

uniform mat4 MVP;
void main()
{
    if(9==8){
        gl_Position =  MVP * vec4(aPosition*aNormal,1.0) ;
    }else{
        gl_Position =  MVP * vec4(aPosition,1.0);
    }


    vTexture = aTexture;
}