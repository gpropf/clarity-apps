#include <map>

#include <clarity.hpp>
#include <PageContent.hpp>
#include "Pixelreactor.hpp"
#include "ClarityNode.hpp"
#include "CLNodeFactory.hpp"

#include "embindings.hpp"
#include "globals.hpp"


template <>
const array<string, 8> CanvasGrid<unsigned char>::colors = {"#F5F5DC", "#00FF00", "#00AA00", "#FF00FF",
                                                            "#AA00AA", "#00AAFF", "#9090AA", "#888888"};

int main() {
    PixelReactor pixelreactor;
    TestFramework testFramework;
    testFramework.content(pixelreactor.content());
    return 0;
}
