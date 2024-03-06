BIN = ../vectors

all::	ldcdec cwcomp snr wsnr clean

.c.o:
	cc -c $< 

snr:	snr.o 
	cc -o snr snr.o

wsnr:	wsnr.o ldcsub.o
	cc -o wsnr wsnr.o ldcsub.o s_rnge.o sig_die.o

ldcdec: ldcdec.o ldcsub.o s_rnge.o sig_die.o
	cc -o ldcdec ldcdec.o ldcsub.o s_rnge.o sig_die.o

cwcomp: cwcomp.o 
	cc -o cwcomp cwcomp.o 

clean:
	$(RM) cwcomp.o ldcdec.o filsub.o ldcsub.o wsnr.o snr.o s_rnge.o sig_die.o

clean-exe:
	$(RM) ldcdec cwcomp snr wsnr

test:
	cwcomp $(BIN)/cw1.bin $(BIN)/incw1.bin
	snr $(BIN)/in4.bin $(BIN)/outb4.bin
	wsnr $(BIN)/in1.bin $(BIN)/cw2.bin
	ldcdec $(BIN)/cw1.bin outa1.REC 0
	snr $(BIN)/outa1.bin outa1.REC
	$(RM) outa1.REC

