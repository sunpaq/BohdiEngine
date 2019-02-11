//
//  MC3DModel.c
//  monkcGame
//
//  Created by Sun YuLi on 16/3/14.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "MC3DModel.h"
#include "BAObjParser.h"
#include "BATrianglization.h"
#include "MCMath.h"
#include "MCLinkedList.h"
#include "MCTextureCache.h"
#include "MCMesh.h"
#include "MCString.h"
//#include "BAMaterial.h"

fun(frame, MC3DFrame)) as(MC3DNode)
    MC3DFrame allframe = (MC3DFrame){0,0,0,0,0,0};
    
    MCLinkedListForEach(it->meshes,
        struct MCMesh* m = (struct MCMesh*)item;
        if (m != null) {
            MC3DFrame mf = m->Frame;
            //MAX
            MCMath_accumulateMaxd(&allframe.xmax, mf.xmax);
            MCMath_accumulateMaxd(&allframe.ymax, mf.ymax);
            MCMath_accumulateMaxd(&allframe.zmax, mf.zmax);
            //MIN
            MCMath_accumulateMind(&allframe.xmin, mf.xmin);
            MCMath_accumulateMind(&allframe.ymin, mf.ymin);
            MCMath_accumulateMind(&allframe.zmin, mf.zmin);
        }
    )
    
    MCLinkedListForEach(it->children,
        struct MC3DModel* m = (struct MC3DModel*)item;
        if (m != null) {
            MC3DFrame mf = computed(m, frame);
            //MAX
            MCMath_accumulateMaxd(&allframe.xmax, mf.xmax);
            MCMath_accumulateMaxd(&allframe.ymax, mf.ymax);
            MCMath_accumulateMaxd(&allframe.zmax, mf.zmax);
            //MIN
            MCMath_accumulateMind(&allframe.xmin, mf.xmin);
            MCMath_accumulateMind(&allframe.ymin, mf.ymin);
            MCMath_accumulateMind(&allframe.zmin, mf.zmin);
        }
    )

    cast(it, MC3DModel)->lastSavedFrame = allframe;
    return allframe;
}

fun(maxlength, double)) as(MC3DModel)
    double max = 0;
    MC3DFrame frame = it->frame(it);
    MCMath_accumulateMaxd(&max, frame.xmax - frame.xmin);
    MCMath_accumulateMaxd(&max, frame.ymax - frame.ymin);
    MCMath_accumulateMaxd(&max, frame.zmax - frame.zmin);
    return max;
}

fun(center, MCVector3)) as(MC3DModel)
    MC3DFrame f = it->frame(it);
    return MCVector3Make((f.xmax-f.xmin)/2.0,
                         (f.ymax-f.ymin)/2.0,
                         (f.zmax-f.zmin)/2.0);
}

fun(release, void)) as(MC3DNode)
    //clean all the cached textures
    //MCTextureCache_cleanAndDestoryShared(0);
    it->release(it);
}

fun(meshLoadFaceElement, void), struct MCMesh* mesh, BAObjData* buff, BAFaceElement e, size_t index, MCColorf color)
{
    MCVector3 v, n;
    MCVector2 t;

    if (e.vi <= 0) {
        error_log("MC3DFileParser: invalide vertex data!\n");
        return;
    }else{
        v = buff->vertexbuff[e.vi-1];
    }
    
    if (buff->mtllib_list) {
        BAMtlLibrary* iter = buff->mtllib_list;
        while (iter) {
            if (iter->diffuse_map_count > 0) {
                buff->shouldCalculateNormal = false;
            }
            iter = iter->next;
        }
    }
    
    if (buff->shouldCalculateNormal) {
        n = MCNormalOfTriangle(buff->vertexbuff[e.vi], buff->vertexbuff[e.vi+1], buff->vertexbuff[e.vi+2]);
        mesh->calculatedNormal = true;
    }
    
    if (e.ni <= 0 || buff->normalbuff == null) {
        if (!buff->shouldCalculateNormal) {
            n = MCNormalOfTriangle(buff->vertexbuff[e.vi], buff->vertexbuff[e.vi+1], buff->vertexbuff[e.vi+2]);
            mesh->calculatedNormal = true;
        }
    }else{
        n = MCVector3From4(buff->normalbuff[e.ni-1]);
    }
    
    if (e.ti <= 0) {
        error_log("MC3DFileParser: empty texcoord data, set to 0!");
        t = (MCVector2){0.0,0.0};
    }else{
        t = buff->texcoorbuff[e.ti-1];
    }
    
    //3D frame max
    MCMath_accumulateMaxd(&buff->Frame.xmax, v.x);
    MCMath_accumulateMaxd(&buff->Frame.ymax, v.y);
    MCMath_accumulateMaxd(&buff->Frame.zmax, v.z);
    //3D frame min
    MCMath_accumulateMind(&buff->Frame.xmin, v.x);
    MCMath_accumulateMind(&buff->Frame.ymin, v.y);
    MCMath_accumulateMind(&buff->Frame.zmin, v.z);

    MCVertexData data = {
            v.x, v.y, v.z,
            n.x, n.y, n.z,
            color.R, color.G, color.B,
            t.x, t.y
            //0,0
    };

    mesh->setVertex(mesh, (uint32_t)index, &data);
}

