#version 150

#define MARCH_EPS 0.01
#define MARCH_STEP 0.8
#define FAR 8.0

uniform float time;

out vec4 fragColor;

uniform vec2 res;

mat2 rot2 (float angle){
	float ca = cos(angle), sa = sin(angle);
	return mat2(ca, sa, -sa, ca);
}

float map (vec3 p) {
	float ry =  (p.x * sin(time * 2.82) * 0.5 + /*sin(time * 2.75) * 0.8 +*/ time);
	p.xz *= rot2(p.y * cos(time * 2.65) * 0.5 + /*cos(time * 2.91) * 0.8 +*/ time);
	p.yx *= rot2(ry);

	vec3 p4 = p*p*p;
	return pow(dot(p4, p4), 0.1) - 0.7;
}

float trace(vec3 ro, vec3 rd, inout float close){
	float d, t = 0.0;

	for (int i = 0; i < 120; i++){
		d = map(ro + rd * t);
		if (abs(d) < MARCH_EPS || t > FAR) break;
		close += d;// min(close, d);
		t += d * MARCH_STEP;
	}

	return (t+1.0) * step(d, MARCH_EPS) - 1.0;
}

void light (inout vec3 lig, float dp, vec3 col){
	dp = max(0.0, dp);
	lig += (dp*0.3 + dp*dp*dp*dp*0.150) * col;
	lig += (dp*0.1 + dp*dp*dp*dp*0.3);
}

void main(void)
{
	vec2 uv = (2.0 * gl_FragCoord.xy - res.xy) / min(res.x, res.y);

	float cost = cos(time);
	float sint = sin(time);


	vec3 ro = vec3(.0, .0, 3.0);//3.0 * vec3(sint, 0.0, cost);
	vec3 rd = normalize(vec3(uv, -1.5));

	//rd.xz *= mat2(cost, sint, -sint, cost);

	float close = 0.0;
	float t = trace(ro, rd, close);
	if (t > 0.0){
		vec3 end = ro + rd * t;

		const vec2 eps = 0.02 * vec2(-1.0, 1.0);
		vec3 nml = normalize(eps.xxx * map(vec3(end + eps.xxx)) +
		                     eps.yyx * map(vec3(end + eps.yyx)) +
		                     eps.xyy * map(vec3(end + eps.xyy)) +
		                     eps.yxy * map(vec3(end + eps.yxy)));

		//float ao = (map(end + nml * 0.05) - map(end)) * 20.0;
		float dp;
		vec3 lig = vec3(0.0);

		//nml.xz *= rot2(time);
		//nml.yz *= rot2(time*0.314159);

		//light(lig, +nml.x, vec3(1., 1., 0.));
		//light(lig, -nml.x, vec3(1., 0., 1.));
		//light(lig, +nml.y, vec3(0., 1., 1.));
		//light(lig, -nml.y, vec3(0., 0., 1.));
		//light(lig, +nml.z, vec3(0., 1., 0.));
		//light(lig, -nml.z, vec3(1., 0., 0.));

		//White light
		light(lig, +nml.x, vec3(1., .5, .5));
		light(lig, +nml.y, vec3(.5, .5, 1.));
		light(lig, +nml.z, vec3(1., 1., 1.));
		light(lig, -nml.x, vec3(1., .5, .5));
		light(lig, -nml.y, vec3(.5, .5, 1.));
		light(lig, -nml.z, vec3(1., 1., 1.));

		fragColor = vec4(lig, 1.0);
	} else {
		float aur = close*0.01;
		aur = aur*5.0*dot(uv, uv);
		fragColor = vec4(aur);
	}
}
