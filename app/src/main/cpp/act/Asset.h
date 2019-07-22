//
// Created by yanny on 22/7/2019.
//

#ifndef RINCONADA_ASSET_H
#define RINCONADA_ASSET_H
#include <android/asset_manager.h>
#include "Log.h"
struct AssetDescriptor{
    int32_t descriptor;
    off_t   start;
    off_t   length;
};
class Asset {
public:

    Asset(const char* pPath);

    int open();
    void close();

    int read(void* pBuffer, size_t pCount);
    int read(void* pBuffer);

    AssetDescriptor descriptor();

    static void setAssetManager(AAssetManager* pAssetManager);
    static void setDataPath(const char * pInternalDataPath,  const char * pExternalDataPath);

private:

    AAsset* _asset;
    const char* _path;

    static AAssetManager * _assetManager;
    static const char * _externalDataPath;
    static const char * _internalDataPath;


};


#endif //RINCONADA_ASSET_H
