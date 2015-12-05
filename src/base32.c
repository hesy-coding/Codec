/**
 Copyright © 2015 2coding. All Rights Reserved.
 
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 3. The name of the author may not be used to endorse or promote products derived
 from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "base32.h"

static const char _standard_table[] = {
   //0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', //00-0f
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7' //10-1f
};

static const byte _standard_decoding_table[] = {
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 00-0f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 10-1f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 20-2f
    0xff, 0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 30-3f 2-7
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, // 40-4f A-O
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,                               // 50-5a P-Z
};

static const byte _standard_ignorecase_decoding_table[] = {
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 00-0f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 10-1f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 20-2f
    0xff, 0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 30-3f 2-7
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, // 40-4f A-O
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff, // 50-5f P-Z
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, // 60-6f 'a' - 'o'
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19                                // 70-7a 'p' - 'z'
    
};

static const char _hex_table[] = {
   //0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', //00-0f
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'  //10-1f
};

static const byte _hex_decoding_table[] = {
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 00-0f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 10-1f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 20-2f
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 30-3f 0-9
    0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, // 40-4f A-O
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,                                                       // 50-57 P-V
};

static const byte _hex_ignorecase_decoding_table[] = {
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 00-0f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 10-1f
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 20-2f
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 30-3f '0' - '9'
    0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, // 40-4f 'A' - 'O'
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 50-5f 'P' - 'V'
    0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, // 60-6f 'a' - 'o'
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f                                                        // 70-7a 'p' - 'v'
};


static CODECode _base32_work(CODECBase *p, const CDCStream *st);
static CODECode _base32_setopt(CODECBase *p, CODECOption opt, va_list args);

void *base32_init(CODECBase *p) {
    struct base32 *ptr = (struct base32 *)p;
    ptr->ignorecase = FALSE;
    ptr->hex = FALSE;
    ptr->work = _base32_work;
    ptr->setup = _base32_setopt;
    baseN_init(&ptr->bn, 5, 5, (char *)_standard_table, (byte *)_standard_decoding_table, 'Z', 0x1f);
    return p;
}

#define _BASE32_DECODING_TABLE(b32) (byte *)((b32)->ignorecase ? \
    ((b32)->hex ? _hex_ignorecase_decoding_table : _standard_ignorecase_decoding_table) :\
    ((b32)->hex ? _hex_decoding_table : _standard_decoding_table))

#define _BASE32_MAXCHAR(b32) ((b32)->ignorecase ? \
    ((b32)->hex ? 'v' : 'z') :\
    ((b32)->hex ? 'V' : 'Z'))
CODECode _base32_setopt(CODECBase *p, CODECOption opt, va_list args) {
    struct base32 *b32 = (struct base32 *)p;
    CODECode code = CODECOk;
    switch (opt) {
        case CODECBase32Hex:
            b32->hex = va_arg(args, long);
            b32->bn.entable = b32->hex ? (char *)_hex_table : (char *)_standard_table;
            b32->bn.detable = _BASE32_DECODING_TABLE(b32);
            b32->bn.maxchr = _BASE32_MAXCHAR(b32);
            break;
            
        case CODECBase32IgnoreCase:
            b32->ignorecase = va_arg(args, long);
            b32->bn.detable = _BASE32_DECODING_TABLE(b32);
            b32->bn.maxchr = _BASE32_MAXCHAR(b32);
            break;
            
        default:
            code = baseN_setup(&b32->bn, opt, args);
            break;
    }
    
    return code;
}

CODECode _base32_work(CODECBase *p, const CDCStream *st) {
    struct base32 *b32 = (struct base32 *)p;
    return baseN_work(&b32->bn, p, st);
}