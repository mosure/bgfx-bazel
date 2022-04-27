#include <bgfx_shader.sh>

uniform vec4 u_times;
#define u_delta_seconds u_times.x
#define u_time u_times.y


const float PI = 3.1415926535897932384626433832795;

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

vec2 movingTiles(vec2 _st, float _zoom, float _speed){
    _st *= _zoom;
    float time = u_time*_speed;
    if( fract(time)>0.5 ){
        if (fract( _st.y * 0.5) > 0.5){
            _st.x += fract(time)*2.0;
        } else {
            _st.x -= fract(time)*2.0;
        }
    } else {
        if (fract( _st.x * 0.5) > 0.5){
            _st.y += fract(time)*2.0;
        } else {
            _st.y -= fract(time)*2.0;
        }
    }
    return fract(_st);
}

float circle(vec2 _st, float _radius){
    vec2 pos = vec2(0.5)-_st;
    return smoothstep(1.0-_radius,1.0-_radius+_radius*0.2,1.-dot(pos,pos)*3.14);
}

void main() {
    vec2 st = gl_FragCoord.xy * u_viewTexel.xy;
    st.x *= u_viewTexel.y / u_viewTexel.x;

    st -= vec2(0.5);
    st = mul(st, rotate2d(u_time / 10.0));
    st += vec2(0.5);

    st = movingTiles(st, 15.0, 0.5);

    float circle_mask = circle(st, 0.3);

    vec3 color = vec3(circle_mask * st.x, 1.0 - circle_mask, abs(sin(u_time)));

    gl_FragColor = vec4(color, 1.0);
}
