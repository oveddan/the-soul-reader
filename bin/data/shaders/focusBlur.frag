// fragment shader

#version 150

uniform sampler2DRect outputGraphic;

in vec2 texCoordVarying;
uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform float u_focusDuration;

out vec4 outputColor;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p){
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.0-2.0*u);
    
    float res = mix(
                    mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
                    mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
    return res*res;
}

mat2 rotate2d(float angle){
    return mat2(cos(angle),-sin(angle),
                sin(angle),cos(angle));
}

void main() {
    
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.y *= u_resolution.y/u_resolution.x;
    
    //float effectStrength = smoothstep(1., 0., length(gl_FragCoord.xy, u_mouse));
    
    vec2 mousePct = u_mouse / u_resolution.xy;
    mousePct.y *= u_resolution.y/u_resolution.x;
    float cursorStrength = smoothstep(.05 + u_focusDuration / 10000., 0., length(mousePct - st));
    
//    cursorStrength *= noise(sin(st * 4.));
    
    float n = noise(sin(texCoordVarying / 10.) + u_time);
    
    vec2 pos = texCoordVarying;
//    vec2 warp = n;
    
    vec3 src = texture(outputGraphic, pos + n * 20. * cursorStrength).rgb;
    
//    float warpN = noise(sin(texCoordVarying / 5.) + u_time);
//    vec3 color = mix(src, 1.-src, smoothstep(.05, 0., length(mousePct - st)) * warpN);
    
    vec2 xy = mousePct - gl_FragCoord.xy;
    float theta = tan(xy.y / xy.x);
    
    float radialNoise = noise(vec2(xy.x, xy.y) / 20.);
    
     vec3 color = mix(src, vec3(1.), cursorStrength * radialNoise * 2.);
    

    
//    if (st.x >= .5) {
//        outputColor = vec4(.5, .5, .5, 1.);
//    } else {
//        outputColor = vec4(src,1.0);
//    }
    
    outputColor = vec4(color, 1.);
}
