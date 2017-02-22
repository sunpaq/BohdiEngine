//
//  BEAssetsManager.c
//  Sapindus
//
//  Created by Sun YuLi on 16/4/30.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <pthread.h>
#endif
#include "BEAssetsManager.h"
#include "SOIL.h"

oninit(BE2DTextureData)
{
    if (init(MCObject)) {
        //const types
        obj->AUTO = 0;
        obj->L    = 1;
        obj->LA   = 2;
        obj->RGB  = 3;
        obj->RGBA = 4;
        
        //input
        //obj->type = obj->RGB;
        obj->path = "";
        
        //output
        obj->raw = null;
        return obj;
    }else{
        return null;
    }
}

utility(BE2DTextureData, BE2DTextureData*, newWithPathType, const char* path, unsigned type)
{
    BE2DTextureData* data = new(BE2DTextureData);
    
    char decodepath[PATH_MAX] = {};
    MCString_percentDecode(path, decodepath);
    
    size_t psize = strlen(decodepath) * sizeof(char);
    data->path = strcpy(malloc(psize), decodepath);

//    if (type >= data->AUTO && type<= data->RGBA ) {
//        data->type = type;
//    }else{
//        data->type = data->RGB;
//    }
    
    data->raw = SOIL_load_image(data->path, &data->width, &data->height, &data->channels, SOIL_LOAD_AUTO);
    return data;
}

utility(BE2DTextureData, BE2DTextureData*, newWithPath, const char* path)
{
    return BE2DTextureData_newWithPathType(path, 3);//default RGB
}

method(BE2DTextureData, void, bye, voida)
{
    if (obj->raw != null) {
        SOIL_free_image_data(obj->raw);
    }
    superbye(MCObject);
}

onload(BE2DTextureData)
{
    if (load(MCObject)) {
        binding(BE2DTextureData, void, bye, voida);
        return cla;
    }else{
        return null;
    }
}

//------------------------------
oninit(BECubeTextureData)
{
    if (init(MCObject)) {
        return obj;
    }else{
        return null;
    }
}

utility(BECubeTextureData, BECubeTextureData*, newWithFacePaths, const char* facepaths[6])
{
    BECubeTextureData* data = new(BECubeTextureData);
    for (int i=0; i<6; i++) {
        BE2DTextureData* aface = BE2DTextureData_newWithPath(facepaths[i]);
        if (aface != null) {
            data->faces[i] = aface;
        }else{
            error_log("BECubeTextureData image %s data is null!\n", facepaths[i]);
            return null;
        }
    }
    return data;
}

utility(BECubeTextureData, BECubeTextureData*, newWithFaces, const char* faces[6])
{
    BECubeTextureData* data = new(BECubeTextureData);
    char pathbuff[PATH_MAX] = {};
    for (int i=0; i<6; i++) {
        if(MCFileGetPath(faces[i], pathbuff)){
            return null;
        }
        pathbuff[PATH_MAX-1] = NUL;
        data->faces[i] = BE2DTextureData_newWithPath(pathbuff);
    }
    return data;
}

method(BECubeTextureData, void, bye, voida)
{
    for (int i=0; i<6; i++) {
        BE2DTextureData* face = obj->faces[i];
        if (face != null) {
            release(face);
        }
    }
    superbye(MCObject);
}

onload(BECubeTextureData)
{
    if (load(MCObject)) {
        binding(BECubeTextureData, void, bye, voida);
        return cla;
    }else{
        return null;
    }
}

#ifdef __ANDROID__
static AAssetManager* assetManager_ = null;
static ANativeWindow* window_ = null;

//File
void MCFileSetAssetManager(AAssetManager* assetManager)
{
	assetManager_ = assetManager;
}

AAssetManager* MCFileGetAssetManager()
{
    return assetManager_;
}
#endif

