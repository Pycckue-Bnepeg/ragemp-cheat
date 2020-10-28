```
`0xD2` - PACKET_RPC?
`0x03` - RPC_?? [Outcoming] запрос какой-то параши?
`0x31` - RPC_?? [Incoming]  хеш файла .listcahce
`0x32` - RPC_?? [Outcoming]  ClientJoin?
```

## RPC_0x03 (outcoming)
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
## RPC_0x02
```
d2                                  packet_id   (PACKET_RPC)
02 00                               rpc_id      (RPC_0x02)

d8 02                               ? (ID)
0b 00                               ? (sizeof last entry)
57 65 69 72 64 4e 65 77 62 69 65    ? (nickname)
```

## RPC_0x03 (incoming)
```
d2              packet_id (PACKET_RPC)
03 00           rpc_id (RPC_0x03)

d9 02           ? (ID or current online)
11              ?
00              ?
00              ?
05 00           ? (size of below)
63 6c 65 61 72  ?
00 00 00 00     ?
00 00 f0 41 
5e 01
1e 00
00
```

## RPC_0x04 (bugged?)
(size in bits)
```
// block 0

0x10
0x58
0x10 (size?)
(size or unk bits)

// block 1

0x20
(if first byte 0 goto block 2)
first byte << 6 bits
THEN GG

// block 2

loop 0xC times {
    0x01 bit
    (if true) 0x18
}

loop 0x8 times {
    0x01 bit
    (if true) 0x10
}

0x01 bit
(end if false)

// block 3

0x18
0x01
(if true) 0x90
0x140
0x10                            O
(todo) 0x28 up to 0xD times
(todo) if O -> 0x20 + (? << 6)
```
