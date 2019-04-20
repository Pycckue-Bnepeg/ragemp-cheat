```
`0xD2` - PACKET_RPC?
`0x03` - RPC_?? [Outcoming] запрос какой-то параши?
`0x31` - RPC_?? [Incoming]  хеш файла .listcahce
`0x32` - RPC_?? [Outcoming]  ClientJoin?
```
## RPC_0x03
RPC_RequestJoin
```
client_version [4 bytes] (на данный момент 6)
???? [1 byte] (выглядит что всегда 0)
???? [4 bytes] (не используется?)
```

## RPC_0x32
RPC_Join
```
strlen(2b) -> nickname
strlen(2b) -> serial
strlen(2b) -> socialclub name
```
## RPC_0x31
RPC_ClientPackages
```
hash [8 bytes]
custom_fastdl_host? [1 bit]
length of host name [2 bytes]
hostname [n bytes] (строка, если custom_fastdl_host true, иначе порт в двух байтах)
voice_sample_rate [2 bytes]
```
0x31 dump
```
0000   02 00 00 00 45 00 00 36 0e 76 00 00 80 11 00 00   ....E..6.v......
0010   7f 00 00 01 7f 00 00 01 55 f5 de c9 00 22 5e 14   ........UõÞÉ."^.
0020   84 02 00 00 40 79 00 01 00 00 d2 31 00 <73 8d 0c   ....@y....Ò1.s..
0030   bd f5 12 b5 4f> 7b 2a 80 00 00                     ½õ.µO{*...
```
