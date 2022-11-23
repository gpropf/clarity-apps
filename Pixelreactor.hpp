#ifndef Pixelreactor_hpp
#define Pixelreactor_hpp

#include "CLNodeFactory.hpp"
#include "CanvasElement.hpp"
#include "ClarityNode.hpp"
#include "clarity.hpp"
#include "globals.hpp"

using namespace clarity;

template <typename U>
class Beaker;

/**
 * @brief This is the complex control that allows the user to edit and run the Beaker. Probably a
 * good thing to copy if you're planning to make your own complex controls.
 *
 * @tparam V This 'V' is actually the Beaker objects themselves. So the `cppVal_` member of this
 * kind of node points to a Beaker.
 */
template <class B>
class BeakerNode : public HybridNode<B> {
   private:
    // ClarityNode *iterationCounter_;

   public:
    BeakerNode(const string &name, const string &tag, bool useExistingDOMElement,
               ClarityNode::AttachmentMode attachmentMode = ClarityNode::AttachmentMode::NEW,
               const string &attachmentId = "")
        : HybridNode<B>(name, tag, useExistingDOMElement, attachmentMode, attachmentId) {}

    virtual void refresh() {
        // this->cppVal_->iterationCount_++;
        //
        HybridNode<B>::refresh();

        
    }

    virtual void finalize() {
        this->cle_.set("clarityNode", this);
         this->cle_.set("beaker", this->cppVal_);
        this->nodelog("BeakerNode::finalize(): ");
         this->cppVal_->beakerNode_ = this;
        // this->cppVal_->setBeakerNode(this);

        val JSProxyNode = val::global("JSProxyNode");
        val doNothing = JSProxyNode["doNothing"];
        val Module = val::global("Module");
        val Beaker = Module["Beaker"];
        val elgMakeNewReactionRuleButtonClicked = val::global("elgMakeNewReactionRuleButtonClicked");
        val elgMakeNewReactionRuleButtonClicked2 = val::global("elgMakeNewReactionRuleButtonClicked2");

        val makeNewReactionRule_st = Beaker["makeNewReactionRule_st"];
        val makeEl = val::global("makeEl");
        //val makeNewReactionRule_el = makeEl(*(this->cppVal_), makeNewReactionRule_st);
        val makeNewReactionRule_el = elgMakeNewReactionRuleButtonClicked(this->cle_);
        val makeNewReactionRule_el2 = elgMakeNewReactionRuleButtonClicked2(this->cppVal_);
        //val makeNewReactionRule_el = val::global("makeNewReactionRule_el");

        CLNodeFactory<HybridNode, string, int> builder("div", "testBeaker");
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(this));

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

        HybridNode<string> *cmdarea;
        CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(this));
        textBuilder.br();
        string *cmdarea_text = new string("This is a textarea.");
        auto *cmdarea_lbl = textBuilder.label(cmdarea, "CMD:", false);
        textBuilder.br();

        cmdarea = textBuilder.withName("cmdarea").textarea(cmdarea_text, 6, 60);
        textBuilder.br();

        auto *ruleFrameWidth_tinp = intBuilder.withName("ruleFrameWidth_tinp")
                                        .withCppVal(&this->cppVal_->gridWidth)
                                        .withAttributes({{"class", val("small_width")}})
                                        .textInput();

        auto *ruleFrameWidth_tinp_lbl =
            intBuilder.label(ruleFrameWidth_tinp, "Width of new rule frames.", true);

        auto *ruleFrameHeight_tinp = intBuilder.withName("ruleFrameHeight_tinp")
                                         .withCppVal(&this->cppVal_->gridHeight)
                                         .withAttributes({{"class", val("small_width")}})
                                         .textInput();

        auto *ruleFrameHeight_tinp_lbl =
            intBuilder.label(ruleFrameHeight_tinp, "Height of new rule frames.", true);

        textBuilder.br();

        auto *newReactionRule_btn =
            intBuilder.button("newReactionRule_btn", "Make reaction rule", makeNewReactionRule_el2);

        textBuilder.br();

        auto *iterationCounter = intBuilder.withAttributes({{"class", val("small_width")}})
                                      .withCppVal(&this->cppVal_->iterationCount_)
                                      .withName("iterationCounter")
                                      .textInput();

        textBuilder.br();
    }

    inline virtual void doNothing() {
        cout << "This method exists so that Embind will create a type when this method in bound."
             << endl;
    }
};

