//
//  MC3DAxis.c
//  Sapindus
//
//  Created by YuliSun on 27/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#include "MC3DAxis.h"

oninit(MC3DAxis)
{
    if (init(MC3DNode)) {
//        sobj->material = new(MCMaterial);
//        sobj->material->illum = 11;
        
        glGenBuffers(1, &obj->buffId);

        return obj;
    } else {
        return null;
    }
}

//override MC3DNode
fun(MC3DAxis, void, draw, voida)
{
    float data[36] = {
        3,0,0, 1,0,0,
        0,0,0, 1,0,0,
        0,2,0, 0,1,0,
        0,0,0, 0,1,0,
        0,0,1, 0,0,1,
        0,0,0, 0,0,1
    };
    
    glLineWidth(5);
    
    glBindBuffer(GL_ARRAY_BUFFER, obj->buffId);
    glBufferData(GL_ARRAY_BUFFER, 36*4, data, GL_STATIC_DRAW);//GL_STREAM_DRAW, GL_DYNAMIC_DRAW
    
    glEnableVertexAttribArray(MCVertexAttribPosition);
    glEnableVertexAttribArray(MCVertexAttribColor);
    glVertexAttribPointer(MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 6*4, MCBUFFER_OFFSET(0));
    glVertexAttribPointer(MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 6*4, MCBUFFER_OFFSET(3*4));
    
    glBindVertexArray(obj->buffId);
    glDrawArrays(MCLines, 0, 6);
    //glBindVertexArray(0);
    
    glLineWidth(1);
}

onload(MC3DAxis)
{
    if (load(MC3DNode)) {
        bid(MC3DAxis, void, draw, voida);
        return cla;
    } else {
        return null;
    }
}
