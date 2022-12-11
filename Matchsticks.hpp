#ifndef MATCHSTICKS_hpp
#define MATCHSTICKS_hpp

#include "CLNodeFactory.hpp"
#include "CanvasElement.hpp"
#include "ClarityNode.hpp"
#include "clarity.hpp"
#include "globals.hpp"

using namespace clarity;

template <typename U>
class StickWorld;

/**
 * @brief This is the complex control that allows the user to edit and run the StickWorld. Probably
 * a good thing to copy if you're planning to make your own complex controls.
 *
 * @tparam V This 'V' is actually the StickWorld objects themselves. So the `cppVal_` member of this
 * kind of node points to a StickWorld.
 */
template <class B>
class StickWorldNode : public HybridNode<B> {
   private:
    // ClarityNode *iterationCounter_;

   public:
    StickWorldNode(const string &name, const string &tag, bool useExistingDOMElement,
                   ClarityNode::AttachmentMode attachmentMode = ClarityNode::AttachmentMode::NEW,
                   const string &attachmentId = "")
        : HybridNode<B>(name, tag, useExistingDOMElement, attachmentMode, attachmentId) {}

    /**
     * @brief In any node representing a complex object a call to HybridNode::refresh() is mandatory
     * as that is the only way child nodes representing values within the complex structure can be
     * updated when the object state changes.
     *
     */
    inline virtual void refresh() {
        HybridNode<B>::refresh();
        // this->swCanvas_->refreshView();
    }

    /**
     * @brief This overload of `finalize()` shows that in the case of complex data types with many
     * subcomponents the finalize method is reponsible for setting up the nodes responsible for
     * manipulating them. This is also where essential things like the pointer in the model object
     * that points back to the GUI node are set up. This pointer is what allows the model object to
     * automatically update the interface when its internal state changes.
     *
     */
    virtual void finalize() {
        this->nodelog("StickWorldNode::finalize(): ");
        // val ghostUrl = val::global("ghostUrl");

        // this->nodelog(clto_str("Ghost Url: ") + ghostUrl.as<string>());

        this->jsProxyNode_.set("clarityNode", this);

        // // This pointer allows the StickWorld to see its StickWorldNode and automatically update
        // it when it
        // // changes state.
        // this->cppVal_->stickWorldNode_ = this;

        // val makeNewReactionRule_el =
        //     val::global("elgMakeNewReactionRuleButtonClicked")(this->cppVal_);

        // val iterate_el = val::global("elgBeakerIterate")(this->cppVal_);

        CLNodeFactory<HybridNode, string, int> builder("div", "testBeaker");
        CLNodeFactory<HybridNode, string, int> stringBuilder(builder.withChildrenOf(this));
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(this));

        // CLNodeFactory<HybridNode, unsigned char, double> canvasBuilder(
        //     builder.withChildrenOf(this));

        beakerName_tinp = stringBuilder.withName("beakerName")
                              //.withCppVal(&this->cppVal_->name_)
                              .withAttributes({{"class", val("medium_width")}})
                              .textInput();      

        auto *swCanvas_ = intBuilder
                              .withName("canvas1")
                              .withTag("canvas")
                              .withAttributes({{"style", val("border: 1px solid green")},
                                               {"width", val(this->cppVal_->gridPixelWidth_)},
                                               {"height", val(this->cppVal_->gridPixelHeight_)}})
                              .canvas("canvasTestPattern");

        // swCanvas_->setCurrentCellVal(2);

        // auto *canvas1CurrentCellColor_tinp =
        //     canvasBuilder.withName("currentCellColor_tinp")
        //         .withCppVal(swCanvas_->getPtr2CurrentCellVal())
        //         .withAttributes({{"style", val("border: 3px solid ##77bbee")}, {"size", val(2)}})
        //         .textInput();

        // Only the main grid needs these controls
        // if (!this->cppVal_->isReactionRule_) {
        //     // Uncomment these two lines to re-activate beaker auto-iteration.
        //     // val beakerIterate = val::global("beakerIterate")(this->cppVal_);
        //     // val::global().call<void>("setInterval", beakerIterate, 500);

        //     //val beakerIterate = val::global("callMethodByName")(this->cppVal_, val("iterate"));
        //     val beakerIterate = val::global("Util")["callMethodByName"](this->cppVal_,
        //     val("iterate"));

        //     //val::global().call<void>("setInterval", beakerIterate, 500);

        //     HybridNode<string> *cmdarea;
        //     CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(this));
        //     textBuilder.br();
        //     string *cmdarea_text = new string("This is a textarea.");
        //     auto *cmdarea_lbl = textBuilder.label(cmdarea, "CMD:", false);
        //     textBuilder.br();

        //     cmdarea = textBuilder.withName("cmdarea").textarea(cmdarea_text, 6, 60);
        //     textBuilder.br();

        //     // auto *beakerGridWidth_tinp = intBuilder.withName("beakerGridWidth_tinp")
        //     //                                  .withCppVal(&this->cppVal_->gridWidth_)
        //     //                                  .withAttributes({{"class", val("small_width")}})
        //     //                                  .textInput();

        //     // auto *beakerGridWidth_tinp_lbl =
        //     //     intBuilder.label(beakerGridWidth_tinp, "StickWorld grid width.", true);

        //     // auto *beakerGridHeight_tinp = intBuilder.withName("beakerGridHeight_tinp")
        //     //                                   .withCppVal(&this->cppVal_->gridHeight_)
        //     //                                   .withAttributes({{"class", val("small_width")}})
        //     //                                   .textInput();

        //     // auto *beakerGridHeight_tinp_lbl =
        //     //     intBuilder.label(beakerGridHeight_tinp, "StickWorld grid height.", true);

