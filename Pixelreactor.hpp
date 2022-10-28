#ifndef Pixelreactor_hpp
#define Pixelreactor_hpp

#include "CLNodeFactory.hpp"
#include "CanvasElement.hpp"
#include "ClarityNode.hpp"
#include "clarity.hpp"

using namespace clarity;

/**
 * @brief The Pixelreactor app from the old ClojureScript site redone in C++.
 *
 */
struct PixelReactor : public PageContent {
    ClarityNode *content(ClarityNode *innerContent = nullptr) {
        val CLElement = val::global("CLElement");
        val blackbody_st = CLElement["blackbody_st"];
        CLNodeFactory<HybridNode, double, double> builder("div", "maindiv");
        auto *maindiv = builder.build();


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



        printf("Setup complete!\n");
        return maindiv;
    }
};

#endif
