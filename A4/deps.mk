4a.o: 4a.c 4a.h nextfit.h
4b.o: 4b.c 4b.h 4a.h nextfit.h
4extended.o: 4extended.c 4extended.h nextfit.h
nextfit.o: nextfit.c nextfit.h
test_4a.o: test_4a.c 4a.h nextfit.h
test_4b.o: test_4b.c 4b.h 4a.h nextfit.h
test_4extended.o: test_4extended.c 4extended.h nextfit.h
