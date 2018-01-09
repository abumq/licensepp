//
//  main.cc
//  License++ Unit Tests
//
//  Copyright © 2018-present Muflihun Labs
//

#include "test.h"
#include "license-manager-test.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return ::testing::UnitTest::GetInstance()->Run();
}
