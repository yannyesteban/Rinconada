//
// Created by yanny on 24/6/2019.
//

#include "FileRead.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

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
        fputs("HOLA MUNDO 8 veces!\n", file);
        fputs("HOLA MUNDO 9 veces!\n", file);
        fputs("HOLA MUNDO 10 veces!\n", file);
        fflush(file);
        fclose(file);
        LOGE("SIP");
    }else{

        LOGE("NOP");
    }

LOGD("QUEEEEEEEEEE");
    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, "hola.txt", AASSET_MODE_UNKNOWN);
    off_t outStart;
    //off_t fileLength = AAsset_getLength(mAsset);
    off_t fileLength = AAsset_getLength(mAsset);
    int g = AAsset_openFileDescriptor(mAsset,&outStart,&fileLength);


    LOGE("fileLength %d", fileLength);
    if (g) {
        LOGE("si G");
    } else{
        LOGE("NO G");
    }
    FILE *fp = NULL;

    FILE * ffpp = fdopen(g,"rb");
    if (ffpp){
        //always enters here
        LOGE("Bien 8000");
    } else{
        //never enters here
        LOGE("Nada X");
    }




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
