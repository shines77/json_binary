
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "base64_fast.h"

#ifndef base64_min
#define base64_min(a, b)    (((a) <= (b)) ? (a) : (b))
#endif
#ifndef base64_max
#define base64_max(a, b)    (((a) >= (b)) ? (a) : (b))
#endif

const unsigned char base64_enc_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const unsigned char base64_dec_table[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,  255, 255, 255, 63,
    52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255, 255, 255, 255, 255,
    255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
    15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  255, 255, 255, 255, 255,
    255, 26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
    41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

ssize_t __base64_encode(const char * src, size_t src_len, char * dest, size_t dest_len)
{
    size_t alloc_size = ((src_len + 2) / 3) * 4 + 1;
    if (dest == NULL) {
        return (dest_len == 0) ? (ssize_t)alloc_size : -1;
    }

    // Get the length of the integer multiple of 3 is obtained.
    size_t multiply3_len = (src_len / 3) * 3;
    // Get the max can output src length
    size_t max_src_len = (dest_len / 4) * 3;
    // The remain bytes of src length.
    size_t remain_len;
    if (max_src_len >= multiply3_len) {
        max_src_len = multiply3_len;
        remain_len = src_len - multiply3_len;
    }
    else {
        remain_len = 0;
    }

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64) || defined(__amd64__) || defined(__x86_64__)
    register const unsigned char * cur = (const unsigned char *)src;
    register const unsigned char * end = cur + max_src_len;
    register unsigned char * out = (unsigned char *)dest;

    while (cur < end) {
        register unsigned int a, b, c;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        c = (unsigned int)(*(cur + 2));
        *(out + 0) = base64_enc_chars[(a >> 2)];
        *(out + 1) = base64_enc_chars[((a << 4) & 0x30U) | (b >> 4)];
        *(out + 2) = base64_enc_chars[((b << 2) & 0x3CU) | (c >> 6)];
        *(out + 3) = base64_enc_chars[(c & 0x3FU)];
        cur += 3;
        out += 4;
#if 0
        //register unsigned int a2, b2, c2;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        c = (unsigned int)(*(cur + 2));
        *(out + 0) = base64_enc_chars[(a >> 2)];
        *(out + 1) = base64_enc_chars[((a << 4) & 0x30U) | (b >> 4)];
        *(out + 2) = base64_enc_chars[((b << 2) & 0x3CU) | (c >> 6)];
        *(out + 3) = base64_enc_chars[(c & 0x3FU)];
        cur += 3;
        out += 4;
#endif
    }
#else
    register const unsigned char * cur = (const unsigned char *)src;
    const unsigned char *          end = cur + max_src_len;
    register unsigned char * out = (unsigned char *)dest;
    // The src address is not align to 4 bytes.
    while (cur < end) {
        register unsigned int a, b, c;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        c = (unsigned int)(*(cur + 2));
        *(out + 0) = base64_enc_chars[(a >> 2)];
        *(out + 1) = base64_enc_chars[((a << 4) & 0x30U) | (b >> 4)];
        *(out + 2) = base64_enc_chars[((b << 2) & 0x3CU) | (c >> 6)];
        *(out + 3) = base64_enc_chars[(c & 0x3FU)];
        out += 4;
        cur += 3;
    }
#endif

    if (remain_len == 1) {
        register unsigned int a;
        a = (unsigned int)(*(cur + 0));
        *out++ = base64_enc_chars[(a >> 2)];
        *out++ = base64_enc_chars[(a << 4) & 0x3F];
        *out++ = '=';
        *out++ = '=';
    }
    else if (remain_len == 2) {
        register unsigned int a, b;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        *out++ = base64_enc_chars[(a >> 2)];
        *out++ = base64_enc_chars[((a << 4) & 0x30) | (b >> 4)];
        *out++ = base64_enc_chars[(b << 2) & 0x3C];
        *out++ = '=';
    }

    *out = '\0';
    ssize_t encoded_size = out - (unsigned char *)dest;
    assert(encoded_size < (ssize_t)alloc_size);
    assert(encoded_size <= (ssize_t)dest_len);
    return encoded_size;
}

