# Inception Synthetic Test Cases

This sub-directory contains synthetic test cases (derived from public code or handmade).
The purpose of this test is to evaluate the correctness of the Inception framework by comparing it with the native execution.
For example, giving an AES algorithm, a key and a plain text, the resulting cipher should be the same on both target.
The comparaison is made on pre-computed data, therefore, there is no need for the real device.

A build.sh script enables user to launch test cases easily.

```
./build.sh <examples> <boards>
```

Supported boards are directory names in /config.
Supported examples are directory names in ./Examples

