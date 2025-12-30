Pro version is no different from previous models, except that in Pro the signal is inverted and the preamble is longer, which is clearly visible in the figure:

![][image1]

Logical zero \- 750\\750 us  
Logical unit \- 1000\\1000 us  
Header \- 1500\\1500 us  
The oscillogram was taken by PowerGraph through a sound card, so the same time intervals of low and high levels seem different, the low level is more elongated.  
The Scher-Khan 7Pro, 8Pro, 9, 10 package consists of one zero start bit and 56 data bits:  
reg\_encr \= 7C 21 04 0D CB 31 04 reg\_decr \= 7C 6D 01 41 A3 87 00  
reg\_encr \= 79 C3 70 1C FB CE 8A reg\_decr \= 79 6E 01 41 A3 87 00  
reg\_encr \= 76 90 C4 8A 16 4 5 2C reg\_decr \= 76 73 01 41 A3 87 00  
reg\_encr \= 73 70 78 D2 1E D4 12 reg\_decr \= 73 74 01 41 A3 87 00  
reg\_encr \= 70 A0 64 C2 C1 15 F8 reg\_decr \= 70 75 01 41 A3 87 00  
reg\_encr \= 7D 23 30 3C FC F9 BC reg\_decr \= 7D 7A 01 41 A3 87 00  
reg\_encr \= 7A 04 50 21 11 E1 B4 reg\_decr \= 7A 7B 01 41 A3 87 00  
reg\_encr \= 77 B0 6A 56 ED A6 1A reg\_decr \= 77 7C 01 41 A3 87 00  
scher-khan-pro — Oscillogram in PowerGraph format.  
The Scher-Khan 3, 4, 5, 6, 7, 8 packet consists of 51 data bits:  
4th button  
03 D5 89 A8 57 82 EE  
03 D5 89 A8 57 82 ED  
3rd button  
03 D5 89 A4 5B 82 EC  
03 D5 89 A4 5B 82 EB  
2nd button  
03 D5 89 A2 5D 82 EA  
03 D5 89 A2 5D 82 E9  
1st button  
03 D5 89 A1 5E 82 E8  
03 D5 89 A1 5E 82 E7  
scher-khan \- Oscillogram in PowerGraph format.  
The Scher-Khan III, IV packet consists of 35 bits of data. The code is static, consists of the key fob serial number and 5 button bits.

reg\_encr \= 7C 21 04 0D CB 31 04 reg\_decr \= 7C 6D 01 41 A3 87 00  
reg\_encr \= 79 C3 70 1C FB CE 8A reg\_decr \= 79 6E 01 41 A3 87 00  
reg\_encr \= 76 90 C4 8A 16 45 2C reg\_decr \= 76 73 01 41 A3 87 00  
reg\_encr \= 73 70 78 D2 1E D4 12 reg\_decr \= 73 74 01 41 A3 87 00  
reg\_encr \= 70 A0 64 C2 C1 15 F8 reg\_decr \= 70 75 01 41 A3 87 00  
reg\_encr \= 7D 23 30 3C FC F9 BC reg\_decr \= 7D 7A 01 41 A3 87 00  
reg\_encr \= 7A 04 50 21 11 E1 B4 reg\_decr \= 7A 7B 01 41 A3 87 00  
reg\_encr \= 77 B0 6A 56 ED A6 1A reg\_decr \= 77 7C 01 41 A3 87 00

reg\_encr \= E3 50 70 72 0C 18 0A reg\_decr \= E3 E6 01 C1 42 24 00  
reg\_encr \= E0 E0 74 E3 49 D9 7C reg\_decr \= E0 E7 01 C1 42 24 00  
reg\_encr \= ED CE 5F C7 A8 07 1E reg\_dec r \= ED 91 23 91 42 24 00  
reg\_encr \= EA C7 EB C7 32 A1 FE reg\_decr \= EA 53 19 91 42 24 00  
reg\_encr \= 79 C3 70 1C FB CE 8A reg\_decr \= 79 6E 01 41 A3 87 00  
reg\_encr \= 76 90 C4 8A 16 4 5 2C reg\_decr \= 76 73 01 41 A3 87 00  
reg\_encr \= 73 70 78 D2 1E D4 12 reg\_decr \= 73 74 01 41 A3 87 00  
reg\_encr \= 70 A0 64 C2 C1 15 F8 reg\_decr \= 70 75 01 41 A3 87 00  
reg\_encr \= 7D 23 30 3C FC F9 BC reg\_decr \= 7D 7A 01 41 A3 87 00  
reg\_encr \= 7A 04 50 21 11 E1 B4 reg\_decr \= 7A 7B 01 41 A3 87 00  
reg\_encr \= 77 B0 6A 56 ED A6 1A reg\_decr \= 77 7C 01 41 A3 87 00

first step is to check the checksum of the received packet. We calculate the last byte based on the first 6\.

static Byte CheckSumm(Byte x, Byte reg35a)  
{  
int i;  
Byte reg35, reg36, reg38;  
reg36 \= x;  
reg35 \= reg35a;  
i \= 8;  
do  
{  
reg38 \= (Byte)(reg35 ^ reg36);  
reg35 \= (Byte)(reg35 \<\< 1);  
if ((reg38 & 0x80) \== 0x80) { reg35 \= (Byte)(reg35 ^ 0x2A); }  
reg36 \= (Byte)(reg36 \<\< 1);  
i \= i \- 1;  
} while (i \> 0);  
return reg35;  
}

And this is how to apply this procedure. As a result, the last byte of the received message and the calculated checksum must match. If it coincides, it means that the Sherkhan worked.

csumm \= 0xFF;  
for (i \= 0; i \< 6; i++)  
{  
csumm \= CheckSumm(reg\_encr\[i\], csumm);  
}

uint8\_t CheckSumm(uint8\_t x, uint8\_t reg35a) {  
    int i;  
    uint8\_t reg35, reg36, reg38;  
    reg36 \= x;  
    reg35 \= reg35a;  
    i \= 8;  
    do {  
        reg38 \= (uint8\_t)(reg35 ^ reg36);  
        reg35 \= (uint8\_t)(reg35 \<\< 1);  
        if ((reg38 & 0x80) \== 0x80) {   
            reg35 \= (uint8\_t)(reg35 ^ 0x2A);   
        }  
        reg36 \= (uint8\_t)(reg36 \<\< 1);  
        i \= i \- 1;  
    } while (i \> 0);  
    return reg35;  
}

00003FFD5 032BCC 30000 Reply MAGIC CODE\*  
00003FFD5 239BCC 30000 Reply MAGIC CODE\*  
00003FFD5 23C7EC 10000 Reply MAGIC CODE\*   
00003FFD5 23DBEC 10000 Reply MAGIC CODE\*   
00003FFD5 235BEC 10000 Reply MAGIC CODE\*  
00003FFD5 031BCC 30000 Reply MAGIC CODE\*  
0000000349C1619E82B3 MAGIC CODE, dynamics   
0000000349C1629D 82B4 MAGIC CODE, dynamics  
00003FFD5 032BCC 30000 Response MAGIC CODE\* unlocked (lock opened)  
00003FFD5 23C7EC 10000 Response MAGIC CODE\* door open and alarm triggered  
00003FFD5 235BEC 10000 Response MAGIC CODE\* STRIKE hammer  
00003FFD5 23DBEC 10000 Reply MAGIC CODE\*

