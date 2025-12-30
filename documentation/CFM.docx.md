**CFM/CFM II package format**

[15.07.201115.07.2011](https://web.archive.org/web/20230319212702/https:/phreakerclub.com/date/2011/07/15) | [DiezDiez](https://web.archive.org/web/20230319212702/https:/phreakerclub.com/author/diez) | 11:23

**Categories:** 

* [**Codegrabber**](https://web.archive.org/web/20230319212702/https:/phreakerclub.com/category/kodgrabber)  
     
* [**Package formats**](https://web.archive.org/web/20230319212702/https:/phreakerclub.com/category/format)

The packet consists of 65 \- 66 bits, the first bit is the start bit, then 64 bits of data, the rest can be ignored, the last bit is the end of the packet flag, perhaps some also transmit the low battery bit, similar to KeeLoq.

 

 

At the beginning of the packet there is a preamble \- a meander of 600 μs. Next, the header is 2400/1000 μs, met 2400/700 μs. The first bit is zero \- it’s also the start bit. Zero bits 600/600 µs, ones 1000/1000 µs.

Discussion [here](https://web.archive.org/web/20230319212702/https:/phreaker.ru/forum/showthread.php?t=18) .

