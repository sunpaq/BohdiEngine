//
//  MCSkyboxShader.h
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//

#ifndef MCSkyboxShader_h
#define MCSkyboxShader_h

static const char* MCSkybox_vsource = S(precision highp float;
                               precision mediump int;
                               
                               layout (location=0) in vec3 position;
                               
                               uniform mat4 boxViewMatrix;
                               uniform mat4 boxProjectionMatrix;
                               out vec3 TexCoords;
                               
                               void main() {
                                   gl_Position = boxProjectionMatrix * boxViewMatrix * vec4(position, 1.0);
                                   TexCoords = position;
                               }
                               );//end vsource

static const char* MCSkybox_fsource = S(precision highp sampler3D;
                               precision highp float;
                               precision lowp int;
                               const float Epsilon = 0.0000001;
                               
                               in vec3 TexCoords;
                               uniform samplerCube cubeSampler;
                               
                               out vec4 color;
                               
                               void main() {
                                   color = texture(cubeSampler, TexCoords);
                               }
                               );//end fsource

#endif /* MCSkyboxShader_h */