311A60080001 \-1beep  
311A60080002 \-2beep  
311A60080003 \-3beep  
311A60080004 \-4beep  
311A60080005 \-5beep  
311A60080006 \-sound  
311A60080007 \-pager1  
311A60080008 \-pager2short  
311A60080009 \-pager3long  
311A6008000A \-alarmshort  
311A6018000A \-Alarmlong  
311A6008000B \-sound2  
311A6008000C \-longbeep  
311A6008000D \-shortbeepsound  
311A6008000E \-ONsound  
311A6008000F \-2beepwithpause

explain about 5 bits at the beginning. the logger that posted exchange works with the old 51-bit encoding, so to make it more convenient to look at packets in hexadecimal form, the logger displays them by adding five zeros in front 5+51=56 \== 7 bytes. Sherkhan PRO was also fed into the same logger through an inverter and got a slightly crooked 5+56 yes \+ more zeros at the end

Due to the fact that there are actually 7 full-fledged bytes, there is no need to attach the start bit anywhere and output it as in my example

     MAGICAR 51 bit 00000001A99121DE83C3 MAGIC CODE, Dynamic  
     0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100  
     0E8C1629D830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101  
     0E8C1649B830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110  
     0E8C16897830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111  
                                                                    Serial         Key  Ser \~Key   CNT  
    

/\*\*  
 \* Analysis of received data  
 \* @param instance Pointer to a SubGhzBlockGeneric\* instance  
 \* @param protocol\_name  
 \*/  
static void subghz\_protocol\_scher\_khan\_check\_remote\_controller(  
    SubGhzBlockGeneric\* instance,  
    const char\*\* protocol\_name) {

    switch(instance-\>data\_count\_bit) {  
    case 35: //MAGIC CODE, Static  
        \*protocol\_name \= "MAGIC CODE, Static";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 51: //MAGIC CODE, Dynamic  
        \*protocol\_name \= "MAGIC CODE, Dynamic";  
        instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) & 0x0F);  
        instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;  
        instance-\>cnt \= instance-\>data & 0xFFFF;  
        break;  
    case 57: //MAGIC CODE PRO / PRO2  
        \*protocol\_name \= "MAGIC CODE PRO/PRO2";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 63: //MAGIC CODE, Dynamic Response  
        \*protocol\_name \= "MAGIC CODE, Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 64: //MAGICAR, Response ???  
        \*protocol\_name \= "MAGICAR, Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 81: //MAGIC CODE PRO / PRO2 Response ???  
        \*protocol\_name \= "MAGIC CODE PRO,\\n Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 82: //MAGIC CODE PRO / PRO2 Response ???  
        \*protocol\_name \= "MAGIC CODE PRO,\\n Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;

    default:  
        \*protocol\_name \= "Unknown";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    }  
}

uint32\_t subghz\_protocol\_decoder\_scher\_khan\_get\_hash\_data(void\* context) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderScherKhan\* instance \= context;  
    return subghz\_protocol\_blocks\_get\_hash\_data\_long(  
        \&instance-\>decoder, (instance-\>decoder.decode\_count\_bit / 8\) \+ 1);  
}

SubGhzProtocolStatus subghz\_protocol\_decoder\_scher\_khan\_serialize(  
    void\* context,  
    FlipperFormat\* flipper\_format,  
    SubGhzRadioPreset\* preset) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderScherKhan\* instance \= context;  
    return subghz\_block\_generic\_serialize(\&instance-\>generic, flipper\_format, preset);  
}

SubGhzProtocolStatus  
    subghz\_protocol\_decoder\_scher\_khan\_deserialize(void\* context, FlipperFormat\* flipper\_format) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderScherKhan\* instance \= context;  
    return subghz\_block\_generic\_deserialize(\&instance-\>generic, flipper\_format);  
}

void subghz\_protocol\_decoder\_scher\_khan\_get\_string(void\* context, FuriString\* output) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderScherKhan\* instance \= context;

    subghz\_protocol\_scher\_khan\_check\_remote\_controller(  
        \&instance-\>generic, \&instance-\>protocol\_name);

    furi\_string\_cat\_printf(  
        output,  
        "%s %dbit\\r\\n"  
        "Key:0x%lX%08lX\\r\\n"  
        "Sn:%07lX Btn:%X Cnt:%04lX\\r\\n"  
        "Pt: %s\\r\\n",  
        instance-\>generic.protocol\_name,  
        instance-\>generic.data\_count\_bit,  
        (uint32\_t)(instance-\>generic.data \>\> 32),  
        (uint32\_t)instance-\>generic.data,  
        instance-\>generic.serial,  
        instance-\>generic.btn,  
        instance-\>generic.cnt,  
        instance-\>protocol\_name);  
}

/\*\*  
 \* Analysis of received data  
 \* @param instance Pointer to a SubGhzBlockGeneric\* instance  
 \* @param protocol\_name  
 \*/  
static void subghz\_protocol\_scher\_khan\_check\_remote\_controller(  
    SubGhzBlockGeneric\* instance,  
    const char\*\* protocol\_name) {  
    /\*  
    \* MAGICAR 51 bit 00000001A99121DE83C3 MAGIC CODE, Dynamic  
    \* 0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100  
    \* 0E8C1629D830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101  
    \* 0E8C1649B830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110  
    \* 0E8C16897830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111  
    \*                             Serial         Key  Ser \~Key   CNT  
    \*/

    switch(instance-\>data\_count\_bit) {  
    case 35: //MAGIC CODE, Static  
        \*protocol\_name \= "MAGIC CODE, Static";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 51: //MAGIC CODE, Dynamic  
        \*protocol\_name \= "MAGIC CODE, Dynamic";  
        instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) & 0x0F);  
        instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;  
        instance-\>cnt \= instance-\>data & 0xFFFF;  
        break;  
    case 57: //MAGIC CODE PRO / PRO2  
        \*protocol\_name \= "MAGIC CODE PRO/PRO2";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 63: //MAGIC CODE, Dynamic Response  
        \*protocol\_name \= "MAGIC CODE, Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 64: //MAGICAR, Response ???  
        \*protocol\_name \= "MAGICAR, Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 81: //MAGIC CODE PRO / PRO2 Response ???  
        \*protocol\_name \= "MAGIC CODE PRO,\\n Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    case 82: //MAGIC CODE PRO / PRO2 Response ???  
        \*protocol\_name \= "MAGIC CODE PRO,\\n Response";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;

    default:  
        \*protocol\_name \= "Unknown";  
        instance-\>serial \= 0;  
        instance-\>btn \= 0;  
        instance-\>cnt \= 0;  
        break;  
    }  
}

uint32\_t subghz\_protocol\_decoder\_scher\_khan\_get\_hash\_data(void\* context) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderScherKhan\* instance \= context;  
    return subghz\_protocol\_blocks\_get\_hash\_data\_long(  
        \&instance-\>decoder, (instance-\>decoder.decode\_count\_bit / 8\) \+ 1);  