ssize_t __base64_decode(const char * src, size_t src_len, char * dest, size_t dest_len)
{
    size_t alloc_size = ((src_len + 3) / 4) * 3;
    if (dest == NULL) {
        return (dest_len == 0) ? (ssize_t)alloc_size : -1;
    }

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64) || defined(__amd64__) || defined(__x86_64__)
    // Get the length of the integer multiple of 4 is obtained.
    size_t multiply4_len = src_len & (~(size_t)(8 - 1));

    register const unsigned char * cur = (const unsigned char *)src;
    const unsigned char * end = cur + multiply4_len;
    register unsigned char * out = (unsigned char *)dest;

    while (cur < end) {
        register unsigned int a, b, c, d;
        register unsigned int value;
        a  = base64_dec_table[*(cur + 0)];
        b  = base64_dec_table[*(cur + 1)];
        c  = base64_dec_table[*(cur + 2)];
        d  = base64_dec_table[*(cur + 3)];
        value = a | b | c | d;
        *(out + 0) = (a << 2) | ((b & 0x30U) >> 4);
        *(out + 1) = (b << 4) | ((c & 0x3CU) >> 2);
        *(out + 2) = ((c & 0x03U) << 6) | (d & 0x3FU);
        cur += 4;
        out += 3;

        a  = base64_dec_table[*(cur + 0)];
        b  = base64_dec_table[*(cur + 1)];
        c  = base64_dec_table[*(cur + 2)];
        d  = base64_dec_table[*(cur + 3)];
        value |= a | b | c | d;
        if ((value & 0x80UL) != 0) {
            // Found '\0', '=' or another chars
            cur -= 4;
            out -= 3;
            break;
        }
        *(out + 0) = (a << 2) | ((b & 0x30U) >> 4);
        *(out + 1) = (b << 4) | ((c & 0x3CU) >> 2);
        *(out + 2) = ((c & 0x03U) << 6) | (d & 0x3FU);
        cur += 4;
        out += 3;
    }
#else
    // Get the length of the integer multiple of 4 is obtained.
    size_t multiply4_len = src_len & (~(size_t)(4 - 1));

    const unsigned char * cur = (const unsigned char *)src;
    const unsigned char * end = cur + multiply4_len;
    unsigned char * out = (unsigned char *)dest;

    while (cur < end) {
        register uint32_t a, b, c, d;
        uint32_t value;
        a  = base64_dec_table[*(cur + 0)];
        b  = base64_dec_table[*(cur + 1)];
        c  = base64_dec_table[*(cur + 2)];
        d  = base64_dec_table[*(cur + 3)];
        value = a | b | c | d;
#if 0
        if ((value & 0x80U) != 0) {
            // Found '\0', '=' or another chars
            break;
        }
#endif
        *(out + 0) = (a << 2) | ((b & 0x30) >> 4);
        *(out + 1) = (b << 4) | ((c & 0x3C) >> 2);
        *(out + 2) = ((c & 0x03) << 6) | (d & 0x3F);
        cur += 4;
        out += 3;
#if 1
        a  = base64_dec_table[*(cur + 0)];
        b  = base64_dec_table[*(cur + 1)];
        c  = base64_dec_table[*(cur + 2)];
        d  = base64_dec_table[*(cur + 3)];
        value |= a | b | c | d;
        *(out + 0) = (a << 2) | ((b & 0x30) >> 4);
        *(out + 1) = (b << 4) | ((c & 0x3C) >> 2);
        *(out + 2) = ((c & 0x03) << 6) | (d & 0x3F);
        if ((value & 0x80U) != 0) {
            // Found '\0', '=' or another chars
            cur -= 4;
            out -= 3;
            break;
        }
        cur += 4;
        out += 3;
#endif
    }