fun(createMeshWithBATriangles, struct MCMesh*), BATriangle* triangles, size_t tricount, BAObjData* buff, MCColorf color)
{
    struct MCMesh* mesh = MCMesh(alloc(MCMesh), (int32_t)tricount*3);
    
    size_t i;
    for (i=0; i<tricount; i++) {
        size_t index = i * 3;
        meshLoadFaceElement(null, mesh, buff, triangles[i].e1, index+0, color);
        meshLoadFaceElement(null, mesh, buff, triangles[i].e2, index+1, color);
        meshLoadFaceElement(null, mesh, buff, triangles[i].e3, index+2, color);
    }
    
    //normalize normal
    mesh->normalizeNormals(mesh);
    
    //frame
    for (i=0; i<6; i++) {
        mesh->Frame.m[i] = buff->Frame.m[i];
    }
    
    return mesh;
}

fun(setDefaultMaterialForNode, void), struct MC3DNode* node)
{
    if (node) {
        node->material->ambientLightColor  = MCVector3Make(0.5, 0.5, 0.5);
        node->material->diffuseLightColor  = MCVector3Make(0.5, 0.5, 0.5);
        node->material->specularLightColor = MCVector3Make(0.5, 0.5, 0.5);
        node->material->specularLightPower = 16.0f;
        node->material->dissolve           = 1.0f;
        node->material->hidden             = 0;
        node->material->illum              = 2;
        
        MCStringFill(node->material->tag, "Default");
        node->material->dataChanged = true;
    }
}

fun(setMaterialForNode, void), struct MC3DNode* node, BAMaterial* mtl)
{
    if (mtl && mtl->name[0] != NUL) {
        MCVector3 ambient  = BAMaterialLightColor(mtl, Ambient);
        MCVector3 diffuse  = BAMaterialLightColor(mtl, Diffuse);
        MCVector3 specular = BAMaterialLightColor(mtl, Specular);
        
        node->material->ambientLightColor  = ambient;
        node->material->diffuseLightColor  = diffuse;
        node->material->specularLightColor = specular;
        node->material->specularLightPower = mtl->specularExponent;
        node->material->dissolve           = mtl->dissolveFactor;
        node->material->hidden             = mtl->hidden;
        node->material->illum              = mtl->illumModelNum;
        
        MCStringFill(node->material->tag, mtl->name);
        node->material->dataChanged = true;
    }else{
        setDefaultMaterialForNode(null, node);
    }
}

