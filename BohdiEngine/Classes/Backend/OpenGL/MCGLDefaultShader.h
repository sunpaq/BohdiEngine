//
//  MCGLDefaultShader.h
//  BohdiEngine
//
//  Created by 孙御礼 on 5/3/30 H.
//

#ifndef MCGLDefaultShader_h
#define MCGLDefaultShader_h

#include "monkc_export.h"

static const char* MCGLDefault_vsource = S(
//version is specified in MCGLContext
precision highp float;
precision mediump int;

//vertex attributes
layout (location=0) in vec4 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec3 color;
layout (location=3) in vec2 texcoord;
                             
//Android GLSL doesn't support struct
uniform mat4 view_view;
uniform mat4 view_projection;
uniform vec3 view_position;
                             
uniform mat4 model_model;
uniform mat3 model_normal;
                             
//varying variables use to pass value between vertex & fragment shader
out vec3 vertexcolor;
out vec2 texturecoord;
out vec3 calculatedNormal;
out vec3 modelPosition;
out vec3 viewPosition;

void main()
{
    //Vertex Color
    vertexcolor = color;
    
    //Texture Coordinate
    texturecoord = texcoord;
    
    //Normal fix the non-uniform scale issue
    //calculate by GPU
    calculatedNormal = normalize(mat3(transpose(inverse(model_model))) * normal);
    //calculate by CPU
    //calculatedNormal = normalize(model_normal * normal);
    
    //Eye normal
    //eyeNormal = normalize(model.normal * normal);
    
    //Specular Light
    modelPosition = vec3(model_model * position);
    viewPosition  = view_position;
    
    //Position
    gl_Position = view_projection * view_view * model_model * position;
}
);//VCODE END