#endif

    /* Each cycle of the loop handles a quantum of 4 input bytes. For the last
       quantum this may decode to 1, 2, or 3 output bytes. */
    end = (const unsigned char *)src + src_len;

    register int x, y;
    while ((cur < end) && (*cur != '\0')) {
        if ((x = base64_dec_table[*cur++]) == 255)
            goto err_exit;
        if ((y = base64_dec_table[*cur++]) == 255)
            goto err_exit;
        *out++ = (x << 2) | (y >> 4);

        if (*cur == '=') {
            cur++;
            if (*cur != '=' || *cur != 0)
                goto err_exit;
            *out = '\0';
        }
        else {
            if ((x = base64_dec_table[*cur++]) == 255)
                goto err_exit;
            *out++ = (y << 4) | (x >> 2);
            if (*cur == '=') {
                cur++;
                if (*cur != 0)
                    goto err_exit;
                *out = '\0';
            }
            else {
                if ((y = base64_dec_table[*cur++]) == 255)
                    goto err_exit;
                *out++ = (x << 6) | y;
            }
        }
    }

    ssize_t decoded_size = out - (unsigned char *)dest;
    assert(decoded_size <= (ssize_t)alloc_size);
    assert(decoded_size <= (ssize_t)dest_len);
    return decoded_size;
err_exit:
    if (dest_len > 0)
        *dest = '\0';
    return -1;
}

ssize_t __base64_decode2(const char * src, size_t src_len, char * dest, size_t dest_len)
{
    size_t alloc_size = ((src_len + 3) / 4) * 3;
    if (dest == NULL) {
        return (dest_len == 0) ? (ssize_t)alloc_size : -1;
    }

    // Get the length of the integer multiple of 4 is obtained.
    size_t multiply4_len = src_len & (~(size_t)(8 - 1));

    const unsigned char * cur = (const unsigned char *)src;
    const unsigned char * end = cur + multiply4_len;
    unsigned char * out = (unsigned char *)dest;

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64) || defined(__amd64__) || defined(__x86_64__)
    while (cur < end) {
        register unsigned char a, b, c, d;
        register uint32_t value;
        a  = base64_dec_table[*cur++];
        b  = base64_dec_table[*cur++];
        c  = base64_dec_table[*cur++];
        d  = base64_dec_table[*cur++];
        value = a | b | c | d;
        *out++ = (a << 2) | ((b & 0x30U) >> 4);
        *out++ = (b << 4) | ((c & 0x3CU) >> 2);
        *out++ = ((c & 0x03U) << 6) | (d & 0x3FU);

        a  = base64_dec_table[*cur++];
        b  = base64_dec_table[*cur++];
        c  = base64_dec_table[*cur++];
        d  = base64_dec_table[*cur++];
        value |= a | b | c | d;
        if ((value & 0x80UL) != 0) {
            // Found '\0', '=' or another chars
            cur -= 8;
            out -= 3;
            break;
        }
        *out++ = (a << 2) | ((b & 0x30U) >> 4);
        *out++ = (b << 4) | ((c & 0x3CU) >> 2);
        *out++ = ((c & 0x03U) << 6) | (d & 0x3FU);
    }
#else
    while (cur < end) {
        register uint32_t a, b, c, d;
        uint32_t value;
        a  = base64_dec_table[*cur++];
        b  = base64_dec_table[*cur++];
        c  = base64_dec_table[*cur++];
        d  = base64_dec_table[*cur++];
        value = a | b | c | d;
        *out++ = (a << 2) | ((b & 0x30) >> 4);
        *out++ = (b << 4) | ((c & 0x3C) >> 2);
        *out++ = ((c & 0x03) << 6) | (d & 0x3F);

        a  = base64_dec_table[*cur++];
        b  = base64_dec_table[*cur++];
        c  = base64_dec_table[*cur++];
        d  = base64_dec_table[*cur++];
        value |= a | b | c | d;
        if ((value & 0x80UL) != 0) {
            // Found '\0', '=' or another chars
            cur -= 8;
            out -= 3;
            break;
        }
        *out++ = (a << 2) | ((b & 0x30) >> 4);
        *out++ = (b << 4) | ((c & 0x3C) >> 2);
        *out++ = ((c & 0x03) << 6) | (d & 0x3F);
    }
