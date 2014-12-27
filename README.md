Simple Protocol Parser
======================

*Actually, this is [ssdb](ssdb.io)'s network protocol, and I think it can
be used on other projects.*

Protocol
--------

```
Packet := Block+ '\n'
Block  := Size '\n' Data '\n'
Size   := literal_integer
Data   := string_bytes
```

For example:

```
3
set
3
key
3
val

```

Install
-------

```
npm install spp
```


Usage
-----

This package only provides parser, because packing is
easy to do.


Parsing example:

```js
var spp = require('spp'),
    parser = new spp.Parser();

parser.feed('2\nok\n\n');

var res;
while((res = parser.get()) !== undefined) {
  ..
}
```
