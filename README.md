<p align="center">
  ﷽
</p>

![banner]

License++ is software licensing library that provides an abstract way to secure your software usage.

[![Build Status](https://img.shields.io/travis/amrayn/licensepp/master.svg?)](https://travis-ci.org/amrayn/licensepp/branches)
[![Build Status](https://img.shields.io/travis/amrayn/licensepp/develop.svg?)](https://travis-ci.org/amrayn/licensepp/branches)
[![Version](https://img.shields.io/github/release/amrayn/licensepp.svg)](https://github.com/amrayn/licensepp/releases/latest)

[![Donate](https://amrayn.github.io/donate.png?v2)](https://amrayn.com/donate)

## Features

 * RSA-Based signing to prevent alteration
 * Custom license keys for your software
 * Anyone can check the license validity
 * Restricted issuance of new licenses
 * Disable issuing authority at anytime

## Getting Started

### Dependencies

  * C++11
  * [Crypto++](https://www.cryptopp.com/) v5.6.5+ [with Pem Pack](https://amrayn.github.io/downloads/pem_pack.zip)
  * [cmake](https://cmake.org/) v2.8.12+

### Installation
 * [Download](https://github.com/amrayn/licensepp/archive/master.zip) or [clone](git@github.com:amrayn/licensepp.git) the repository
 * Install Crypto++
     ```
     git clone https://github.com/amrayn/licensepp
     git clone https://github.com/weidai11/cryptopp.git
     git clone https://github.com/amraynweb/cryptopp-pem.git
     cp cryptopp-pem/* cryptopp/
     cd cryptopp
     make
     sudo make install
     ```
 * Use CMake to build the project
     ```
     cd licensepp
     mkdir build
     cd build
     cmake ..
     make
     sudo make install

     ## build with test
     cmake -Dtest=ON ..
     make
     sudo make install
     ./licensepp-unit-tests
     ```
 * You can build [cli](/cli) tool to ensure license++ is installed properly

### Integration
 Once you have license++ properly installed please check out [sample](/sample) code for integration


## Generate New Authority Key
 Authority key is what is in [key register](/cli/licensing/license-manager-key-register.cc) (`LICENSE_ISSUING_AUTHORITIES`) You will need to generate a custom key for your software. This key is used to sign the digital license.

 Authority key is essentially RSA keypair with `:` seperator. Format of this key can be defined as:

 ```
 base64_encode(
  pem_format(
    rsa_private_key
  )
 ):base64_encode(
  pem_format(
    rsa_public_key
  )
 )
 ```

You can use [ripe](https://github.com/amrayn/ripe) command to generate new authority key or you can use openssl CLI tool to do so
```
ripe -g --rsa --length 2048 [--secret <secret>]
```

This will generate something like:

```
LS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlFb2dJQkFBS0NBUUVBM0NGOENidXQyUlIvdXo4T3JmZ1VVNGtmTUdRZWtTNDQzbVVxQkFTeUpibjU3bmhPCllQbjA3N0hSaXZVNk9mYWp6YWFVRFVOZGFseDlFVmJ1YnYwNGQ0KzBHeTFMbDRPUXFITTg3eVRXSG81Z1JYbGcKNGlCeUFDRnVYMGVaNm1iSC9SWmFQcjZHT2ZrVTdJMVJ6V1dDUlYwc1IyN0N5Q1VkM0lScnduS2JSdzRUQkt0Tgpkakk5M3BncDZGWXN1UjFtU3FWSlVqWjhPY29EWTd1MHJUOStadEhMckQyeGh4YWE5N2RFOEtiMEJhbnlwMThaCmFORmY4M2pRbVZia0xra0F6c1RZczdyekRyVDNCbXN2SG1uTFlUTHBUSjM5SktLRmMvWXZBbjFQN3VWMDlXaG8KSTVPL2pyWXFoRmNEYTN0dkVCTkJPT3Z3aUhZZVZ2bjJuWW5QUXdJQkVRS0NBUUFNOHVrdHY3N3VwdHBXVHdEZApEcGZJcmJhS1lENElpaitVbklvQVBJTHpLUTZ6ckxsUS82VC9Dblc4NFQrcERvSWJKK3FYWGxES0JYREV5T0RaCldqQi9nT3g2RWJrbkI3MDNGZFpvYTVRZjZrSGwrQlRDQWVpV21JNEZtc3pSalpOTEgyNjRaWTlzMG1xa2dNaVQKbkk4eFFiZFBmdnhtSUUwTStMc0xjQ2RBYWlGZnBqdGtoK3d4Wm9UV29EMVQraXgyNGxQa3JBVHpiVXU4bDZwYgoyTW0xbGJnWE1icGIvaFdtcVR4TUFFcW9hVTNXS1lTb1RpQ2kwVHZNNXJ1aStya3cvRm5XMnpzTGFHYWhQWGlqClR5RVAzdXJDSGdIUG9EMG1hbWlIVnpmQUJFSFJVWURjZlNOOTJlQlRsZlVLUHBRYVI3SWY4TjVQNDVucXZLalAKRUNSUkFvR0JBTjlOSmdjSmVpR0xOS1daa3p2QTIvVkl2b1cwbEFuRmhROFRNb3BMYzdmQUtPZXZEMkwyclpXSQpjUFhuWjR4Q2ZLaDlYbmtISklST3dsVmxpVmd1cjBrQkFtbkZiVU4vZFJXR0hGVnJpTlgvVnNXZDRib3oyeEpOClhSRXNIenZUdmh0bnVJZjFzNzViOFlBQ3d4ek9IcDgvR0hhZVJnRndjNmtUeEhhU0Jxd3hBb0dCQVB4ZGUxUy8Kc1cwTGxMSU9IdDVrcEgwYzN6RnVOaDVLQXV6S2R0cElabUdYdDZweTRPNTBpWlZlQ1UzOU9uS1VMeDd4UHRFMApmTERVMFlLWWV4WXM4MnVOQUhlcHZzUE5VUkRMT2RiYW44TlVEb3pLdmlRbkkyT1ppQ3paazJCYTN2cW5SbEdKCmtLWFhUTVQwbnFEdk5Qb1NOL1o3cHl2SjVUeXNya3BMY0xtekFvR0JBSjJmM3B1T09CZXRqcE1DL29TSUlzdEMKYUY1aFdYQlBNTDlZMkdHZXJBazhXUndER2V1QTh3QWtFNEJuRytxSlNPOW5qZnNVR2NiT0x0TGVRdFRWcU9nOAo4cVVEMUtnZHZBOHhmV2w1RlV2RFRFOUNSUHZvWG1kRnF4c3VOQ283SE1nTStycFRGWGRQOWNQRnR1Y29GWjJWCjh5YU4xeEFUSkhkWlAyTEJiaDh4QW9HQVorcEIxNW81LzdsNWRuNUk4aWx3NkRrUW0rSVdTTFVRUTJKdExMUm0KWkd1bCt1UUNSQkhlVEpBaDh1L014Y1NMMzVDRFJ4V2N3VWlTZ1JHY0dEQ2dkNVJwbXE5T2pOd1NValdRU1drVQptN2t6S3VvU0R1THdkRTVISVliRU5yd0Jka1RndUNtVjZlODl1b0xYNStvR3dWTEx2OWlRSVJibDY4NmlIcGVJCnhPOENnWUVBa21CWEV4dHZ0d3ZJOXhleEFpalVhS1dqalNiM2VnbS9rV3VXRjhVUmRxeTR4US80WVBYSTFuZUIKNlBlSjcrSkFIakJtNGlhTm9tMkptM3J2aVA3dStFMzlwNDhrSSszWlFpd2FzeTUyTmdkL3kxS3lJeFNTM2JFWApoZXdIQU1qbWh4YVh3R1c4TWJZbW4xSklsTWhzTmZIcUhHQ1lQNklYR3hzR0pqeXFJdmc9Ci0tLS0tRU5EIFJTQSBQUklWQVRFIEtFWS0tLS0tCg==
```

Which is base64-encoded keypair seperated with `:`

## Generate New Signature Key
License++ signature key is what's used to sign the licensee's signature. This is to protect the information with AES-CBC-128. Signature key is defined in 128-bit array in [key register](/cli/licensing/license-manager-key-register.cc) (`LICENSE_MANAGER_SIGNATURE_KEY`)

You can use [ripe](https://github.com/amrayn/ripe) to generate new key

```
ripe -g --aes --length 128
```

This will produce something like `82F36C25A912389ABFF8091C759303D2`. You then seperate each hex to create C++ array, i.e,

```
0x82, 0xF3, 0x6C, 0x25, 0xA9, 0x12, 0x38, 0x9A, 0xBF, 0xF8, 0x09, 0x1C, 0x75, 0x93, 0x03, 0xD2
```

This will now go in to the key register.

## CLI
CLI tool provide ability to generate new licenses and validate existing license. Each CLI tool's version is linked directly with your version of key register.


### Issue New License
```
./license-manager --issue --licensee john-citizen --period 3600 --authority sample-license-authority [--signature <signature>] [--additional-payload <some string>] 
```

### Validate License

```
./license-manager --validate license.file [--signature <signature>]
```

## License Format
Licenses generated using License++ are base64 encoded JSON. They look like as follows:

```
{
    "authority_signature":"632DBBF8BC35A2CAA8CBC2952615839F86A40965A00D6F7420BEFF3DA1D35B9E360B5781D326AD6853991BB31452290FF3415C788255B33D391373A4AA9590E0849C8649FCFC98A6DC827DE2CE61AF15DE21E62E069CE1DB20C72352E6C15A825C0A8140CE42FE7B56C1A53981B3598EEE3AD4EDDD07461269AFC2C8B25B4BDD0C7E6A92986E2F0EAEC5E7C4C673C47AB25B3EE46EF0F3B9C8120865B63E68A505DBC2E85F0646BDF3FD082CA62BB647A0BEFF34E0C7CCE40BD84EABFEA7D902A26D33ADA01F75E7291EC35472B4ED328E0E3AE654E85AAEAEB3FFC9E5A7DE4CB08C9ADAC803367DC2845821E0C310BA26EA26DB3196A65445528AABF45888F6",
    "expiry_date":1528386596,
    "issue_date":1515426596,
    "issuing_authority":"sample-license-authority",
    "licensee":"john-citizen",
    "licensee_signature":"61663531383163626334303566613461363362343232316632663764393638383A35574F4F4C466B532F7A4F79376446364F6155776C673D3D0D0A0D0A",
    "additional_payload":"SomeTestData"
}
```

### Format Explained

 | Item | Description |
 | ---- | ----------- |
 | `authority_signature` | Signature of the authority which prevents alteration in the license |
 | `expiry_date` | License expiry date epoch |
 | `issue_date` | License issue date epoch |
 | `issuing_authority` | ID of issuing authority as per key register |
 | `licensee` | Name of the license holder |
 | `licensee_signature` | If licensee signed this license this is encrypted against key provided in key register. All the licenses signed by licensee will be validated against it at validation time. |
 | `additional_payload` | Any string to be embedded into the license |

## License
```
Copyright (c) 2018-present Amrayn Web Services

https://github.com/amrayn
https://amrayn.com
https://muflihun.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

 [banner]: https://github.com/amrayn/licensepp/raw/master/licensepp.png
