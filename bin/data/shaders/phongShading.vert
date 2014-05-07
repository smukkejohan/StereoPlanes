#version 150

in vec4 position;
in vec3 normal;

flat out vec3 vertexNormalFlat;
flat out vec3 cameraSpacePositionFlat;

out vec3 vertexNormal;
out vec3 cameraSpacePosition;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

uniform NoisePoints
{
    int numberOfPoints;
    vec4 points [100];
} vertexNoise;

uniform float globalNoiseAmount;
uniform float time;


/*uniform NormalMatrix{
	mat3 matrix;
}normalMatrix;
*/

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	//vertexNormal = normalize(normalMatrix.matrix * normal);
    // this is a dirty hack!!
    vertexNormal = vec3(modelViewMatrix * vec4(normal,0.0));
    vertexNormalFlat = vec3(modelViewMatrix * vec4(normal,0.0));
    vec3 vertexOffset;
    /*
     vertexOffset.x = noise1(vec3(position.xy,time));
    vertexOffset.y = noise1(vec3(position.zy,time));
    vertexOffset.z = noise1(vec3(position.zx,time));
     */
    vertexOffset.x = rand(vec2(position.x,time));
    vertexOffset.y = rand(vec2(position.y,time));
    vertexOffset.z = rand(vec2(position.z,time));
    vertexOffset -= vec3(0.5,0.5,0.5);
    
    float accumNoise = 0.0;
    for (int i = 0; i < vertexNoise.numberOfPoints; i++) {
        vec4 noisePoint = vertexNoise.points[i];
        vec3 noisePosition = noisePoint.xyz;
        float noisePositionSize = noisePoint.w;
        if(noisePositionSize > 0.0){
            vec4 noiseDifference = vec4(noisePosition.xyz - (modelViewMatrix*position).xyz, 0.0);
            float noiseDistance = sqrt(dot(noiseDifference,noiseDifference));
            float noiseAmount = 1.0-(min(noisePositionSize, noiseDistance)/noisePositionSize);
            accumNoise+= noiseAmount;
        }
    }
    
    vertexOffset*=(accumNoise*globalNoiseAmount);
    cameraSpacePosition = (modelViewMatrix * position).xyz;
    cameraSpacePosition += vertexOffset;
    cameraSpacePositionFlat = (modelViewMatrix * position).xyz;
    cameraSpacePositionFlat += vertexOffset;
	gl_Position = modelViewProjectionMatrix * (position+vec4(vertexOffset,0.0));
}