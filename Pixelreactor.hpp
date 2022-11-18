#ifndef Pixelreactor_hpp
#define Pixelreactor_hpp

#include "CLNodeFactory.hpp"
#include "CanvasElement.hpp"
#include "ClarityNode.hpp"
#include "clarity.hpp"
#include "globals.hpp"

using namespace clarity;

template <typename U>
class BeakerNode;

template <typename V>
class Beaker {
   public:
    void makeNewReactionRule() { cout << "makeNewReactionRule()" << endl; }

    static void makeNewReactionRule_st(Beaker *b) { b->makeNewReactionRule(); }

   protected:
    int gridWidth = 5, gridHeight = 3;
    V *gridArray;

    template <typename U>
    friend class BeakerNode;
};

template <typename V>
class BeakerNode : public HybridNode<V> {
   public:
    BeakerNode(const string &name, const string &tag, bool useExistingDOMElement,
               ClarityNode::AttachmentMode attachmentMode = ClarityNode::AttachmentMode::NEW,
               const string &attachmentId = "")
        : HybridNode<V>(name, tag, useExistingDOMElement, attachmentMode, attachmentId) {}

    virtual void finalize() {
        val JSProxyNode = val::global("JSProxyNode");
        val doNothing = JSProxyNode["doNothing"];
        val Module = val::global("Module");
        val Beaker = Module["Beaker"];
        val makeNewReactionRule_el = Beaker["makeNewReactionRule_st"];

        val makeEl = Module["makeEl"];
        // val makeNewReactionRule_el = makeEl(*(this->cppVal_), makeNewReactionRule);

        //val makeNewReactionRule_el = val([this](val ev) { (this->cppVal_)->makeNewReactionRule(); });
        //val makeNewReactionRule_el = val([this](val ev) { Beaker<unsigned char>::makeNewReactionRule_st(this->cppVal_); });

        //val makeNewReactionRule_st = Beaker["makeNewReactionRule_st"];

        CLNodeFactory<HybridNode, string, int> builder("div", "testBeaker");
        // auto *this = builder.build();

        // CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(this));
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(this));

        // builder = builder->

        CLNodeFactory<HybridNode, unsigned char, double> canvasBuilder(
            builder.withChildrenOf(this));

        CanvasGrid<unsigned char> *beakerCnv =
            canvasBuilder.withName("canvas1")
                .withTag("canvas")
                .withAttributes({{"style", val("border: 1px solid green")},
                                 {"width", val(600)},
                                 {"height", val(400)}})
                .canvasGrid(60, 40, 600, 400);

        beakerCnv->setCurrentCellVal(2);

        auto *canvas1CurrentCellColor_tinp =
            canvasBuilder.withName("currentCellColor_tinp")
                .withCppVal(beakerCnv->getPtr2CurrentCellVal())
                .withAttributes({{"style", val("border: 3px solid ##77bbee")}, {"size", val(2)}})
                .textInput();

        this->appendChild(beakerCnv);
        this->appendChild(canvas1CurrentCellColor_tinp);

        builder.br();

        CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(this));
        string *cmdarea_text = new string("This is a textarea.");
        auto *cmdarea = textBuilder.withName("cmdarea").textarea(cmdarea_text, 6, 80);
        textBuilder.br();
        auto *cmdarea_lbl = textBuilder.label(cmdarea, "CMD:", true);
        textBuilder.br();

        auto *ruleFrameWidth_tinp = intBuilder.withName("ruleFrameWidth_tinp")
                                        .withCppVal(&this->cppVal_->gridWidth)
                                        .textInput();
        auto *ruleFrameWidth_tinp_lbl =
            intBuilder.label(ruleFrameWidth_tinp, "Width of new rule frames.", true);
        auto *ruleFrameHeight_tinp = intBuilder.withName("ruleFrameHeight_tinp")
                                         .withCppVal(&this->cppVal_->gridHeight)
                                         .textInput();
        auto *ruleFrameHeight_tinp_lbl =
            intBuilder.label(ruleFrameHeight_tinp, "Height of new rule frames.", true);

        textBuilder.br();

        auto *newReactionRule_btn =
            intBuilder.button("newReactionRule_btn", "Make reaction rule", makeNewReactionRule_el);

        textBuilder.br();
    }

    inline virtual void doNothing() {
        cout << "This method exists so that Embind will create a type when this method in bound."
             << endl;
    }
};

EMSCRIPTEN_BINDINGS(PixelReactor) {
    class_<HybridNode<Beaker<unsigned char>>>("BeakerNode_h")
        .function("doNothing", &HybridNode<Beaker<unsigned char>>::doNothing, allow_raw_pointers());

    class_<Beaker<unsigned char>>("Beaker")
        .function("makeNewReactionRule", &Beaker<unsigned char>::makeNewReactionRule,
                  allow_raw_pointers())
        .class_function("makeNewReactionRule_st", &Beaker<unsigned char>::makeNewReactionRule_st,
                        allow_raw_pointers());
}

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
        // val blackbody_st = ClarityNode::JSProxyNode_["blackbody_st"];
        CLNodeFactory<HybridNode, double, double> builder("div", "maindiv");
        auto *maindiv = builder.build();

        CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(maindiv));
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(maindiv));

        CLNodeFactory<HybridNode, unsigned char, double> canvasBuilder(builder);

        CLNodeFactory<BeakerNode, Beaker<unsigned char>, int> beakerBuilder(
            builder.withChildrenOf(maindiv));
        auto *b = new Beaker<unsigned char>();
        auto *bn = beakerBuilder.withTag("div").withName("mainBeaker").withCppVal(b).build();

        printf("Setup complete!\n");
        return maindiv;
    }
};

#endif
