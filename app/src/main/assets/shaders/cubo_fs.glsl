#version 120

precision mediump float;
uniform sampler2D uTexture;
varying vec2 vTexture;
varying vec3 f_color;



void main()
{

    //gl_FragColor = vec4(0.9, 0.1, 0.0, 1.0);
    //gl_FragColor = vec4(f_color.r, f_color.g, f_color.b, 1.0);
    gl_FragColor = texture2D(uTexture, vTexture);

}