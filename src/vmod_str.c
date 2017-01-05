#include "config.h"

#include <stdio.h>
#include <stdlib.h>

/* need vcl.h before vrt.h for vmod_evet_f typedef */
#include "vcl.h"
#include "vrt.h"
#include "cache/cache.h"

#include "vtim.h"
#include "vcc_str_if.h"

VCL_INT
vmod_count(VRT_CTX, VCL_STRING s)
{
	if (s == NULL)
		return (-1);
	else
		return (strlen(s));
}


VCL_BOOL
vmod_startswith(VRT_CTX, VCL_STRING s1, VCL_STRING s2)
{
	if (s1 == NULL || s2 == NULL)
		return (0);
	while (*s2) {
		if (!*s1)
			return (0);
		s1++;
		s2++;
	}
	return (1);
}

VCL_BOOL
vmod_endswith(VRT_CTX, VCL_STRING s1, VCL_STRING s2)
{
	const char *p;

	if (s1 == NULL || s2 == NULL)
		return (0);
	p = s1 + strlen(s1) - strlen(s2);

	if (p < s1)
		return (0);

	return (!strcmp(p, s2));
}

VCL_STRING
vmod_take(VRT_CTX, VCL_STRING s, VCL_INT n, VCL_INT o)
{
	char *p;
	size_t l;

	if (s == NULL)
		return (NULL);

	l = strlen(s);

	if (o < 0 || (o == 0 && n < 0))		/* anchor right?*/
		o += l;

	if (n < 0) {				/* make n positive */
		n = -n;
		o -= n;
	}

	if (o + n < 0 || (o > 0 && o > l) || n == 0)	/* easy special cases*/
		return ("");

	if (o < 0) {				/* clip before string */
		n += o;
		o = 0;
	}
	if (o + n > l)				/* clip after string */
		n = l - o;

	s += o;

	if (n >= WS_Reserve(ctx->ws, n + 1)) {
		WS_Release(ctx->ws, 0);
		return (NULL);
	}

	p = ctx->ws->f;				/* Front of workspace area */
	memcpy(p, s, n);
	p[n] = '\0';

	/* Update work space with what we've used */
	WS_Release(ctx->ws, n + 1);

	return (p);
}

VCL_STRING
vmod_reverse(VRT_CTX, VCL_STRING s)
{
	char *p;
	size_t l, l2;

	if (s == NULL)
		return (NULL);

	l = l2 = strlen(s);
	if (l >= WS_Reserve(ctx->ws, l + 1)) {
		WS_Release(ctx->ws, 0);
		return (NULL);
	}

	p = ctx->ws->f;
	while(l2) {
		p[l - l2] = s[l2 - 1];
		l2--;
	}
	p[l] ='\0';

	WS_Release(ctx->ws, l + 1);

	return (p);
}
