precision mediump float;


varying vec3 vColor;
void main()
{
    if(2==2){
        gl_FragColor = vec4(vColor,1.0);
    }else{
        gl_FragColor = vec4(0.5,0.5,0.0,1.0);
    }
}
