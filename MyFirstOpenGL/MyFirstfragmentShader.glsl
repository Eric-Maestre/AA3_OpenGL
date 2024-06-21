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

//para determinar si es de d�a o de noche
uniform bool moonActive = false; 

//ambientales generales
uniform vec3 ambientDay;
uniform vec3 ambientNight;

//parametros flashlight
uniform vec3 cameraPosition;
uniform vec3 cameraDirection;
uniform float cameraAngle;

uniform bool flashlightOn;

//distancia maxima de la linterna
uniform float maxDistance;

void main() {
        
        vec2 adjustedTexCoord = vec2(uvsFragmentShader.x, 1.0 - uvsFragmentShader.y);
        vec4 baseColor = texture(textureSampler, adjustedTexCoord);

        vec3 lightDirection = normalize(sourceLight - primitivePosition.xyz);
        float sourceLightAngle = dot(normalsFragmentShader, lightDirection);

        //determinar ambiental dependiendo del momento dle ciclo
        vec3 globalAmbient = moonActive ? ambientNight : ambientDay;

        vec3 finalColor = baseColor.rgb * (ambient + globalAmbient) * diffuse * sourceLightAngle;

        float distanceToModel = length(cameraPosition - primitivePosition.xyz);

        if(flashlightOn && distanceToModel <= maxDistance)
        {
        //direccion camara a modelo
        vec3 cameraModelDir = normalize(cameraPosition - primitivePosition.xyz);
        float spotEffect = dot(cameraModelDir, normalize (-cameraDirection));
        float flashlightIntensity = 0.f;

        if(spotEffect > cos(radians(cameraAngle)))
        {
            flashlightIntensity = max(dot(normalsFragmentShader, cameraModelDir), 0.0);
        }

        //color con linterna encendida
        finalColor += baseColor.rgb * diffuse * flashlightIntensity;
        }

        fragColor = vec4(finalColor, opacity);

}
