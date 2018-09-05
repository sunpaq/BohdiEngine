//
//  MCSkysphereShader.h
//  Pods
//
//  Created by 孙御礼 on 7/15/30 H.
//

#ifndef MCSkysphereShader_h
#define MCSkysphereShader_h

static const char* MCSkysphere_vsource = S(
                               //version is specified in MCGLContext
                               precision highp float;
                               precision mediump int;
                               
                               //vertex attributes
                               layout (location=0) in vec3 position;
                               layout (location=1) in vec2 texcoord;
                               
                               //uniforms
                               uniform mat4 sphViewMatrix;
                               uniform mat4 sphProjectionMatrix;
                               
                               //output
                               out vec2 TexCoords;
                               
                               void main()
{
    gl_Position = sphProjectionMatrix * sphViewMatrix * vec4(position, 1.0);
    TexCoords = texcoord;
}
                               );//vsource end

static const char* MCSkysphere_fsource = S(
                               //version is specified in MCGLContext
                               precision highp sampler2D;
                               precision highp float;
                               precision lowp int;
                               const float Epsilon = 0.0000001;
                               
                               //input
                               in vec2 TexCoords;
                               
                               //uniforms
                               uniform sampler2D sampler;
                               
                               //output
                               out vec4 color;
                               
                               void main()
{
    color = texture(sampler, TexCoords);
}
                               );//fsource end

#endif /* MCSkysphereShader_h */
