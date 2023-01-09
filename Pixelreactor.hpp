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
    inline virtual void refresh() {
        HybridNode<B>::refresh();
        this->beakerCanvas_->drawGrid();
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
        this->nodelog("BeakerNode::finalize(): ");
        // val ghostUrl = val::global("ghostUrl");
        // this->nodelog(clto_str("Ghost Url: ") + ghostUrl.as<string>());
        this->jsProxyNode_.set("clarityNode", this);
        // This pointer allows the Beaker to see its BeakerNode and automatically update it when it
        // changes state.
        this->cppVal_->beakerNode_ = this;

        CLNodeFactory<HybridNode, string, int> builder("div", "testBeaker");
        CLNodeFactory<HybridNode, string, int> stringBuilder(builder.withChildrenOf(this));
        CLNodeFactory<HybridNode, int, int> intBuilder(builder.withChildrenOf(this));
        CLNodeFactory<HybridNode, typename Beaker<B>::priorityT, typename Beaker<B>::priorityT>
            priorityBuilder(builder.withChildrenOf(this));

        CLNodeFactory<HybridNode, unsigned char, double> canvasBuilder(
            builder.withChildrenOf(this));

        val makeBeakerDirtyEL = val::global("elgMakeBeakerDirty")(this->cppVal_);

        beakerCanvas_ =
            canvasBuilder.withName("canvas1")
                .withTag("canvas")
                .withAttributes({{"style", val("border: 1px solid green")},
                                 {"width", val(this->cppVal_->gridPixelWidth_)},
                                 {"height", val(this->cppVal_->gridPixelHeight_)}})
                .canvasGrid(this->cppVal_->gridWidth_, this->cppVal_->gridHeight_,
                            this->cppVal_->gridPixelWidth_, this->cppVal_->gridPixelHeight_);

        beakerCanvas_->setCurrentCellVal(2);
        beakerCanvas_->addEventListener(makeBeakerDirtyEL, "click");

        auto *canvas1CurrentCellColorTextInput =
            canvasBuilder.withName("currentCellColorTextInput")
                .withCppVal(beakerCanvas_->getPtr2CurrentCellVal())
                .withAttributes({{"style", val("border: 3px solid ##77bbee")}, {"size", val(2)}})
                .textInput();

        stringBuilder.labelGivenNode(canvas1CurrentCellColorTextInput, "Current Color Index");

        if (this->cppVal_->isReactionRule_) {
            priorityTextInput_ = intBuilder.withName("priority")
                               .withCppVal(&this->cppVal_->successionPriority_)
                               .withAttributes({{"class", val("small_width")}})
                               .textInput();

            stringBuilder.withHoverText("Rule priority; lower numbers are higher priority")
                .labelGivenNode(priorityTextInput_, "Rule priority");

            auto beakerNameSetSuccessor = [this](HybridNode<string> *hn, string *v) {
                auto pos = v->find(':');
                if (pos != std::string::npos) {
                    cout << ": found at position " << pos << endl;
                    auto succName = v->substr(pos + 1, std::string::npos);
                    hn->setCppVal(v->substr(0, pos));
                    hn->refresh();
                    cout << "Successor:" << succName << endl;
                    auto successorBeaker = this->cppVal_->parentBeaker_->findRuleByName(succName);
                    if (successorBeaker != nullptr) {
                        this->cppVal_->successor_ = successorBeaker;
                        this->successorNameTextInput_->setCppValPtr(&successorBeaker->name_);
                        this->successorNameTextInput_->refresh();
                        cout << "this->cppVal_->successor_: " << this->cppVal_->successor_ << endl;
                    }
                }
                // if (*v != hn->getCppVal()) this->cppVal_->parentBeaker_->beakerNode_->refresh();
            };

            beakerNameTextInput_ = stringBuilder.withName("beakerName")
                                       .withCppVal(&this->cppVal_->name_)
                                       .withAttributes({{"class", val("medium_width")}})
                                       .withStateFunction(beakerNameSetSuccessor)
                                       .textInput();

            // auto *successorOffset = stringBuilder
            //                             .withName("successorOffset")
            //                             //.withCppVal(&this->cppVal_->name_)
            //                             .withAttributes({{"class", val("medium_width")}})
            //                             .textInput();

            // stringBuilder.withHoverText("Enter successor offset as x,y pair, e.g. 2,-1")
            //     .labelGivenNode(successorOffset, "Successor Offset");

            auto *xOffsetTextInput = intBuilder.withName("xOffset")
                                         .withClass("small_width")
                                         .withCppVal(&this->cppVal_->successorOffsetX_)
                                         .textInput();

            stringBuilder.withHoverText("x offset")
                .labelGivenNode(xOffsetTextInput, "x offset of successor rule");

            auto *yOffsetTextInput = intBuilder.withName("yOffset")
                                         .withClass("small_width")
                                         .withCppVal(&this->cppVal_->successorOffsetY_)
                                         .textInput();

            stringBuilder.withHoverText("y offset")
                .labelGivenNode(yOffsetTextInput, "y offset of successor rule");

            stringBuilder
                .withHoverText(
                    "To set the rule's successor, just type ':' after the rule name, followed by "
                    "the name of the rule "
                    "you want as sucessor and hit enter.")
                .labelGivenNode(beakerNameTextInput_, "Reaction Rule Name");

            // stringBuilder.br();

            successorNameTextInput_ = stringBuilder.withName("successorNameTextInput_")
                                          .withCppVal(&this->cppVal_->successor_->name_)
                                          .withAttributes({{"class", val("medium_width")}})
                                          .withDisable()
                                          .textInput();

            stringBuilder.withHoverText("Name of successor rule")
                .labelGivenNode(successorNameTextInput_, "Successor Rule Name");
            stringBuilder.br();

        } else {
            // mainBeaker_ = this->cppVal_;

            val makeNewReactionRuleEL =
                val::global("elgMakeNewReactionRuleButtonClicked")(this->cppVal_);

            val iterateEL = val::global("elgBeakerIterate")(this->cppVal_);
            // Uncomment these two lines to re-activate beaker auto-iteration.
            // val beakerIterate = val::global("beakerIterate")(this->cppVal_);
            // val::global().call<void>("setInterval", beakerIterate, 500);

            // val beakerIterate = val::global("callMethodByName")(this->cppVal_, val("iterate"));
            // val beakerIterate =
            //     val::global("Util")["callMethodByName"](this->cppVal_, val("iterate"));

            // val::global().call<void>("setInterval", beakerIterate, 500);

            HybridNode<string>
                *cmdarea;  // Declaring this before creating it so the label can be created first
                           // and will thus be above the textarea. Obviously need an option for this
                           // in the label() method.
            CLNodeFactory<HybridNode, string, double> textBuilder(builder.withChildrenOf(this));
            textBuilder.br();

            string *cmdarea_text =
                new string("You type commands here. This feature not currently available.");
            auto *cmdarea_lbl = textBuilder.label(cmdarea, "CMD:", false);
            textBuilder.br();

            cmdarea = textBuilder
                          .withDisable()
                          //.withMergeAttributes({{"disabled", val("disabled")}})
                          .withName("cmdarea")
                          .textarea(cmdarea_text, 6, 60);
            textBuilder.br();

            auto *beakerGridWidthTextInput = intBuilder.withName("beakerGridWidthTextInput")
                                                 .withCppVal(&this->cppVal_->gridWidth_)
                                                 .withAttributes({{"class", val("small_width")}})
                                                 .textInput();

            auto *beakerGridWidthLabel =
                intBuilder.label(beakerGridWidthTextInput, "Beaker grid width.", true);

            auto *beakerGridHeightTextInput = intBuilder.withName("beakerGridHeightTextInput")
                                                  .withCppVal(&this->cppVal_->gridHeight_)
                                                  .withAttributes({{"class", val("small_width")}})
                                                  .textInput();

            auto *beakerGridHeightLabel =
                intBuilder.label(beakerGridHeightTextInput, "Beaker grid height.", true);

            textBuilder.br();

            auto *ruleFrameWidthTextInput = intBuilder.withName("ruleFrameWidthTextInput")
                                                .withCppVal(&this->cppVal_->ruleGridWidth_)
                                                .withAttributes({{"class", val("small_width")}})
                                                .textInput();

            auto *ruleFrameWidthLabel =
                intBuilder.label(ruleFrameWidthTextInput, "Width of new rules.", true);

            auto *ruleFrameHeightTextInput = intBuilder.withName("ruleFrameHeightTextInput")
                                                 .withCppVal(&this->cppVal_->ruleGridHeight_)
                                                 .withAttributes({{"class", val("small_width")}})
                                                 .textInput();

            auto *ruleFrameHeightLabel =
                intBuilder.label(ruleFrameHeightTextInput, "Height of new rules.", true);

            textBuilder.br();

            val beakerIterateEL =
                val::global("Util")["callMethodByName"](this->cppVal_, val("iterate"), val(true));
            auto *iterate_btn =
                intBuilder.button("iterate_btn", "Iterate the reaction", beakerIterateEL);

            val clearGridEL = val::global("Util")["callMethodByName"](
                this->cppVal_, val("clearGrid"), val(true));
            auto *makePixelList_btn =
                intBuilder.button("makePixelList_btn", "Clear Grid", clearGridEL);

            auto *newReactionRule_btn = intBuilder.button(
                "newReactionRule_btn", "Make reaction rule", makeNewReactionRuleEL);

            textBuilder.br();

            auto *iterationCounter =
                intBuilder
                    .withAttributes({{"class", val("small_width")}, {"disabled", val("disabled")}})
                    .withCppVal(&this->cppVal_->iterationCount_)
                    .withName("iterationCounter")
                    .textInput();

            reactionRulesDiv_ = intBuilder.withTag("div").withName("reactionRulesDiv").build();

            textBuilder.br();
        }

        // successionPriority_

        // Only the main grid needs these controls
        if (!this->cppVal_->isReactionRule_) {
        }
    }

    inline virtual void doNothing() {
        cout << "This method exists so that Embind will create a type when this method in bound."
             << endl;
    }

   public:
    // FIXME: Would be nice to keep these members protected or private but
    // this is an easy way to let Beaker see it for now.

    ClarityNode *reactionRulesDiv_;
    ClarityNode *beakerNameTextInput_;
    HybridNode<string> *successorNameTextInput_;
    ClarityNode *priorityTextInput_;
    CanvasGrid<unsigned char> *beakerCanvas_;
    // Beaker<unsigned char> *mainBeaker_;
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
    typedef short int
        priorityT;  // I'm using an int type for the text input field for priorities because the
                    // values weren't "sticking" otherwise. This is a type related bug.
    typedef pair<V, priorityT> valuePriorityPairT;
    typedef unsigned short int gridCoordinateT;
    typedef pair<gridCoordinateT, gridCoordinateT> gridCoordinatePairT;
    typedef pair<gridCoordinatePairT, V> gridCoordinatesValueTripletT;
    // typedef

    Beaker(int gridWidth, int gridHeight, int gridPixelWidth, int gridPixelHeight,
           const string &name = "", bool isReactionRule = false)
        : gridWidth_(gridWidth),
          gridHeight_(gridHeight),
          gridPixelWidth_(gridPixelWidth),
          gridPixelHeight_(gridPixelHeight),
          name_(name),
          isReactionRule_(isReactionRule) {
        // if (!isReactionRule_)
        //     successionGrid_ = new vector<valuePriorityPairT>[gridWidth * gridHeight];
    }

    /**
     * @brief Creates a new smaller BeakerNode to serve as a reaction pattern to run in the main
     * grid.
     *
     */
    void makeNewReactionRule() {
        Beaker *reactionRule = new Beaker(this->ruleGridWidth_, this->ruleGridHeight_, 150, 150,
                                          "rule-" + clto_str(++this->ruleCount_), true);

        reactionRule->parentBeaker_ = this;
        this->reactionRules_.push_back(reactionRule);

        CLNodeFactory<BeakerNode, Beaker<V>, int> beakerBuilder("div", "rr");

        BeakerNode<Beaker<V>> *bn = beakerBuilder.withChildrenOf(beakerNode_->reactionRulesDiv_)
                                        .withTag("div")
                                        .withName("reactionRule")
                                        .withCppVal(reactionRule)
                                        .build();
        beakerNode_->refresh();
    }

    Beaker<unsigned char> *findRuleByName(const string &ruleName) {
        auto nameIs = [&ruleName](Beaker *b) { return (b->name_ == ruleName); };
        auto it = find_if(reactionRules_.begin(), reactionRules_.end(), nameIs);
        if (it == reactionRules_.end()) return nullptr;
        return *it;
    }

    vector<gridCoordinatesValueTripletT> makePixelList() {
        vector<gridCoordinatesValueTripletT> pixels;
        for (gridCoordinateT i = 0; i < this->gridWidth_; i++) {
            for (gridCoordinateT j = 0; j < this->gridHeight_; j++) {
                V pixelVal = this->beakerNode_->beakerCanvas_->getValXY(i, j);
                gridCoordinatePairT xy = pair(i, j);
                gridCoordinatesValueTripletT xyv = pair(xy, pixelVal);
                if (pixelVal != 0) {
                    this->newPixelList_.push_back(xyv);
                } else {
                    this->backgroundPixelList_.push_back(xyv);
                }
            }
        }
        // for (auto rule : reactionRules_) {
        //     rule->makePixelList();
        // }
        // this->newPixelList_ = pixels;
        return pixels;
    }

    /**
     * @brief Just a debug method to see if the successor setting code works.
     *
     */
    void printRuleStats() {
        int i = 0;
        for (auto rule : this->reactionRules_) {
            cout << "Rule " << i++ << ": " << rule->name_ << ", succ: " << rule->successor_->name_
                 << endl;
        }
    }

    bool matchList(vector<gridCoordinatesValueTripletT> &rulePixelList,
                   gridCoordinatePairT matchCoordiates) {
        auto [mx, my] = matchCoordiates;
        for (auto pixel : rulePixelList) {
            auto [pixelLocation, pixelVal] = pixel;
            int px = pixelLocation.first;
            int py = pixelLocation.second;
            px += mx;
            py += my;
            this->beakerNode_->beakerCanvas_->wrapCoordiates(px, py);
            V mainGridVal = this->beakerNode_->beakerCanvas_->getValXY(px, py);
            if (mainGridVal != pixelVal) return false;
        }
        return true;
    }

    bool matchesAt(Beaker<V> &rule, gridCoordinatePairT matchCoordiates) {
        // findRuleByName("rule-1");
        bool match = matchList(rule.newPixelList_, matchCoordiates);
        match = match && matchList(rule.backgroundPixelList_, matchCoordiates);
        return match;
    }

    /**
     * @brief This is just to make sure that we are always calculating addresses within our grids
     * the same way since there are two ways to organize these memory spaces.
     *
     * @param x
     * @param y
     * @return int
     */
    inline int linearizeGridCoordinates(gridCoordinateT x, gridCoordinateT y) {
        return (y * gridWidth_ + x);
    }

    void clearGrid() {
        cout << "clearGrid()" << endl;
        this->beakerNode_->beakerCanvas_->clearGridToValue(0);
    }

    void laydownMatchPixels(vector<gridCoordinatesValueTripletT> &rulePixelList,
                            gridCoordinatePairT matchCoordiates, Beaker<V> &reactionRule) {
        auto [mx, my] = matchCoordiates;
        int c = 0;
        for (auto pixel : rulePixelList) {
            auto [gridCoords, value] = pixel;
            int px = gridCoords.first;
            int py = gridCoords.second;
            px += mx + reactionRule.successorOffsetX_;
            py += my + reactionRule.successorOffsetY_;
            this->beakerNode_->beakerCanvas_->wrapCoordiates(px, py);
            // auto linearGridAddress = linearizeGridCoordinates(px, py);
            valuePriorityPairT vp = pair(value, reactionRule.successionPriority_);
            // cout << "Linear address for push_back is " << linearGridAddress << endl;
            successionMap_[pair(px, py)].push_back(vp);
            // successionGrid_[linearGridAddress].push_back(vp);
            c++;
        }
        cout << "Done laying down " << c << " match pixels for rule: " << reactionRule.name_
             << endl;

        // std::cout << key << " has value " << value << std::endl;
        // std::cout << it->first << " => " << it->second << '\n';
    }

    void updateGrid() {
        clean_ = false;
        for (const auto &[key, value] : this->successionMap_) {
            auto [px, py] = key;
            vector<valuePriorityPairT> vpStack = value;
            cout << "update coordinate: " << px << ", " << py << endl;
            if (!vpStack.empty()) {
                auto [val, pri] = vpStack.back();
                this->beakerNode_->beakerCanvas_->setValXYNoDraw(px, py, val);
            }
            // vpStack.pop
        }
    }

    bool toggleClean() {
        clean_ = !clean_;
        cout << "BEAKER CLEAN STATUS = " << clean_ << endl;
        return clean_;
    }

    void makeDirty() {
        clean_ = false;
        cout << "BEAKER IS DIRTY!" << endl;
    }

    /**
     * @brief Performs basic hygeine functions to maintain the lists that are used in matching and
     * iteration.
     *
     */
    void update() {
        for (auto reactionRule : reactionRules_) {
            reactionRule->update();
        }
        if (clean_) return;
        this->successionMap_.clear();
        this->newPixelList_.clear();
        this->backgroundPixelList_.clear();
        makePixelList();
        clean_ = true;
    }

    /**
     * @brief Apply replacement rules to main grid one time.
     *
     */
    void iterate() {
        this->update();
        this->beakerNode_->nodelog("ITERATING...");
        this->iterationCount_++;

        for (gridCoordinateT j = 0; j < gridHeight_; j++) {
            //string vals = "";
            for (gridCoordinateT i = 0; i < gridWidth_; i++) {
                for (auto reactionRule : reactionRules_) {
                    if (reactionRule->successor_ == reactionRule) continue;
                    bool matches = matchesAt(*reactionRule, pair(i, j));
                    if (matches) {
                        beakerNode_->nodelog("Match at " + clto_str(i) + "," + clto_str(j));
                        laydownMatchPixels(reactionRule->successor_->newPixelList_, pair(i, j),
                                           *reactionRule);
                        laydownMatchPixels(reactionRule->successor_->backgroundPixelList_, pair(i, j),
                                           *reactionRule);
                    }
                }

                // V xyVal = this->beakerNode_->beakerCanvas_->getValXY(i, j);

                // if (xyVal != 0) {
                //     this->beakerNode_->beakerCanvas_->setValXYNoDraw(i, j, xyVal - 1);
                // }
                // vals = "x: " + clto_str(i) + ", y: " + clto_str(j) + " = " + clto_str(int(xyVal))
                // + "\n";
                // vals += clto_str(int(xyVal)) + " ";
            }
            // cout << vals << endl;
            //  this->beakerNode_->nodelog(vals);
            //   cellCount++;
        }
        // this->beakerNode_->beakerCanvas_->drawGrid();
        // this->beakerNode_->beakerCanvas_->drawGrid();
        this->beakerNode_->refresh();
        this->printRuleStats();

        for (const auto &[key, value] : this->successionMap_) {
            auto [px, py] = key;
            vector<valuePriorityPairT> vpStack = value;
            cout << "coordinate: " << px << ", " << py << endl;
            for (auto [val, pri] : vpStack) {
                cout << "\tval = " << int(val) << ", pri = " << pri << endl;
            }
        }
        updateGrid();
        this->beakerNode_->beakerCanvas_->drawGrid();
    }

    static void makeNewReactionRule_st(Beaker *b) { b->makeNewReactionRule(); }

   protected:
    string name_;
    bool isReactionRule_ = false;  //!< Set to true if this Beaker is being used as a reaction rule
                                   //!< for an enclosing Beaker.

    int gridPixelWidth_ = 600;   //!< Width in pixels of beaker grid.
    int gridPixelHeight_ = 400;  //!< Height in pixels of beaker grid.

    int gridWidth_ = 60;   //!< Width of beaker grid in cells.
    int gridHeight_ = 40;  //!< Height of beaker grid in cells.

    int ruleGridWidth_ = 5;   //!< Width of new rule grid in cells.
    int ruleGridHeight_ = 3;  //!< Height of new rule grid in cells.

    V *gridArray;  //!< The actual grid data to be used by the CanvasGrid in BeakerNode.

    // vector<valuePriorityPairT>
    //     *successionGrid_;  //!< Grid with same dimensions as main grid but allows us to store
    //                        //!< "stacks" of value-priority pairs so that we can calculate correct
    //                        //!< succession value for each point in the grid.

    map<gridCoordinatePairT, vector<valuePriorityPairT>> successionMap_;

    int iterationCount_ =
        0;  //!< Counter that advances every time the rules are applied to the grid.
    BeakerNode<Beaker<V>> *beakerNode_;  //!< Pointer back to containing BN so that BN->refresh()
                                         //!< can be called when this updates.

    vector<Beaker *> reactionRules_;

    Beaker *parentBeaker_;

    Beaker *successor_ = this;  //!< The pattern we replace this one with.
    int successorOffsetX_ = 0;  //!< X offset of replacement pattern.
    int successorOffsetY_ = 0;  //!< Y offset of replacement pattern.
    // priorityT successionPriority_ = 1;  //!<
    int successionPriority_ = 1;  //!< Priority assigned to pixels replaced by application of
                                  //!< this pattern. Lower values take precedence.

    int ruleCount_ = 0;

    bool clean_ = true;  //!< We set this to false when the user modifies a rule or the main grid.

    vector<gridCoordinatesValueTripletT> newPixelList_;
    vector<gridCoordinatesValueTripletT> backgroundPixelList_;

    // template <typename U>
    friend class BeakerNode<Beaker<V>>;
};