fun(setTextureForNode, void), struct MC3DNode* node, BAObjData* buff, BAMesh* mesh)
{
    //object texture
    if (mesh->object[0]) {
        BATexture* tex = BAFindTextureByAttachedObject(buff->mtllib_list, mesh->object);
        if (tex && tex->filename[0]) {
            struct MCTextureCache* tcache = MCTextureCache_shared();
            struct MCTexture* mctex = tcache->findTextureNamed(tcache, tex->filename);
            if (mctex == null) {
                mctex = new(MCTexture);
                mctex->initWithFileName(mctex, tex->filename);
                tcache->cacheTextureNamed(tcache, mctex, tex->filename);
                Release(mctex);
            }
            node->diffuseTexture = mctex;
            return;
        }
    }
    
    //group texture
    if (mesh->group[0]) {
        BATexture* tex = BAFindTextureByAttachedGroup(buff->mtllib_list, mesh->group);
        if (tex && tex->filename[0]) {
            struct MCTextureCache* tcache = MCTextureCache_shared();
            struct MCTexture* mctex = tcache->findTextureNamed(tcache, tex->filename);
            if (mctex == null) {
                mctex = new(MCTexture);
                mctex->initWithFileName(mctex, tex->filename);
                tcache->cacheTextureNamed(tcache, mctex, tex->filename);
                Release(mctex);
            }
            node->diffuseTexture = mctex;
            return;
        }
    }
    
    //material texture
    BAMaterial* mtl = mesh->usemtl;
    if (mtl) {
        if (mtl->diffuseMapName[0]) {
            struct MCTextureCache* tcache = MCTextureCache_shared();
            struct MCTexture* mctex = tcache->findTextureNamed(tcache, mtl->diffuseMapName);
            if (mctex == null) {
                mctex = new(MCTexture);
                mctex->initWithFileName(mctex, mtl->diffuseMapName);
                tcache->cacheTextureNamed(tcache, mctex, mtl->diffuseMapName);
                Release(mctex);
            }
            node->diffuseTexture = mctex;
        }
        if (mtl->specularMapName[0]) {
            struct MCTextureCache* tcache = MCTextureCache_shared();
            struct MCTexture* mctex = tcache->findTextureNamed(tcache, mtl->specularMapName);
            if (mctex == null) {
                mctex = new(MCTexture);
                mctex->initWithFileName(mctex, mtl->specularMapName);
                tcache->cacheTextureNamed(tcache, mctex, mtl->specularMapName);
                Release(mctex);
            }
            node->specularTexture = mctex;
        }
        if (mtl->normalMapName[0]) {
            struct MCTextureCache* tcache = MCTextureCache_shared();
            struct MCTexture* mctex = tcache->findTextureNamed(tcache, mtl->normalMapName);
            if (mctex == null) {
                mctex = new(MCTexture);
                mctex->initWithFileName(mctex, mtl->normalMapName);
                tcache->cacheTextureNamed(tcache, mctex, mtl->normalMapName);
                Release(mctex);
            }
            node->normalTexture = mctex;
        }
    }
}

//size_t fcursor, BAMaterial* mtl, size_t facecount,
fun(initModel, struct MC3DModel*), BAObjData* buff, BAMesh* bamesh, MCColorf color)
{
    struct MC3DModel* model = (struct MC3DModel*)any;
    if (model && bamesh) {
        BAFace* faces = &buff->facebuff[bamesh->startFaceCount];
        BAMaterial* mtl = bamesh->usemtl;
        
        BATriangle* triangles = createTrianglesBuffer(faces, bamesh->totalFaceCount);
        size_t tricount = trianglization(triangles, faces, bamesh->totalFaceCount, buff->vertexbuff);
        struct MCMesh* mesh = createMeshWithBATriangles(null, triangles, tricount, buff, color);
        
        model->Super.material = new(MCMaterial);
        model->Super.diffuseTexture  = null;
        model->Super.specularTexture = null;
        model->Super.normalTexture = null;
        model->Super.meshes->addItem(model->Super.meshes, (struct MCItem*)mesh);
        
        //set mtl
        if (mtl) {
            setMaterialForNode(null, &model->Super, mtl);
        }else{
            setDefaultMaterialForNode(null, &model->Super);
        }
        
        //set texture
        setTextureForNode(null, &model->Super, buff, bamesh);
        
        //set name
        MCStringFill(model->name, buff->name);
        releaseTrianglesBuffer(triangles);
        return model;
    }else{
        return null;
    }
}

