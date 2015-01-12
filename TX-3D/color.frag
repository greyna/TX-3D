#version 400

//in
in vec3 position_eye, normal_eye; // position and normal in eye space
in vec2 text_coord;

//uniform
uniform mat4 view;
uniform vec3 light_position, Ls, Ld, La;
uniform sampler2D basic_texture;

//out
out vec4 frag_colour;

// surface reflectance
vec3 Ks = vec3 (0.5, 0.5, 0.5); // half reflect specular light
//vec3 Kd = vec3 (1.0, 0.5, 0.0); // orange diffuse surface reflectance
vec3 Kd = vec3 (1.0, 1.0, 1.0); // fully diffuse texture color
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 80.0; // specular 'power'


void main() {
	// ambient intensity
	vec3 Ia = La * Ka;

	// diffuse intensity
	// raise light position to eye space
	vec3 light_position_eye = vec3 (view * vec4 (light_position, 1.0));
	vec3 distance_to_light_eye = light_position_eye - position_eye;
	vec3 direction_to_light_eye = normalize (distance_to_light_eye);
	float dot_prod = dot (direction_to_light_eye, normal_eye);
	dot_prod = max (dot_prod, 0.0);
	vec3 Id = Ld * Kd * dot_prod; // final diffuse intensity
	
	// specular intensity
	vec3 surface_to_viewer_eye = normalize (-position_eye);
	
	//vec3 reflection_eye = reflect (-direction_to_light_eye, normal_eye);
	//float dot_prod_specular = dot (reflection_eye, surface_to_viewer_eye);
	//dot_prod_specular = max (dot_prod_specular, 0.0);
	//float specular_factor = pow (dot_prod_specular, specular_exponent);
	
	// blinn
	vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
	float dot_prod_specular = max (dot (half_way_eye, normal_eye), 0.0);
	float specular_factor = pow (dot_prod_specular, specular_exponent);
	
	vec3 Is = Ls * Ks * specular_factor; // final specular intensity
	
	// Tests :
	//frag_colour = vec4 (Is + Id + Ia, 1.0); // debug to show light
	//frag_colour = vec4(text_coord, 0.0, 1.0); // debug to show text_coord : (bot-left black ; top-right yellow ; right red ; top green)
	//frag_colour = vec4(0.2,0.0,0.0,1.0); // debug vertices to show constant color
	//frag_colour = vec4(normal_eye, 1.0); // debug to show normals
	
	vec4 texel = texture (basic_texture, text_coord);
	frag_colour = vec4 ( texel.xyz * (Is + Id + Ia), 1.0);
}
