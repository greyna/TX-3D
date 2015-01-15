#version 400

//in
in vec3 position_eye, normal_eye; // position and normal in eye space
in vec2 text_coord;

//uniform
uniform mat4 view;
uniform vec3 light_position, Ls, Ld, La; // point light
uniform sampler2D basic_texture;

//out
out vec4 frag_colour;

//constant directionnal light
//vec3 Ld_directional = vec3(0.5, 0.8660254, 0.0); // inverse direction as necessary
//vec3 Ld_directional = vec3(0.7071067, 0.7071067, 0.0); // inverse direction as necessary
vec3 Ld_directional = vec3(0.8660254, 0.5, 0.0); // inverse direction as necessary
vec3 Lda = vec3(0.3, 0.3, 0.3);
vec3 Ldd = vec3(0.7, 0.7, 0.7);
vec3 Lds = vec3(1.0, 1.0, 1.0);

// surface reflectance
vec3 Ks = vec3 (0.5, 0.5, 0.5); // half reflect specular light
//vec3 Kd = vec3 (1.0, 0.5, 0.0); // orange diffuse surface reflectance
vec3 Kd = vec3 (1.0, 1.0, 1.0); // fully diffuse texture color
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 80.0; // specular 'power'


void main() {
	// raise light position to eye space
	vec3 light_position_eye = vec3 (view * vec4 (light_position, 1.0));
	vec3 distance_to_light_eye = light_position_eye - position_eye;
	vec3 direction_to_light_eye = normalize (distance_to_light_eye);

	vec3 Ld_directional_eye = vec3(view * vec4(Ld_directional, 0.0));


	float factor = 1.0;
	if (length(distance_to_light_eye) > 10.0) {
		// if we are too far away from the point light, ignore the point light
		factor = 0.0;
	}


	// ambient intensity
	vec3 Ia = La * Ka * factor;

	vec3 Ida = Lda * Ka;


	// diffuse intensity
	float dot_prod = dot (direction_to_light_eye, normal_eye);
	dot_prod = max (dot_prod, 0.0);
	vec3 Id = Ld * Kd * dot_prod * factor; // final diffuse intensity

	dot_prod = dot (Ld_directional_eye, normal_eye);
	dot_prod = max (dot_prod, 0.0);
	vec3 Idd = Ldd * Kd * dot_prod;
	

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
	vec3 Is = Ls * Ks * specular_factor * factor; // final specular intensity

	half_way_eye = normalize (surface_to_viewer_eye + Ld_directional_eye);
	dot_prod_specular = max (dot (half_way_eye, normal_eye), 0.0);
	specular_factor = pow (dot_prod_specular, specular_exponent);
	vec3 Ids = Lds * Ks * specular_factor;
	

	// Tests :
	//frag_colour = vec4 (Is + Id + Ia, 1.0); // debug to show light
	//frag_colour = vec4(text_coord, 0.0, 1.0); // debug to show text_coord : (bot-left black ; top-right yellow ; right red ; top green)
	//frag_colour = vec4(0.2,0.0,0.0,1.0); // debug vertices to show constant color
	//frag_colour = vec4(normal_eye, 1.0); // debug to show normals
	
	// Final color :
	vec4 texel = texture (basic_texture, text_coord);
	frag_colour = vec4 ( texel.xyz * (Is + Id + Ia + Ids + Idd + Ida), 1.0);
}
