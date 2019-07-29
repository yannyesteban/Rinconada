precision mediump float;

uniform sampler2D uTexture;
varying vec2 vTexture;


void main()
{

    vec4 texColor = texture2D(uTexture, vTexture);
    if(texColor.a < 0.1){
        discard;
    }

    gl_FragColor = texColor;

    //gl_FragColor = vec4(vTexture,1.0);

}