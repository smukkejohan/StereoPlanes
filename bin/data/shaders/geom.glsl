#version 150

layout(lines) in;
layout(triangle_strip, max_vertices = 5) out;

//in vec3 vColor[]; // Output from vertex shader for each vertex

//out vec3 fColor; // Output to fragment shader

uniform float thickness;
uniform vec3 lightDir;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

void main() {
    
    //fColor = vColor[0];
    
    vec4 p0m = gl_in[0].gl_Position.xyzw;
	vec4 p1m = gl_in[1].gl_Position.xyzw;
	
    vec3 p0 = p0m.xyz;
    vec3 p1 = p1m.xyz;
    
	vec4 upm = vec4(1.0, 0.0, 0.0, 1.0); // * modelViewProjectionMatrix;	// arbitrary up vector
	
    vec3 up = upm.xyz;
    
	vec3 dir   = normalize(p1 - p0);			// normalized direction vector from p0 to p1
	vec3 right = normalize(cross(dir, up));	// right vector
	
    //vec3 norm = cross(right, dir);
	//float fColMult = abs(dot(norm, lightDir));
	//vec4 colMult = vec4(fColMult, fColMult, fColMult, 1.0);
	
	right *= thickness;
    
    gl_Position =  vec4(p0 - right, 1.0) * modelViewProjectionMatrix;
	//gl_FrontColor = gl_FrontColorIn[0] * colMult;
	EmitVertex();
	
	gl_Position =  vec4(p0 + right, 1.0) * modelViewProjectionMatrix;
	//gl_FrontColor = gl_FrontColorIn[0] * colMult;
	EmitVertex();
	
	gl_Position =  vec4(p1 - right, 1.0) * modelViewProjectionMatrix;
	//gl_FrontColor = gl_FrontColorIn[1] * colMult;
	EmitVertex();
    
	gl_Position =  vec4(p1 + right, 1.0) * modelViewProjectionMatrix;
	//gl_FrontColor = gl_FrontColorIn[1] * colMult;
	EmitVertex();
    
    
    //EndPrimitive();
}












/***********************************************
 Geometry shader to convert lines into triangle strips
 Memo Akten
 www.memo.tv

************************************************/

/*#version 150

uniform float thickness;
uniform vec3 lightDir;

uniform mat4 modelViewProjectionMatrix;

//in vec4 FrontColor[];
//out vec4 color;

void main() {
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	
	vec3 up = vec3(0, 0, 1);	// arbitrary up vector
	
	vec3 dir = normalize(p1 - p0);			// normalized direction vector from p0 to p1
	vec3 right = normalize(cross(dir, up));	// right vector
	vec3 norm = cross(right, dir);
	float fColMult = abs(dot(norm, lightDir));
	vec4 colMult = vec4(fColMult, fColMult, fColMult, 1.0);
	
	right *= thickness;
	
	gl_Position = modelViewProjectionMatrix * vec4(p0 - right, 1.0);
	//color = FrontColor[0] * colMult;
	EmitVertex();
	
	gl_Position = modelViewProjectionMatrix * vec4(p0 + right, 1.0);
	//color = FrontColor[0] * colMult;
	EmitVertex();
	
	gl_Position = modelViewProjectionMatrix * vec4(p1 - right, 1.0);
	//color = FrontColor[1] * colMult;
	EmitVertex();

	gl_Position = modelViewProjectionMatrix * vec4(p1 + right, 1.0);
	//color = FrontColor[1] * colMult;
	EmitVertex();

}*/

/*#version 420

layout(triangles) in;
layout (triangle_strip, max_vertices=6) out;

layout (std140) uniform Matrices {
    mat4 projModelViewMatrix;
    mat3 normalMatrix;
};

in VertexData {
    vec2 texCoord;
    vec3 normal;
} VertexIn[];

out VertexData {
    vec2 texCoord;
    vec3 normal;
} VertexOut;

void main()
{
    for(int i = 0; i < gl_in.length(); i++)
    {
        // copy attributes
        gl_Position = projModelViewMatrix * gl_in[i].gl_Position;
        VertexOut.normal = normalize(normalMatrix * VertexIn[i].normal);
        VertexOut.texCoord = VertexIn[i].texCoord;
        
        // done with the vertex
        EmitVertex();
    }
    EndPrimitive();
    
    for(int i = 0; i < gl_in.length(); i++)
    {
        // copy attributes and displace copy
        gl_Position = projModelViewMatrix * (gl_in[i].gl_Position + vec4(20.0, 0.0, 0.0, 0.0));
        VertexOut.normal = normalize(normalMatrix * VertexIn[i].normal);
        VertexOut.texCoord = VertexIn[i].texCoord;
        
        // done with the vertex
        EmitVertex();
    }
    EndPrimitive();
}
*/
