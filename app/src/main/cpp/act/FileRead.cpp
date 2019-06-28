//
// Created by yanny on 24/6/2019.
//

#include "FileRead.h"
#include <fstream>
#include <iostream>
#include <string>

const char *  FileRead::Open(AAssetManager* mAssetManager, const char * pPath){

    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, pPath, AASSET_MODE_UNKNOWN);

    if(mAsset!= NULL){
        LOGW("------------COOOOOLLLLL----------");
    }else{
        LOGW("ERRRRRRRRRRRRRRROOORRRRRRRRR");
    }

    off_t fileLength = AAsset_getLength(mAsset);
    char *dataBuffer = (char *) malloc(fileLength);

    AAsset_read(mAsset, dataBuffer, fileLength);
    //the data has been copied to dataBuffer2, so , close it
    AAsset_close(mAsset);


    //LOGW("%s",(const char*)dataBuffer);
    return dataBuffer;
    //free(dataBuffer2);

}

void FileRead::test(AAssetManager *mAssetManager, const char * paths, const char * paths2) {

    LOGE("%s", paths);
    LOGE("%s", paths2);

    char * paths3=strcat((char *)paths2,"/hello.txt");

    //FILE* file = fopen("/storage/emulated/0/Android/data/com.stable.rinconada/files/hello.txt","w+");
    //FILE* file = fopen("//storage/emulated/0/txt/newfile.txt","w+");
    FILE * file = fopen(paths3, "w+");
    if (file != NULL)
    {
        fputs("HOLA MUNDO tres veces!\n", file);
        fflush(file);
        fclose(file);
        LOGE("YESSSS");
    }else{

        LOGE("NOOOO");
    }


    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, "hola.txt", AASSET_MODE_UNKNOWN);
    off_t fileLength = AAsset_getLength(mAsset);
//AAsset_openFileDescriptor(mAsset,0,0);

    FILE *fp = NULL;


    fp = fopen( "/sdcard/txt/newfile.txt", "rb" );
    if (fp){
        //always enters here
        LOGE("MAL 2200X");
    } else{
        //never enters here
        LOGE("BIEN 2200X");
    }

    //FILE f = fopen("/data/data/<application name>/<file name>", "r");
}
