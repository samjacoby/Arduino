#Tiny Touch

This code provides rudimentary single-pin touch capabilities to an AtTiny. I'm
sure this mirrors some of the implementations in Atmel's Qtouch library. The
meat of the operation comes courtesy of David Mellis.

You can do sensing on five pins--assuming you disable the reset--leaving one
open for output or bit-banged serial or whatever you want.
