
DIRS := kernel 
all:
	@for i in $(DIRS); do ( cd $$i; $(MAKE) all ); \
		if [ $$? -ne 0 ]; then exit 1; fi done
clean:
	@for i in $(DIRS); do ( cd $$i; $(MAKE) clean ); \
		if [ $$? -ne 0 ]; then exit 1; fi done
