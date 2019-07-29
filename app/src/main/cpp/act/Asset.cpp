//
// Created by yanny on 22/7/2019.
//

#include <cstring>
#include "Asset.h"

const char * Asset::_externalDataPath = "";
const char * Asset::_internalDataPath = "";
AAssetManager* Asset::_assetManager = nullptr;

Asset::Asset(const char *pPath):
    _path(pPath),
    _asset(nullptr)
    {
}

void Asset::setAssetManager(AAssetManager *pAssetManager) {
    Asset::_assetManager = pAssetManager;
}

void Asset::setDataPath(const char* pInternalDataPath, const char* pExternalDataPath) {
    _internalDataPath = pInternalDataPath;
    _externalDataPath = pExternalDataPath;
}

int Asset::open() {
    _asset = AAssetManager_open(_assetManager, _path, AASSET_MODE_UNKNOWN);
    return (_asset != NULL) ? 1 : 0;
}

void Asset::close() {
    if (_asset != NULL) {
        AAsset_close(_asset);
        _asset = NULL;
    }
}

int Asset::read(void *pBuffer, size_t pCount) {
    int32_t readCount = AAsset_read(_asset, pBuffer, pCount);
    return (readCount == pCount) ?1 :0;
}
int Asset::read(void *pBuffer) {
    off_t pCount = AAsset_getLength(_asset);
    int32_t readCount = AAsset_read(_asset, pBuffer, pCount);
    return (readCount == pCount) ?1 :0;
}

AssetDescriptor Asset::descriptor() {
    AssetDescriptor lDescriptor = { -1, 0, 0 };
    AAsset* lAsset = AAssetManager_open(_assetManager, _path, AASSET_MODE_UNKNOWN);
    if (lAsset != NULL) {
        lDescriptor.descriptor = AAsset_openFileDescriptor(lAsset, &lDescriptor.start, &lDescriptor.length);
        AAsset_close(lAsset);
    }
    return lDescriptor;
 }


bool Asset::operator==(const Asset& pOther) {
    return !strcmp(_path, pOther._path);
}

off_t Asset::getLength() {
    return AAsset_getLength(_asset);
}
