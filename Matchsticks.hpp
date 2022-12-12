#ifndef MATCHSTICKS_hpp
#define MATCHSTICKS_hpp

#include "CLNodeFactory.hpp"
#include "CanvasElement.hpp"
#include "ClarityNode.hpp"
#include "clarity.hpp"
#include "globals.hpp"

using namespace clarity;

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

    inline void tick() {
        cout << "SWNode TICK TOCK!" << endl;
        this->cppVal_->iterationCount_++;
        int i = this->cppVal_->iterationCount_;
        val ctx = swCanvas_->getContext2d();
        ctx.set("fillStyle", val("maroon"));
        ctx.call<void>("fillRect", val(10), val(8), val(10 * i), val(8 * i));
    }

    inline CanvasElement<int> *getSWCanvas() { return this->swCanvas_; }

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

        CLNodeFactory<HybridNode, string, int> builder("div", "stickworldDiv");
        CLNodeFactory<HybridNode, string, int> stringBuilder(builder.withChildrenOf(this));
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(this));
        CLNodeFactory<HybridNode, double, double> doubleBuilder(builder.withChildrenOf(this));

        stickworldName_tinp = stringBuilder
                                  .withName("stickworldName")
                                  //.withCppVal(&this->cppVal_->name_)
                                  .withAttributes({{"class", val("medium_width")}})
                                  .textInput();

        swCanvas_ = intBuilder.withName("canvas1")
                        .withTag("canvas")
                        .withAttributes({{"style", val("border: 1px solid green")},
                                         {"width", val(this->cppVal_->swCanvasWidth_)},
                                         {"height", val(this->cppVal_->swCanvasHeight_)}})
                        .canvasElement();

        auto *lineLength_tinp = doubleBuilder.withName("lineLength_tinp")
                                    .withLabelText("Line Length")
                                    .withHoverText("Line Length")
                                    .withCppVal(&this->cppVal_->lineLength_)
                                    .textInput();

        // val canvasFillcolor = val::global("canvasFillcolor");
        // val ctx = swCanvas_->getDomElement().call<val>("getContext", val("2d"));
        val ctx = swCanvas_->getContext2d();
        ctx.set("fillStyle", val("aqua"));
        ctx.call<void>("fillRect", val(0), val(0), val(this->cppVal_->getSWCanvasWidth()),
                       val(this->cppVal_->getSWCanvasHeight()));
        // fillRect(0, 0, w, h);

        val::global("setTickerSWNode")(*this);
        // swCanvas_->runDrawFunction();
    }

    // inline virtual void doNothing() {
    //     cout << "This method exists so that Embind will create a type when this method in bound."
    //          << endl;
    // }

   public:
    // FIXME: Would be nice to keep these members protected or private but
    // this is an easy way to let StickWorld see it for now.

    ClarityNode *reactionRulesDiv_;
    CanvasElement<int> *swCanvas_;
    ClarityNode *stickworldName_tinp;
    // ClarityNode *swCanvas_;
};

typedef pair<double, double> coordinatePair;

struct ColorRGBA {
    unsigned char r, g, b, a;
    ColorRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

struct Matchstick {
    coordinatePair fromPoint_, toPoint_;
    ColorRGBA stickColor_;  //, toCol_;

    Matchstick(coordinatePair fromPoint, coordinatePair toPoint, ColorRGBA stickColor)
        : fromPoint_(fromPoint), toPoint_(toPoint), stickColor_(stickColor) {}
};

/**
 * @brief Represents a single "reaction vessel" in which our experiments can take place. The
 * reaction rules that determine how patterns in the canvas transform will use the same CanvasGrid
 * control the stickWorld itself does.
 *
 * @tparam V This is the type we are using for the canvas elements. The original app I wrote in
 * ClojureScript used small positive integers for the colors so the expected type here is `unsigned
 * char`. Theoretically, it's possible to use other types though.
 */

class StickWorld {
   public:
    StickWorld(int swCanvasWidth, int swCanvasHeight)
        : swCanvasWidth_(swCanvasWidth), swCanvasHeight_(swCanvasHeight) {}

    inline int getSWCanvasWidth() { return this->swCanvasWidth_; }
    inline int getSWCanvasHeight() { return this->swCanvasHeight_; }

    inline void tick() {
        cout << "TICK TOCK!" << endl;
        CanvasElement<int> *swCanvas = stickWorldNode_->getSWCanvas();
        val ctx = swCanvas->getContext2d();
        ctx.set("fillStyle", val("maroon"));
        // ctx.call<void>("fillRect", val(10), val(8), val(10 * iterationCount_),
        //                val(8 * iterationCount_));
        iterationCount_++;
    }

   protected:
    int swCanvasWidth_ = 600;   //!< Width in pixels of stickWorld canvas.
    int swCanvasHeight_ = 400;  //!< Height in pixels of stickWorld canvas.

    int iterationCount_ = 0;

    double lineLength_ = 50;

    StickWorldNode<StickWorld>
        *stickWorldNode_;  //!< Pointer back to containing SWN so that SWN->refresh()
                           //!< can be called when this updates.

    friend class StickWorldNode<StickWorld>;
};

EMSCRIPTEN_BINDINGS(Matchsticks) {
    // class_<HybridNode<StickWorld>>("HybridNode_h")
    //     .function("doNothing", &HybridNode<StickWorld>::doNothing,
    //     allow_raw_pointers());

    class_<StickWorldNode<StickWorld>>("StickWorldNode")
        .function("tick", &StickWorldNode<StickWorld>::tick, allow_raw_pointers());
    // .function("doNothing", &StickWorldNode<StickWorld>::doNothing,
    //           allow_raw_pointers());

    class_<StickWorld>("StickWorld").function("tick", &StickWorld::tick, allow_raw_pointers());

    class_<CanvasElement<int>>("CanvasElement_i");
    //    .function("runDrawFunction", &CanvasElement<int>::runDrawFunction, allow_raw_pointers());
    // .function("iterate", &StickWorld::iterate, allow_raw_pointers())
    // .function("makeNewReactionRule", &StickWorld::makeNewReactionRule,
    //           allow_raw_pointers());
    // .class_function("makeNewReactionRule_st", &StickWorld::makeNewReactionRule_st,
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
#endif

        CLNodeFactory<StickWorldNode, StickWorld, int> stickworldBuilder(
            builder.withChildrenOf(maindiv));

        StickWorld *stickWorld = new StickWorld(600, 400);

        StickWorldNode<StickWorld> *stickWorldNode =
            stickworldBuilder.withTag("div").withName("stickWorld").withCppVal(stickWorld).build();

        cout << "Setup complete!" << endl;
        return maindiv;
    }
};

#endif