/**
 * @brief Represents a single "reaction vessel" in which our experiments can take place. The
 * reaction rules that determine how patterns in the grid transform will use the same CanvasGrid
 * control the beaker itself does.
 *
 * @tparam V This is the type we are using for the grid elements. The original app I wrote in
 * ClojureScript used small positive integers for the colors so the expected type here is `unsigned
 * char`. Theoretically, it's possible to use other types though.
 */
template <typename V>
class Beaker {
   public:
    static Beaker<V> *staticBeaker;
    static std::function<void()> lbd;
    //static std::function<void()> makeNewReactionRuleLbd;

    int jiveCount = 0;    //!< A phony counter just to prove we can maintain state using the 'make
                          //!< rule' button as the app runs.
    int gridWidth = 60;   //!< Width of beaker grid in cells.
    int gridHeight = 40;  //!< Height of beaker grid in cells.
    V *gridArray;         //!< The actual grid data to be used by the CanvasGrid in BeakerNode.
    int iterationCount_ =
        0;  //!< Counter that advances every time the rules are applied to the grid.
    BeakerNode<Beaker<V>> *beakerNode_;  //!< Pointer back to containing BN so that BN->refresh()
                                         //!< can be called when this updates.
    void makeNewReactionRule() {
        this->iterationCount_++;
        this->jiveCount++;
        cout << "makeNewReactionRule(), jiveCount = " << jiveCount << endl;
        cout << "makeNewReactionRule(), iterationCount_ = " << this->iterationCount_ << endl;
        beakerNode_->refresh();
    }

    // inline void setBeakerNode(BeakerNode<Beaker<V>> * beakerNode) { this->beakerNode_ =
    // beakerNode;}
    static void makeNewReactionRule_st(Beaker *b) { b->makeNewReactionRule(); }

   protected:
    // template <typename U>
    friend class BeakerNode<V>;
};

//Beaker *Beaker<unsigned char>::staticBeaker = nullptr;

EMSCRIPTEN_BINDINGS(PixelReactor) {
    // class_<HybridNode<Beaker<unsigned char>>>("HybridNode_h")
    //     .function("doNothing", &HybridNode<Beaker<unsigned char>>::doNothing, allow_raw_pointers());

    class_<BeakerNode<Beaker<unsigned char>>>("BeakerNode_h")
        .function("doNothing", &BeakerNode<Beaker<unsigned char>>::doNothing, allow_raw_pointers());

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
        val CLElement = val::global("CLElement");
        // val blackbody_st = ClarityNode::JSProxyNode_["blackbody_st"];
        CLNodeFactory<HybridNode, double, double> builder("div", "maindiv");
        auto *maindiv = builder.build();

        CLNodeFactory<BeakerNode, Beaker<unsigned char>, int> beakerBuilder(
            builder.withChildrenOf(maindiv));
        Beaker<unsigned char> *b = new Beaker<unsigned char>();
        BeakerNode<Beaker<unsigned char>> *bn =
            beakerBuilder.withTag("div").withName("mainBeaker").withCppVal(b).build();
        b->beakerNode_ = bn;


        //Beaker<unsigned char>::makeNewReactionRuleLbd = [=] { Beaker<unsigned char>::staticBeaker->makeNewReactionRule(); };
         

        // maindiv->appendChild(bn);
        cout << "Setup complete!" << endl;
        return maindiv;
    }
};

#endif
