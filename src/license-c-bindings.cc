
#include <license++/issuing-authority.h>
#include <license++/license-c-bindings.h>
#include <license++/license-exception.h>
#include <license++/license.h>
#include <stdio.h>
#include <string.h>

#include <cstring>

#include "license++/base-license-manager.h"

namespace licensepp {
const std::array<unsigned char, 16>
    CLicenseKeysRegister::LICENSE_MANAGER_SIGNATURE_KEY = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const std::vector<IssuingAuthority>
    CLicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES =
        std::vector<IssuingAuthority>();

void CLicenseKeysRegister::initialize_license_issuing_authorities(
    const unsigned char* license_manager_signature_key,
    const IssuingAuthorityParameters* issuing_authority_parameters) {
  auto authorities =
      (std::vector<
          IssuingAuthority>*)&CLicenseKeysRegister::LICENSE_ISSUING_AUTHORITIES;
  authorities->clear();

  auto p = issuing_authority_parameters;
  while (p) {
    authorities->emplace_back(
        ::licensepp::IssuingAuthority(p->authority_id, p->authority_name,
                                      p->keypair, p->max_validity, p->active));
    p = p->next;
  }

  auto signature_key =
      (std::array<unsigned char,
                  16>*)&CLicenseKeysRegister::LICENSE_MANAGER_SIGNATURE_KEY;
  memcpy(signature_key->data(), license_manager_signature_key,
         16 * sizeof(unsigned char));
}
}  // namespace licensepp

// License Key Register
extern "C" void license_key_register_init(
    const unsigned char* license_manager_signature_key,
    const IssuingAuthorityParameters* issuing_authority_parameters) {
  ::licensepp::CLicenseKeysRegister::initialize_license_issuing_authorities(
      license_manager_signature_key, issuing_authority_parameters);
}

// License
extern "C" void* license_create() { return new ::licensepp::License(); }

extern "C" void license_delete(void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  delete p;
}

extern "C" void license_set_licensee(void* license, const char* licensee) {
  ::licensepp::License* p = (::licensepp::License*)license;
  p->setLicensee(licensee);
}

extern "C" void license_set_issuing_authority_id(
    void* license, const char* issuing_authority_id) {
  ::licensepp::License* p = (::licensepp::License*)license;
  p->setIssuingAuthorityId(issuing_authority_id);
}

extern "C" void license_set_licensee_signature(void* license,
                                               const char* licensee_signature) {
  ::licensepp::License* p = (::licensepp::License*)license;
  p->setLicenseeSignature(licensee_signature);
}

extern "C" void license_set_authority_signature(
    void* license, const char* authority_signature) {
  ::licensepp::License* p = (::licensepp::License*)license;
  p->setAuthoritySignature(authority_signature);
}

extern "C" void license_set_expiry_date(void* license,
                                        const uint64_t expiry_date) {
  ::licensepp::License* p = (::licensepp::License*)license;
  p->setExpiryDate(expiry_date);
}

extern "C" void license_set_issue_date(void* license,
                                       const uint64_t issue_date) {
  ::licensepp::License* p = (::licensepp::License*)license;
  p->setIssueDate(issue_date);
}

extern "C" void license_set_additional_payload(void* license,
                                               const char* additional_payload) {
  ::licensepp::License* p = (::licensepp::License*)license;
  p->setAdditionalPayload(additional_payload);
}

extern "C" const char* license_get_licensee(const void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  return p->licensee().c_str();
}

extern "C" const char* license_get_issuing_authority_id(const void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  return p->issuingAuthorityId().c_str();
}

extern "C" const char* license_get_licensee_signature(const void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  return p->licenseeSignature().c_str();
}

extern "C" const char* license_get_authority_signature(const void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  return p->authoritySignature().c_str();
}

extern "C" uint64_t license_get_expiry_date(const void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  return p->expiryDate();
}

extern "C" uint64_t license_get_issue_date(const void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  return p->issueDate();
}

extern "C" const char* license_get_additional_payload(const void* license) {
  ::licensepp::License* p = (::licensepp::License*)license;
  return p->additionalPayload().c_str();
}

