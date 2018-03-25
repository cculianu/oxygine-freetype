#include "FontEffects.h"
#include "ResFontFT.h"
#include "fe/fe.h"
#include "ox/Font.hpp"
#include "ox/Image.hpp"

void* _fe_alloc(size_t size)
{
    return malloc(size);
}

void _fe_free(void *ptr)
{
    free(ptr);
}

namespace oxfe
{
    fe_node* out_nodes[32];
    using namespace oxygine;

    void glyphProcessor(ResFontFT::postProcessData &p)
    {
        if (!p.opt)
        {
            ftGenDefault(p);
            return;
        }

        fe_node *node = out_nodes[p.opt];
        if (!node)
        {
            ftGenDefault(p);
            return;
        }

        fe_im im;
        fe_im res;

        int sz = p.font->getSize();
        im.x = p.gl->offset_x;
        im.y = -p.gl->offset_y;

        im.image.bytespp = p.src->bytespp;
        im.image.data = p.src->data;
        im.image.pitch = p.src->pitch;
        im.image.w = p.src->w;
        im.image.h = p.src->h;
        im.image.format = FE_IMG_A8;
        im.image.free = 0;

        fe_node_apply2(p.font->getSize(), &im, node, &res);

        Image& dest = *p.dest;

        ImageData src;
        src.data = res.image.data;
        src.bytespp = res.image.bytespp;
        src.w = res.image.w;
        src.h = res.image.h;
        src.format = TF_R8G8B8A8;
        src.pitch = res.image.pitch;

        dest.init(src);

        p.gl->offset_x = res.x;
        p.gl->offset_y = -res.y;

        fe_image_free(&res.image);
    }


    void init() 
    {
        ResFontFT::setGlyphPostProcessor(glyphProcessor);
    }
}