void subghz\_protocol\_decoder\_scher\_khan\_get\_string(void\* context, FuriString\* output) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderScherKhan\* instance \= context;

    subghz\_protocol\_scher\_khan\_check\_remote\_controller(  
        \&instance-\>generic, \&instance-\>protocol\_name);

    furi\_string\_cat\_printf(  
        output,  
        "%s %dbit\\r\\n"  
        "Key:0x%lX%08lX\\r\\n"  
        "Sn:%07lX Btn:%X Cnt:%04lX\\r\\n"  
        "Pt: %s\\r\\n",  
        instance-\>generic.protocol\_name,  
        instance-\>generic.data\_count\_bit,  
        (uint32\_t)(instance-\>generic.data \>\> 32),  
        (uint32\_t)instance-\>generic.data,  
        instance-\>generic.serial,  
        instance-\>generic.btn,  
        instance-\>generic.cnt,  
        instance-\>protocol\_name);  
}

/\*\*  
 \* Analysis of received data  
 \* @param instance Pointer to a SubGhzBlockGeneric\* instance  
 \* @param keystore Pointer to a SubGhzKeystore\* instance  
 \* @param manufacture\_name  
 \*/  
static void subghz\_protocol\_sher\_khan\_check\_remote\_controller(  
    SubGhzBlockGeneric\* instance,  
    SubGhzKeystore\* keystore,  
    const char\*\* manufacture\_name) {  
    uint64\_t key \= subghz\_protocol\_blocks\_reverse\_key(instance-\>data, instance-\>data\_count\_bit);  
    uint32\_t key\_fix \= key \>\> 32;  
    uint32\_t key\_hop \= key & 0x00000000ffffffff;

    subghz\_protocol\_sher\_khan\_check\_remote\_controller\_selector(  
        instance, key\_fix, key\_hop, keystore, manufacture\_name);

    instance-\>serial \= key\_fix & 0x00FFFFFF;  
    instance-\>btn \= key\_fix \>\> 24;  
}

uint32\_t subghz\_protocol\_decoder\_sher\_khan\_get\_hash\_data(void\* context) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderSherKhan\* instance \= context;  
    return subghz\_protocol\_blocks\_get\_hash\_data\_long(  
        \&instance-\>decoder, (instance-\>decoder.decode\_count\_bit / 8\) \+ 1);  
void subghz\_protocol\_decoder\_sher\_khan\_get\_string(void\* context, FuriString\* output) {  
    furi\_assert(context);  
    SubGhzProtocolDecoderSherKhan\* instance \= context;

    subghz\_protocol\_sher\_khan\_check\_remote\_controller(  
        \&instance-\>generic, instance-\>keystore, \&instance-\>manufacture\_name);

    uint32\_t code\_found\_hi \= instance-\>generic.data \>\> 32;  
    uint32\_t code\_found\_lo \= instance-\>generic.data & 0x00000000ffffffff;

    uint64\_t code\_found\_reverse \= subghz\_protocol\_blocks\_reverse\_key(  
        instance-\>generic.data, instance-\>generic.data\_count\_bit);  
    uint32\_t code\_found\_reverse\_hi \= code\_found\_reverse \>\> 32;  
    uint32\_t code\_found\_reverse\_lo \= code\_found\_reverse & 0x00000000ffffffff;  
    furi\_string\_cat\_printf(  
        output,  
        "%s %dbit\\r\\n"  
        "Key:%08lX%08lX\\r\\n"  
        "Fix:0x%08lX    Cnt:%04lX\\r\\n"  
        "Hop:0x%08lX    Btn:%02X\\r\\n"  
        "MF:%s\\r\\n",  
        instance-\>generic.protocol\_name,  
        instance-\>generic.data\_count\_bit,  
        (uint32\_t)(instance-\>generic.data \>\> 32),  
        (uint32\_t)instance-\>generic.data,  
        code\_found\_hi,  
        code\_found\_lo,  
        code\_found\_reverse\_hi,  
        instance-\>generic.cnt,  
        code\_found\_reverse\_lo,  
        instance-\>generic.btn,  
        instance-\>manufacture\_name);  
}

KEY:0xE8C1619E801B  
SN: 0E8C169 Btn: 1 Ctn: 801B  
(but in my new code its showing   
RAW data:1B809E61C1E8  
key:0x0000E8C1619E801B Sn: 006C000 Btn: 00 Cnt: 801B )

key:0xE8C1619E801C  
Sn: 0E8C169 Btn: 1 Cnt:801C  
(but in my code it shows   
raw data:1C809E61C1E8 key:0x0000E8C1619E801C Sn:001C000 Btn:00 Ctn:801c Reversed key:0x0001C00BCC3418B8(x)

    MAGICAR 51 bit 00000001A99121DE83C3 MAGIC CODE, Dynamic  
     0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100  
     0E8C1629D830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101  
     0E8C1649B830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110  
     0E8C16897830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111  
                                                                    Serial         Key  Ser \~Key   CNT

0E8C1619E801B  
0E8C1619E801C

0E8C1619E830C  
0E8C1629D830D  
0E8C1649B830E  
0E8C16897830F

00 00 E8 C1 61 9E 80 1B  
00 00 E8 C1 61 9E 80 1C

Key: 00 01 35 06 E9 AF F4 0B  
Key: 00 00 E8 C1 61 9E 80 1B  
Key: 00 00 E8 C1 61 9E 80 1C  
Key: 00 00 E8 C1 61 9E 80 1B  
Key: 00 00 E8 C1 61 9E 80 1C

Key: 00 01 35 06 E9 AF F4 0B

\_\_\_\_\*\*\*\*\_\_\_\_\*\_\*\_\*\_\*\*\*\*NEW\!\!  NEW UPDATED UPDATED PROPER INFORMATION THAT HASN’T BEEN MODIFED FROM ITS ORIGINAL FORM \_\_\*\*\_\*\_\*\*

does this context/information align with the next  number changing to the next one after etc?  
ChatGPT  
ChatGPT  
Based on the provided context and pattern from the given examples, it seems like the counter (cnt) is incrementing in each step, and you're asking for the next value in the sequence given this pattern. Here’s how you can approach it:

* Observing the Pattern:  
  * In the examples given:  
    * 0xE8C1619E801B → Counter: 801B  
    * 0xE8C1619E801C → Counter: 801C  
  * The counter appears to be incrementing by 1\.  
* Predicting the Next Value:  
  * Following this pattern, the next counter value after 801C would be 801D.  
  * Therefore, if the rest of the data (serial, button) remains constant, and only the counter is incrementing, the next key in the sequence would be 0xE8C1619E801D.  
* Regarding the 5 Bits and 51-bit Encoding:  
  * Your explanation about the 5 additional bits added to the 51-bit encoding to make it more convenient for hexadecimal representation aligns with having 56 bits in total, which fits into 7 full bytes.  
  * This padding for alignment does not affect the actual data content but is something to keep in mind when decoding or encoding these values.  
* Putting it All Together:  
  * If we consider the pattern and your explanation about the encoding, the next hexadecimal sequence following 0xE8C1619E801C would likely be 0xE8C1619E801D given the incremental pattern in the counter (cnt).  
  * However, it's essential to verify that this incremental pattern holds consistently in the actual use case or if there are other factors (like button presses or specific serial number changes) that might affect this sequence.  
* Dynamic Nature of the Code:  
  * The mention of MAGIC CODE, Dynamic and the way the serial and button numbers are extracted suggests that the encoding scheme is dynamic and might have conditions under which different parts of the code change. This dynamic nature should be carefully considered in the actual application to ensure the pattern holds true.

So, in conclusion, assuming the pattern of incrementing the counter by one continues, and the other parts of the data remain constant, the next expected hex value should be 0xE8C1619E801D. However, it’s crucial to verify this against the actual logic and usage of the protocol you’re working with.  
![User][image2]  
You  
so if the btn press changes the center to the next number pattern...

does my exmple 1\. and 2\.

also align with the example changes for the next vaule also with regards to these examples  
\*\*\*wrong\*\*\*  
51 bit 00000001A99121DE83C3 MAGIC CODE, Dynamic  
     0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100  
     0E8C1629D830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101  
     0E8C1649B830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110  
     0E8C16897830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111  
                                                                    Serial         Key  Ser \~Key   CNT  
//MAGIC CODE, Dynamic  
        \*protocol\_name \= "MAGIC CODE, Dynamic";  
        instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) & 0x0F);  
        instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;  
        instance-\>cnt \= instance-\>data & 0xFFFF;

