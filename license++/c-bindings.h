#ifndef LICENSEPP_C_Bindings_h
#define LICENSEPP_C_Bindings_h

#include <stdint.h>

// License
#ifdef __cplusplus
extern "C"
#endif
    void*
    license_create();

#ifdef __cplusplus
extern "C"
#endif
    void
    license_delete(void* license);

#ifdef __cplusplus
extern "C"
#endif
    int
    license_load(void* license, const char* license_contents_base64);

#ifdef __cplusplus
extern "C"
#endif
    void
    license_set_licensee(void* license, const char* licensee);

#ifdef __cplusplus
extern "C"
#endif
    void
    license_set_issuing_authority_id(void* license,
                                     const char* issuing_authority_id);

#ifdef __cplusplus
extern "C"
#endif
    void
    license_set_licensee_signature(void* license,
                                   const char* licensee_signature);

#ifdef __cplusplus
extern "C"
#endif
    void
    license_set_authority_signature(void* license,
                                    const char* authority_signature);

#ifdef __cplusplus
extern "C"
#endif
    void
    license_set_expiry_date(void* license, const uint64_t expiry_date);

#ifdef __cplusplus
extern "C"
#endif
    void
    license_set_issue_date(void* license, const uint64_t issue_date);

#ifdef __cplusplus
extern "C"
#endif
    void
    license_set_additional_payload(void* license,
                                   const char* additional_payload);

#ifdef __cplusplus
extern "C"
#endif
    const char*
    license_get_licensee(const void* license);

#ifdef __cplusplus
extern "C"
#endif
    const char*
    license_get_issuing_authority_id(const void* license);

#ifdef __cplusplus
extern "C"
#endif
    const char*
    license_get_licensee_signature(const void* license);

#ifdef __cplusplus
extern "C"
#endif
    const char*
    license_get_authority_signature(const void* license);

#ifdef __cplusplus
extern "C"
#endif
    uint64_t
    license_get_expiry_date(const void* license);

#ifdef __cplusplus
extern "C"
#endif
    uint64_t
    license_get_issue_date(const void* license);

#ifdef __cplusplus
extern "C"
#endif
    const char*
    license_get_additional_payload(const void* license);

// Issuing Authority
#ifdef __cplusplus
extern "C"
#endif
    void*
    issuing_authority_create(const char* id, const char* name,
                             const char* keypair, unsigned int max_validity,
                             int active);

#ifdef __cplusplus
extern "C"
#endif
    void
    issuing_authority_delete(void* issuing_authority);

#ifdef __cplusplus
extern "C"
#endif
    const char*
    issuing_authority_get_id(const void* issuing_authority);

#ifdef __cplusplus
extern "C"
#endif
    const char*
    issuing_authority_get_name(const void* issuing_authority);

#ifdef __cplusplus
extern "C"
#endif
    int
    issuing_authority_get_active(const void* issuing_authority);

#ifdef __cplusplus
extern "C"
#endif
    unsigned int
    issuing_authority_get_max_validity(const void* issuing_authority);

#ifdef __cplusplus
extern "C"
#endif
    void*
    issuing_authority_issue(const void* issuing_authority, const char* licensee,
                            unsigned int validity_period,
                            const char* master_key, const char* secret,
                            const char* licensee_signature,
                            const char* additional_payload);

#ifdef __cplusplus
extern "C"
#endif
    int
    issuing_authority_validate(const void* issuing_authority,
                               const void* license, const char* master_key,
                               int validate_signature,
                               const char* licensee_signature);

// License Manager
#ifdef __cplusplus
extern "C"
#endif
    void*
    license_manager_create();

#ifdef __cplusplus
extern "C"
#endif
    void
    license_manager_delete(void* license_manager);

#ifdef __cplusplus
extern "C"
#endif
    const void*
    license_manager_get_issuing_authority(const void* license_manager,
                                          const void* license);

#ifdef __cplusplus
extern "C"
#endif
    const void*
    license_manager_issue(const void* license_manager, const char* licensee,
                          unsigned int validity_period,
                          const void* issuing_authority,
                          const char* issuing_authority_secret,
                          const char* licensee_signature,
                          const char* additional_payload);

#ifdef __cplusplus
extern "C"
#endif
    int
    license_manager_validate(const void* license_manager, const void* license,
                             int verify_licensee_signature,
                             const char* licensee_signature);

typedef struct IssuingAuthorityParameters {
  const char* authority_id;
  const char* authority_name;
  const char* keypair;
  unsigned int max_validity;
  int active;
  struct IssuingAuthorityParameters* next;
} IssuingAuthorityParameters;

#ifdef __cplusplus
extern "C"
#endif
    void
    license_key_register_init(
        const unsigned char* license_manager_signature_key,
        const IssuingAuthorityParameters* issuing_authority_parameters);

#endif /* LICENSEPP_C_Bindings_h */