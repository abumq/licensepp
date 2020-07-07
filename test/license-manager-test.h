//
//  license-manager-test.h
//  License++ Tests
//
//  Copyright © 2018-present Amrayn Web Services
//  https://amrayn.com
//
//  See https://github.com/amrayn/licensepp/blob/master/LICENSE 
//

#ifndef LICENSE_MANAGER_TEST_H
#define LICENSE_MANAGER_TEST_H

#include "test.h"
#include "test/license-manager-for-test.h"
#include <license++/license.h>

using namespace licensepp;

TEST(LicenseManagerTest, LicenseGenerationAndVerification)
{
    LicenseManagerForTest licenseManager;

    const IssuingAuthority* authority0 = &(LicenseManagerKeyRegister::LICENSE_ISSUING_AUTHORITIES.at(0));
    const IssuingAuthority* authority1 = &(LicenseManagerKeyRegister::LICENSE_ISSUING_AUTHORITIES.at(1));

    std::cout << "Generating licenceWithSignature using " << authority0->id() << std::endl;
    License licenceWithSignature = licenseManager.issue("licensepp unit-test", 24U, authority0, "", "fasdf");
    std::cout << "Generating licenceWithoutSignature using " << authority0->id() << std::endl;
    License licenceWithoutSignature = licenseManager.issue("licensepp unit-test", 24U, authority0);

    std::cout << "Generating licenseFromOtherAuthority using " << authority1->id() << std::endl;
    License licenseFromOtherAuthority = licenseManager.issue("licensepp unit-test license", 24U, authority1);

    ASSERT_TRUE(licenseManager.validate(&licenceWithSignature, true, "fasdf"));
    ASSERT_TRUE(licenseManager.validate(&licenceWithoutSignature, true, "fasdf"));
    ASSERT_FALSE(licenseManager.validate(&licenceWithSignature, true, "wrong-sign"));
    ASSERT_FALSE(licenseManager.validate(&licenceWithSignature, false)); // need signature
    ASSERT_FALSE(licenseManager.validate(&licenceWithSignature, true)); // need signature
    ASSERT_TRUE(licenseManager.validate(&licenceWithoutSignature, false, "fasdf"));
    ASSERT_TRUE(licenseManager.validate(&licenceWithoutSignature, false, ""));

    ASSERT_EQ(licenceWithSignature.licensee(), "licensepp unit-test");
    ASSERT_EQ(licenceWithoutSignature.licensee(), "licensepp unit-test");
    ASSERT_EQ(licenceWithSignature.issuingAuthorityId(), "unittest-issuer-1");
    ASSERT_EQ(licenceWithoutSignature.issuingAuthorityId(), "unittest-issuer-1");
    ASSERT_EQ(licenseFromOtherAuthority.issuingAuthorityId(), "unittest-issuer-2");

    ASSERT_TRUE(licenseManager.validate(&licenseFromOtherAuthority, false, ""));
}

#ifndef LICENSEPP_ON_CI
TEST(LicenseManagerTest, LicenseGenerationAndVerificationUsingSecureAuthority)
{
    const IssuingAuthority* authority2 = &(LicenseManagerKeyRegister::LICENSE_ISSUING_AUTHORITIES.at(2));
    LicenseManagerForTest licenseManager;
    std::cout << "Generating licenseWithSecureAuthority using " << authority2->id() << std::endl;
    License licenseWithSecureAuthority = licenseManager.issue("licensepp unit-test license", 25U, authority2, "unit-test-issuer-secret");
    ASSERT_EQ(licenseWithSecureAuthority.issuingAuthorityId(), "unittest-issuer-3");

    ASSERT_TRUE(licenseManager.validate(&licenseWithSecureAuthority, false));
}
#endif

