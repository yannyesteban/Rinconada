//
// Created by yanny on 24/6/2019.
//

#include "FileRead.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <android/asset_manager_jni.h>
using namespace std;

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

void FileRead::test(AAssetManager *mAssetManager, const char * externalDataPath, const char * internalDataPath) {

    LOGE("%s", externalDataPath);
    LOGE("%s", internalDataPath);

    char * paths3=strcat((char *)internalDataPath,"/hello5.txt");

    //FILE* file = fopen("/storage/emulated/0/Android/data/com.stable.rinconada/files/hello.txt","w+");
    //FILE* file = fopen("//storage/emulated/0/txt/newfile.txt","w+");

    char * paths4 =strcat((char *)externalDataPath,"/storage/18EE-0616/txt/newfile.txt");

    FILE * file = fopen(internalDataPath, "w+");
    if (file != NULL)
    {
        fputs("HOLA MUNDO 18 veces!\n", file);
        fputs("HOLA MUNDO 19 veces!\n", file);
        fputs("HOLA MUNDO 20 veces!\n", file);
        fflush(file);
        fclose(file);
        LOGE("SIP");
    }else{

        LOGE("NOP");
    }

LOGD("QUEEEEEEEEEE");
    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, "hola.txt", AASSET_MODE_UNKNOWN);
    off_t outStart, fileLength;
    //off_t fileLength = AAsset_getLength(mAsset);
    //off_t fileLength = AAsset_getLength(mAsset);
    int g = AAsset_openFileDescriptor(mAsset,&outStart,&fileLength);
    //int myfd = dup(g);

    FILE * ffpp;
    ffpp = fdopen(g, "rb");
    if (ffpp != NULL){
        //always enters here
        LOGE("Bien 8000");
    } else{
        //never enters here
        LOGE("Nada X");
    }


    AAsset_close(mAsset);

    //LOGE("fileLength %d", fileLength);

    FILE *fp = NULL;

    char  buf[50]="";
    size_t count;
    //ssize_t read(g, buf, count);




return;

    char *dataBuffer = (char *) malloc(fileLength);

    AAsset_read(mAsset, dataBuffer, fileLength);
    LOGE("data buffer: %s", dataBuffer);
    if (NULL == mAsset) {
        LOGE("No asset");
    } else{
        LOGE("si  asset");
    }



    fp = fopen( internalDataPath,"rb+");
    if (fp){
        //always enters here

        char  linea[20];
        size_t * lon;
       //read(fp,linea,lon);
       while(fgets(linea,20,fp)!=NULL){
        LOGE("%s",linea);
       }

fputs("yanny esteban", fp);
        LOGE("MAL 2200X");
    } else{
        //never enters here
        LOGE("BIEN 2200X");
    }
LOGE ("END....");
    //FILE f = fopen("/data/data/<application name>/<file name>", "r");
fclose(fp);
   // fclose(ffpp);.
    LOGE("%s",internalDataPath);
}
