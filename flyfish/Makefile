
SOURCEDIR:= $(CURDIR)/src
SUBDIRS := $(SOURCEDIR) 

all:
#	@echo $(SOURCEDIR)
	@$(foreach N,$(SUBDIRS),make -C $(N))

.PHONY:clean

clean:
	@$(foreach N,$(SUBDIRS),make -C $(N) clean)