ChatGPT  
ChatGPT  
Based on your examples and the provided context, it seems like both the button press (represented by btn) and the counter (cnt) are significant in determining the next value in the sequence. Let's analyze how they might change together:

### Given Examples

* Example 1 and 2:  
  * 0xE8C1619E801B → SN: 0E8C169, Btn: 1, Cnt: 801B  
  * 0xE8C1619E801C → SN: 0E8C169, Btn: 1, Cnt: 801C

### Pattern Analysis

* From your additional examples, it appears that the serial part (0E8C16) remains constant, and the subsequent parts (19E8, 29D8, 49B8, 8978\) represent changes related to button presses and potentially other factors.  
* The last part (30C, 30D, 30E, 30F) seems to represent the counter (cnt) incrementing, as observed in the initial examples.

### Deduction

* If the btn press alters the pattern significantly, as seen in the 19E8, 29D8, 49B8, 8978 parts of the examples, we should expect a similar significant change when moving from one btn press to another.  
* For the counter part (cnt), a simple incrementation pattern is observed, which likely continues unless affected by a btn press or other dynamic factors.

### Next Value Hypothesis

* Given the observed patterns, if we were to predict the next value after 0xE8C1619E801C considering a button press might change the pattern, we would need to understand how the btn press affects the middle part of the sequence.  
* For instance, if after a certain btn press the middle part changes from 19E8 to 29D8 (as in your examples), then we need to identify the rule or pattern governing this change.

### Conclusion

* If the btn press leads to a new pattern, and assuming cnt continues to increment, we need more data to accurately predict the next sequence. The cnt will likely increment to 801D, but the middle part affected by btn will depend on the specific logic or pattern of the Scher-Khan protocol.  
* Therefore, aligning with the examples you provided, if the button press advances to the next significant change, we might see something like this (hypothetically):  
  * Next btn press: Changes the middle part  
  * Next cnt: Increment to the next value (801D)  
* However, to pinpoint the exact next value, we would need the specific logic or algorithm that the Scher-Khan protocol uses to determine how button presses affect the sequence.

Given the complexity and specificity of the protocol's encoding, having the actual logic or algorithm that defines how the serial number, button presses, and counter interact would be essential for making an accurate prediction of the next sequence.  
![User][image2]  
You  
so if the last bit go es up by one and after all are used could we assume the bit afte would go up 1 vaule like counting up but in hex?  if we were to use that logic  is there a hex calculation for determining the next number for the key sn btn and ctn?  
ChatGPT  
ChatGPT  
Yes, if we assume that the hexadecimal values are incrementing in a manner similar to a standard counting system, then once all possible values for a particular digit are exhausted, the next higher digit would increment by one. This is analogous to how in decimal counting, after 9 comes 10, where the next higher digit is incremented. In hexadecimal, which is base-16, the counting goes from 0 to 9, then A to F, before rolling over to the next higher digit.  
Given this logic, let's consider the incrementation for the key, serial number (SN), button (Btn), and counter (Ctn):

* Counter (Ctn):  
  * If Ctn is 801C and increments by one, it becomes 801D.  
  * After 801F, the next value would be 8020, incrementing the next higher digit.  
* Button (Btn):  
  * If we assume Btn changes less frequently and is part of a larger pattern (e.g., changes after a full cycle of Ctn), it would increment after all possible Ctn values have been cycled through.  
  * Without specific logic, it's hard to determine exactly when Btn would increment, but if it follows a simple increment, after F it would reset or move to the next significant change in the pattern.  
* Serial Number (SN):  
  * Typically, a serial number remains constant for a set of operations and might not increment in the same way as Ctn or Btn. However, if it does increment, it would likely follow the same hexadecimal counting logic.  
* Key:  
  * The key is likely a combination of the SN, Btn, and Ctn, and possibly other factors. If each of these components increments as described, the key would change accordingly.

### Hexadecimal Calculation for the Next Number

To calculate the next number in a sequence where each part increments after exhausting its range:

* Start with the least significant part (Ctn):  
  * Increment it until it rolls over, then increment the next significant part (Btn).  
* Apply the same logic to Btn and SN:  
  * If Btn reaches its maximum value and rolls over, check if SN should increment.

### Example:

* If SN \= 0E8C169, Btn \= 1, Ctn \= 801C:  
  * Increment Ctn: 801D, 801E, ..., 801F, then 8020 (if counting continues).  
  * If Ctn completes a full cycle (8000 to 8FFF, hypothetically), then increment Btn (e.g., from 1 to 2).  
  * SN typically remains constant unless it is part of a larger cycle or system where it also increments.

This hypothetical mechanism assumes a simple linear increment, which may not accurately reflect complex protocols like Scher-Khan, where encryption, encoding, or protocol-specific rules could affect the sequence. Understanding the exact protocol specification or algorithm is crucial for accurately predicting the sequence of numbers.  
![User][image2]  
You  
if btn just represents what btn your pressing on the remote... so btn doesnt change it just represents one of the 4 or 5 btns whichever it is but we wont worry about the other btn vaules yet....  what would be the overall full hex value for the next predicted value? if we were to use the logic we have just presented    hex value representation that could be used on my example and the other 4 examples etc    
ChatGPT  
ChatGPT  
If the button (Btn) simply represents one of the buttons on the remote and does not change in this context, and we're focusing on the incrementation of the counter (Ctn) as the primary changing element, then the prediction would be based on the counter incrementing within the given serial number (SN) range.  
Given your examples:

* KEY:0xE8C1619E801B → SN: 0E8C169, Btn: 1, Cnt: 801B  
* KEY:0xE8C1619E801C → SN: 0E8C169, Btn: 1, Cnt: 801C

Following this pattern, assuming the Cnt increments by 1, and the Btn stays constant (as it represents a specific button being pressed):

* If Cnt for the last known value is 801C, incrementing it by 1 would give 801D.

Therefore, the next full hexadecimal value, assuming no other changes (like button presses or serial number changes), would be:

* KEY:0xE8C1619E801D

Here’s how it aligns:

* The SN part 0E8C16 remains constant.  
* The Btn is not factored into this hexadecimal string directly; it's more about the context or functionality it represents (e.g., which button is pressed).  
* The Cnt increments within the context of the SN and the specific button action represented by Btn.

