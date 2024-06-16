#version 440 core

uniform vec2 windowSize;
uniform sampler2D textureSampler;

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;

out vec4 fragColor;

uniform float opacity;
uniform vec3 ambient;
uniform vec3 diffuse;

uniform vec3 sourceLight;

//para determinar si es de día o de noche
uniform bool moonActive = false; 

//ambientales generales
uniform vec3 ambientDay;
uniform vec3 ambientNight;

void main() {
        
        vec2 adjustedTexCoord = vec2(uvsFragmentShader.x, 1.0 - uvsFragmentShader.y);
        vec4 baseColor = texture(textureSampler, adjustedTexCoord);

        vec3 lightDirection = normalize(sourceLight - primitivePosition.xyz);
        float sourceLightAngle = dot(normalsFragmentShader, lightDirection);

        //determinar ambiental dependiendo del momento dle ciclo
        vec3 globalAmbient = moonActive ? ambientNight : ambientDay;

        vec3 finalColor = baseColor.rgb * (ambient + globalAmbient) * diffuse * sourceLightAngle;

        fragColor = vec4(finalColor, opacity);

}
