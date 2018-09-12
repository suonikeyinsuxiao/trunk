
incdirs := ../apple/include \
			../orange/include \
			../grape/include \
			../pear/include

srcdirs := ../apple/source/ \
			../grape/source/ \
			../orange/source/ \
			../pear/source/

srcs := apple.c grape.c orange.c pear.c


#VPATH = $(incdirs)
vpath %.c $(srcdirs)
#vpath %.h $(incdirs)

#$(info $(patsubst /,/,$(incdirs)))
#CCINC = $(patsubst %,-I%,$(patsubst /,/,$(incdirs)))
CCINC = $(patsubst %,-I%,$(incdirs))

objdir := obj
objlist := $(patsubst %.c, $(objdir)/%.o, $(srcs))
#$(info "objlist:"$(objlist))


all: $(objdir) $(objlist) 
	@echo  "compile Done"

$(objdir):
	@echo "crate obj direcotory: $(objdir)"
	mkdir -p $(objdir)


$(objdir)/%.o: %.c
	gcc -c -MD $(CCINC) -o $@ $<

