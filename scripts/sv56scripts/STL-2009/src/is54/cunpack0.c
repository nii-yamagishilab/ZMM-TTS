/**************************************************************************

                (C) Copyright 1990, Motorola Inc., U.S.A.

Note:  Reproduction and use for the development of North American digital
       cellular standards or development of digital speech coding
       standards within the International Telecommunications Union -
       Telecommunications Standardization Sector is authorized by Motorola 
       Inc.  No other use is intended or authorized.


Systems Research Laboratories
Chicago Corporate Research and Development Center
Motorola Inc.

**************************************************************************/

/* 
  --------------------------------------------------------------------------
  void unpack0_ (int *wrds, int *blen, char *packed);
  ~~~~~~~~~~~~~
  Description:
  
  A function which accepts ascii (data) in the form of hex characters
  what are to be unpacked into the wrds array. The number of bits in 
  each wrd is indicated by the parallel array blen. The number of words 
  is indicated by wrds[0]=blen[0]. The  hexidecimal numbers expressed as 
  ASCII characters '0'..'F'.
  
  --------------------------------------------------------------------------
*/
void            unpack0_(wrds, blen, packed)
  int            *wrds;
  int            *blen;
  char           *packed;
{
  int             hex, bcentcnt, bhexcnt, centcnt;


  bhexcnt = 0;
  for (centcnt = 1; centcnt <= blen[0]; centcnt++)
  {
    /* now fill new word */
    for (wrds[centcnt] = 0, bcentcnt = blen[centcnt];
	 bcentcnt > 0; bcentcnt--, bhexcnt--)
    {
      /* any bit left in input? */
      if (bhexcnt <= 0)
      {
	bhexcnt = 4;
	hex = *packed++;
	if (hex >= 'A')
	{
	  hex = hex - 'A' + 10;
	}
	else
	{
	  hex -= '0';
	}
      }
      if (hex & 1 << bhexcnt - 1)
      {				/* input is a one */
	wrds[centcnt] += 1 << bcentcnt - 1;
      }
    }
  }
}
/* .......................... End of unpack0_() ......................... */

