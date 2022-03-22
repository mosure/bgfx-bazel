#include <bgfx_shader.sh>


SAMPLER2D(s_logo, 0);

uniform vec4 u_times;
#define u_delta_seconds u_times.x
#define u_time u_times.y


mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

void main()
{
    vec2 st = gl_FragCoord.xy * u_viewTexel.xy;

    st = (st - 0.5) * 2 + 0.5;

    //st *= 10.0;              // Scale up the space by 3
    //st = fract(st);         // Wrap around 1.0

    vec4 logo = texture2D(s_logo, st);

    vec3 background = vec3(0.133, 0.149, 0.161);
    vec3 color = logo.rgb * logo.a + background * (1.0 - logo.a);

    gl_FragColor = vec4(color, 1.0);
}
