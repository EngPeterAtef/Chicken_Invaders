#version 330

uniform vec3 eye;//camera position
uniform mat4 VP;
uniform mat4 M;
uniform mat4 M_IT;

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 tex_coord;
layout(location=3) in vec3 normal;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

void main(){
    vec3 world = (M * vec4(position, 1.0)).xyz;//the position of the vertex in world space
    gl_Position = VP * vec4(world, 1.0);//from world to view space
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    // we need to normalize the normal to calculate the diffuse in the fragment shader
    //also the normal transformed using ((M)^-1).T
    vs_out.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);
    //direction from the vertex to the camera 34an na 3aiz al vector ali ma4i mno lel camera
    vs_out.view = eye - world;
    // send the position of the point in the world space
    vs_out.world = world;
}