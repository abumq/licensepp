## CLI
This CLI provides basic functionalities for using the library like issuing the license and validating it.

### Build
Before building, make sure the license++ is locally installed.

```
make
```

### Usage
The usage for license-manager CLI is:

```bash
## validate
license-manager [--validate <file> --signature <signature>]
## issue
license-manager [--issue --licensee <licensee> --signature <licensee_signature> --period <validation_period> --authority <issuing_authority> --passphrase <passphrase_for_issuing_authority> [--additional-payload <additional data>]]
```

### Example
Now issue the license using the existing licensee register (see `licensing/license-manager-key-register.cc` file)

```
./license-manager --issue --licensee john-citizen --period 3600 --authority firewebkit-licensing
```

The expected output is something similar to:

```
eyJleHBpcnlfZGF0ZSI6MTcwMjc3OTk0NiwiaXNzdWVfZGF0ZSI6MTY4OTgxOTk0NiwiaXNzdWluZ19hdXRob3JpdHkiOiJmaXJld2Via2l0LWxpY2Vuc2luZyIsImxpY2Vuc2VlIjoiam9obi1jaXRpemVuIn0=
Licensed to john-citizen
Subscription is active until 17 Dec, 2023 02:25 UTC
```

The `eyJleHBpcnlfZGF0ZSI6MTcwMjc3OTk0NiwiaXNzdWVfZGF0ZSI6MTY4OTgxOTk0NiwiaXNzdWluZ19hdXRob3JpdHkiOiJmaXJld2Via2l0LWxpY2Vuc2luZyIsImxpY2Vuc2VlIjoiam9obi1jaXRpemVuIn0=` is the license.

You can give this license to the user. The user can then use this license to access your application.

This is very basic license, you can even have licensee signature or passphrase to access the license (see usage above)