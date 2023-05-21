#version 330
uniform sampler2D tex;
in  vec2  tex_coord;
out vec4  frag_color;
// texture coordinates offsets for a 3x3 kernel
uniform vec2 offsets[9] = vec2[](
    vec2(-1.0f / 200.0f, 1.0f / 200.0f),    // top-left
    vec2( 0.0f, 1.0f / 200.0f),             // top-center
    vec2( 1.0f / 200.0f, 1.0f / 200.0f),    // top-right
    vec2(-1.0f / 200.0f, 0.0f),             // center-left
    vec2( 0.0f, 0.0f),                      // center-center
    vec2( 1.0f / 200.0f, 0.0f),             // center - right
    vec2(-1.0f / 200.0f, -1.0f / 200.0f),   // bottom-left
    vec2(0.0f, -1.0f / 200.0f),             // bottom-center
    vec2(1.0f / 200.0f, -1.0f / 200.0f)     // bottom-right    
);

// Edge detection kernel
uniform int edge_kernel[9] = int[](
    1, 1, 1,
    1,-8, 1,
    1, 1, 1
);
void main()
{
    vec3 sample[9];
    // Sample the texture at each offset
    for(int i = 0; i < 9; i++)
        sample[i] = vec3(texture(tex, tex_coord.st + offsets[i]));
    // Apply the edge detection kernel by convolution:
    // Multiply each sample by its corresponding kernel value and accumulating the result in frag_color
    for(int i = 0; i < 9; i++)
        frag_color += vec4(sample[i] * edge_kernel[i], 0.0f);
    frag_color.a = 1.0f;
}