For a complete pattern including multiple button values, you would sequentially increment the Cnt for each button and then possibly cycle through the button values, but since we are keeping the button constant here, only the counter part changes.  
This approach provides a linear and predictable incrementation pattern. However, in real-world implementations, especially in security and remote control protocols like Scher-Khan, additional factors like encryption, rolling codes, or non-linear incrementation might come into play, complicating the prediction.  
![User][image2]  
You  
and what hex total value calculation that could we use to apply to either my example or the other 4 values example that would represent what the next value would/could be?  
ChatGPT  
ChatGPT  
To calculate the next value in a sequence like the ones provided, especially in a hexadecimal context, you typically increment the last portion of the sequence (the counter in this case) and keep the rest of the structure (like the serial number and button identifier) constant, unless they also follow a predictable incrementation pattern.  
Given the pattern in your examples:

* KEY:0xE8C1619E801B → SN: 0E8C169, Btn: 1, Cnt: 801B  
* KEY:0xE8C1619E801C → SN: 0E8C169, Btn: 1, Cnt: 801C

If we increment the Cnt part by one for the next sequence, keeping SN and Btn constant, we get:

* Next Cnt after 801C would be 801D

Therefore, the next full hexadecimal value should be:

* KEY:0xE8C1619E801D

To apply this logic more broadly, here’s how you could approach it:

* Extract the last part of the sequence (the counter, Cnt) from the hexadecimal value.  
* Increment this part by one (or by the appropriate increment step based on the pattern or system logic).  
* Recombine the incremented part with the original prefix (the constant part of the sequence, in this case, 0xE8C1619E from the SN and possibly Btn).

For the examples you provided (0E8C1619E830C, 0E8C1629D830D, etc.), if we assume a simple incrementation:

* Take the last part of the sequence (30C, 30D, etc.).  
* Increment this part (considering hexadecimal notation).  
* Append this incremented part back to the initial portion of the sequence.

For a generic hexadecimal increment function in programming:  
plaintext  
Copy code  
1\. Convert the hexadecimal counter part to an integer.  
2\. Add 1 to the integer.  
3\. Convert the result back to hexadecimal.  
4\. Concatenate the result with the static part of the key.

Applying this to a hypothetical function:

* Assuming we are dealing with the counter 30C and it needs to increment:  
  * Convert 30C to an integer (780 in decimal).  
  * Add 1, getting 781\.  
  * Convert 781 back to hexadecimal, yielding 30D.  
  * Concatenate this with the static part of your key, e.g., 0E8C1619E, resulting in 0E8C1619E30D.

This method gives a straightforward way to predict the next value in a sequence if the pattern follows a simple incrementation logic. However, in systems like remote controls or security protocols, where rolling codes or encryption may be used, the actual logic could be more complex and not strictly linear.  
![User][image2]  
You  
hypothetical function that you presented..  
to represent these 3 diff sets of numbers for the calculation to align with it all... what formula or python or c++ code could you apply to guess the next one?

0E8C1619E801B  
0E8C1619E801C

00 00 E8 C1 61 9E 80 1B  
00 00 E8 C1 61 9E 80 1C

0E8C1619E830C  
0E8C1629D830D  
0E8C1649B830E  
0E8C16897830F

51 bit 00000001A99121DE83C3 MAGIC CODE, Dynamic  
     0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100  
     0E8C1629D830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101  
     0E8C1649B830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110  
     0E8C16897830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111  
                                                                    Serial         Key  Ser \~Key   CNT

(context to what part of the overall value represents which part to just handle changing the next part for predicting the next possible value)

KEY:0xE8C1619E801B  
SN: 0E8C169 Btn: 1 Ctn: 801B

key:0xE8C1619E801C  
Sn: 0E8C169 Btn: 1 Cnt:801C

0E8C1619E801B  
0E8C1619E801C

0E8C1619E830C  
0E8C1629D830D  
0E8C1649B830E  
0E8C16897830F

0E8C16X9X830C  
0E8C16X9X830D  
0E8C16X9X830E  
0E8C16X9X830F

0E8C1619E801B  
0E8C1619E801C

0E8C1619E830C  
0E8C1619E830D  
0E8C1619E830E  
0E8C1619E830F

00 00 E8 C1 61 9E 80 1B  
00 00 E8 C1 61 9E 80 1C

NEW UPDATED PROPER PATTERN VALUES  
0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100     Serial         Key   Ser \~Key   CNT  
0E8C1619E830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101     Serial         Key   Ser \~Key   CNT  
0E8C1619E830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110     Serial         Key   Ser \~Key   CNT  
0E8C1619E830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111     Serial         Key   Ser \~Key   CNT

00000001A99121DE83C3  
     OLD  
     0E8C1619E830C  
     0E8C1629D830D  
     0E8C1649B830E  
     0E8C16897830F  
NEW  
0E8C1619E830C  
0E8C1619E830D  
0E8C1619E830E  
0E8C1619E830F

\*\*\*\*\*incorrect values\*\*\*\*\*\*\*  
    MAGICAR 51 bit 00000001A99121DE83C3 MAGIC CODE, Dynamic  
     0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100  
     0E8C1629D830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101  
     0E8C1649B830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110  
     0E8C16897830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111  
                                                                    Serial         Key  Ser \~Key   CNT

NEW UPDATED PROPER PATTERN VALUES  
0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100     Serial         Key   Ser \~Key   CNT  
0E8C1619E830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101     Serial         Key   Ser \~Key   CNT  
0E8C1619E830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110     Serial         Key   Ser \~Key   CNT  
0E8C1619E830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111     Serial         Key   Ser \~Key   CNT

first step is to check the checksum of the received packet. We calculate the last byte based on the first 6\.

static Byte CheckSumm(Byte x, Byte reg35a)  
{  
int i;  
Byte reg35, reg36, reg38;  
reg36 \= x;  
reg35 \= reg35a;  
i \= 8;  
do  
{  
reg38 \= (Byte)(reg35 ^ reg36);  
reg35 \= (Byte)(reg35 \<\< 1);  
if ((reg38 & 0x80) \== 0x80) { reg35 \= (Byte)(reg35 ^ 0x2A); }  
reg36 \= (Byte)(reg36 \<\< 1);  
i \= i \- 1;  
} while (i \> 0);  
return reg35;  
}

And this is how to apply this procedure. As a result, the last byte of the received message and the calculated checksum must match. If it coincides, it means that the Sherkhan worked.

csumm \= 0xFF;  
for (i \= 0; i \< 6; i++)  
{  
csumm \= CheckSumm(reg\_encr\[i\], csumm);  
}

some context/info from a diff a.i.

