### Sample

After you build using `make` run:

```
./license-manager-sample --license sample.licensepp
```
or
```
LD_LIBRARY_PATH=/usr/local/lib/ ./license-manager-sample --license sample.licensepp
```
or
```
LD_LIBRARY_PATH=../build ./license-manager-sample --license sample.licensepp
```

```
./license-manager-sample --license sample-with-signature.licensepp --signature sample-signature
```
or
```
LD_LIBRARY_PATH=/usr/local/lib/ ./license-manager-sample --license sample-with-signature.licensepp --signature sample-signature
```
or
```
LD_LIBRARY_PATH=../build ./license-manager-sample --license sample-with-signature.licensepp --signature sample-signature
```