int MCFileGetPath(const char* filename, char* buffer)
{
    char basename[256] = {0};
    char extension[64] = {0};
    
    MCString_extensionFromFilename(filename, basename, extension);
    printf("MCFileGetPath - filename/basename/extension -> %s/%s/%s\n", filename, basename, extension);
    
#ifdef __ANDROID__
    if (assetManager_ != null) {
		const char* subpath;
		if (strcmp(extension, "fsh") == 0) {
			subpath = "shaders";
		} else if (strcmp(extension, "vsh") == 0) {
			subpath = "shaders";
		} else if (strcmp(extension, "obj") == 0 || strcmp(extension, "mtl") == 0) {
			subpath = "raw";
		} else if (strcmp(extension, "png") == 0) {
			subpath = "textures";
		} else if (strcmp(extension, "jpg") == 0) {
			subpath = "textures";
        } else if (strcmp(extension, "tga") == 0) {
            subpath = "textures";
		} else {
			subpath = "raw";
            error_log("can not detect use raw folder\n");
		}

		AAssetDir* rootdir = AAssetManager_openDir(assetManager_, subpath);
        if (rootdir) {
            const char* name;
            char fullpath[PATH_MAX] = {};
            while ((name=AAssetDir_getNextFileName(rootdir)) != NULL) {
                if (strcmp(filename, name) == 0) {
                    sprintf(fullpath, "%s/%s", subpath, name);
                    strcpy(buffer, fullpath);
                }
            }
        }else{
            error_log("can not find rootdir\n");
            return -1;
        }
	}
    return 0;
#else
    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    
    CFStringRef fname = CFStringCreateWithCString(NULL, basename, kCFStringEncodingUTF8);
    CFStringRef  fext = CFStringCreateWithCString(NULL, extension, kCFStringEncodingUTF8);
    CFURLRef      url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), fname, fext, NULL);
    if (url) {
        CFStringRef  path = CFURLCopyPath(url);
        CFStringGetCString(path, buffer, PATH_MAX, kCFStringEncodingUTF8);
        if(path) CFRelease(path);
        if(url)  CFRelease(url);
        if(fname)CFRelease(fname);
        if(fext) CFRelease(fext);
    } else {
        error_log("BEAssetManager can not find path of (%s).(%s)\n", basename, extension);
        if(fname)CFRelease(fname);
        if(fext) CFRelease(fext);
        pthread_mutex_unlock(&lock);
        return -1;
    }
    
    pthread_mutex_unlock(&lock);
    return 0;
#endif
}

const char* MCFileCopyContentWithPath(const char* filepath)
{
#ifdef __ANDROID__
    if (assetManager_ != null) {
        debug_log("MCFileCopyContentWithPath %s\n", filepath);
        AAsset* f = AAssetManager_open(assetManager_, filepath, AASSET_MODE_BUFFER);
        if (f) {
            const char* abuff = AAsset_getBuffer(f);
            if (abuff) {
                off_t size = AAsset_getLength(f);
                char* buff = (char*)malloc((size + 1) * sizeof(char));
                memcpy(buff, abuff, size);
                buff[size] = NUL;
                AAsset_close(f);
                return buff;
            }else{
                error_log("MCFileCopyContentWithPath(%s) AAsset_getBuffer() failed\n", filepath);
            }
        }else{
            error_log("MCFileCopyContentWithPath(%s) Android assetManager_ can not open\n", filepath);
        }
    }
    error_log("MCFileCopyContent(%s) Android assetManager_ is null\n", filepath);
    return null;
#else
    char decodepath[PATH_MAX] = {};
    FILE* f = fopen(MCString_percentDecode(filepath, decodepath), "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        char* const buffer = (char*)malloc(size * sizeof(char));
        char* iter = buffer;
        
        if (f != NULL) {
            char c;
            while ((c = fgetc(f)) != EOF) {
                *iter++ = c;
            }
            *iter = NUL;
        }
        
        return buffer;
    }else{
        error_log("MCFileCopyContent(%s) fopen return null\n", filepath);
        return null;
    }

#endif
}

void MCFileReleaseContent(void* buff)
{
	free(buff);
}
