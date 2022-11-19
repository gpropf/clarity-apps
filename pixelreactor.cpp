#include <map>

#include "clarity.hpp"
#include "PageContent.hpp"
#include "Pixelreactor.hpp"
#include "ClarityNode.hpp"
#include "CLNodeFactory.hpp"

#include "embindings.hpp"
#include "globals.hpp"

int main() {
    PixelReactor pixelreactor;
    TestFramework testFramework;
    testFramework.content(pixelreactor.content());
    return 0;
}
