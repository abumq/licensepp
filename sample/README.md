### Sample

After you build using `make` run:

```
┗━━━━ $ ./license-manager-sample --license sample.licensepp
Licensed to sample-license
Subscription is active until 07 Jan, 2028 12:23 UTC
```

```
┗━━━━ $ ./license-manager-sample --license sample-with-signature.licensepp --signature sample-signature
Licensed to sample-license
Subscription is active until 07 Jan, 2028 12:24 UTC
```

```
┗━━━━ $ ./license-manager-sample --license sample-with-signature.licensepp
License is not valid
```

