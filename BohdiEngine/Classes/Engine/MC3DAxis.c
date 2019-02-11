//
//  MC3DAxis.c
//  Sapindus
//
//  Created by YuliSun on 27/02/2017.
//  Copyright © 2017 oreisoft. All rights reserved.
//

#include "MC3DAxis.h"

//override MC3DNode
fun(release, void)) as(MC3DAxis)
    float data[36] = {
        3,0,0, 1,0,0,
        0,0,0, 1,0,0,
        0,2,0, 0,1,0,
        0,0,0, 0,1,0,
        0,0,1, 0,0,1,
        0,0,0, 0,0,1
    };
    
    glLineWidth(5);
    
    glBindBuffer(GL_ARRAY_BUFFER, it->buffId);
    glBufferData(GL_ARRAY_BUFFER, 36*4, data, GL_STATIC_DRAW);//GL_STREAM_DRAW, GL_DYNAMIC_DRAW
    
    glEnableVertexAttribArray(MCVertexAttribPosition);
    glEnableVertexAttribArray(MCVertexAttribColor);
    glVertexAttribPointer(MCVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 6*4, MCBUFFER_OFFSET(0));
    glVertexAttribPointer(MCVertexAttribColor,    3, GL_FLOAT, GL_FALSE, 6*4, MCBUFFER_OFFSET(3*4));
    
    glBindVertexArray(it->buffId);
    glDrawArrays(MCLines, 0, 6);
    //glBindVertexArray(0);
    
    glLineWidth(1);
}

fun(draw, void)) {
    
}

constructor(MC3DAxis)) {
    MC3DNode(any);
    as(MC3DAxis)
        glGenBuffers(1, &it->buffId);
    };
    dynamic(MC3DAxis)
        funbind(release);
        funbind(draw);
    };
    return any;
}


