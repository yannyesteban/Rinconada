//
// Created by yanny on 22/7/2019.
//

#ifndef RINCONADA_LOADPNG_H
#define RINCONADA_LOADPNG_H


#include <cstring>
struct dinfo{
    AAsset* mAsset;
};
struct text_png{
    GLuint texture;
    GLint format;
    png_byte* image;
    png_uint_32 width;
    png_uint_32 height;

};

struct PNG_PROP{
    GLint format;

    png_byte* image;
    png_uint_32 width;
    png_uint_32 height;
};
void callback_readPng(png_structp pStruct, png_bytep pData, png_size_t pSize) {
    //LOGW("xxx2.0 callback size:%d", pSize);
    dinfo * d = ((dinfo*)png_get_io_ptr(pStruct));
    //LOGW("xxx2.1 callback size:%d", pSize);

    int32_t readCount = AAsset_read(d->mAsset, pData, pSize);
    //LOGW("xxx2.2 callback size:%d", pSize);
    if(readCount == pSize) {
       // LOGW("xxx error psize readcount %d", readCount);
        //ERROR
    };
    //LOGW("xxx2.3 callback size:%d", pSize);

    if (d->mAsset != NULL) {
        //LOGW("xxx2.4 callback size:%d", pSize);
        //AAsset_close(d->mAsset);
        //d->mAsset = NULL;
    }
    //LOGW("xxx2.5 callback OUT size:%d", pSize);
}
PNG_PROP readPNG(AAssetManager* mAssetManager, const char * pPath, PNG_PROP & PNG){


    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, pPath, AASSET_MODE_UNKNOWN);

    if(mAsset!= NULL){
        //LOGW("------------COOOOOLLLLL----------");
    }else{
        //LOGW("ERRRRRRRRRRRRRRROOORRRRRRRRR");
    }

    off_t outStart;

    off_t fileLength = AAsset_getLength(mAsset);
    int descriptor = AAsset_openFileDescriptor(mAsset, &outStart, &fileLength);
//LOGW("xxx longitud: %d", fileLength);

    GLuint texture;
    GLint format;

    png_byte header[8];
    png_structp pngPtr = NULL;
    png_infop infoPtr = NULL;
    png_byte* image = NULL;
    png_bytep* rowPtrs = NULL;
    png_int_32 rowSize;
    bool transparency;

    /* leer el encabezado*/
    dinfo d;
    d.mAsset = mAsset;
    int32_t readCount = AAsset_read(d.mAsset, header, sizeof(header));
    if (png_sig_cmp(header, 0, 8) != 0){
        //LOGE("ERROR");

    }else{
        // LOGW("xxx 8 header bien");
    }


    //LOGW("xxx  Creating required structures.");
    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr) {
        //ERROR
    }
    infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) {
        //ERROR
    }
    // Prepares reading operation by setting-up a read callback.
    png_set_read_fn(pngPtr, &d, callback_readPng);

    if (setjmp(png_jmpbuf(pngPtr))) {
        //ERROR
    };



    // Ignores first 8 bytes already read.


    png_set_sig_bytes(pngPtr, 8);

    // Retrieves PNG info and updates PNG struct accordingly.
    png_read_info(pngPtr, infoPtr);
    png_int_32 depth, colorType;
    png_uint_32 width, height;

    //LOGW("xxx hasta aqui tres3");
    //return;
    png_get_IHDR(pngPtr, infoPtr, &width, &height,
                 &depth, &colorType, NULL, NULL, NULL);

    // Creates a full alpha channel if transparency is encoded as
    // an array of palette entries or a single transparent color.
    transparency = false;
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pngPtr);
        transparency = true;
    }

    // Expands PNG with less than 8bits per channel to 8bits.
    if (depth < 8) {
        png_set_packing (pngPtr);
        // Shrinks PNG with 16bits per color channel down to 8bits.
    } else if (depth == 16) {
        png_set_strip_16(pngPtr);
    }

    //LOGW("xxx hasta aqui cuatro");

    // Indicates that image needs conversion to RGBA if needed.
    switch (colorType) {
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB:
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            format = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_GRAY:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = transparency ? GL_LUMINANCE_ALPHA:GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_GA:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = GL_LUMINANCE_ALPHA;
            break;
    }
    // Validates all transformations.
    //LOGW("xxx hasta aqui cinco");
    png_read_update_info(pngPtr, infoPtr);
    /******************/
    // Get row size in bytes.
    rowSize = png_get_rowbytes(pngPtr, infoPtr);
    if (rowSize <= 0) {

        //error
    }
    // Ceates the image buffer that will be sent to OpenGL.
    image = new png_byte[rowSize * height];
    if (!image) {

        //ERROR;
    }
    // Pointers to each row of the image buffer. Row order is
    // inverted because different coordinate systems are used by
    // OpenGL (1st pixel is at bottom left) and PNGs (top-left).
    //LOGW("xxx hasta aqui seis");
    rowPtrs = new png_bytep[height];
    if (!rowPtrs) {

        //ERROR
    }
    for (int32_t i = 0; i < height; ++i) {
        rowPtrs[height - (i + 1)] = image + i * rowSize;
    }
    //LOGW("xxx hasta aqui siete");
    // Reads image content.
    png_read_image(pngPtr, rowPtrs);

    if (mAsset != NULL) {
        AAsset_close(mAsset);
        mAsset = NULL;
    }

    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    delete[] rowPtrs;
    PNG.image = image;
    PNG.width = width;
    PNG.height = height;
    PNG.format = format;

    return PNG;


}

