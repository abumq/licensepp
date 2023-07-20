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
eyJhdXRob3JpdHlfc2lnbmF0dXJlIjoiNTdGNDU0QzM3NTUzMjU1NzBERkUzRDhCRTg3OURFN0RCQTg1MjUyNzJEMzBDRjc1MDlBRDhCODYxRjI0Nzk4RkE1REI3NzJDNkVFODUxQUNGNTcyQzM3MThDNzk4RDQwQzlFNjA1ODBDODE3RTNCRjBFRkEwMTVEOTkwMUY3QzJBNEM3NzkyRjhEREMxNkExMjM5RUJEQTM1MjQyRDdCRUE5N0FDNUYyN0U2RUVFMjJEQUI4ODk5MUEzRUExMkQ0NTE0RDBCN0VBOTlFQTk4RDA0MzJDNTM5QzY2QjVCMTI0RDExMTE3MzY3Njc2QjI0QzU2RDM3OEQxRURCRDQwNzhEN0JENTJDQzU4MEFDOEY5MDg0QjE4RjY4REQ3MUFDMTJEMkY4MDk1RkVCRUZBMTk3RDc5OTE2Q0U2MUQwNzg5QUY3MUQ2OUE2MzgzRjIzQ0Q1NDQ1NjEzQkU4QzE2NTMxMzc3N0IwRDcxQUM2NDk4NjM4NjBEQUExMEU4NTJCRDA0QTEzMjRDNDlGQjdDNTQyRjdDMTRGMjIxNDMzRTVDNDZCMDc1QUJEQkM1N0M3Rjk2NDVBNERDRkUzNkRFODZFNDBGMEJDRjdBRDIyMTk0Q0VEQzM1NkFGRThFRUVCN0NGQjA3RDM3N0JBRThDRDhFQzEyREFCNzE2NUQyMEQiLCJleHBpcnlfZGF0ZSI6MTcwMjc4MzQwNSwiaXNzdWVfZGF0ZSI6MTY4OTgyMzQwNSwiaXNzdWluZ19hdXRob3JpdHkiOiJmaXJld2Via2l0LWxpY2Vuc2luZyIsImxpY2Vuc2VlIjoiam9obi1jaXRpemVuIn0=
Licensed to john-citizen
Subscription is active until 17 Dec, 2023 03:23 UTC
```

Here `eyJhdXRob3JpdHlfc2ln ... XRpemVuIn0=` is the signed license.

You can give this license to the user. The user can then use this license to access your application.

This is very basic license, you can even have licensee signature or passphrase to access the license (see usage above)