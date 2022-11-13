#ifndef Pixelreactor_hpp
#define Pixelreactor_hpp

#include "CLNodeFactory.hpp"
#include "CanvasElement.hpp"
#include "ClarityNode.hpp"
#include "clarity.hpp"
#include "globals.hpp"

using namespace clarity;

/**
 * @brief The Pixelreactor app from the old ClojureScript site redone in C++.
 *
 */
struct PixelReactor : public PageContent {

    
    
    
    
    int *ruleFrameWidth = new int(5);
    int *ruleFrameHeight = new int(3);


    ClarityNode *content(ClarityNode *innerContent = nullptr) {
        ClarityNode::addJSAuxScript("clapps-aux.js");
        ClarityNode::runJSAuxScripts();
        
        val CLElement = val::global("CLElement");
        val blackbody_st = ClarityNode::JSProxyNode_["blackbody_st"];
        CLNodeFactory<HybridNode, double, double> builder("div", "maindiv");
        auto *maindiv = builder.build();

        CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(maindiv));
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(maindiv));
        //builder = builder->

        CLNodeFactory<HybridNode, unsigned char, double> canvasBuilder(builder);

        CanvasGrid<unsigned char> *canvas1 =
            canvasBuilder.withName("canvas1")
                .withTag("canvas")
                .withAttributes({{"style", val("border: 1px solid green")},
                                 {"width", val(600)},
                                 {"height", val(400)}})
                .canvasGrid(60, 40, 600, 400);

        canvas1->setCurrentCellVal(4);

        auto *canvas1CurrentCellColor_tinp =
            canvasBuilder.withName("currentCellColor_tinp")
                .withCppVal(canvas1->getPtr2CurrentCellVal())
                .withAttributes({{"style", val("border: 3px dashed purple")}, {"size", val(2)}})
                .textInput();

        maindiv->appendChild(canvas1);
        maindiv->appendChild(canvas1CurrentCellColor_tinp);
        builder.br();

        //CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(maindiv));
        string *cmdarea_text = new string("This is a textarea.");
        auto *cmdarea = textBuilder.withName("cmdarea").textarea(cmdarea_text, 6, 80);
        textBuilder.br();
        auto *cmdarea_lbl = textBuilder.label(cmdarea, "CMD:", true);
        textBuilder.br();

        auto * ruleFrameWidth_tinp = intBuilder.withName("ruleFrameWidth_tinp").withCppVal(ruleFrameWidth).textInput();
        auto * ruleFrameWidth_tinp_lbl = intBuilder.label(ruleFrameWidth_tinp, "Width of new rule frames.", true);
        auto * ruleFrameHeight_tinp = intBuilder.withName("ruleFrameHeight_tinp").withCppVal(ruleFrameHeight).textInput();
        auto * ruleFrameHeight_tinp_lbl = intBuilder.label(ruleFrameHeight_tinp, "Height of new rule frames.", true);



        textBuilder.br();

        printf("Setup complete!\n");
        return maindiv;
    }
};

#endif
