#version 410 core
out vec4 fragColor;         //The final color

in vec3 normalTransposed;   //Normal of fragment, already transposed
in vec3 fragmentPosition;
in vec2 UV;       //for textures

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform float ambientStrengt = 0.3;
uniform float lightStrengt = 0.5;
uniform vec3 lightColor = vec3(0.8, 0.8, 0.3);  //yellowish
uniform float specularStrength = 0.9;
uniform int specularExponent = 32;

uniform vec3 objectColor = vec3(0.7, 0.7, 0.7); //grey

uniform sampler2D mtextureSampler;

void main() {
    //ambient
    vec3 ambient = ambientStrengt * lightColor;

    //diffuse8
    vec3 normalCorrected = normalize(normalTransposed);
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);
    float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
    vec3 diffuse = angleFactor * objectColor * lightColor * lightStrengt;

    //specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    float spec = 0.0;
    if (angleFactor > 0.0)     //calculations only needed if diffuse is above 0.0
    {
        vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
    }
    vec3 specular = spec * lightColor * specularStrength;

    vec3 result = ambient + diffuse + specular; //diffuse // + ;ambient,

    fragColor = vec4(result, 1.0);
}

//Using calculations in world space,
//https://learnopengl.com/Lighting/Basic-Lighting
//but could just as easy be done in camera space
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
