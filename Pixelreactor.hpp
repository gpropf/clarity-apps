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

    /**
     * @brief In any node representing a complex object a call to HybridNode::refresh() is mandatory
     * as that is the only way child nodes representing values within the complex structure can be
     * updated when the object state changes.
     *
     */
    inline virtual void refresh() { HybridNode<B>::refresh(); }

    /**
     * @brief This overload of `finalize()` shows that in the case of complex data types with many
     * subcomponents the finalize method is reponsible for setting up the nodes responsible for
     * manipulating them. This is also where essential things like the pointer in the model object
     * that points back to the GUI node are set up. This pointer is what allows the model object to
     * automatically update the interface when its internal state changes.
     *
     */
    virtual void finalize() {
        this->nodelog("BeakerNode::finalize(): ");

        this->jsProxyNode_.set("clarityNode", this);

        // This pointer allows the Beaker to see its BeakerNode and automatically update it when it
        // changes state.
        this->cppVal_->beakerNode_ = this;

        val JSProxyNode = val::global("JSProxyNode");
        val doNothing = JSProxyNode["doNothing"];
        val Module = val::global("Module");
        val Beaker = Module["Beaker"];

        val elgMakeNewReactionRuleButtonClicked =
            val::global("elgMakeNewReactionRuleButtonClicked");

        val makeEl = val::global("makeEl");
        // val makeNewReactionRule_el = makeEl(*(this->cppVal_), makeNewReactionRule_st);

        val makeNewReactionRule_el = elgMakeNewReactionRuleButtonClicked(this->cppVal_);
        // val makeNewReactionRule_el = val::global("makeNewReactionRule_el");

        CLNodeFactory<HybridNode, string, int> builder("div", "testBeaker");
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(this));

        CLNodeFactory<HybridNode, unsigned char, double> canvasBuilder(
            builder.withChildrenOf(this));

        CanvasGrid<unsigned char> *beakerCnv =
            canvasBuilder.withName("canvas1")
                .withTag("canvas")
                .withAttributes({{"style", val("border: 1px solid green")},
                                 {"width", val(this->cppVal_->gridPixelWidth_)},
                                 {"height", val(this->cppVal_->gridPixelHeight_)}})
                .canvasGrid(this->cppVal_->gridWidth_, this->cppVal_->gridHeight_,
                            this->cppVal_->gridPixelWidth_, this->cppVal_->gridPixelHeight_);

        beakerCnv->setCurrentCellVal(2);

        if (!this->cppVal_->isReactionRule_) {
            auto *canvas1CurrentCellColor_tinp =
                canvasBuilder.withName("currentCellColor_tinp")
                    .withCppVal(beakerCnv->getPtr2CurrentCellVal())
                    .withAttributes(
                        {{"style", val("border: 3px solid ##77bbee")}, {"size", val(2)}})
                    .textInput();

            HybridNode<string> *cmdarea;
            CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(this));
            textBuilder.br();
            string *cmdarea_text = new string("This is a textarea.");
            auto *cmdarea_lbl = textBuilder.label(cmdarea, "CMD:", false);
            textBuilder.br();

            cmdarea = textBuilder.withName("cmdarea").textarea(cmdarea_text, 6, 60);
            textBuilder.br();

            auto *beakerGridWidth_tinp = intBuilder.withName("beakerGridWidth_tinp")
                                             .withCppVal(&this->cppVal_->gridWidth_)
                                             .withAttributes({{"class", val("small_width")}})
                                             .textInput();

            auto *beakerGridWidth_tinp_lbl =
                intBuilder.label(beakerGridWidth_tinp, "Beaker grid width.", true);

              

            auto *beakerGridHeight_tinp = intBuilder.withName("beakerGridHeight_tinp")
                                              .withCppVal(&this->cppVal_->gridHeight_)
                                              .withAttributes({{"class", val("small_width")}})
                                              .textInput();

            auto *beakerGridHeight_tinp_lbl =
                intBuilder.label(beakerGridHeight_tinp, "Beaker grid height.", true);
            
            textBuilder.br();

            auto *ruleFrameWidth_tinp = intBuilder.withName("ruleFrameWidth_tinp")
                                            .withCppVal(&this->cppVal_->ruleGridWidth_)
                                            .withAttributes({{"class", val("small_width")}})
                                            .textInput();

            auto *ruleFrameWidth_tinp_lbl =
                intBuilder.label(ruleFrameWidth_tinp, "Width of new rules.", true);

            auto *ruleFrameHeight_tinp = intBuilder.withName("ruleFrameHeight_tinp")
                                             .withCppVal(&this->cppVal_->ruleGridHeight_)
                                             .withAttributes({{"class", val("small_width")}})
                                             .textInput();

            auto *ruleFrameHeight_tinp_lbl =
                intBuilder.label(ruleFrameHeight_tinp, "Height of new rules.", true);

            textBuilder.br();

            auto *newReactionRule_btn = intBuilder.button(
                "newReactionRule_btn", "Make reaction rule", makeNewReactionRule_el);

            textBuilder.br();

            auto *iterationCounter = intBuilder.withAttributes({{"class", val("small_width")}})
                                         .withCppVal(&this->cppVal_->iterationCount_)
                                         .withName("iterationCounter")
                                         .textInput();

            reactionRulesDiv_ = intBuilder.withTag("div").withName("reactionRulesDiv").build();

            // CLNodeFactory<BeakerNode, ::Beaker<unsigned char>, int> reactionRulesBuilder(
            //     intBuilder.withChildrenOf(reactionRulesDiv_));

            // for (auto *rr : this->cppVal_->reactionRules_) {
            // }

            textBuilder.br();
        }
    }

    inline virtual void doNothing() {
        cout << "This method exists so that Embind will create a type when this method in bound."
             << endl;
    }

   public:
    ClarityNode *reactionRulesDiv_;  // FIXME: Would be nice to keep this protected or private but
                                     // this is an easy way to let Beaker see it for now.
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
    Beaker(int gridWidth, int gridHeight, int gridPixelWidth, int gridPixelHeight,
           bool isReactionRule = false)
        : gridWidth_(gridWidth),
          gridHeight_(gridHeight),
          gridPixelWidth_(gridPixelWidth),
          gridPixelHeight_(gridPixelHeight),
          isReactionRule_(isReactionRule) {}

    int jiveCount = 0;  //!< A phony counter just to prove we can maintain state using the 'make
                        //!< rule' button as the app runs.

    void makeNewReactionRule() {
        this->iterationCount_++;
        this->jiveCount++;
        cout << "makeNewReactionRule(), jiveCount = " << jiveCount << endl;
        cout << "makeNewReactionRule(), iterationCount_ = " << this->iterationCount_ << endl;

        Beaker *reactionRule =
            new Beaker(this->ruleGridWidth_, this->ruleGridHeight_, 150, 150, true);
        this->reactionRules_.push_back(reactionRule);

        CLNodeFactory<BeakerNode, Beaker<unsigned char>, int> beakerBuilder("div", "rr");

        BeakerNode<Beaker<unsigned char>> *bn =
            beakerBuilder.withChildrenOf(beakerNode_->reactionRulesDiv_)
                .withTag("div")
                .withName("reactionRule")
                .withCppVal(reactionRule)
                .build();
        beakerNode_->refresh();
    }

    static void makeNewReactionRule_st(Beaker *b) { b->makeNewReactionRule(); }

   protected:
    bool isReactionRule_ = false;  //!< Set to true if this Beaker is being used as a reaction rule
                                   //!< for an enclosing Beaker.

    int gridPixelWidth_ = 600;   //!< Width in pixels of beaker grid.
    int gridPixelHeight_ = 400;  //!< Height in pixels of beaker grid.

    int gridWidth_ = 60;   //!< Width of beaker grid in cells.
    int gridHeight_ = 40;  //!< Height of beaker grid in cells.

    int ruleGridWidth_ = 5;   //!< Width of new rule grid in cells.
    int ruleGridHeight_ = 3;  //!< Height of new rule grid in cells.

    V *gridArray;  //!< The actual grid data to be used by the CanvasGrid in BeakerNode.
    int iterationCount_ =
        0;  //!< Counter that advances every time the rules are applied to the grid.
    BeakerNode<Beaker<V>> *beakerNode_;  //!< Pointer back to containing BN so that BN->refresh()
                                         //!< can be called when this updates.

    vector<Beaker *> reactionRules_;
    // template <typename U>
    friend class BeakerNode<Beaker<V>>;
};

