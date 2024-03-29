//
// Created by yanny on 24/6/2019.
//

#include "FileRead.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <regex>
#include <glm/glm.hpp>
using namespace std;
const char * FileRead::_externalDataPath = "";
const char * FileRead::_internalDataPath = "";
AAssetManager* FileRead::_assetManager = nullptr;

const char *  FileRead::Open(AAssetManager* mAssetManager, const char * pPath){

    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, pPath, AASSET_MODE_UNKNOWN);

    if(mAsset!= NULL){
        //LOGW("------------COOOOOLLLLL----------");
    }else{
       // LOGW("ERRRRRRRRRRRRRRROOORRRRRRRRR");
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

   // LOGE("%s", externalDataPath);
    //LOGE("%s", internalDataPath);

    char aux[100];// = (char *)externalDataPath;
    strcpy(aux, externalDataPath);
    //LOGE("AUX %s", aux);
    char *paths3 = strcat((char *) internalDataPath, "/hello5.txt");

    //FILE* file = fopen("/storage/emulated/0/Android/data/com.stable.rinconada/files/hello.txt","w+");
    //FILE* file = fopen("//storage/emulated/0/txt/newfile.txt","w+");

    char *paths4 = strcat((char *) externalDataPath, "/storage/18EE-0616/txt/newfile.txt");
    //LOGE("AUX %s", aux);
    FILE *file = fopen(internalDataPath, "w+");
    if (file != NULL) {
        fputs("HOLA MUNDO 8 veces!\n", file);
        fputs("HOLA MUNDO 9 veces!\n", file);
        fputs("HOLA MUNDO 10 veces!\n", file);
        fflush(file);
        fclose(file);
        //LOGE("SIP");
    } else {

        //LOGE("NOP");
    }

   //LOGD("QUEEEEEEEEEE");
    AAsset *mAsset;
    mAsset = AAssetManager_open(mAssetManager, "hola.txt", AASSET_MODE_UNKNOWN);
    off_t outStart;
    //off_t fileLength = AAsset_getLength(mAsset);
    off_t fileLength = AAsset_getLength(mAsset);
    int h = AAsset_openFileDescriptor(mAsset, &outStart, &fileLength);

    int g = dup(h);
    lseek(g, outStart, SEEK_CUR); //NOTICE

    //LOGE("fileLength %d", fileLength);

    char *dataBuffer2 = (char *) malloc(fileLength);
    memset(dataBuffer2, 0, fileLength);
    read(g, dataBuffer2, fileLength);
    //LOGE("un buffer es %s, tamaño (%d)", dataBuffer2, strlen(dataBuffer2));

    if (g) {
       // LOGE("si G");
    } else{
        //LOGE("NO G");
    }
    FILE *fp = NULL;

    FILE * ffpp = fdopen(g,"rb");
    if (ffpp){
        //always enters here
        //LOGE("Bien 8000");
    } else{
        //never enters here
       // LOGE("Nada X");
    }



    /************************/

    AAssetDir* assetDir = AAssetManager_openDir(mAssetManager, "");
    const char* filename = (const char*)NULL;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
        char * paths8 =strcat((char *)aux,filename);

        AAsset* asset = AAssetManager_open(mAssetManager, filename, AASSET_MODE_STREAMING);
        char buf[BUFSIZ];
        int nb_read = 0;

        FILE* out = fopen(paths8, "w+");
        //LOGE("%s", paths8);
        while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
            fwrite(buf, nb_read, 1, out);
        fclose(out);

        AAsset_close(asset);
    }
    AAssetDir_close(assetDir);


    /*************************/




    char *dataBuffer = (char *) malloc(fileLength);

    AAsset_read(mAsset, dataBuffer, fileLength);
    //LOGE("data buffer: %s", dataBuffer);
    if (NULL == mAsset) {
        //LOGE("No asset");
    } else{
       // LOGE("si  asset");
    }



    //fp = fopen( internalDataPath,"rb+");
    fp = fopen( "assets/hola.txt","rb+");
    if (fp){
        //always enters here

        char  linea[20];
        size_t * lon;
       //read(fp,linea,lon);
       while(fgets(linea,20,fp)!=NULL){
        //LOGE("*******%s",linea);
       }

fputs("yanny esteban", fp);
        //LOGE("MAL 2200X");
    } else{
        //never enters here
        //LOGE("BIEN 2200X");
    }