"To calculate the next value in a sequence like the ones provided, especially in a hexadecimal context, you typically increment the last portion of the sequence (the counter in this case) and keep the rest of the structure (like the serial number and button identifier) constant, unless they also follow a predictable incrementation pattern.  
Given the pattern in your examples:  
KEY:0xE8C1619E801B → SN: 0E8C169, Btn: 1, Cnt: 801B  
KEY:0xE8C1619E801C → SN: 0E8C169, Btn: 1, Cnt: 801C  
If we increment the Cnt part by one for the next sequence, keeping SN and Btn constant, we get:  
Next Cnt after 801C would be 801D  
Therefore, the next full hexadecimal value should be:  
KEY:0xE8C1619E801D  
To apply this logic more broadly, here’s how you could approach it:  
Extract the last part of the sequence (the counter, Cnt) from the hexadecimal value.  
Increment this part by one (or by the appropriate increment step based on the pattern or system logic).  
Recombine the incremented part with the original prefix (the constant part of the sequence, in this case, 0xE8C1619E from the SN and possibly Btn).  
For the examples you provided (0E8C1619E830C, 0E8C1629D830D, etc.), if we assume a simple incrementation:  
Take the last part of the sequence (30C, 30D, etc.).  
Increment this part (considering hexadecimal notation).  
Append this incremented part back to the initial portion of the sequence.  
For a generic hexadecimal increment function in programming:  
plaintext

Copy code  
1\. Convert the hexadecimal counter part to an integer. 2\. Add 1 to the integer. 3\. Convert the result back to hexadecimal. 4\. Concatenate the result with the static part of the key.   
Applying this to a hypothetical function:  
Assuming we are dealing with the counter 30C and it needs to increment:Convert 30C to an integer (780 in decimal).  
Add 1, getting 781\.  
Convert 781 back to hexadecimal, yielding 30D.

## Concatenate this with the static part of your key, e.g., 0E8C1619E, resulting in 0E8C1619E30D."

## (these values arent nessecarily related to each other or the same "remote" just diff real life examples from say different "remotes" just examples of the next real life real value  so using those values we were trying to come up with a formula or hex value to add to any current hex total value that coule be used to then "predict" what the next one could be and most likely might be))

for using to predict the next value that isnt listed

## but using 3 diff sets of numbers for real life examples of what the next alue is   calculate   get a formula that we can apply to the 3 diff sets of numbers that align/corelate to the present numbers  tha would work in any context or value that we had...  bsed on the 3 diff sets of value patterns here...

Excellent\! This breakdown of the different parts of the values is incredibly useful. Here's how we can use it to refine our prediction strategy:  
Understanding the Structure

* SN (Serial Number): 0E8C169 \- likely remains constant for a set of operations.  
* Btn (Button): Represents the button pressed. In our earlier discussions, we assumed this stays constant for a particular prediction.  
* Cnt (Counter): The part that primarily changes (e.g., 801B, 801C) and is the focus of our incremental prediction.

Refining the Approach

1. Counter-Focused Prediction: We can absolutely leverage the Python code from earlier examples to predict the next counter value:

2. Python

def predict\_next\_value(current\_hex\_value):  
    \# ... (same code as before to increment the counter)

3.   
4.   
5. Incorporating Button Changes (Future): When you're ready to handle scenarios where the button influences the pattern, we'll need:

   * More Data: Examples of how the value changes with different button presses.  
   * Pattern Analysis: Discern if there's a consistent rule governing how the button press modifies the value.

Example Usage  
Python  
current\_value \= "0E8C1619E801C"  \# Example  
next\_value \= predict\_next\_value(current\_value)  
print(f"Predicted next value: {next\_value}")  \# Output: Predicted next value: 0E8C1619E801D 

Important Considerations

* Dynamic Nature: The mention of "MAGIC CODE, Dynamic" suggests complexity. Values might change unpredictably based on encryption or other factors.  
* Context is Key: Understanding the specific encoding protocols used by the system generating these values will be essential for reliable prediction in complex scenarios.

Next Steps

1. Test: Use the provided code to see if it correctly predicts the next counter-based values in the sets you have.

2. Button Change Exploration: Let's work on gathering examples of how the button influences the value once you're ready to tackle that part.

Let me know how the counter prediction works on your examples\!

def predict\_next\_value(current\_hex\_value):  
    """Predicts the next hexadecimal value assuming counter incrementation.

    Args:  
        current\_hex\_value: The current hexadecimal value as a string. Assumes the  
                           structure KEY:SN:Btn:Cnt (e.g., "0xE8C1619E801C")

    Returns:  
        The predicted next hexadecimal value as a string.  
    """

    prefix \= current\_hex\_value\[:-4\]  \# Everything except the last 4 characters  
    counter\_hex \= current\_hex\_value\[-4:\]   
    counter\_int \= int(counter\_hex, 16\)   
    next\_counter\_int \= counter\_int \+ 1   
    next\_counter\_hex \= hex(next\_counter\_int)\[2:\].upper().zfill(4)    
    return prefix \+ next\_counter\_hex

\# Example Usage:  
values \= \[  
    "0E8C1619E801B",  \# Representative prefix  
    "0E8C1619E801C",    
    "0E8C1619E830C",    
    "0E8C1619E830D",   
    "0E8C1619E830E",   
    "0E8C1619E830F"     
\]

for value in values:  
    next\_value \= predict\_next\_value(value)  
    print(f"Next value after {value}: {next\_value}")

\#include \<iostream\>  
\#include \<string\>  
\#include \<iomanip\>  
\#include \<sstream\>

std::string predict\_next\_value(const std::string& current\_hex\_value) {  
    // Separate prefix and counter  
    std::string prefix \= current\_hex\_value.substr(0, current\_hex\_value.size() \- 4);  
    std::string counter\_hex \= current\_hex\_value.substr(current\_hex\_value.size() \- 4);

    // Convert counter from hex to integer  
    int counter\_int \= std::stoi(counter\_hex, nullptr, 16);

    // Increment the counter  
    counter\_int++;

    // Convert counter back to hex string  
    std::stringstream ss;  
    ss \<\< std::hex \<\< std::uppercase \<\< std::setw(4) \<\< std::setfill('0') \<\< counter\_int;  
    std::string next\_counter\_hex \= ss.str();

    // Construct the new value  
    return prefix \+ next\_counter\_hex;  
}

int main() {  
    std::vector\<std::string\> values \= {  
    "0E8C1619E801B",  \# Representative prefix  
    "0E8C1619E801C",    
    "0E8C1619E830C",     "0E8C1619E830D",   
    "0E8C1619E830E",   
    "0E8C1619E830F"     
    };

    for (const auto& value : values) {  
        std::string next\_value \= predict\_next\_value(value);  
        std::cout \<\< "Next value after " \<\< value \<\< ": " \<\< next\_value \<\< std::endl;  
    }

    return 0;  
}

NEW UPDATED PROPER PATTERN VALUES  
0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100     Serial           
0E8C1619E830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101     Serial           
0E8C1619E830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110     Serial           
0E8C1619E830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111     Serial                          Hex(decoded)       \-\>Key                                                    Ser \~Key   CNT

0E8C1619E830C \-\> 000011101000110000010110 0001 1001 1110 1000001100001100     Serial           
0E8C1619E830D \-\> 000011101000110000010110 0010 1001 1101 1000001100001101     Serial           
0E8C1619E830E \-\> 000011101000110000010110 0100 1001 1011 1000001100001110     Serial           
0E8C1619E830F \-\> 000011101000110000010110 1000 1001 0111 1000001100001111     Serial                          \-\>Key                   \-\> Serial-Key                                              Cnt         Btn

