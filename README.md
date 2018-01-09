

## Generate New Authority Key

```
ripe -g --rsa --length 2048
```

## Generate New Signature Key

```
ripe -g --aes --length 128
```

## Issue New License

```
./license-manager --issue --licensee john-citizen --period 3600 --authority sample-license-authority [--signature <signature>]
```

## Validate License

```
./license-manager --validate license.file [--signature <signature>]
```
