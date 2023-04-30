#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varyings{
    vec3 color;
}vs_out;

uniform vec2 scale = vec2(1.0, 1.0);
uniform vec2 translation = vec2(0.0, 0.0);

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

// Declare an input layout for the vertex shader

void main(){
    vec3 position[3]=vec3[3](
        vec3(-0.5,-0.5,0.0),//0
        vec3(0.5,-0.5,0.0),//1
        vec3(0.0,0.5,0.0)//2
    );
    // Apply the transformation to each vertex
    vec3 transformed = vec3(scale,1.0) * position[gl_VertexID] + vec3(translation,0.0);

    // Assign the transformed vertex to gl_Position
    gl_Position = vec4(transformed, 1.0);

    // Assign a different color to each vertex based on its position
    if (position[gl_VertexID].x < 0.0) {
        vs_out.color = vec3(1.0,0.0,0.0);
    } else if (position[gl_VertexID].y < 0.0) {
        vs_out.color = vec3(0.0,1.0,0.0);
    } else {
        vs_out.color = vec3(0.0,0.0,1.0);
    }

    // if(gl_VertexID==0){
    //     vs_out.color = vec3(1.0,0.0,0.0);
    // }
    // else if(gl_VertexID==1){
    //     vs_out.color = vec3(0.0,1.0,0.0);
    // }
    // else if(gl_VertexID==2){
    //     vs_out.color = vec3(0.0,0.0,1.0);
    // }

}