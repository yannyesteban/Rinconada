precision mediump float;

uniform sampler2D uTexture;
varying vec2 vTexture;
varying vec3 vColor;
varying vec4 FinalColor;


void main()
{

    vec4 texColor = texture2D(uTexture, vTexture);
    if(texColor.a < 0.1){
        discard;
    }
    if(8==8){
        gl_FragColor = texColor*FinalColor;
    }else if(7==7){
        //gl_FragColor = FinalColor*texColor;
        gl_FragColor = FinalColor;//*vec4(vColor, 1.0);
        //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
    }else{

        gl_FragColor = vec4(vColor, 1.0);
    }


    //gl_FragColor = texColor;

}