\*\*\*still applies\*\*  
case 51: //MAGIC CODE, Dynamic  
        \*protocol\_name \= "MAGIC CODE, Dynamic";  
        instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) 0x0F);  
        instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;  
        instance-\>cnt \= instance-\>data & 0xFFFF;  
        break;

NEW\!

// Serial Number Extraction    
instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) & 0x0F);

// Button Extraction  
instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;

// Counter Extraction  
instance-\>cnt \= instance-\>data & 0xFFFF; 

\*\*\*\*\*\*\*\*\*\*\*ENCODER\*\*\*\*\*\*\*\*\*\*\*\*\*  
bool subghz\_protocol\_scher\_khan\_gen\_data(SubGhzProtocolEncoderScherKhan\* instance, uint8\_t btn) {  
    // 1\. Assemble Message  
    uint64\_t assembled\_data \= 0;  
    assembled\_data |= (uint64\_t)instance-\>generic.serial \<\< 28; // 28 bits for Serial  
    assembled\_data |= (uint64\_t)btn \<\< 24; // 4 bits for Button  
    assembled\_data |= (uint64\_t)instance-\>generic.cnt; // 16 bits for Counter 

    // 2\. Calculate Checksum   
    uint8\_t checksum \= CheckSumm(assembled\_data); // Replace with your exact function call  
    assembled\_data |= (uint64\_t)checksum \<\< 8; // Adjust checksum placement if needed

    // 3\. Finalize  
    instance-\>generic.data \= assembled\_data;  
    return true;   
}

**Possible new????**  
**instance-\>serial \= ((instance-\>data \>\> 16\) & 0xFF); // Incorrect shift**  
**instance-\>btn \= (instance-\>data \>\> 20\) & 0x01);  //  Incorrect shift**   
**instance-\>cnt \= instance-\>data & 0xFFFF; // Needs bit reversal**

**def predict\_next\_value(current\_hex\_value):**  
    **prefix \= current\_hex\_value\[:-4\]  \# Everything except the last 4 characters**  
    **counter\_hex \= current\_hex\_value\[-4:\]**   
    **counter\_int \= int(counter\_hex, 16\)**   
    **next\_counter\_int \= counter\_int \+ 1**   
    **next\_counter\_hex \= hex(next\_counter\_int)\[2:\].upper().zfill(4)**    
    **return prefix \+ next\_counter\_hex**

if this is true  
instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) 0x0F);  
instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;  
instance-\>cnt \= instance-\>data & 0xFFFF;  
break;

does the following code make that also true?

uint8\_t key\_data\[sizeof(uint64\_t)\] \= {0};  
for(size\_t i \= 0; i \< sizeof(uint64\_t); i++) {  
key\_data\[sizeof(uint64\_t) \- i \- 1\] \= (instance-\>generic.data \>\> i \* 8\) & 0xFF;

**No, the given code will not make the following true:**  
**instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) & 0x0F);**  
**instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;**  
**instance-\>cnt \= instance-\>data & 0xFFFF;**

**The given code is trying to extract the serial number, button number, and count from the instance-\>data variable, which is a uint64\_t data type. However, the code is not using the correct bit shifting operations to extract the required information from the data.**  
**To extract the serial number, button number, and count from the instance-\>data variable, you would need to use the following code:**

**uint8\_t key\_data\[sizeof(uint64\_t)\] \= {0};**  
**for (size\_t i \= 0; i \< sizeof(uint64\_t); i++) {**  
    **key\_data\[sizeof(uint64\_t) \- i \- 1\] \= (instance-\>data \>\> (i \* 8)) & 0xFF;**  
**}**

**instance-\>serial \= key\_data\[0\] \<\< 24 | key\_data\[1\];**  
**instance-\>btn \= key\_data\[2\];**  
**instance-\>cnt \= key\_data\[3\] \<\< 8 | key\_data\[4\];**  
**This code first initializes an array of uint8\_t values called key\_data with the same size as a uint64\_t data type. It then loops through each byte of the instance-\>data variable and extracts the corresponding byte from the key\_data array. Finally, it uses bit shifting operations to extract the serial number, button number, and count from the key\_data array.**

Other scher\_khan references in other parts of code that have been added….

  case SetTypeScherKhan\_433\_92:  
            gen\_info \= (GenInfo){  
                .type \= GenScherKhan,  
                .mod \= "AM650",  
                .freq \= 433920000,  
                .scher\_khan.serial \= key & 0xFFFFFF0,  
                .scher\_khan.btn \= 0x0F,  
                .scher\_khan.cnt \= 0xFFFF,  
                .scher\_khan \= false};  
            break;

