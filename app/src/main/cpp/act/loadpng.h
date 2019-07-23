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
    //LOGW("xxx hasta aqui nueve");
    // Loads image data into OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, image);
    // Finished working with the texture.
    //glBindTexture(GL_TEXTURE_2D, 0);
    delete[] image;
    if (glGetError() != GL_NO_ERROR) {
        //error
    }
    //LOGW("Texture size: %d x %d", width, height);

}


bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;

    if ((fp = fopen(name, "rb")) == NULL)
        return false;

    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);

    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;

    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int i = 0; i < outHeight; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);

    /* That's it */
    return true;
}

#endif //RINCONADA_LOADPNG_H
