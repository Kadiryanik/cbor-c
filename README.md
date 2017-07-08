Simple CBOR decoder
===================

A simple implementation of Concise Binary Object Representation (CBOR).
[http://tools.ietf.org/html/rfc7049](http://tools.ietf.org/html/rfc7049)
#### Example
```
    Input Data:
    { 0x83, 0x61, 0x61, 0xa1, 0x61, 0x62, 0x61, 0x63, 0xa2, 0x03, 0x61, 0x65, 0x04, 0x61, 0x61 }
    Output Data:
    ["a", {"b": "c"}, {3: "e", 4: "a"}]
```
#### Todo
 - All type support
 - Human readable format to cbor