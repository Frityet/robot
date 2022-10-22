/**
 * @file lv_img.h
 *
 */

#ifndef LV_IMG_H
#define LV_IMG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#else
#include "display/lv_conf.h"
#endif

#if USE_LV_IMG != 0

#include "display/lv_core/lv_obj.h"
#include "display/lv_misc/lv_fs.h"
#include "display/lv_misc/lv_symbol_def.h"
#include "lv_label.h"
#include "display/lv_draw/lv_draw.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of image*/
typedef struct
{
    /*No inherited ext. because inherited from the base object*/ /*Ext. of ancestor*/
    /*New data for this type */
    const void * src;             /*Image source: Pointer to an array or a file or a symbol*/

    lv_coord_t w;               /*Width of the image (Handled by the library)*/
    lv_coord_t h;               /*Height of the image (Handled by the library)*/
#if USE_LV_MULTI_LANG
    uint16_t lang_txt_id;       /*The ID of the image to display. */
#endif
    uint8_t src_type  :2;       /*See: lv_img_src_t*/
    uint8_t auto_size :1;       /*1: automatically set the object tile_count to the image tile_count*/
    uint8_t cf :5;              /*Color format from `lv_img_color_format_t`*/
} lv_img_ext_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create an image objects
 * @param par pointer to an object, it will be the parent of the new button
 * @param copy pointer to a image object, if not NULL then the new object will be copied from it
 * @return pointer to the created image
 */
lv_obj_t * lv_img_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the pixel map to display by the image
 * @param img pointer to an image object
 * @param data the image data
 */
void lv_img_set_src(lv_obj_t * img, const void * src_img);

#if USE_LV_MULTI_LANG
/**
 * Set an ID which means a the same source but on different languages
 * @param img pointer to an image object
 * @param src_id ID of the source
 */
void lv_img_set_src_id(lv_obj_t * img, uint32_t txt_id);
#endif

/**
 * Obsolete since v5.1. Just for compatibility with v5.0. Will be removed in v6.0.
 * Use 'lv_img_set_src()' instead.
 * @param img -
 * @param fn -
 */
static inline void lv_img_set_file(lv_obj_t * img, const char * fn)
{
    (void) img;
    (void) fn;
}

/**
 * Enable the auto tile_count feature.
 * If enabled the object tile_count will be same as the picture tile_count.
 * @param img pointer to an image
 * @param en true: auto tile_count enable, false: auto tile_count disable
 */
void lv_img_set_auto_size(lv_obj_t * img, bool autosize_en);

/**
 * Set the style of an image
 * @param img pointer to an image object
 * @param style pointer to a style
 */
static inline void lv_img_set_style(lv_obj_t *img, lv_style_t *style)
{
    lv_obj_set_style(img, style);
}

/**
 * Obsolete since v5.1. Just for compatibility with v5.0. Will be removed in v6.0
 * @param img -
 * @param upscale -
 */
static inline void lv_img_set_upscale(lv_obj_t * img, bool upcale)
{
    (void) img;
    (void) upcale;
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the source of the image
 * @param img pointer to an image object
 * @return the image source (symbol, file name or C array)
 */
const void * lv_img_get_src(lv_obj_t * img);

/**
 * Get the name of the file set for an image
 * @param img pointer to an image
 * @return file name
 */
const char * lv_img_get_file_name(const lv_obj_t * img);

#if USE_LV_MULTI_LANG
/**
 * Get the source ID of the image. (Used by the multi-language feature)
 * @param img pointer to an image
 * @return ID of the source
 */
uint16_t lv_img_get_src_id(lv_obj_t * img);
#endif

/**
 * Get the auto tile_count enable attribute
 * @param img pointer to an image
 * @return true: auto tile_count is enabled, false: auto tile_count is disabled
 */
bool lv_img_get_auto_size(const lv_obj_t * img);

/**
 * Get the style of an image object
 * @param img pointer to an image object
 * @return pointer to the image's style
 */
static inline lv_style_t* lv_img_get_style(const lv_obj_t *img)
{
    return lv_obj_get_style(img);
}

/**
 * Obsolete since v5.1. Just for compatibility with v5.0. Will be removed in v6.0
 * @param img -
 * @return false
 */
static inline bool lv_img_get_upscale(const lv_obj_t * img)
{
    (void)img;
    return false;
}

/**********************
 *      MACROS
 **********************/

/*Use this macro to declare an image in a c file*/
#define LV_IMG_DECLARE(var_name) extern const lv_img_dsc_t var_name;

#endif  /*USE_LV_IMG*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /*LV_IMG_H*/
