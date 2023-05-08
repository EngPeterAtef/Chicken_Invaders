#version 330

#define MAX_LIGHTS 16

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
};

uniform Light lights[MAX_LIGHTS];
uniform int light_count=0;

struct Sky{
    vec3 top,horizon,bottom;
};

uniform Sky sky;

//texture maps for each property check the slides for more info
struct Material{
    sampler2D albedo;
    sampler2D specular;
    sampler2D ambient_occlusion;
    sampler2D roughness;
    sampler2D emissive;    //the light ali 5arg mn al object nfso han3mlo bel texture da

};
/*
this function to calculate the ambient light from 3 directions because the color of the amber light differes
from the source of the light because the sky will give us blue ambient light and the ground will give us green
so we need to know from where we should take our ambient light
if the normal of the surface so we will take the color from the sky(Top)
if the normal of the surface is in the bottom so we will take the color from the ground(Bottom)
*/
vec3 compute_sky_light(vec3 normal){
    vec3 extreme=normal.y>0?sky.top:sky.bottom;//34an a3rf al normal bass le fo2 wla le t7t
    //mix= interpolate between 2 values (horizon and extreme) based on the normal.y * normal.y value (0 to 1)
    /*
    ^ extreme(y or -y)
    |        ^ normal
    |        |
    -------------------------> horizion
    */
    return mix(sky.horizon,extreme,normal.y*normal.y);
}

uniform Material material;

in Varyings{
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;//point in world space
}fs_in;

out vec4 frag_color;

float lambert(vec3 normal,vec3 world_to_light_direction){
    return max(0.,dot(normal,world_to_light_direction));
}

float phong(vec3 reflected,vec3 view,float shininess){
    return pow(max(0.,dot(reflected,view)),shininess);
}
void main(){
    vec3 view=normalize(fs_in.view);
    vec3 normal=normalize(fs_in.normal);
    
    vec3 material_diffuse=texture(material.albedo,fs_in.tex_coord).rgb;
    vec3 material_specular=texture(material.specular,fs_in.tex_coord).rgb;
    vec3 material_ambient=material_diffuse*texture(material.ambient_occlusion,fs_in.tex_coord).r;
    
    float material_roughness=texture(material.roughness,fs_in.tex_coord).r;
    //to tell that roughness is between 0.001 and 0.999 so that if it's larger than 0.999 it will be 0.999
    // and same for 0.001. we did that to avoid division by zero and to avoid shininess to be zero
    float material_shininess=(2./pow(clamp(material_roughness,.001,.999),4.))-2.;
    
    vec3 material_emissive=texture(material.emissive,fs_in.tex_coord).rgb;
    
    vec3 ambient_light=compute_sky_light(normal);
        
    int clamped_light_count=min(MAX_LIGHTS,light_count);
    
    vec3 world_to_light_dir;
    vec3 color=material_emissive+ambient_light*material_ambient;
    
    for(int i=0;i<clamped_light_count;i++){
        Light light=lights[i];
        // light.direction=normalize(light.direction);
        float attenuation=1;
        
        //in case of the direction light the vector from the point to the light source is the opposite of the light direction
        if(light.type==DIRECTIONAL){
            world_to_light_dir=-light.direction;
        }else{
            // in case of spot and point light we calculate the light direction as the vector from the point to the light position
            world_to_light_dir=light.position-fs_in.world;
            float d=length(world_to_light_dir);//magnitude
            world_to_light_dir/=d;//normalize
            //attenuation = 1/ ad^2+ad+a where a is light.attenuation
            attenuation=1./dot(light.attenuation,vec3(d*d,d,1.));
            
            if(light.type==SPOT){
                //in case of spot light there is a difference between the vector from the point to the light source and
                //the direction of the spot light itself
                //we calculate the angle betweem them to know the attenuation
                float angle=acos(dot(light.direction,-world_to_light_dir));
                //smooth step is like sigmoid function
                //outer angle(2a2l value)  inner(akbr value)  input(angle)
                attenuation*=smoothstep(light.cone_angles.y,light.cone_angles.x,angle);
            }
        }
        
        vec3 computed_diffuse=light.color*material_diffuse*lambert(normal,world_to_light_dir);
        
        vec3 reflected=reflect(-world_to_light_dir,normal);
        vec3 computed_specular=light.color*material_specular*phong(reflected,view,shininess);
        
        color+=(computed_diffuse+computed_specular)*attenuation;
        // vec3 direction_to_light = -light.direction;
        // if(light.type != DIRECTIONAL){
            //     direction_to_light = normalize(light.position - fs_in.world);
        // }
        
        // vec3 diffuse = light.diffuse * material_diffuse * max(0, dot(normal, direction_to_light));
        
        // vec3 reflected = normalize(reflect(-direction_to_light, normal));//Added normalization
        
        // vec3 specular = light.specular * material_specular * pow(max(0, dot(view, reflected)), material_shininess);
        
        // if(light.type != DIRECTIONAL){
            //     float d = distance(light.position, fs_in.world);
            //     attenuation /= dot(light.attenuation, vec3(d*d, d, 1));
            //     if(light.type == SPOT){
                //         float angle = acos(dot(-direction_to_light, light.direction));
                //         attenuation *= max(0,1-(1-cos(angle-light.cone_angles.y))/(1-cos(light.cone_angles.x)));
            //     }
        // }
        
        // frag_color.rgb += (diffuse + specular) * attenuation;
    }
    frag_color=vec4(color,1.);
    
}