#include <map>

#include "clarity.hpp"
#include "PageContent.hpp"
#include "CLDemo.hpp"
#include "ClarityNode.hpp"
#include "CLNodeFactory.hpp"

#include "embindings.hpp"
#include "globals.hpp"


int main() {
    CLDemo cldemo;
    TestFramework testFramework;
    testFramework.content(cldemo.content());
    return 0;
}
