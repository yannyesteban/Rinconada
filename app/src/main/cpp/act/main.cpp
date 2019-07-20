//
// Created by yanny on 14/6/2019.
//
#include <android_native_app_glue.h>
//#include "Header.h"
#include "Log.h"
#include "Windows.h"
#include "FileRead.h"
void android_main(struct android_app* app) {


    FileRead::setAssetManager(app->activity->assetManager);
    FileRead::setDataPath(app->activity->internalDataPath, app->activity->externalDataPath);

FileRead::AA();


    FileRead F = FileRead();


    //F.print("hola.txt");
    //return;
    //F.test(app->activity->assetManager, app->activity->externalDataPath, app->activity->internalDataPath);
    //F.test(app->activity->assetManager, app->activity->internalDataPath);
//app->activity->env->

//return;
    WindowInfo wInfo(app, 100, 100);

    Windows Windows(&wInfo);
    Windows.start();



}