initState()
{

  T_NEW.k = (FTYPE *) calloc(14 * NP, sizeof(FTYPE));	/* Note: it is  */
  T_NEW.a = T_NEW.k + NP;	/* necessary that these coef sets be */
  T_NEW.widen = T_NEW.a + NP;	/* stored sequentially. DONT REARRANGE. */

  T_OLD.k = T_NEW.widen + NP;	/* Note: it is necessary that these */
  T_OLD.a = T_OLD.k + NP;	/* coef sets be stored sequentially. */
  T_OLD.widen = T_OLD.a + NP;	/* DONT REARRANGE. */

  R_NEW.k = T_OLD.widen + NP;	/* Note: it is necessary that these */
  R_NEW.a = R_NEW.k + NP;	/* coef sets be stored sequentially. */
  R_NEW.widen = R_NEW.a + NP;	/* DONT REARRANGE. */
  R_NEW.sst = R_NEW.widen + NP;

  R_OLD.k = R_NEW.sst + NP;	/* Note: it is necessary that these */
  R_OLD.a = R_OLD.k + NP;	/* coef sets be stored sequentially. */
  R_OLD.widen = R_OLD.a + NP;	/* DONT REARRANGE. */
  R_OLD.sst = R_OLD.widen + NP;

}


swap_byte(shPtr, n)
short *shPtr;
long n;
{
  short *s_Ptr;
  short register tmp;
  long register count;
  
  for (count=0; count<n; count++)
  {
    tmp = (*shPtr << 8) + ((*shPtr >> 8) & 0x00ff);
    *shPtr++ = tmp;
  }
}
/* ......................... End of swap_byte() ............................ */