TEST(LicenseManagerTest, VerificationUsingSecureAuthority)
{
    const IssuingAuthority* authority2 = &(LicenseManagerKeyRegister::LICENSE_ISSUING_AUTHORITIES.at(2));
    std::string validLicense = "eyJhdXRob3JpdHlfc2lnbmF0dXJlIjoiN0UyMDgyNjkwRkJEMzcwOTA2NkFFQzExOTJGMEFDOUY4NjI0RkI0ODYwMTNCMDhEMjVDMEMxQTI2MDZERThGODk0MENEREFFQUFFMkM2NTM5QTI3RTJCNTI3RTc4NjZCQUFBNUQ1ODg1RkJENTIzMEQzOTk3OEVBQzE3REY3NDZCNUFBNUJCM0RFN0VFNTU1QzgyMEE5QzFBRUNEQzZGMDQyNEUzRTgyQzBCQkNBMzhEMjVFQkYxNzNFNTIwNUM1MjVFQ0I5NkJCNTQ2OUI5REJDOUIxMTY1RjU0OEE2RTkzREMxNDk2Q0FDMTFBMDhFNTJERTY0REEwQzQzMDQzQiIsImV4cGlyeV9kYXRlIjoyMjkxMTY3NDgwLCJpc3N1ZV9kYXRlIjoxNTAyNzY3NDgwLCJpc3N1aW5nX2F1dGhvcml0eSI6InVuaXR0ZXN0LWlzc3Vlci0zIiwibGljZW5zZWUiOiJyZXNpZHVlIHVuaXQtdGVzdCBsaWNlbnNlIn0=";

    LicenseManagerForTest licenseManager;
    std::cout << "Verify using " << authority2->id() << std::endl;
    License license;
    license.load(validLicense);
    ASSERT_TRUE(licenseManager.validate(&license, false));
}

TEST(LicenseManagerTest, VerificationUsingSecureAuthorityAndSignature)
{
    const IssuingAuthority* authority2 = &(LicenseManagerKeyRegister::LICENSE_ISSUING_AUTHORITIES.at(2));
    std::string validLicense = "eyJhdXRob3JpdHlfc2lnbmF0dXJlIjoiODU5NzhEMzIwOEY4MTJGOUM1RTU2RTlDNDNCRTY1MTVBRDc2NTNENDk5QzBGNDcxNTQxOEVCMUJFRjQ4NEQ2RTI0NjdDODEyQTdBREM0QUY4QjM5N0M5MTJBMzk5MjMzNUQ3M0YyNEQxQzBDMDhFNDdCOEE1QTE1MTFBNjMyQTE3MThBRkVBMzI4M0YxNTlCREIzNjI2MzVCQjE3QjlCNzc5RDJDMUFFRDEzNDhCNjdEOEIyQjlCM0M3NzQ4NEU2MjA4QjQ5ODBEODczNDZFNDIzOUFDOTQ0RDhDMkI4RjA5NURDQUVCMDJCQjVCQTlGOUY4MkExQjdFRDNCRjI4QkVGOURBRTlGQzY1RUM1MjBCRjRGODY0QjlGQkFFREU4RTlCMDk0MjNDMTMzMjZBMzRBNjA4MzEwNTUwNTJDMDlEMjA0OEIxRTgxN0YyQjQwMDcwNjYyMEU4OUY1MTk1MzM0NzlDRjBBNzBGMDlDRkU5MDMwRkFFNkQzRDQ0NTFENTFERDE3QzVERjE4Q0RDREI0NzA5RDcxMDVGMjg1QTgyRTlDMzgyMjUxNTk4RTdFODVCNDZGMjZFOTRFOTRDMjg0NEM5M0M2QTE5NkEzNDI2MDU2NjE1QTEyNzlGQTJFOTA1OTZFOEJBM0U0RkM1N0I5NEY2Q0RCNEIyM0YyMzIiLCJleHBpcnlfZGF0ZSI6MTgzMDc5MTUwNiwiaXNzdWVfZGF0ZSI6MTUxNTQzMTUwNiwiaXNzdWluZ19hdXRob3JpdHkiOiJzYW1wbGUtbGljZW5zZS1hdXRob3JpdHkiLCJsaWNlbnNlZSI6InRpbmctdG9uZyIsImxpY2Vuc2VlX3NpZ25hdHVyZSI6IjM4Mzc2MjMzNjYzNTYzMzUzMTYxNjM2MTM5NjI2NTM2NjMzMDY1MzE2MjYxNjEzNjM5NjY2MTY1MzQ2MTMxMzkzQTRDMzk3OTcyNEM3ODQzNzA0NzUzMkI1NjM5NjE2MTU1NTYyRjU3NTk2MTZCNDI3NDZCNzk0NzU3MzM0MjQzNjYzNTU5MzIzNTRFNEU1NDcwNjc2MzZGM0QwRDBBMEQwQSJ9";

    LicenseManagerForTest licenseManager;
    std::cout << "Verify using " << authority2->id() << std::endl;
    License license;
    license.load(validLicense);
    ASSERT_TRUE(licenseManager.validate(&license, true, "unittest-signature"));
    ASSERT_FALSE(licenseManager.validate(&license, true, "wrong-signature"));
    ASSERT_FALSE(licenseManager.validate(&license, false));
    ASSERT_EQ(license.formattedExpiry(), "06 Jan, 2028 17:11 UTC");
}