#endif

    /* Each cycle of the loop handles a quantum of 4 input bytes. For the last
       quantum this may decode to 1, 2, or 3 output bytes. */
    end = (const unsigned char *)src + src_len;

    register int x, y;
    while ((cur < end) && (*cur != '\0')) {
        if ((x = base64_dec_table[*cur++]) == 255)
            goto err_exit;
        if ((y = base64_dec_table[*cur++]) == 255)
            goto err_exit;
        *out++ = (x << 2) | (y >> 4);

        if (*cur == '=') {
            cur++;
            if (*cur != '=' || *cur != 0)
                goto err_exit;
            *out = '\0';
        }
        else {
            if ((x = base64_dec_table[*cur++]) == 255)
                goto err_exit;
            *out++ = (y << 4) | (x >> 2);
            if (*cur == '=') {
                cur++;
                if (*cur != 0)
                    goto err_exit;
                *out = '\0';
            }
            else {
                if ((y = base64_dec_table[*cur++]) == 255)
                    goto err_exit;
                *out++ = (x << 6) | y;
            }
        }
    }

    ssize_t decoded_size = out - (unsigned char *)dest;
    assert(decoded_size <= (ssize_t)alloc_size);
    assert(decoded_size <= (ssize_t)dest_len);
    return decoded_size;
err_exit:
    if (dest_len > 0)
        *dest = '\0';
    return -1;
}

ssize_t base64_encode_malloc(const char * src, size_t src_len, char ** dest)
{
    size_t alloc_size = ((src_len + 2) / 3) * 4 + 1;
    if (dest == NULL)
        return alloc_size;

    unsigned char * encoded = (unsigned char *)malloc(alloc_size * sizeof(unsigned char));
    if (encoded == NULL)
        return -1;

    // Get the length of the integer multiple of 3 is obtained.
    size_t multiply3_len = (src_len / 3) * 3;
    // The remain bytes of src length.
    size_t remain_len = src_len - multiply3_len;

    const unsigned char * cur = (const unsigned char *)src;
    const unsigned char * end = cur + multiply3_len;
    unsigned char * out = (unsigned char *)encoded;

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64) || defined(__amd64__) || defined(__x86_64__)
    while (cur < end) {
        register unsigned int a, b, c;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        c = (unsigned int)(*(cur + 2));
        *(out + 0) = base64_enc_chars[(a >> 2)];
        *(out + 1) = base64_enc_chars[((a << 4) & 0x30) | (b >> 4)];
        *(out + 2) = base64_enc_chars[((b << 2) & 0x3C) | (c >> 6)];
        *(out + 3) = base64_enc_chars[(c & 0x3F)];
        out += 4;
        cur += 3;
    }
#else
    // The src address is not align to 4 bytes.
    while (cur < end) {
        register unsigned int a, b, c;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        c = (unsigned int)(*(cur + 2));
        *(out + 0) = base64_enc_chars[(a >> 2)];
        *(out + 1) = base64_enc_chars[((a << 4) & 0x30) | (b >> 4)];
        *(out + 2) = base64_enc_chars[((b << 2) & 0x3C) | (c >> 6)];
        *(out + 3) = base64_enc_chars[(c & 0x3F)];
        out += 4;
        cur += 3;
    }
#endif

    if (remain_len == 1) {
        register unsigned int a;
        a = (unsigned int)(*(cur + 0));
        *out++ = base64_enc_chars[(a >> 2)];
        *out++ = base64_enc_chars[(a << 4) & 0x3F];
        *out++ = '=';
        *out++ = '=';
    }
    else if (remain_len == 2) {
        register unsigned int a, b;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        *out++ = base64_enc_chars[(a >> 2)];
        *out++ = base64_enc_chars[((a << 4) & 0x30) | (b >> 4)];
        *out++ = base64_enc_chars[(b << 2) & 0x3C];
        *out++ = '=';
    }

    *out = '\0';
    ssize_t encoded_size = out - encoded;
    assert(encoded_size < (ssize_t)alloc_size);
    if (dest != NULL)
        *dest = (char *)encoded;
    return encoded_size;
}

