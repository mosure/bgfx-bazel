#include <bgfx_shader.sh>

uniform vec4 u_times;
#define u_delta_seconds u_times.x
#define u_time u_times.y

float circle(in vec2 _st, in float _radius){
   vec2 l = _st-vec2(0.5);
   return 1.-smoothstep(_radius-(_radius*0.01),
                        _radius+(_radius*0.01),
                        dot(l,l)*4.0);
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

void main()
{
   vec2 st = gl_FragCoord.xy * u_viewTexel.xy;
   vec3 color = vec3(0.0);

   st *= 3.0;      // Scale up the space by 3
   st = fract(st); // Wrap around 1.0

   color = vec3(st,0.0);
   color = vec3(circle(st,0.5));

    gl_FragColor = vec4(color,1.0);
}
