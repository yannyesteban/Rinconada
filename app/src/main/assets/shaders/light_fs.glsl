precision mediump float;

uniform sampler2D uTexture;
varying vec2 vTexture;
varying vec4 FinalColor;


void main()
{

    vec4 texColor = texture2D(uTexture, vTexture);
    if(texColor.a < 0.1){
        discard;
    }
    if(8==9){
        gl_FragColor = texColor;
    }else{
        //gl_FragColor = FinalColor*texColor;
        gl_FragColor = FinalColor*texColor;
    }


    //gl_FragColor = vec4(vTexture,1.0);

}