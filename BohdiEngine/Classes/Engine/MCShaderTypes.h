//
//  MCShaderTypes.h
//  Pods
//
//  Created by 孙御礼 on 7/20/30 H.
//

#ifndef MCShaderTypes_h
#define MCShaderTypes_h

typedef union {
    struct {
        float x;
        float y;
        float z;
        float nx;
        float ny;
        float nz;
        float r;
        float g;
        float b;
        float u;
        float v;
    };
    float data[11];
} MCVertexData;

#endif /* MCShaderTypes_h */
