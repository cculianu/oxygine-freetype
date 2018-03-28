#include "oxygine-framework.h"
#include <functional>
#include "ResFontFT.h"
#include "FontEffects.h"
#include "fe/fe.h"
#include "test.h"

using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;


fe_bundle *bundle = 0;

enum {
    EFFECT_A = 1,
    EFFECT_B
};

void example_preinit() {}


class TestActor : public Test
{
public:

    spTextField _text;
    TestActor()
    {

        addButton("scale+", "scale+");
        addButton("scale-", "scale-");
        addButton("ne", "next effect");

        //create TextField Actor
        _text = new TextField();
        //attach it as child to button
        _text->attachTo(content);
        //centered
        //_text->setX(content->getWidth() / 4);
        _text->setY(content->getHeight() / 2);

        //initialize text style
        TextStyle style;
        style.font = gameResources.getResFont("main");
        style.fontSize = 80;

        style.vAlign = TextStyle::VALIGN_BOTTOM;
        style.hAlign = TextStyle::HALIGN_LEFT;
        style.baselineScale = 0.7f;

        //apply our custom option
        style.options = EFFECT_A;

        _text->setStyle(style);
    }

    void doUpdate(const UpdateState& us)
    {
        char txt[255];
        safe_sprintf(txt, " <div opt='2'>tm:</div> %d", getTimeMS());
        _text->setHtmlText(txt);
    }
    void clicked(string id)
    {
        if (id == "scale+")
        {
            _text->setScale(_text->getScaleX() + 0.05f);
        }
        if (id == "scale-")
        {
            _text->setScale(_text->getScaleX() - 0.05f);
        }

        if (id == "ne")
        {
            static int i = 0;
            ++i;
            fe_effect *effect = fe_bundle_get_effect(bundle, i % bundle->num);
            oxfe::out_nodes[EFFECT_A] = fe_effect_find_node_by_type(effect, fe_node_type_out);      

            ((ResFontFT*)_text->getFont())->clear();
            _text->setText("");
        }

    }
};


//called from entry_point.cpp
void example_init()
{
    ResFontFT::initLibrary();
    ResFontFT::setAtlasSize(2048, 1024);
    
    oxfe::init();

    //load xml file with resources definition
    gameResources.loadXML("res.xml");


    file::buffer buf;
    file::read("fonts/example.fe", buf);
    bundle = fe_bundle_load(buf.getData(), buf.getSize());

    fe_effect *effect;

    effect = fe_bundle_get_effect_by_name(bundle, "beta");
    oxfe::out_nodes[EFFECT_B] = fe_effect_find_node_by_type(effect, fe_node_type_out);

    effect = fe_bundle_get_effect_by_name(bundle, "kappa");
    oxfe::out_nodes[EFFECT_A] = fe_effect_find_node_by_type(effect, fe_node_type_out);

    Test::init();

    Test::instance = new TestActor;
    getStage()->addChild(Test::instance);
}


//called each frame from entry_point.cpp
void example_update()
{
}

//called each frame from entry_point.cpp
void example_destroy()
{
    fe_bundle_free(bundle);

    //free previously loaded resources
    gameResources.free();
    ResFontFT::freeLibrary();
    Test::free();
}