fun(initWithFilePathColor, struct MC3DModel*), const char* path, MCColorf color) as(MC3DModel)
    debug_log("MC3DModel - initWithFilePathColor: %s\n", path);
    
    BAObjMeta Meta;
    BAObjData* buff = BAObjDataNewWithFilepath(path, &Meta);
    if (!buff) {
        error_log("MC3DModel initWithFilePathColor BAObjNew() failed exit\n");
        exit(-1);
    }
    debug_log("MC3DModel - BAObjNew success: %s\n", path);
    
    BAObjDumpInfo(buff);

    //separate model by mesh
    if (Meta.mesh_count <= 1) {
        BAMesh* m = &buff->meshbuff[0];
        if (m) {
            struct MC3DModel* model = new(MC3DModel);
            initModel(model, buff, m, color);
            it->Super.children->addItem(it->Super.children, (struct MCItem*)model);
        }
    } else {
        for (size_t i=0; i<Meta.mesh_count; i++) {
            BAMesh* m = &buff->meshbuff[i];
            if (m) {
                struct MC3DModel* model = new(MC3DModel);
                initModel(model, buff, m, color);
                it->Super.children->addItem(it->Super.children, (struct MCItem*)model);
            }
        }
    }
    
    BAObjRelease(buff);
    cpt(frame);
    
    debug_log("MC3DModel - return obj: %s\n", path);
    return any;
}

fun(initWithFilePath, struct MC3DModel*), const char* path) as(MC3DModel)
    return it->initWithFilePathColor(it, path, it->defaultColor);
}

fun(initWithFileNameColor, struct MC3DModel*), const char* name, MCColorf color) as(MC3DModel)
    if (it) {
        MCStringFill(it->name, name);
        char path[PATH_MAX] = {0};
        if (MCFileGetPath(name, path)) {
            return null;
        }
        debug_log("MC3DModel - find path: %s\n", path);
        return it->initWithFilePathColor(it, path, color);
    }else{
        return null;
    }
}

fun(initWithFileName, struct MC3DModel*), const char* name) as(MC3DModel)
    return it->initWithFileNameColor(it, name, it->defaultColor);
}

fun(translateToOrigin, void)) as(MC3DModel)
    MCVector3 center = it->center(it);
    MCVector3 rcenter = MCVector3Reverse(center);
    cast(it, MC3DNode)->translateVec3(it, &rcenter, false);
}

fun(rotateAroundSelfAxisX, void), double ccwRadian) as(MC3DNode)
    MCMatrix4 RX = MCMatrix4FromMatrix3(MCMatrix3MakeXAxisRotation(ccwRadian));
    it->transform = MCMatrix4Multiply(it->transform, RX);
}

fun(rotateAroundSelfAxisY, void), double ccwRadian) as(MC3DNode)
    MCMatrix4 RY = MCMatrix4FromMatrix3(MCMatrix3MakeYAxisRotation(ccwRadian));
    it->transform = MCMatrix4Multiply(it->transform, RY);
}

fun(rotateAroundSelfAxisZ, void), double ccwRadian) as(MC3DNode)
    MCMatrix4 RZ = MCMatrix4FromMatrix3(MCMatrix3MakeZAxisRotation(ccwRadian));
    it->transform = MCMatrix4Multiply(it->transform, RZ);
}

fun(resizeToFit, void), double maxsize) as(MC3DModel)
    if (it->fitted == false) {
        double maxl = it->maxlength(it);
        if (maxl > DBL_EPSILON) {
            double scale = maxsize / maxl;
            MCVector3 scaleVec = MCVector3Make(scale, scale, scale);
            cast(it, MC3DNode)->scaleVec3(it, &scaleVec, false);
            it->fitted = true;
            debug_log("MC3DModel - model maxlength=%lf scale=%lf\n", maxl, scale);
        }
    }
}

constructor(MC3DModel)) {
    MC3DNode(any);
    as(MC3DModel)
        it->tag = -1;
        it->name[0] = NUL;
    
        it->defaultColor = (MCColorf){0.9, 0.9, 0.9, 1.0};
        it->defaultExtension = "obj";
        it->textureOnOff = false;
        it->fitted = false;
    
        it->lastSavedFrame = (MC3DFrame){0,0,0,0,0,0};
    
        funadd(frame);
        funadd(maxlength);
        funadd(center);
    }
    dynamic(MC3DModel)
        funbind(release);
        funbind(initWithFilePath);
        funbind(initWithFileName);
        funbind(initWithFilePathColor);
        funbind(initWithFileNameColor);
        funbind(rotateAroundSelfAxisX);
        funbind(rotateAroundSelfAxisY);
        funbind(rotateAroundSelfAxisZ);
        funbind(resizeToFit);
        funbind(translateToOrigin);
    }
    return any;
}