GLuint Texture(AAssetManager* mAssetManager, const char * pPath){
    GLuint texture;
    PNG_PROP PNG;
    readPNG(mAssetManager, pPath,PNG);
    //LOGW("xxx hasta aqui ocho");
    GLenum errorResult;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set-up texture properties.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Loads image data into OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, PNG.format, PNG.width, PNG.height, 0, PNG.format,
                 GL_UNSIGNED_BYTE, PNG.image);
    // Finished working with the texture.
    //glBindTexture(GL_TEXTURE_2D, 0);
    delete[] PNG.image;
    if (glGetError() != GL_NO_ERROR) {
        //error
    }
    //LOGW("Texture size: %d x %d", width, height);
    return texture;
}






void loadTexture1(AAssetManager* mAssetManager, const char * pPath, text_png & PNG){


    AAsset* mAsset;
    mAsset = AAssetManager_open(mAssetManager, pPath, AASSET_MODE_UNKNOWN);

    if(mAsset!= NULL){
        //LOGW("------------COOOOOLLLLL----------");
    }else{
        //LOGW("ERRRRRRRRRRRRRRROOORRRRRRRRR");
    }

    off_t outStart;

    off_t fileLength = AAsset_getLength(mAsset);
    int descriptor = AAsset_openFileDescriptor(mAsset, &outStart, &fileLength);
//LOGW("xxx longitud: %d", fileLength);

    GLuint texture;
    GLint format;

    png_byte header[8];
    png_structp pngPtr = NULL;
    png_infop infoPtr = NULL;
    png_byte* image = NULL;
    png_bytep* rowPtrs = NULL;
    png_int_32 rowSize;
    bool transparency;

    /* leer el encabezado*/
    dinfo d;
    d.mAsset = mAsset;
    int32_t readCount = AAsset_read(d.mAsset, header, sizeof(header));
    if (png_sig_cmp(header, 0, 8) != 0){
        //LOGE("ERROR");

    }else{
       // LOGW("xxx 8 header bien");
    }


    //LOGW("xxx  Creating required structures.");
    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr) {
        //ERROR
    }
    infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) {
        //ERROR
    }
    // Prepares reading operation by setting-up a read callback.
    png_set_read_fn(pngPtr, &d, callback_readPng);

    if (setjmp(png_jmpbuf(pngPtr))) {
        //ERROR
    };



    // Ignores first 8 bytes already read.


    png_set_sig_bytes(pngPtr, 8);

    // Retrieves PNG info and updates PNG struct accordingly.
    png_read_info(pngPtr, infoPtr);
    png_int_32 depth, colorType;
    png_uint_32 width, height;

    //LOGW("xxx hasta aqui tres3");
    //return;
    png_get_IHDR(pngPtr, infoPtr, &width, &height,
                 &depth, &colorType, NULL, NULL, NULL);

    // Creates a full alpha channel if transparency is encoded as
    // an array of palette entries or a single transparent color.
    transparency = false;
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pngPtr);
        transparency = true;
    }

    // Expands PNG with less than 8bits per channel to 8bits.
    if (depth < 8) {
        png_set_packing (pngPtr);
    // Shrinks PNG with 16bits per color channel down to 8bits.
    } else if (depth == 16) {
        png_set_strip_16(pngPtr);
    }

    //LOGW("xxx hasta aqui cuatro");

    // Indicates that image needs conversion to RGBA if needed.
    switch (colorType) {
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB:
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            format = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_GRAY:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = transparency ? GL_LUMINANCE_ALPHA:GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_GA:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = GL_LUMINANCE_ALPHA;
            break;
    }
    // Validates all transformations.
    //LOGW("xxx hasta aqui cinco");
    png_read_update_info(pngPtr, infoPtr);
    /******************/
    // Get row size in bytes.
    rowSize = png_get_rowbytes(pngPtr, infoPtr);
    if (rowSize <= 0) {

        //error
    }
    // Ceates the image buffer that will be sent to OpenGL.
    image = new png_byte[rowSize * height];
    if (!image) {

        //ERROR;
    }
    // Pointers to each row of the image buffer. Row order is
    // inverted because different coordinate systems are used by
    // OpenGL (1st pixel is at bottom left) and PNGs (top-left).
    //LOGW("xxx hasta aqui seis");
    rowPtrs = new png_bytep[height];
    if (!rowPtrs) {

        //ERROR
    }
    for (int32_t i = 0; i < height; ++i) {
        rowPtrs[height - (i + 1)] = image + i * rowSize;
    }
    //LOGW("xxx hasta aqui siete");
    // Reads image content.
    png_read_image(pngPtr, rowPtrs);

    if (mAsset != NULL) {
        AAsset_close(mAsset);
        mAsset = NULL;
    }

    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    delete[] rowPtrs;

    //LOGW("xxx hasta aqui ocho");
    GLenum errorResult;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set-up texture properties.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Loads image data into OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, image);
    // Finished working with the texture.
    //glBindTexture(GL_TEXTURE_2D, 0);
    delete[] image;
    if (glGetError() != GL_NO_ERROR) {
        //error
    }
    _LOGE("Texture size: %d x %d -> %d", width, height, texture);

}


bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {

    return true;
}

#endif //RINCONADA_LOADPNG_H
