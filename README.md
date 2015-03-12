Simple Protocol Parser
======================

*Actually, this is [ssdb](http://ssdb.io)'s network protocol, and I think it can
be used on other projects.*

- Lua Port: https://github.com/hit9/spp_lua
- Python Port: https://github.com/hit9/spp_py


Support Engines
--------------

- nodejs (>=v0.10.30)
- iojs (>=v1.0.4)

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
  console.log(res); // ['ok']
}
```

API Ref
-------

- parser.feed(buffer/string)
- parser.get()
- parser.clear()

Benchmark
---------

```
$ node bench.js
spp parser: 500000 in 4.127s => 121153 ops
nodejs parser: 500000 in 6.143s => 81393 ops
```

License
-------

MIT (c) 2014, hit9 (Chao Wang).