TEST(LicenseManagerTest, VerificationOfAdditionalPayloadUsingSecureAuthority)
{
    const IssuingAuthority* authority2 = &(LicenseManagerKeyRegister::LICENSE_ISSUING_AUTHORITIES.at(3));
    std::string validLicense = "eyJhZGRpdGlvbmFsX3BheWxvYWQiOiJTb21lUmFuZG9tU3RyaW5nIiwiYXV0aG9yaXR5X3NpZ25hdHVyZSI6IjdBNjU2Qzg3QjdBOUVCRjhFMEVEMzQyMTdEMzIwQjNGQzZCMjAxNEJDRTI1OUZEQTQ3RUI5MzE1OUJGMzQzRTUwMjc5N0YwRjJCMjMwMDFFNjMwMEQwQTAzOTFCNjAzMkI1NUNCQzY4QTAzRkU2OTQ4MTZCN0JGMDA4NzJERTE0NEI2RkNDNkFGQ0E5MjRGNzEzRTZCQUQzQkZBRUI1QzU5RkUwNkUyNEI5RDA3QjMxN0QzRTc2N0RBOTcwNUY3REFDOUU2QzcxMUNCRDBBMzc3RjA5NTY5MTlENkZGMjk5NkMzN0U4OEM0NzFENzM3QjQwMUIxMzk1NzVCNEZDMzJGQzlBMkEyRUJGMTY0NjIxMjg1Qzk2NkM5NjZGRjI2MEMwOEQ1ODA2NUU5RkJFQTk5QUE5OTcwRDVCODZENzJDMDc2Nzg1NzkwQzk5NDIzRjYwMTdBMjcwNzY4RkQ1QzBGQjhFRDU4MERFOTg1QzlDMTM3OEQ3MTQ1QkQ4QzhCRDIzQzRBRDdBMkE0QzhDQ0NFQUU4NkI3OEYxODU4NDk4ODFGODZBNjBEQzhGMUFCNjI0M0I2RDBGOTUzNjI1QkMzREExMkRGNjE5Njc2MDIwQjEyQkU1QUEyNUIwQTVGMDJGMzFDREQ1ODExOUIxQzFDMjI1RTZDQUE5RjM2QzQ4IiwiZXhwaXJ5X2RhdGUiOjE5MDk1MTY1MjIsImlzc3VlX2RhdGUiOjE1OTQxNTY1MjIsImlzc3VpbmdfYXV0aG9yaXR5Ijoic2FtcGxlLWxpY2Vuc2UtYXV0aG9yaXR5IiwibGljZW5zZWUiOiJ1bml0LXRlc3QifQ==";
    LicenseManagerForTest licenseManager;
    std::cout << "Verify using " << authority2->id() << std::endl;
    License license;
    license.load(validLicense);
    ASSERT_TRUE(licenseManager.validate(&license, false));
    ASSERT_EQ(license.additionalPayload(),"SomeRandomString");
}

#endif // LICENSE_MANAGER_TEST_H
