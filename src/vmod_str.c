#include "config.h"

#include <stdio.h>
#include <stdlib.h>

/* need vcl.h before vrt.h for vmod_evet_f typedef */
#include "vcl.h"
#include "vrt.h"
#include "cache/cache.h"

#include "vtim.h"
#include "vcc_str_if.h"

const size_t infosz = 64;
char	     *info;

/*
 * handle vmod internal state, vmod init/fini and/or varnish callback
 * (un)registration here.
 *
 * malloc'ing the info buffer is only indended as a demonstration, for any
 * real-world vmod, a fixed-sized buffer should be a global variable
 */

int __match_proto__(vmod_event_f)
event_function(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e e)
{
	char	   ts[VTIM_FORMAT_SIZE];
	const char *event = NULL;

	(void) ctx;
	(void) priv;

	switch (e) {
	case VCL_EVENT_LOAD:
		info = malloc(infosz);
		if (! info)
			return (-1);
		event = "loaded";
		break;
	case VCL_EVENT_WARM:
		event = "warmed";
		break;
	case VCL_EVENT_COLD:
		event = "cooled";
		break;
	case VCL_EVENT_DISCARD:
		free(info);
		return (0);
		break;
	default:
		return (0);
	}
	AN(event);
	VTIM_format(VTIM_real(), ts);
	snprintf(info, infosz, "vmod_str %s at %s", event, ts);

	return (0);
}

VCL_STRING
vmod_info(VRT_CTX)
{
	(void) ctx;

	return (info);
}

VCL_STRING
vmod_hello(VRT_CTX, VCL_STRING name)
{
	char *p;
	unsigned u, v;

	u = WS_Reserve(ctx->ws, 0); /* Reserve some work space */
	p = ctx->ws->f;		/* Front of workspace area */
	v = snprintf(p, u, "Hello, %s", name);
	v++;
	if (v > u) {
		/* No space, reset and leave */
		WS_Release(ctx->ws, 0);
		return (NULL);
	}
	/* Update work space with what we've used */
	WS_Release(ctx->ws, v);
	return (p);
}

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
