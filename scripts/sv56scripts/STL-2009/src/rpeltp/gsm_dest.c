/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/kbs/jutta/src/gsm/gsm-1.0/src/RCS/gsm_destroy.c,v 1.1 1992/10/28 00:15:50 jutta Exp $ */

#include <stdlib.h>
#include "private.h"
#include "gsm.h"
#include "proto.h"

void gsm_destroy P1((S), gsm S)
{
	if (S) free((char *)S);
}