static const char* MCGLDefault_fsource = S(
//version is specified in MCGLContext
precision highp sampler3D;
precision highp float;
precision lowp int;
const float Epsilon = 0.0000001;
                             
//varying variables use to pass value between vertex & fragment shader
in vec3 vertexcolor;
in vec2 texturecoord;
in vec3 calculatedNormal;
in vec3 modelPosition;
in vec3 viewPosition;

out vec4 FragColor;

//Android GLSL doesn't support struct
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;
uniform vec3 light_color;
uniform vec3 light_position;

uniform vec3  material_ambient;
uniform vec3  material_diffuse;
uniform vec3  material_specular;
uniform float material_dissolve;
uniform float material_shininess;

//uniforms not in context
//texture sampling must in fragment shader
uniform sampler2D diffuse_sampler;
uniform sampler2D specular_sampler;
uniform sampler2D normal_sampler;
//0=none 1=diffuse 2=specular 3=normal
uniform int usetexture;
uniform int illum;

bool floatEqual(float A, float B)
{
    if (abs(A-B) < Epsilon) {
        return true;
    }
    return false;
}
                             
vec3 halfwayVector(vec3 L, vec3 V)
{
    return (L+V)/abs(L+V);
}
                             
vec3 reflectVector(vec3 L, vec3 N)
{
    //    float dotNL = max(0.0,dot(N,L));
    //    return 2.0*dotNL*N - L;
    return reflect(-L,N);
}
                             
//doen't contains Ka*Ia
vec3 lambertianShading(vec3 Kd, vec3 Id, vec3 L, vec3 N)
{
    float dotNL = max(0.0,dot(N,L));
    return Kd*Id*dotNL;
}
                             
//doen't contains Ka*Ia
vec3 phongShading(vec3 Kd, vec3 Ks, vec3 Id, vec3 Is, float Es, vec3 L, vec3 N, vec3 V, bool simplified)
{
    //Lambertian shading and Blinn's interpretation of Phong's specular illumination model (BLIN77)
    float dotNL = max(0.0,dot(N,L));
    if (simplified == true) {
        vec3 H = halfwayVector(L,V);
        float dotNH = max(0.0,dot(N,H));
        if (dotNH > 0.0 && Es > 0.0) {
            return Kd*Id*dotNL + Ks*Is*pow(dotNH,Es);
        } else {
            return Kd*Id*dotNL;
        }
    }
    else {
        vec3 R = reflectVector(L,N);
        float dotVR = max(0.0,dot(V,R));
        if (dotVR > 0.0 && Es > 0.0) {
            return Kd*Id*dotNL + Ks*Is*pow(dotVR,Es);
        } else {
            return Kd*Id*dotNL;
        }
    }
}
                             
//doen't contains Ka*Ia
vec3 phongShading_withIr(vec3 Kd, vec3 Ks, vec3 Id, vec3 Is, float Es, vec3 L, vec3 N, vec3 V, bool simplified, vec3 Ir)
{
    //Lambertian shading and Blinn's interpretation of Phong's specular illumination model (BLIN77)
    float dotNL = max(0.0,dot(N,L));
    if (simplified == true) {
        vec3 H = halfwayVector(L,V);
        float dotNH = max(0.0,dot(N,H));
        if (dotNH > 0.0 && Es > 0.0) {
            return Kd*Id*dotNL + Ks*(Is*pow(dotNH,Es) + Ir);
        } else {
            return Kd*Id*dotNL;
        }
    }
    else {
        vec3 R = reflectVector(L,N);
        float dotVR = max(0.0,dot(V,R));
        if (dotVR > 0.0 && Es > 0.0) {
            return Kd*Id*dotNL + Ks*(Is*pow(dotVR,Es) + Ir);
        } else {
            return Kd*Id*dotNL;
        }
    }
}
                             
void main()
{
    //Color Output
    vec3 color = material_diffuse;
    vec3 normal = calculatedNormal;
    float alpha = 1.0;
    if (usetexture >= 1) {
        vec4 texcolor = texture(diffuse_sampler, vec2(texturecoord.x, 1.0-texturecoord.y));
        //vec4 texcolor = texture(diffuse_sampler, texturecoord);
        alpha = texcolor.a;
        color = texcolor.rgb;
    }
    if (usetexture == 3) {
        vec4 texcolor = texture(normal_sampler, vec2(texturecoord.x, 1.0-texturecoord.y));
        normal = texcolor.rgb;
    }
    
    //Illuminate Mode
    vec3 Ka = material_ambient;
    vec3 Ia = light_ambient * light_color;
    
    vec3 Kd = color;
    vec3 Ks = material_specular;
    vec3 Id = light_diffuse * light_color;
    vec3 Is = light_specular * light_color;
    
    float Es = material_shininess;
    
    vec3 L  = normalize(light_position - modelPosition);
    vec3 N  = normal;
    vec3 V  = normalize(viewPosition - modelPosition);
    
    if (illum == 0) {
        //color = Kd
        FragColor = vec4(Kd, alpha);
        return;
    }
    else if (illum == 1) {
        //Lambertian shading
        FragColor = vec4(Ka*Ia+lambertianShading(Kd,Id,L,N), alpha);
        return;
    }
    else if (illum == 2) {
        //Lambertian shading and Blinn's interpretation of Phong's specular illumination model (BLIN77)
        FragColor = vec4(Ka*Ia+phongShading(Kd,Ks,Id,Is,Es,L,N,V,false), alpha);
        return;
    }
    else if (illum == 3) {
        //Ir = (intensity of reflection map) + (ray trace)
        vec3 Ir = texture(specular_sampler, vec2(texturecoord.x, 1.0-texturecoord.y)).rgb;
        FragColor = vec4(Ka*Ia+phongShading_withIr(Kd,Ks,Id,Is,Es,L,N,V,false, Ir), alpha);
        return;
    }
    else if (illum == 4) {
        //glass
        //Ir = (intensity of reflection map) + (ray trace)
        vec3 Ir = vec3(0.0,0.0,0.0);
        FragColor = vec4(Ka*Ia+phongShading_withIr(Kd,Ks,Id,Is,Es,L,N,V,true, Ir), 0.1);
        return;
    }
    else if (illum == 5) {
        
    }
    else if (illum == 6) {
        
    }
    else if (illum == 7) {
        
    }
    else if (illum == 8) {//3 without ray trace
        //Ir = (intensity of reflection map)
        vec3 Ir = vec3(0.0,0.0,0.0);
        FragColor = vec4(Ka*Ia+phongShading_withIr(Kd,Ks,Id,Is,Es,L,N,V,true, Ir), alpha);
        return;
    }
    else if (illum == 9) {//4 without ray trace
        //glass
        //Ir = (intensity of reflection map)
        vec3 Ir = texture(specular_sampler, vec2(texturecoord.x, 1.0-texturecoord.y)).rgb;
        FragColor = vec4(Ka*Ia+phongShading_withIr(Kd,Ks,Id,Is,Es,L,N,V,true, Ir), 0.1);
        return;
    }
    else if (illum == 10) {
        //color = Pixel color. The pixel color of a shadowmatte material is always black.
        FragColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }
    else {
        //Lambertian shading
        FragColor = vec4(Ka*Ia+lambertianShading(Kd,Id,L,N), alpha);
        return;
    }
}
);//FCODE END

#endif /* MCGLDefaultShader_h */