// Issuing Authority
extern "C" void* issuing_authority_create(const char* id, const char* name,
                                          const char* keypair,
                                          unsigned int max_validity,
                                          bool active) {
  std::string _id(id);
  std::string _name(name);
  std::string _keypair(keypair);
  return new ::licensepp::IssuingAuthority(_id, _name, _keypair, max_validity,
                                           active);
}

extern "C" void issuing_authority_delete(void* issuing_authority) {
  ::licensepp::IssuingAuthority* p =
      (::licensepp::IssuingAuthority*)issuing_authority;
  delete p;
}

extern "C" const char* issuing_authority_get_id(const void* issuing_authority) {
  ::licensepp::IssuingAuthority* p =
      (::licensepp::IssuingAuthority*)issuing_authority;
  return p->id().c_str();
}

extern "C" const char* issuing_authority_get_name(
    const void* issuing_authority) {
  ::licensepp::IssuingAuthority* p =
      (::licensepp::IssuingAuthority*)issuing_authority;
  return p->name().c_str();
}

extern "C" bool issuing_authority_get_active(const void* issuing_authority) {
  ::licensepp::IssuingAuthority* p =
      (::licensepp::IssuingAuthority*)issuing_authority;
  return p->active();
}

extern "C" unsigned int issuing_authority_get_max_validity(
    const void* issuing_authority) {
  ::licensepp::IssuingAuthority* p =
      (::licensepp::IssuingAuthority*)issuing_authority;
  return p->maxValidity();
}

extern "C" void* issuing_authority_issue(
    const void* issuing_authority, const char* licensee,
    unsigned int validity_period, const char* master_key, const char* secret,
    const char* licensee_signature, const char* additional_payload) {
  ::licensepp::IssuingAuthority* p =
      (::licensepp::IssuingAuthority*)issuing_authority;
  std::string _licensee(licensee);
  std::string _master_key(master_key);
  std::string _secret(secret);
  std::string _licensee_signature(licensee_signature);
  std::string _additional_payload(additional_payload);

  auto license = p->issue(_licensee, validity_period, _master_key, _secret,
                          _licensee_signature, _additional_payload);
  return new ::licensepp::License(license);
}

extern "C" bool issuing_authority_validate(const void* issuing_authority,
                                           const void* license,
                                           const char* master_key,
                                           bool validate_signature,
                                           const char* licensee_signature) {
  ::licensepp::IssuingAuthority* p =
      (::licensepp::IssuingAuthority*)issuing_authority;
  ::licensepp::License* _license = (::licensepp::License*)license;
  std::string _master_key(master_key);
  std::string _licensee_signature(licensee_signature);
  return p->validate(_license, _master_key, validate_signature,
                     _licensee_signature);
}

// License Manager
extern "C" void* license_manager_create() {
  return new ::licensepp::BaseLicenseManager<
      ::licensepp::CLicenseKeysRegister>();
}

extern "C" void license_manager_delete(void* license_manager) {
  ::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>* p =
      (::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>*)
          license_manager;
  delete p;
}

extern "C" const void* license_manager_get_issuing_authority(
    const void* license_manager, const void* license) {
  ::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>* p =
      (::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>*)
          license_manager;
  return p->getIssuingAuthority((const ::licensepp::License*)license);
}

extern "C" const void* license_manager_issue(
    const void* license_manager, const char* licensee,
    unsigned int validity_period, const void* issuing_authority,
    const char* issuing_authority_secret, const char* licensee_signature,
    const char* additional_payload) {
  ::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>* p =
      (::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>*)
          license_manager;
  return new ::licensepp::License(p->issue(
      licensee, validity_period,
      (const ::licensepp::IssuingAuthority*)issuing_authority,
      issuing_authority_secret, licensee_signature, additional_payload));
}

extern "C" bool license_manager_validate(const void* license_manager,
                                         const void* license,
                                         bool verify_licensee_signature,
                                         const char* licensee_signature) {
  ::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>* p =
      (::licensepp::BaseLicenseManager< ::licensepp::CLicenseKeysRegister>*)
          license_manager;
  return p->validate((const ::licensepp::License*)license,
                     verify_licensee_signature, licensee_signature);
}