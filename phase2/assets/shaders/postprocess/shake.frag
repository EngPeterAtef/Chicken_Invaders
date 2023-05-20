#version 330
uniform sampler2D tex;
in  vec2  tex_coord;
out vec4  frag_color;

uniform vec2 offsets[9] = vec2[](
    vec2( -1.0f / 200.0f,  1.0f / 200.0f  ),  // top-left
    vec2(  0.0f,    1.0f / 200.0f  ),  // top-center
    vec2(  1.0f / 200.0f,  1.0f / 200.0f  ),  // top-right
    vec2( -1.0f / 200.0f,  0.0f    ),  // center-left
    vec2(  0.0f,    0.0f    ),  // center-center
    vec2(  1.0f / 200.0f,  0.0f    ),  // center - right
    vec2( -1.0f / 200.0f, -1.0f / 200.0f  ),  // bottom-left
    vec2(  0.0f,   -1.0f / 200.0f  ),  // bottom-center
    vec2(  1.0f / 200.0f, -1.0f / 200.0f  )   // bottom-right    
);

uniform float blur_kernel[9] = float[](              // Sharpening filter with some dimming (-0.3)
        1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f,
        1.0f / 9.0f,  1.0f / 9.0f, 1.0f / 9.0f,
        1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f
    );

void main()
{
    vec3 sample[9];
    for(int i = 0; i < 9; i++)
        sample[i] = vec3(texture(tex, tex_coord.st + offsets[i]));

    for(int i = 0; i < 9; i++)
        frag_color += vec4(sample[i] * blur_kernel[i], 0.0f);
    frag_color.a = 1.0f;
}