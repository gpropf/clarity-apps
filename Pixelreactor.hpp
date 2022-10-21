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
        HybridNode<double> *maindiv = builder.build();

        printf("Setup complete!\n");
        return maindiv;
    }
};

#endif
