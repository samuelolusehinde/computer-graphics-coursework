#version 330 core

struct LightSource 
{
    vec3 position;	
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
   
    float focalStrength;
    float specularIntensity;
};

#define TOTAL_LIGHTS 2

in vec3 fragmentPosition;
in vec3 fragmentVertexNormal;
in vec2 fragmentTextureCoordinate;
in mat3 TBN;

out vec4 outFragmentColor;

uniform bool bUseNormAndSpec=false;
uniform bool bUseTexture=true;
uniform bool bUseLighting=true;
uniform bool toggleLight1=true;
uniform bool toggleLight2=true;
uniform vec4 objectColor = vec4(1.0f);

uniform sampler2D diffuseMap;
uniform sampler2D specMap;
uniform sampler2D normalMap;

uniform vec3 viewPosition;
uniform vec2 UVscale = vec2(1.0f, 1.0f);
uniform LightSource lightSources[TOTAL_LIGHTS];

uniform  float Ka;
uniform  float Ks;
uniform  float Kd;
uniform  float Ns;

// function prototypes
vec3 CalcLightSource(LightSource light, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection);
vec3 calcDirectionalLight(vec3 Dir, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection);

vec3 UserMaterial = vec3(1.0f);

void main()
{
    vec3 phongResult = vec3(0.0f);
    vec3 lightNormal = normalize(fragmentVertexNormal);
    vec3 viewDirection = normalize(viewPosition - fragmentPosition);

    if(bUseNormAndSpec)
	{		
		// obtain normals from normals map in range [0,1]
		lightNormal = texture(normalMap, fragmentTextureCoordinate).rgb;
		
		// transform normals vector to range [-1,1]
		lightNormal = normalize(lightNormal * 2.0 - 1.0);  
		
		lightNormal = (TBN * lightNormal);//transformation to tangent space
	}


   if(bUseLighting == true)
   {
      // properties
      phongResult += calcDirectionalLight(vec3(0, 1, 0.7), lightNormal, fragmentPosition, viewDirection);

      for(int i = 0; i < TOTAL_LIGHTS; i++)
      {
        if(!toggleLight1 && i == 0 ) {
            continue;
         }

        if(!toggleLight2 && i == 1 ) {
            continue;
         }


         else{
           phongResult += CalcLightSource(lightSources[i], lightNormal, fragmentPosition, viewDirection); 
         }
      }   
    
      if(bUseTexture == true)
      {
         vec4 textureColor = texture(diffuseMap, fragmentTextureCoordinate * UVscale);
         outFragmentColor = vec4(phongResult * textureColor.xyz, 1.0);
      }
      else
      {
         outFragmentColor = vec4(phongResult * objectColor.xyz, objectColor.w);
      }
   }
   else 
   {
      if(bUseTexture == true)
      {
         outFragmentColor = texture(diffuseMap, fragmentTextureCoordinate * UVscale);
      }
      else
      {
         outFragmentColor = objectColor;
      }
   }


    //outFragmentColor = vec4(lightSources[0].position, 1.0f);
}


vec3 calcDirectionalLight(vec3 Dir, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection)
{
   float intensity = 0.50f;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Calculate light direction and distance
    vec3 lightDirection = normalize(Dir); 


    // Ambient
    ambient = vec3(0.5f) * Ka;

    // Diffuse
    float impact = max(dot(lightNormal, lightDirection), 0.0);
    diffuse = impact * vec3(0.5f) * intensity;

    // Specular
    vec3 reflectDir = reflect(-lightDirection, lightNormal);

  
    return (ambient + diffuse);
}

// calculates the color when using a directional light.
vec3 CalcLightSource(LightSource light, vec3 lightNormal, vec3 vertexPosition, vec3 viewDirection)
{
    float intensity = 50.0f;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Calculate light direction and distance
    vec3 lightDirection = normalize(light.position - vertexPosition); 
    float distance = length(light.position - vertexPosition);

    // === Hardcoded attenuation factors ===
    float constant = 1.0;
    float linear = 0.00014;
    float quadratic = 0.00003;

    float attenuation = 1.0 / (constant + (linear * distance) + ( quadratic * distance * distance) );

    // Ambient
    ambient = light.ambientColor * Ka;

    // Diffuse
    float impact = max(dot(lightNormal, lightDirection), 0.0);
    diffuse = impact * light.diffuseColor * intensity;

    // Specular
    vec3 reflectDir = reflect(-lightDirection, lightNormal);
    float specularComponent = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0f);

    if(bUseNormAndSpec)
    {
        specular = specularComponent * (light.specularIntensity * Ns) * light.specularColor * intensity * texture(specMap, fragmentTextureCoordinate).rgb;
    }
    else{
        specular = specularComponent * (light.specularIntensity * Ns) * light.specularColor * intensity;
    }

    // Apply attenuation
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
