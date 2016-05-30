#pragma once

#include <malloc.h>
#include <string.h>

static const unsigned char base64_enc_chars[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char base64_dec_table[] = {
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

unsigned char * acl_base64_encode(const char * src, int len)
{
	const unsigned char * clear = (const unsigned char *)src;
	unsigned char * code = NULL;
	unsigned char * p;

	code = (unsigned char *)malloc(4 * ((len + 2) / 3) + 1);
	p = code;

	while (len-- >0) {
		register int x, y;

		x = *clear++;
		*p++ = base64_enc_chars[(x >> 2) & 63];

		if (len-- <= 0) {
			*p++ = base64_enc_chars[(x << 4) & 63];
			*p++ = '=';
			*p++ = '=';
			break;
		}

		y = *clear++;
		*p++ = base64_enc_chars[((x << 4) | ((y >> 4) & 15)) & 63];

		if (len-- <= 0) {
			*p++ = base64_enc_chars[(y << 2) & 63];
			*p++ = '=';
			break;
		}

		x = *clear++;
		*p++ = base64_enc_chars[((y << 2) | ((x >> 6) & 3)) & 63];

		*p++ = base64_enc_chars[x & 63];
	}

	*p = 0;
	return (code);
}

int acl_base64_decode(const char * src, char ** pptr_in)
{
	const unsigned char * code = (const unsigned char *)src;
	unsigned char ** pptr = (unsigned char **) pptr_in;
	register int x, y;
	unsigned char * result, * result_saved = NULL;

#undef	ERETURN
#define	ERETURN(x) {            \
	if (result_saved != NULL)   \
		free(result_saved);     \
	*pptr = NULL;               \
	return (x);                 \
}

	result_saved = result = (unsigned char *)malloc(3 * (strlen((const char *) code) / 4) + 1);
	*pptr = result;

	/* Each cycle of the loop handles a quantum of 4 input bytes. For the last
	   quantum this may decode to 1, 2, or 3 output bytes. */
	while ((x = (*code++)) != 0) {
		if (x > 127 || (x = base64_dec_table[x]) == 255)
			ERETURN (-1);
		if ((y = (*code++)) == 0 || (y = base64_dec_table[y]) == 255)
			ERETURN (-1);
		*result++ = (x << 2) | (y >> 4);

		if ((x = (*code++)) == '=') {
			if (*code++ != '=' || *code != 0)
				ERETURN (-1);
		} else {
			if (x > 127 || (x = base64_dec_table[x]) == 255)
				ERETURN (-1);
			*result++ = (y << 4) | (x >> 2);
			if ((y = (*code++)) == '=') {
				if (*code != 0)
					ERETURN (-1);
			} else {
				if (y > 127 || (y = base64_dec_table[y]) == 255)
					ERETURN (-1);
				*result++ = (x << 6) | y;
			}
		}
	}

	*result = 0;
	return (int) (result - *pptr);
}

static std::string base64_encode(const char * src, int len)
{
	const unsigned char * clear = (const unsigned char *)src;

	std::string encoded;
	encoded.resize(4 * ((len + 2) / 3) + 1);
	auto p = &encoded[0];

	while (len-- > 0)
	{
		register int x, y;

		x = *clear++;
		*p++ = base64_enc_chars[(x >> 2) & 63];

		if (len-- <= 0)
		{
			*p++ = base64_enc_chars[(x << 4) & 63];
			*p++ = '=';
			*p++ = '=';
			break;
		}

		y = *clear++;
		*p++ = base64_enc_chars[((x << 4) | ((y >> 4) & 15)) & 63];

		if (len-- <= 0)
		{
			*p++ = base64_enc_chars[(y << 2) & 63];
			*p++ = '=';
			break;
		}

		x = *clear++;
		*p++ = base64_enc_chars[((y << 2) | ((x >> 6) & 3)) & 63];

		*p++ = base64_enc_chars[x & 63];
	}

	//*p = '\0';
    encoded.resize(p - &encoded[0]);
    encoded += '\0';
	return encoded;
}

static std::ptrdiff_t base64_encode(const char * src, int len, std::string & encoded)
{
	const unsigned char * clear = (const unsigned char *)src;

	encoded.resize(4 * ((len + 2) / 3) + 1);
	auto p = &encoded[0];

	while (len-- > 0)
	{
		register int x, y;

		x = *clear++;
		*p++ = base64_enc_chars[(x >> 2) & 63];

		if (len-- <= 0)
		{
			*p++ = base64_enc_chars[(x << 4) & 63];
			*p++ = '=';
			*p++ = '=';
			break;
		}

		y = *clear++;
		*p++ = base64_enc_chars[((x << 4) | ((y >> 4) & 15)) & 63];

		if (len-- <= 0)
		{
			*p++ = base64_enc_chars[(y << 2) & 63];
			*p++ = '=';
			break;
		}

		x = *clear++;
		*p++ = base64_enc_chars[((y << 2) | ((x >> 6) & 3)) & 63];

		*p++ = base64_enc_chars[x & 63];
	}

	std::ptrdiff_t encoded_size = p - &encoded[0];
    encoded.resize(encoded_size);
    encoded += '\0';
	return encoded_size;
}

static std::string base64_decode(const std::string & src)
{
	std::string decoded;
	decoded.resize(3 * (src.size() / 4) + 1);
	auto p = &decoded[0];

	/* Each cycle of the loop handles a quantum of 4 input bytes. For the last
	   quantum this may decode to 1, 2, or 3 output bytes. */

	auto it = src.cbegin();
	register int x, y;
	while ((x = (*it++)) != 0)
	{
		if (x > 127 || (x = base64_dec_table[x]) == 255)
			return {};
		if ((y = (*it++)) == 0 || (y = base64_dec_table[y]) == 255)
			return {};
		*p++ = (x << 2) | (y >> 4);

		if ((x = (*it++)) == '=')
		{
			if (*it++ != '=' || *it != 0)
				return {};
		}
		else
		{
			if (x > 127 || (x = base64_dec_table[x]) == 255)
				return {};
			*p++ = (y << 4) | (x >> 2);
			if ((y = (*it++)) == '=') {
				if (*it != 0)
					return {};
			}
			else
			{
				if (y > 127 || (y = base64_dec_table[y]) == 255)
					return {};
				*p++ = (x << 6) | y;
			}
		}
	}

	decoded.resize(p - &decoded[0]);
	return decoded;
}

static std::ptrdiff_t base64_decode(const std::string & src, std::string & decoded)
{
	decoded.resize(3 * (src.size() / 4) + 1);
	auto p = &decoded[0];

	/* Each cycle of the loop handles a quantum of 4 input bytes. For the last
	   quantum this may decode to 1, 2, or 3 output bytes. */

	auto it = src.cbegin();
	register int x, y;
	while ((x = (*it++)) != 0)
	{
		if (x > 127 || (x = base64_dec_table[x]) == 255)
			goto err_exit;
		if ((y = (*it++)) == 0 || (y = base64_dec_table[y]) == 255)
			goto err_exit;
		*p++ = (x << 2) | (y >> 4);

		if ((x = (*it++)) == '=')
		{
			if (*it++ != '=' || *it != 0)
				goto err_exit;
		}
		else
		{
			if (x > 127 || (x = base64_dec_table[x]) == 255)
				return -1;
			*p++ = (y << 4) | (x >> 2);
			if ((y = (*it++)) == '=') {
				if (*it != 0)
					goto err_exit;
			}
			else
			{
				if (y > 127 || (y = base64_dec_table[y]) == 255)
					return -1;
				*p++ = (x << 6) | y;
			}
		}
	}

	std::ptrdiff_t decoded_size = p - &decoded[0];
	decoded.resize(decoded_size);
	return decoded_size;
err_exit:
	decoded.clear();
	return -1;
}

static std::ptrdiff_t base64_encode_new(const char * src, std::size_t len, std::string & encoded)
{
	std::size_t alloc_size = ((len + 2) / 3) * 4 + 1;
	encoded.resize(alloc_size);

	// Get the length of the integer multiple of 3 is obtained.
	std::size_t multiply3_len = (len / 3) * 3;
	// The remain bytes of src length.
	std::size_t remain_len = len - multiply3_len;
	// Get the repeat times in loop
	std::size_t repeat_cnt = (len / 3);

    const unsigned char * cur = (const unsigned char *)src;
    const unsigned char * end = cur + multiply3_len;
	unsigned char * dest = (unsigned char *)&encoded[0];

	while (cur < end) {
		register unsigned int a, b, c;
		unsigned int x, y, z, l;

		a = (unsigned int)(*(cur + 0));
		b = (unsigned int)(*(cur + 1));
		c = (unsigned int)(*(cur + 2));
        x = (a >> 2);
        y = ((a << 4) & 0x30) | (b >> 4);
        z = ((b << 2) & 0x3C) | (c >> 6);
        l = (c & 0x3F);
		*(dest + 0) = base64_enc_chars[x];
		*(dest + 1) = base64_enc_chars[y];
		*(dest + 2) = base64_enc_chars[z];
        *(dest + 3) = base64_enc_chars[l];
        dest += 4;
		cur += 3;
	}

    if (remain_len == 1) {
        register unsigned int a;
		a = (unsigned int)(*(cur + 0));
        *dest++ = base64_enc_chars[(a >> 2)];
        *dest++ = base64_enc_chars[(a << 4) & 0x3F];
        *dest++ = '=';
        *dest++ = '=';
    }
    else if (remain_len == 2) {
        register unsigned int a, b;
        a = (unsigned int)(*(cur + 0));
        b = (unsigned int)(*(cur + 1));
        *dest++ = base64_enc_chars[(a >> 2)];
        *dest++ = base64_enc_chars[((a << 4) & 0x30) | (b >> 4)];
        *dest++ = base64_enc_chars[(b << 2) & 0x3C];
        *dest++ = '=';
    }

	std::ptrdiff_t encoded_size = dest - (unsigned char *)&encoded[0];
    encoded.resize(encoded_size);
    encoded += '\0';
	return encoded_size;
}

static std::ptrdiff_t base64_decode_new(const std::string & src, std::string & decoded)
{
	std::size_t alloc_size = ((src.length() + 3) / 4) * 3;
	decoded.resize(alloc_size);

	// Get the length of the integer multiple of 4 is obtained.
	std::size_t multiply4_len = src.length() & (~(std::size_t)(4 - 1));
	// The remain bytes of src length.
	std::size_t remain_len = src.length() - multiply4_len;
	// Get the repeat times in loop
	std::size_t repeat_cnt = src.length() >> 2;

    const unsigned char * cur = (const unsigned char *)&src[0];
    const unsigned char * end = cur + multiply4_len;
	unsigned char * dest = (unsigned char *)&decoded[0];

	/* Each cycle of the loop handles a quantum of 4 input bytes. For the last
	   quantum this may decode to 1, 2, or 3 output bytes. */

    if (((std::uint32_t)(cur) & 0x03U) == 0) {
        // The src address is align to 4 bytes.
        const uint32_t * cur4 = (const uint32_t *)&src[0];
        const uint32_t * end4 = cur4 + (multiply4_len >> 2);
        while (cur4 < end4) {
            uint32_t value = *cur4;
            uint32_t value1, value2;
            // If the one of char is > 127 in the four bytes.
            if ((value & 0xC0C0C0C0UL) != 0) {
                cur = (const unsigned char *)cur4;
                break;
            }
            value &= 0x3F3F3F3FUL;

            value1 = value2 = value;
            cur4++;
        }
    }
    else {
        // The src address is not align to 4 bytes.
        //
    }

	register int x, y;
	while ((x = (*cur++)) != 0) {
		if (x > 127 || (x = base64_dec_table[x]) == 255)
			goto err_exit;
		if ((y = (*cur++)) == 0 || (y = base64_dec_table[y]) == 255)
			goto err_exit;
		*dest++ = (x << 2) | (y >> 4);

		if ((x = (*cur++)) == '=')
		{
			if (*cur++ != '=' || *cur != 0)
				goto err_exit;
		}
		else
		{
			if (x > 127 || (x = base64_dec_table[x]) == 255)
				return -1;
			*dest++ = (y << 4) | (x >> 2);
			if ((y = (*cur++)) == '=') {
				if (*cur != 0)
					goto err_exit;
			}
			else
			{
				if (y > 127 || (y = base64_dec_table[y]) == 255)
					return -1;
				*dest++ = (x << 6) | y;
			}
		}
	}

	std::ptrdiff_t decoded_size = dest - (unsigned char *)&decoded[0];
	assert(decoded_size <= alloc_size);
	decoded.resize(decoded_size);
	return decoded_size;
err_exit:
	decoded.clear();
	return -1;
}
