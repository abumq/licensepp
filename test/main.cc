//
//  main.cc
//  License++ Unit Tests
//
//  Copyright © 2018-present Zuhd Web Services
//  https://zuhd.org
//
//  See https://github.com/zuhd-org/licensepp/blob/master/LICENSE 
//

#include "test.h"
#include "license-manager-test.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return ::testing::UnitTest::GetInstance()->Run();
}
