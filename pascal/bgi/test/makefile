GPC = gpc

PASOPTS = -O

BGI2GRX = bgi2grx.o
BCC2GRX = ../../lib/dj/libbcc2.a
GRX     = -lgrx20

EXES = palette.exe small.exe
OBJS = $(EXES:.exe=.o)

all: $(EXES)

clean:
	del *.exe
	del *.o
	del *.gpi

$(EXES): %.exe: %.o $(BGI2GRX)
	gpc -s -o $*.exe $*.o $(BGI2GRX) $(BCC2GRX) $(GRX)

$(OBJS): $(BGI2GRX)

$(OBJS): %.o : %.pas
	$(GPC) $(PASOPTS) -c $*.pas

$(BGI2GRX): %.o : ../%.pas
	$(GPC) $(PASOPTS) -c $^ -o $@