ssize_t base64_decode_malloc(const char * src, size_t src_len, char ** dest)
{
    size_t alloc_size = ((src_len + 3) / 4) * 3;
    if (dest == NULL)
        return alloc_size;

    unsigned char * decoded = (unsigned char *)malloc(alloc_size * sizeof(unsigned char));
    if (decoded == NULL)
        return -1;

    // Get the length of the integer multiple of 4 is obtained.
    size_t multiply4_len = src_len & (~(size_t)(4 - 1));

    const unsigned char * cur = (const unsigned char *)src;
    const unsigned char * end = cur + multiply4_len;
    unsigned char * out = (unsigned char *)decoded;

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64) || defined(__amd64__) || defined(__x86_64__)
    while (cur < end) {
        register unsigned char a, b, c, d;
        register uint32_t value;
        a  = base64_dec_table[*(cur + 0)];
        b  = base64_dec_table[*(cur + 1)];
        c  = base64_dec_table[*(cur + 2)];
        d  = base64_dec_table[*(cur + 3)];
        value = (d << 24) | (c << 16) | (b << 8) | a;
        if ((value & 0x80808080UL) != 0) {
            // Found '\0', '=' or another chars
            break;
        }
        *(out + 0) = (a << 2) | ((b & 0x30) >> 4);
        *(out + 1) = (b << 4) | ((c & 0x3C) >> 2);
        *(out + 2) = ((c & 0x03) << 6) | (d & 0x3F);
        cur += 4;
        out += 3;
    }
#else
    while (cur < end) {
        register uint32_t a, b, c, d;
        uint32_t value;
        a  = base64_dec_table[*(cur + 0)];
        b  = base64_dec_table[*(cur + 1)];
        c  = base64_dec_table[*(cur + 2)];
        d  = base64_dec_table[*(cur + 3)];
        value = (d << 24) | (c << 16) | (b << 8) | a;
        *(out + 0) = (a << 2) | ((b & 0x30) >> 4);
        *(out + 1) = (b << 4) | ((c & 0x3C) >> 2);
        *(out + 2) = ((c & 0x03) << 6) | (d & 0x3F);
        if ((value & 0x80808080UL) != 0) {
            // Found '\0', '=' or another chars
            break;
        }
        cur += 4;
        out += 3;
    }
#endif

    /* Each cycle of the loop handles a quantum of 4 input bytes. For the last
       quantum this may decode to 1, 2, or 3 output bytes. */
    end = (const unsigned char *)src + src_len;

    register int x, y;
    while ((cur < end) && (*cur != '\0')) {
        if ((x = base64_dec_table[*cur++]) == 255)
            goto err_exit;
        if ((y = base64_dec_table[*cur++]) == 255)
            goto err_exit;
        *out++ = (x << 2) | (y >> 4);

        if (*cur == '=') {
            cur++;
            if (*cur != '=' || *cur != 0)
                goto err_exit;
            *out = '\0';
        }
        else {
            if ((x = base64_dec_table[*cur++]) == 255)
                goto err_exit;
            *out++ = (y << 4) | (x >> 2);
            if (*cur == '=') {
                cur++;
                if (*cur != 0)
                    goto err_exit;
                *out = '\0';
            }
            else {
                if ((y = base64_dec_table[*cur++]) == 255)
                    goto err_exit;
                *out++ = (x << 6) | y;
            }
        }
    }

    ssize_t decoded_size = out - decoded;
    assert(decoded_size <= (ssize_t)alloc_size);
    if (dest != NULL)
        *dest = (char *)decoded;
    return decoded_size;
err_exit:
    if (decoded)
        free(decoded);
    decoded = (unsigned char *)malloc(1);
    *decoded = '\0';
    if (dest != NULL)
        *dest = (char *)decoded;
    return -1;
}

ssize_t base64_encode_fast(const char * src, size_t src_len, char * dest, size_t dest_len)
{
    size_t alloc_size = ((src_len + 2) / 3) * 4 + 1;
    if (dest == NULL) {
        return (dest_len == 0) ? (ssize_t)alloc_size : -1;
    }

    // Get the length of the integer multiple of 3 is obtained.
    size_t multiply3_len = (src_len / 3) * 3;
    // Get the max can output src length
    size_t max_src_len = (dest_len / 4) * 3;
    // The remain bytes of src length.
    size_t remain_len;
    if (max_src_len >= multiply3_len) {
        max_src_len = multiply3_len;
        remain_len = src_len - multiply3_len;
    }
    else {
        remain_len = 0;
    }

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64) || defined(__amd64__) || defined(__x86_64__)
    register const unsigned char * cur = (const unsigned char *)src;
    register const unsigned char * end = cur + max_src_len;
    register unsigned char *       out = (unsigned char *)dest;

