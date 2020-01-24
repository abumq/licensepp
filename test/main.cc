//
//  main.cc
//  License++ Unit Tests
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#include "test.h"
#include "license-manager-test.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return ::testing::UnitTest::GetInstance()->Run();
}
