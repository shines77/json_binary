#pragma once

#include <malloc.h>
#include <string.h>

static const unsigned char to_b64_tab[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char un_b64_tab[] = {
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
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0
};

unsigned char * acl_base64_encode(const char * in, int len)
{
	const unsigned char * clear = (const unsigned char *)in;
	unsigned char * code = NULL;
	unsigned char * p;

	code = (unsigned char *)malloc(4 * ((len + 2) / 3) + 1);
	p = code;

	while (len-- >0) {
		register int x, y;

		x = *clear++;
		*p++ = to_b64_tab[(x >> 2) & 63];

		if (len-- <= 0) {
			*p++ = to_b64_tab[(x << 4) & 63];
			*p++ = '=';
			*p++ = '=';
			break;
		}

		y = *clear++;
		*p++ = to_b64_tab[((x << 4) | ((y >> 4) & 15)) & 63];

		if (len-- <= 0) {
			*p++ = to_b64_tab[(y << 2) & 63];
			*p++ = '=';
			break;
		}

		x = *clear++;
		*p++ = to_b64_tab[((y << 2) | ((x >> 6) & 3)) & 63];

		*p++ = to_b64_tab[x & 63];
	}

	*p = 0;
	return (code);
}

int acl_base64_decode(const char * in, char ** pptr_in)
{
	const unsigned char * code = (const unsigned char *)in;
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
		if (x > 127 || (x = un_b64_tab[x]) == 255)
			ERETURN (-1);
		if ((y = (*code++)) == 0 || (y = un_b64_tab[y]) == 255)
			ERETURN (-1);
		*result++ = (x << 2) | (y >> 4);

		if ((x = (*code++)) == '=') {
			if (*code++ != '=' || *code != 0)
				ERETURN (-1);
		} else {
			if (x > 127 || (x = un_b64_tab[x]) == 255)
				ERETURN (-1);
			*result++ = (y << 4) | (x >> 2);
			if ((y = (*code++)) == '=') {
				if (*code != 0)
					ERETURN (-1);
			} else {
				if (y > 127 || (y = un_b64_tab[y]) == 255)
					ERETURN (-1);
				*result++ = (x << 6) | y;
			}
		}
	}

	*result = 0;
	return (int) (result - *pptr);
}

static std::string base64_encode(const char * in, int len)
{
	const unsigned char * clear = (const unsigned char *)in;

	std::string encoded;
	encoded.resize(4 * ((len + 2) / 3) + 1);
	auto p = &encoded[0];

	while (len-- > 0)
	{
		register int x, y;

		x = *clear++;
		*p++ = to_b64_tab[(x >> 2) & 63];

		if (len-- <= 0)
		{
			*p++ = to_b64_tab[(x << 4) & 63];
			*p++ = '=';
			*p++ = '=';
			break;
		}

		y = *clear++;
		*p++ = to_b64_tab[((x << 4) | ((y >> 4) & 15)) & 63];

		if (len-- <= 0)
		{
			*p++ = to_b64_tab[(y << 2) & 63];
			*p++ = '=';
			break;
		}

		x = *clear++;
		*p++ = to_b64_tab[((y << 2) | ((x >> 6) & 3)) & 63];

		*p++ = to_b64_tab[x & 63];
	}

	//*p = '\0';
    encoded.resize(p - &encoded[0]);
    encoded += '\0';
	return encoded;
}

static std::ptrdiff_t base64_encode(const char * in, int len, std::string & encoded)
{
	const unsigned char * clear = (const unsigned char *)in;

	encoded.resize(4 * ((len + 2) / 3) + 1);
	auto p = &encoded[0];

	while (len-- > 0)
	{
		register int x, y;

		x = *clear++;
		*p++ = to_b64_tab[(x >> 2) & 63];

		if (len-- <= 0)
		{
			*p++ = to_b64_tab[(x << 4) & 63];
			*p++ = '=';
			*p++ = '=';
			break;
		}

		y = *clear++;
		*p++ = to_b64_tab[((x << 4) | ((y >> 4) & 15)) & 63];

		if (len-- <= 0)
		{
			*p++ = to_b64_tab[(y << 2) & 63];
			*p++ = '=';
			break;
		}

		x = *clear++;
		*p++ = to_b64_tab[((y << 2) | ((x >> 6) & 3)) & 63];

		*p++ = to_b64_tab[x & 63];
	}

	std::ptrdiff_t encoded_size = p - &encoded[0];
    encoded.resize(encoded_size);
    encoded += '\0';
	return encoded_size;
}

static std::string base64_decode(const std::string & in)
{
	std::string decoded;
	decoded.resize(3 * (in.size() / 4) + 1);
	auto p = &decoded[0];

	/* Each cycle of the loop handles a quantum of 4 input bytes. For the last
	   quantum this may decode to 1, 2, or 3 output bytes. */

	auto it = in.cbegin();
	register int x, y;
	while ((x = (*it++)) != 0)
	{
		if (x > 127 || (x = un_b64_tab[x]) == 255)
			return {};
		if ((y = (*it++)) == 0 || (y = un_b64_tab[y]) == 255)
			return {};
		*p++ = (x << 2) | (y >> 4);

		if ((x = (*it++)) == '=')
		{
			if (*it++ != '=' || *it != 0)
				return {};
		}
		else
		{
			if (x > 127 || (x = un_b64_tab[x]) == 255)
				return {};
			*p++ = (y << 4) | (x >> 2);
			if ((y = (*it++)) == '=') {
				if (*it != 0)
					return {};
			}
			else
			{
				if (y > 127 || (y = un_b64_tab[y]) == 255)
					return {};
				*p++ = (x << 6) | y;
			}
		}
	}

	decoded.resize(p - &decoded[0]);
	return decoded;
}

static std::ptrdiff_t base64_decode(const std::string & in, std::string & decoded)
{
	decoded.resize(3 * (in.size() / 4) + 1);
	auto p = &decoded[0];

	/* Each cycle of the loop handles a quantum of 4 input bytes. For the last
	   quantum this may decode to 1, 2, or 3 output bytes. */

	auto it = in.cbegin();
	register int x, y;
	while ((x = (*it++)) != 0)
	{
		if (x > 127 || (x = un_b64_tab[x]) == 255)
			goto err_exit;
		if ((y = (*it++)) == 0 || (y = un_b64_tab[y]) == 255)
			goto err_exit;
		*p++ = (x << 2) | (y >> 4);

		if ((x = (*it++)) == '=')
		{
			if (*it++ != '=' || *it != 0)
				goto err_exit;
		}
		else
		{
			if (x > 127 || (x = un_b64_tab[x]) == 255)
				return -1;
			*p++ = (y << 4) | (x >> 2);
			if ((y = (*it++)) == '=') {
				if (*it != 0)
					goto err_exit;
			}
			else
			{
				if (y > 127 || (y = un_b64_tab[y]) == 255)
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
