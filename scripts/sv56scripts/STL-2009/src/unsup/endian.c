#include <stdio.h>
#include <string.h>

/* 
  ---------------------------------------------------------------------------
  Function to byte-swap n samples from short array a to short array b

  06.Oct.97  v1.0 Created <simao.campos@comsat.com>
  ---------------------------------------------------------------------------
*/

reverse_endian_short(a, b, n)
unsigned short *a, *b;
long n;
{
  long i;
  unsigned short register tmp;
  for (i=0;i<n; i++)
  {
    tmp = *a++;
    *b++ = (tmp>>8) | (tmp<<8);
  }
}

/* 
  ---------------------------------------------------------------------------
  Function to convert the byte order of n `long' samples from 
  array a to array b

  06.Oct.97  v1.0 Created <simao.campos@comsat.com>
  ---------------------------------------------------------------------------
*/
reverse_endian_long(a, b, n)
unsigned long *a, *b;
long n;
{
  long i;
  union {
    unsigned long tmp;
    unsigned char c[4];
  } in, out;

  for (i=0;i<n; i++)
  {
    in.tmp = a[i];
    out.c[0] = in.c[3];
    out.c[1] = in.c[2];
    out.c[2] = in.c[1];
    out.c[3] = in.c[0];
    b[i] = out.tmp;
  }
}

int is_little_endian()
{
  /* Hex version of the string ABCD */
  unsigned long tmp = 0x41424344; 
  
  /* compare the hex version of the 4 characters with the ascii version */
  return(strncmp("ABCD",(char *)&tmp,4));
}

#ifdef RUN
void test_s()
{
  short a,b;
  while (1)
  {
    fread(&a,1,sizeof(short),stdin);
    if (feof(stdin))
      break;
    reverse_endian_short(&a, &b, 1l);
    fwrite(&b,1,sizeof(short),stdout);
  }
}

void test_l()
{
  long a,b;
  while (1)
  {
    fread(&a,1,sizeof(long),stdin);
    if (feof(stdin))
      break;
    reverse_endian_long(&a, &b, 1l);
    fwrite(&b,1,sizeof(long),stdout);
  }
}

void main()
{
#ifdef TEST_ME
  fprintf(stderr, "System is %s endian\n",
	  is_little_endian()? "little" : "big");
# ifdef TEST_SHORT
  test_s();
# else
  test_l();
# endif
#else

#if defined(MSDOS)|| defined (_MSC_VER)|| defined (__MSDOS__)
  printf("%s", is_little_endian()? "little" : "big");
#else
  printf("%s\n", is_little_endian()? "little" : "big");
#endif /* MSDOS */

#endif
}
#endif