        //     textBuilder.br();

        //     auto *ruleFrameWidth_tinp = intBuilder.withName("ruleFrameWidth_tinp")
        //                                     .withCppVal(&this->cppVal_->ruleGridWidth_)
        //                                     .withAttributes({{"class", val("small_width")}})
        //                                     .textInput();

        //     auto *ruleFrameWidth_tinp_lbl =
        //         intBuilder.label(ruleFrameWidth_tinp, "Width of new rules.", true);

        //     auto *ruleFrameHeight_tinp = intBuilder.withName("ruleFrameHeight_tinp")
        //                                      .withCppVal(&this->cppVal_->ruleGridHeight_)
        //                                      .withAttributes({{"class", val("small_width")}})
        //                                      .textInput();

        //     auto *ruleFrameHeight_tinp_lbl =
        //         intBuilder.label(ruleFrameHeight_tinp, "Height of new rules.", true);

        //     textBuilder.br();

        //     val beakerIterate_el = val::global("Util")["callMethodByName"](this->cppVal_,
        //     val("iterate"), val(true)); auto *iterate_btn =
        //         intBuilder.button("iterate_btn", "Iterate the reaction", beakerIterate_el);

        //     auto *newReactionRule_btn = intBuilder.button(
        //         "newReactionRule_btn", "Make reaction rule", makeNewReactionRule_el);

        //     textBuilder.br();

        //     auto *iterationCounter =
        //         intBuilder
        //             .withAttributes({{"class", val("small_width")}, {"disabled",
        //             val("disabled")}}) .withCppVal(&this->cppVal_->iterationCount_)
        //             .withName("iterationCounter")
        //             .textInput();

        //     reactionRulesDiv_ = intBuilder.withTag("div").withName("reactionRulesDiv").build();

        //     textBuilder.br();
        // }
    }

    inline virtual void doNothing() {
        cout << "This method exists so that Embind will create a type when this method in bound."
             << endl;
    }

   public:
    // FIXME: Would be nice to keep these members protected or private but
    // this is an easy way to let StickWorld see it for now.

    ClarityNode *reactionRulesDiv_;
    ClarityNode *beakerName_tinp;
    // ClarityNode *swCanvas_;
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
class StickWorld {
   public:
    StickWorld(int gridPixelWidth, int gridPixelHeight)
        : gridPixelWidth_(gridPixelWidth), gridPixelHeight_(gridPixelHeight) {}

    

   protected:
    int gridPixelWidth_ = 600;   //!< Width in pixels of beaker grid.
    int gridPixelHeight_ = 400;  //!< Height in pixels of beaker grid.

    // int ruleGridWidth_ = 5;   //!< Width of new rule grid in cells.
    // int ruleGridHeight_ = 3;  //!< Height of new rule grid in cells.

    // V *gridArray;  //!< The actual grid data to be used by the CanvasGrid in StickWorldNode.
    int iterationCount_ =
        0;  //!< Counter that advances every time the rules are applied to the grid.
    StickWorldNode<StickWorld<V>>
        *stickWorldNode_;  //!< Pointer back to containing SWN so that SWN->refresh()
                           //!< can be called when this updates.

    friend class StickWorldNode<StickWorld<V>>;
};

EMSCRIPTEN_BINDINGS(Matchsticks) {
    // class_<HybridNode<StickWorld<unsigned char>>>("HybridNode_h")
    //     .function("doNothing", &HybridNode<StickWorld<unsigned char>>::doNothing,
    //     allow_raw_pointers());

    class_<StickWorldNode<StickWorld<unsigned char>>>("BeakerNode_h")
        .function("doNothing", &StickWorldNode<StickWorld<unsigned char>>::doNothing,
                  allow_raw_pointers());

    class_<StickWorld<unsigned char>>("StickWorld");

    class_<CanvasElement<int>>("CanvasElement_i");
    // .function("setColorReactionRules", &StickWorld<unsigned char>::setColorReactionRules,
    //           allow_raw_pointers())
    // .function("iterate", &StickWorld<unsigned char>::iterate, allow_raw_pointers())
    // .function("makeNewReactionRule", &StickWorld<unsigned char>::makeNewReactionRule,
    //           allow_raw_pointers());
    // .class_function("makeNewReactionRule_st", &StickWorld<unsigned char>::makeNewReactionRule_st,
    //                 allow_raw_pointers());
}

/**
 * @brief The Pixelreactor app from the old ClojureScript site redone in C++.
 *
 */
struct Matchsticks : public PageContent {
  

    ClarityNode *content(ClarityNode *innerContent = nullptr) {
        CLNodeFactory<HybridNode, double, double> builder("div", "maindiv");

#ifdef USETF
        auto *maindiv = builder.build();
#else
        auto *maindiv = builder.withAttachmentId("hookid")
                            .withAttachmentMode(clarity::ClarityNode::AttachmentMode::REPLACE_ID)
                            .build();

//        auto *maindiv = builder.build();
#endif

        CLNodeFactory<StickWorldNode, StickWorld<unsigned char>, int> beakerBuilder(
            builder.withChildrenOf(maindiv));

        StickWorld<unsigned char> *b = new StickWorld<unsigned char>(600, 400);

        StickWorldNode<StickWorld<unsigned char>> *bn =
            beakerBuilder.withTag("div").withName("mainBeaker").withCppVal(b).build();

        //  StickWorldNode<StickWorld<unsigned char>> *bn =
        //     beakerBuilder.withTag("div").withName("mainBeaker").build();

        cout << "Setup complete!" << endl;
        return maindiv;
    }
};

#endif