/\*\*  
 \* New remote generation.  
 \* @param context Pointer to a SubGhzProtocolEncoderScherKhan instance  
 \* @param flipper\_format Pointer to a FlipperFormat instance  
 **\* @param serial Serial number 24 bit**  
 **\* @param btn Button number, 8 bit**  
 **\* @param cnt Counter value, 16 bit**  
 **\* @param preset Modulation, SubGhzRadioPreset**  
 **\* @return true On success**  
 **\*/**  
**bool subghz\_protocol\_scher\_khan\_create\_data(**  
    **void\* context,**  
    FlipperFormat\* flipper\_format,  
    uint32\_t serial,  
    uint8\_t btn,  
    uint16\_t cnt,  
    SubGhzRadioPreset\* preset);  
case GenScherKhan:  
            generated\_protocol \= subghz\_txrx\_gen\_scher\_khan\_protocol(  
                subghz-\>txrx,  
                gen\_info.mod,  
                gen\_info.freq,  
                gen\_info.scher\_khan.serial,  
                gen\_info.scher\_khan.btn,  
                gen\_info.scher\_khan.cnt,  
                gen\_info.scher\_khan);  
            break;

bool subghz\_txrx\_gen\_scher\_khan\_protocol(  
    void\* context,  
    const char\* preset\_name,  
    uint32\_t frequency,  
    uint32\_t serial,  
    uint8\_t btn,  
    uint16\_t cnt,  
    bool nice\_one) {  
    SubGhzTxRx\* txrx \= context;

    bool res \= false;

    txrx-\>transmitter \=  
        subghz\_transmitter\_alloc\_init(txrx-\>environment, SUBGHZ\_PROTOCOL\_SCHER\_KHAN\_NAME);  
    subghz\_txrx\_set\_preset(txrx, preset\_name, frequency, 0, 0, NULL, 0);

    if(txrx-\>transmitter && subghz\_protocol\_scher\_khan\_create\_data(  
                                subghz\_transmitter\_get\_protocol\_instance(txrx-\>transmitter),  
                                txrx-\>fff\_data,  
                                serial,  
                                btn,  
                                cnt,  
                                txrx-\>preset,  
                                nice\_one)) {  
        res \= true;  
    }

    subghz\_transmitter\_free(txrx-\>transmitter);

    return res;  
}

instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) 0x0F);  
instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;  
instance-\>cnt \= instance-\>data & 0xFFFF;

uint8\_t key\_data\[sizeof(uint64\_t)\] \= {0};  
	for (size\_t i \= 0; i \< sizeof(uint64\_t); i++) {  
   	    key\_data\[sizeof(uint64\_t) \- i \- 1\] \= (instance-\>data \>\> (i \* 8)) & 0xFF;  
}

bool subghz\_txrx\_gen\_scher\_khan\_protocol(  
    void\* context,  
    const char\* preset\_name,  
    uint32\_t frequency,  
    uint32\_t serial,  
    uint8\_t btn,  
    uint16\_t cnt);

bool subghz\_txrx\_gen\_scher\_khan\_protocol(  
    void\* context,  
    const char\* preset\_name,  
    uint32\_t frequency,  
    uint32\_t serial,  
    uint8\_t btn,  
    uint16\_t cnt) {  
    SubGhzTxRx\* txrx \= context;

    bool res \= false;

    txrx-\>transmitter \=  
        subghz\_transmitter\_alloc\_init(txrx-\>environment, SUBGHZ\_PROTOCOL\_SCHER\_KHAN\_NAME);  
    subghz\_txrx\_set\_preset(txrx, preset\_name, frequency, 0, 0, NULL, 0);

    if(txrx-\>transmitter && subghz\_protocol\_scher\_khan\_create\_data(  
                                subghz\_transmitter\_get\_protocol\_instance(txrx-\>transmitter),  
                                txrx-\>fff\_data,  
                                serial,  
                                btn,  
                                cnt,  
                                txrx-\>preset)) {  
        res \= true;  
    }

### 

    subghz\_transmitter\_free(txrx-\>transmitter);

    return res;  
}

\*protocol\_name \= "MAGIC CODE, Dynamic";  
 instance-\>serial \= ((instance-\>data \>\> 24\) & 0xFFFFFF0) | ((instance-\>data \>\> 20\) 0x0F);  
 instance-\>btn \= (instance-\>data \>\> 24\) & 0x0F;  
 instance-\>cnt \= instance-\>data & 0xFFFF;

Timings... example...   
(scher-khan as a reference for context)  
Logical zero \- 750\\750 us  
Logical unit \- 1000\\1000 us  
Header \- 1500\\1500 us

sher\_khan is .te\_short \= 750,  
    .te\_long \= 1100,  
    .te\_delta \= 150,  
    .min\_count\_bit\_for\_found \= 35

keeloq timings are .te\_short \= 400,  
    .te\_long \= 800,  
    .te\_delta \= 140,  
    .min\_count\_bit\_for\_found \= 64

starline is .te\_short \= 250,  
    .te\_long \= 500,  
    .te\_delta \= 120,  
    .min\_count\_bit\_for\_found \= 64

kia is .te\_short \= 250,  
    .te\_long \= 500,  
    .te\_delta \= 100,  
    .min\_count\_bit\_for\_found \= 61

faac is .te\_short \= 255,  
    .te\_long \= 595,  
    .te\_delta \= 100,  
    .min\_count\_bit\_for\_found \= 64

alutech is .te\_short \= 400,  
    .te\_long \= 800,  
    .te\_delta \= 140,  
    .min\_count\_bit\_for\_found \= 72

came\_atomo is .te\_short \= 600,  
    .te\_long \= 1200,  
    .te\_delta \= 250,  
    .min\_count\_bit\_for\_found \= 6

came\_twee is .te\_short \= 500,  
    .te\_long \= 1000,  
    .te\_delta \= 250,  
    .min\_count\_bit\_for\_found \= 54

came is   .te\_short \= 320,  
    .te\_long \= 640,  
    .te\_delta \= 150,  
    .min\_count\_bit\_for\_found \= 12

nice\_flo is .te\_short \= 700,  
    .te\_long \= 1400,  
    .te\_delta \= 200,  
    .min\_count\_bit\_for\_found \= 12

nice\_flor\_s is .te\_short \= 500,  
    .te\_long \= 1000,  
    .te\_delta \= 300,  
    .min\_count\_bit\_for\_found \= 52

(these next few use manchester encoding/decoding)  
somfey\_keytis is .te\_short \= 640,  
    .te\_long \= 1280,  
    .te\_delta \= 250,  
    .min\_count\_bit\_for\_found \= 80

somfey\_telis is .te\_short \= 640,  
    .te\_long \= 1280,  
    .te\_delta \= 250,  
    .min\_count\_bit\_for\_found \= 56

Real world  samples/values with hex/decimal difference between the other next number

00000001D600C4FB  
(0x69dbcd6d, 1776012653\)  
000000023FDC9268  
(0xc3e40085, 3286499461\)  
0000000303C092ED  
(0xe8be5dd76d2e, 255904315829550\)  
0000E8C16198001B  
(0x1, 1\)  
0000E8C16198001C  
(0x68000, 425984\)  
0000E8C1619E801C  
(0xea7741ce2e1e4, 4124766816625124\)  
000F90357E816200  
(0x1700c0000000000, 103595985569054720\)  
017F9C357E816200  
(0x51139ffc0000000, 365136815394390016\)  
0690D6353E816200  
(0x27a02003ffffe00, 178457335331552768\)  
090AD8357E816000  
(0x40100803fc200, 1127001570132480\)  
090ED935FEC12200  
(0x1ff00000184, 2194728288644\)  
090EDB34FEC12384  
(0x1b2fe00ffffde7c, 122439420140838524\)  
0AC1D935FEC10200  
(0xfa7dc24000, 1075851706368\)  
0AC1DA307C834200  
(0x3527804c1fe2000, 239385692033916928\)  
0E1452353E816200  
(0x52cca003fffe000, 372894971587452928\)  
13411C357E814200  
(0x1b3f40000002000, 122709895706320896\)  
14F510357E816200  
(0x1ffc0000000, 2197949513728\)  
14F512353E816200  
(0x3db820840000000, 277958773958508544\)  
18D0943D7E816200  
(0x530c9733e41a100, 374020265874661632\)  
1E015DB0BCC30300  
(0x34b284c1be3f00, 14832982044851968\)  
1E3610357E814200  
(0x1ffc0000000, 2197949513728\)  
1E3612353E814200  
(0x4c4fe0040002000, 343678748614205440\)  
22FB10357E816200  
(0x189440000000000, 110694432637976576\)  
248454357E816200  
(0x500c6ff803fc184, 360506770860261764\)  
29851B34FEC12384  
(0xb8d7f003fc01e7c, 832461145179692668\)  
35129A353E814200  
(0x1f9b80000002000, 142347173378400256\)  
370C52353E816200  
(0x43e060040000000, 305688422851280896\)  
3B4A58357E816200  
(0x383400000000000, 253116372807057408\)  
3ECD98357E816200  
(0xd101ffa0200000, 58830467547267072\)  
3F9E9A351EA16200

[image1]: <data:image/png;base64,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>

[image2]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAMAAADXqc3KAAADAFBMVEUAAAB7H6KcWLmhYL2CLKfu4/P///+VTLR/JqSYUbeUSrOVS7SPQrDw5vTx6PWSR7KKOaz6+PuNP678+/zq3fDRs9/bwubk0uyYUbb9/P3ZwOSqb8OkZL7fyej9/f738/qPQa+6is769vv38vqweseUSbLjz+rfyemNPq6JN6uXULaJN6wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABVpIPyAAAAAXRSTlMAQObYZgAAADtJREFUeF5jZGTADpjQBWBgMEmwwlnE6kAA+kqAXKqBkGCGBzsHgyTfE+U3cBlGOFBnY1NH8BiHVkQBAH/LAvPYvV0zAAAAAElFTkSuQmCC>