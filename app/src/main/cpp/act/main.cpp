//
// Created by yanny on 14/6/2019.
//
#include <android_native_app_glue.h>
//#include "Header.h"
#include "Log.h"
#include "Windows.h"
void android_main(struct android_app* app) {


    WindowInfo wInfo(app, 100, 100);

    Windows Windows(&wInfo);
    Windows.start();

}