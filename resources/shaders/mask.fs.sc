#include <bgfx_shader.sh>


SAMPLER2D(s_dog, 0);
SAMPLER2D(s_dog_mask, 1);

uniform vec4 u_times;
#define u_delta_seconds u_times.x
#define u_time u_times.y


void main()
{
    vec2 st = gl_FragCoord.xy * u_viewTexel.xy;

    vec3 dog = texture2D(s_dog, st).rgb;
    float dog_mask = texture2D(s_dog_mask, st).r;

    gl_FragColor = vec4(dog * dog_mask, 1.0);
}