#if 1
    while (cur < end) {
        register uint64_t cur64 = *(uint64_t *)cur;
        cur64 = __byteswap64(cur64);
        *(out + 0) = base64_enc_chars[(cur64 >> 58)];
        *(out + 1) = base64_enc_chars[(cur64 >> 52) & 0x3FU];
        *(out + 2) = base64_enc_chars[(cur64 >> 46) & 0x3FU];
        *(out + 3) = base64_enc_chars[(cur64 >> 40) & 0x3FU];
        *(out + 4) = base64_enc_chars[(cur64 >> 34) & 0x3FU];
        *(out + 5) = base64_enc_chars[(cur64 >> 28) & 0x3FU];
        *(out + 6) = base64_enc_chars[(cur64 >> 22) & 0x3FU];
        *(out + 7) = base64_enc_chars[(cur64 >> 16) & 0x3FU];
        out += 8;
        cur += 6;
    }
#else
    while (cur < end) {
        register uint64_t cur64 = *(uint64_t *)cur;
        cur64 = __byteswap64(cur64);
        *out++ = base64_enc_chars[(cur64 >> 58)];
        *out++ = base64_enc_chars[(cur64 >> 52) & 0x3FU];
        *out++ = base64_enc_chars[(cur64 >> 46) & 0x3FU];
        *out++ = base64_enc_chars[(cur64 >> 40) & 0x3FU];
        *out++ = base64_enc_chars[(cur64 >> 34) & 0x3FU];
        *out++ = base64_enc_chars[(cur64 >> 28) & 0x3FU];
        *out++ = base64_enc_chars[(cur64 >> 22) & 0x3FU];
        *out++ = base64_enc_chars[(cur64 >> 16) & 0x3FU];
        cur += 6;
    }
#endif
#else
    register const unsigned char * cur = (const unsigned char *)src;
    register const unsigned char * end = cur + max_src_len;
    register unsigned char *       out = (unsigned char *)dest;
    // The src address is not align to 4 bytes.
#if 1
    while (cur < end) {
        register uint32_t cur32 = *(uint32_t *)cur;
        cur32 = __byteswap32(cur32);
        *out++ = base64_enc_chars[(cur32 >> 26)];
        *out++ = base64_enc_chars[(cur32 >> 20) & 0x3FU];
        *out++ = base64_enc_chars[(cur32 >> 14) & 0x3FU];
        *out++ = base64_enc_chars[(cur32 >>  8) & 0x3FU];
        cur += 3;
    }
#else
    while (cur < end) {
        register uint32_t cur32 = *(uint32_t *)cur;
        cur32 = __byteswap32(cur32);
        *(out + 0) = base64_enc_chars[(cur32 >> 26)];
        *(out + 1) = base64_enc_chars[(cur32 >> 20) & 0x3FU];
        *(out + 2) = base64_enc_chars[(cur32 >> 14) & 0x3FU];
        *(out + 3) = base64_enc_chars[(cur32 >>  8) & 0x3FU];
        out += 4;
        cur += 3;
    }
#endif
#endif // _WIN64

    if (remain_len == 1) {
        register unsigned int a;
        a = (unsigned int)(*(cur + 0));
        *out++ = base64_enc_chars[(a >> 2)];
        *out++ = base64_enc_chars[(a << 4) & 0x3F];
        *out++ = '=';
        *out++ = '=';
    }
    else if (remain_len == 2) {
        register unsigned int a, b;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        *out++ = base64_enc_chars[(a >> 2)];
        *out++ = base64_enc_chars[((a << 4) & 0x30) | (b >> 4)];
        *out++ = base64_enc_chars[(b << 2) & 0x3C];
        *out++ = '=';
    }

    *out = '\0';
    ssize_t encoded_size = out - (unsigned char *)dest;
    assert(encoded_size < (ssize_t)alloc_size);
    assert(encoded_size <= (ssize_t)dest_len);
    return encoded_size;
}

ssize_t base64_decode_fast(const char * src, size_t src_len, char * dest, size_t dest_len)
{
    return __base64_decode(src, src_len, dest, dest_len);
}
