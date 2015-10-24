OBJDIR:=$(CURDIR)/../obj
TRUE:=0 
FALSE:=1

all:
	@mv $(CURDIR)/*.c $(OBJDIR)
	@-make -C $(OBJDIR)
	@mv  $(OBJDIR)/*.c $(CURDIR)


.PHONY:clean

clean:
	@make -C $(OBJDIR) clean
