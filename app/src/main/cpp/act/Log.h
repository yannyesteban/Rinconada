//
// Created by yanny on 17/6/2019.
//

#ifndef RINCONADA_LOG_H
#define RINCONADA_LOG_H


#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#endif //RINCONADA_LOG_H
