precision mediump float;

uniform sampler2D uTexture;
varying vec2 vTexture;


void main()
{

    //gl_FragColor = vec4(0.9, 0.1, 0.0, 1.0);
   //gl_FragColor = texture2D(uTexture, vTexture);

    vec4 texColor = texture2D(uTexture, vTexture);
        if(texColor.a < 0.1)
            discard;
    gl_FragColor = texColor;
}