EMSCRIPTEN_BINDINGS(PixelReactor) {
    class_<HybridNode<Beaker<unsigned char>::priorityT>>("HybridNode_priorityT")
        .function("updateNodeFromDom",
                  &HybridNode<Beaker<unsigned char>::priorityT>::updateNodeFromDom,
                  allow_raw_pointers());

    class_<BeakerNode<Beaker<unsigned char>>>("BeakerNode_h")
        .function("doNothing", &BeakerNode<Beaker<unsigned char>>::doNothing, allow_raw_pointers());

    class_<Beaker<unsigned char>>("Beaker")
        .function("toggleClean", &Beaker<unsigned char>::toggleClean, allow_raw_pointers())
        .function("clearGrid", &Beaker<unsigned char>::clearGrid, allow_raw_pointers())
        .function("makeDirty", &Beaker<unsigned char>::makeDirty, allow_raw_pointers())
        .function("iterate", &Beaker<unsigned char>::iterate, allow_raw_pointers())
        .function("makePixelList", &Beaker<unsigned char>::makePixelList, allow_raw_pointers())
        .function("makeNewReactionRule", &Beaker<unsigned char>::makeNewReactionRule,
                  allow_raw_pointers());

    register_vector<Beaker<unsigned char>::gridCoordinatesValueTripletT>(
        "vector<gridCoordinatesValueTripletT>");
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

#ifdef USETF
        auto *maindiv = builder.build();
#else
        auto *maindiv = builder.withAttachmentId("hookid")
                            .withAttachmentMode(clarity::ClarityNode::AttachmentMode::REPLACE_ID)
                            .build();
#endif

        CLNodeFactory<BeakerNode, Beaker<unsigned char>, int> beakerBuilder(
            builder.withChildrenOf(maindiv));

        Beaker<unsigned char> *b = new Beaker<unsigned char>(60, 40, 600, 400, "Beaker");

        BeakerNode<Beaker<unsigned char>> *bn =
            beakerBuilder.withTag("div").withName("mainBeaker").withCppVal(b).build();

        cout << "Setup complete!" << endl;
        return maindiv;
    }
};

#endif