//LOGE ("END....");
    //FILE f = fopen("/data/data/<application name>/<file name>", "r");
fclose(fp);
   // fclose(ffpp);.
    //LOGE("%s",internalDataPath);
}

void FileRead::AA() {

//LOGE("%d", FileRead::s_value);
}

int FileRead::s_value=4;

void FileRead::setAssetManager(AAssetManager *mmAssetManager) {
    FileRead::_assetManager = mmAssetManager;
}


int FileRead::print(const char *path, GMMesh* mesh) {
    AAsset *mAsset;
    mAsset = AAssetManager_open(_assetManager, path, AASSET_MODE_UNKNOWN);
    off_t outStart;
    off_t fileLength = AAsset_getLength(mAsset);

    char *dataBuffer = (char *) malloc(fileLength);

    AAsset_read(mAsset, dataBuffer, fileLength);


    std::stringstream ss(dataBuffer);
    std::string to;
    //_LOGE("FILEREAD uno");
    if (dataBuffer != NULL)
    {
        //_LOGE("FILEREAD dos");
        while (std::getline(ss, to)) {//, '\n'
            //LOGE("tres");
            char lineHeader[128];
            // Lee la primera palabra de la línea

            //_LOGE("FILEREAD .%s", to.c_str());
            //std::cout << to.c_str() << std::endl;
            //cout << "x: " << to << endl;

            sscanf(to.c_str(), "%s", lineHeader);
            //const char * xx, *xy, *xz;

            //LOGE("%s", to.c_str());
            if ( strcmp( lineHeader, "v" ) == 0 ){
                glm::vec3 vertex;
                sscanf(to.c_str(), "%*s %f %f %f", &vertex.x, &vertex.y, &vertex.z );
                mesh->v.push_back(vertex);
            }else if(strcmp( lineHeader, "vn" ) == 0){
                glm::vec3 vertex;
                sscanf(to.c_str(), "%*s %f %f %f", &vertex.x, &vertex.y, &vertex.z );
                mesh->n.push_back(vertex);
            }else if(strcmp( lineHeader, "vt" ) == 0){
                glm::vec2 vertex;
                sscanf(to.c_str(), "%*s %f %f", &vertex.x, &vertex.y);
                mesh->t.push_back(vertex);

            }else if(strcmp( lineHeader, "f" ) == 0){

                GLushort vIndex[3], uvIndex[3], nIndex[3];
                sscanf(to.c_str(), "%*s %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu",
                       &vIndex[0], &uvIndex[0], &nIndex[0],
                       &vIndex[1], &uvIndex[1], &nIndex[1],
                       &vIndex[2], &uvIndex[2], &nIndex[2]);
                GLushort delta=1;
                mesh->vi.push_back(vIndex[0]-delta);
                mesh->vi.push_back(vIndex[1]-delta);
                mesh->vi.push_back(vIndex[2]-delta);

                mesh->ti.push_back(uvIndex[0]-delta);
                mesh->ti.push_back(uvIndex[1]-delta);
                mesh->ti.push_back(uvIndex[2]-delta);

                mesh->ni.push_back(nIndex[0]-delta);
                mesh->ni.push_back(nIndex[1]-delta);
                mesh->ni.push_back(nIndex[2]-delta);


            }


        }
    }







    free(dataBuffer);
    return 0;
}



void FileRead::setDataPath(const char *internalDataPath, const char *externalDataPath) {
    _internalDataPath=internalDataPath;
    _externalDataPath=externalDataPath;
}
