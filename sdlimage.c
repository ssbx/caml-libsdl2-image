/* OCamlSDL2_Image - An OCaml interface to the SDL2_Image library
 Copyright (C) 2013 Florent Monnier

 This software is provided "AS-IS", without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from
 the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely.
*/
#define CAML_NAME_SPACE
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>

#include <SDL_image.h>
#include "sdlrwops.h"
#include "sdltexture.h"
#include "sdlsurface.h"

static inline IMG_InitFlags
IMG_InitFlag_ml2c(value v)
{
    if (v == caml_hash_variant("JPG"))  return IMG_INIT_JPG;
    if (v == caml_hash_variant("PNG"))  return IMG_INIT_PNG;
    if (v == caml_hash_variant("TIF"))  return IMG_INIT_TIF;
    if (v == caml_hash_variant("WEBP")) return IMG_INIT_WEBP;
    caml_failwith("Sdlimage.init_flags");
}

static inline IMG_InitFlags
IMG_InitFlags_val(value flag_list)
{
    IMG_InitFlags c_mask = 0;
    while (flag_list != Val_emptylist)
    {
        value head = Field(flag_list, 0);
        flag_list = Field(flag_list, 1);
        c_mask |= IMG_InitFlag_ml2c(head);
    }
    return c_mask;
}

CAMLprim value
caml_SDL_IMG_Init(value flags)
{
    IMG_InitFlags c_flags = IMG_InitFlags_val(flags);
    int si = IMG_Init(c_flags);
    if (si == 0) caml_failwith("Sdlimage.init");
    //if (si != c_flags) caml_failwith("Sdlimage.init: "
    //    "uncomplete initialisation");
    return Val_unit;
}

CAMLprim value
caml_SDL_IMG_Quit(value unit)
{
    IMG_Quit();
    return Val_unit;
}

/* Functions to detect a file type, given a seekable source */

CAMLprim value caml_SDL_IMG_isICO(value img) { return Val_bool(IMG_isICO(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isCUR(value img) { return Val_bool(IMG_isCUR(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isBMP(value img) { return Val_bool(IMG_isBMP(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isGIF(value img) { return Val_bool(IMG_isGIF(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isJPG(value img) { return Val_bool(IMG_isJPG(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isLBM(value img) { return Val_bool(IMG_isLBM(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isPCX(value img) { return Val_bool(IMG_isPCX(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isPNG(value img) { return Val_bool(IMG_isPNG(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isPNM(value img) { return Val_bool(IMG_isPNM(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isTIF(value img) { return Val_bool(IMG_isTIF(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isXCF(value img) { return Val_bool(IMG_isXCF(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isXPM(value img) { return Val_bool(IMG_isXPM(SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isXV (value img) { return Val_bool(IMG_isXV (SDL_RWops_val(img))); }
CAMLprim value caml_SDL_IMG_isWEBP(value img) { return Val_bool(IMG_isWEBP(SDL_RWops_val(img))); }

CAMLprim value
caml_SDL_IMG_GetCompiledVersion(value unit)
{
    CAMLparam0();
    CAMLlocal1(ret);

    SDL_version img_version;
    SDL_IMAGE_VERSION(&img_version);

    ret = caml_alloc(3, 0);
    Store_field(ret, 0, Val_int(img_version.major));
    Store_field(ret, 1, Val_int(img_version.minor));
    Store_field(ret, 2, Val_int(img_version.patch));
    CAMLreturn(ret);
}

CAMLprim value
caml_SDL_IMG_GetLinkedVersion(value unit)
{
    CAMLparam0();
    CAMLlocal1(ret);

    const SDL_version *img_version;
    img_version = IMG_Linked_Version();

    ret = caml_alloc(3, 0);
    Store_field(ret, 0, Val_int(img_version->major));
    Store_field(ret, 1, Val_int(img_version->minor));
    Store_field(ret, 2, Val_int(img_version->patch));
    CAMLreturn(ret);
}

CAMLprim value
caml_SDL_IMG_LoadPNG_RW(value img)
{
    SDL_Surface *surf =
        IMG_LoadPNG_RW(
            SDL_RWops_val(img));

    if (!surf) caml_failwith("Sdlimage.load_png_rw");

    return Val_SDL_Surface(surf);
}

#define caml_SDL_IMG_LoadT_RW(T, k) \
  CAMLprim value \
  caml_SDL_IMG_Load##T##_RW(value img) { \
      SDL_Surface *surf = \
        IMG_Load##T##_RW( \
          SDL_RWops_val(img)); \
      if (!surf) caml_failwith("Sdlimage.load_" #k "_rw"); \
      return Val_SDL_Surface(surf); \
  }

caml_SDL_IMG_LoadT_RW(ICO, ico)
caml_SDL_IMG_LoadT_RW(CUR, cur)
caml_SDL_IMG_LoadT_RW(BMP, bmp)
caml_SDL_IMG_LoadT_RW(GIF, gif)
caml_SDL_IMG_LoadT_RW(JPG, jpg)
caml_SDL_IMG_LoadT_RW(LBM, lbm)
caml_SDL_IMG_LoadT_RW(PCX, pcx)
caml_SDL_IMG_LoadT_RW(PNM, pnm)
caml_SDL_IMG_LoadT_RW(TGA, tga)
caml_SDL_IMG_LoadT_RW(TIF, tif)
caml_SDL_IMG_LoadT_RW(XCF, xcf)
caml_SDL_IMG_LoadT_RW(XPM, xpm)
caml_SDL_IMG_LoadT_RW(XV, xv)
caml_SDL_IMG_LoadT_RW(WEBP, webp)

CAMLprim value
caml_SDL_IMG_SavePNG(value surf, value filename)
{
    int r =
        IMG_SavePNG(
            SDL_Surface_val(surf),
            String_val(filename));

    if (r) caml_failwith("Sdlimage.save_png");

    return Val_unit;
}

/* vim: set ts=4 sw=4 et: */