EMSCRIPTEN_BINDINGS(PixelReactor) {
    // class_<HybridNode<Beaker<unsigned char>>>("HybridNode_h")
    //     .function("doNothing", &HybridNode<Beaker<unsigned char>>::doNothing,
    //     allow_raw_pointers());

    class_<BeakerNode<Beaker<unsigned char>>>("BeakerNode_h")
        .function("doNothing", &BeakerNode<Beaker<unsigned char>>::doNothing, allow_raw_pointers());

    class_<Beaker<unsigned char>>("Beaker").function(
        "makeNewReactionRule", &Beaker<unsigned char>::makeNewReactionRule, allow_raw_pointers());
    // .class_function("makeNewReactionRule_st", &Beaker<unsigned char>::makeNewReactionRule_st,
    //                 allow_raw_pointers());
}

/**
 * @brief The Pixelreactor app from the old ClojureScript site redone in C++.
 *
 */
struct PixelReactor : public PageContent {
    int *ruleFrameWidth = new int(5);
    int *ruleFrameHeight = new int(3);

    ClarityNode *content(ClarityNode *innerContent = nullptr) {
        CLNodeFactory<HybridNode, double, double> builder("div", "maindiv");
        auto *maindiv = builder.build();

        CLNodeFactory<BeakerNode, Beaker<unsigned char>, int> beakerBuilder(
            builder.withChildrenOf(maindiv));

        Beaker<unsigned char> *b = new Beaker<unsigned char>(60, 40, 600, 400);

        BeakerNode<Beaker<unsigned char>> *bn =
            beakerBuilder.withTag("div").withName("mainBeaker").withCppVal(b).build();

        cout << "Setup complete!" << endl;
        return maindiv;
    }
};

#endif
