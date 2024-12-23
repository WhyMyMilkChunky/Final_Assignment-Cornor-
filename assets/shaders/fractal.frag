#version 460

out vec4 FragColor;
in vec2 uv;

uniform float u_time;
uniform vec2 u_resolution;

//https://iquilezles.org/articles/palettes/
vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

//https://www.shadertoy.com/view/mtyGWy
void main()
{
    vec2 ndc = (gl_FragCoord.xy * 2.0 - u_resolution) / u_resolution.y;
    vec2 ndc0 = ndc;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 4.0; i++) {
        ndc = fract(ndc * 1.5) - 0.5;
    
        float d = length(ndc) * exp(-length(ndc0));
    
        vec3 col = palette(length(ndc0) + i * 0.4 + u_time * 0.4);
    
        d = sin(d * 8.0 + u_time) / 8.0;
        d = abs(d);
    
        d = pow(0.01 / d, 1.2);
    
        finalColor += col * d;
    }
    
    FragColor = vec4(finalColor, 1.0);
}