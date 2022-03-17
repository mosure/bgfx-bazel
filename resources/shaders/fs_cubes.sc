#include <bgfx_shader.sh>

uniform vec4 u_times;
#define u_delta_seconds u_times.x
#define u_time u_times.y


mat2 rotate2d(float _angle){
    return mtxFromRows(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

void main()
{
    vec2 st = gl_FragCoord.xy * u_viewTexel.xy;

    st -= vec2_splat(0.5);
    st = mul(st, rotate2d(u_time));
    st += vec2_splat(0.5);

    gl_FragColor = vec4(st.x, st.y, abs(sin(u_time)), 1.0);
}
