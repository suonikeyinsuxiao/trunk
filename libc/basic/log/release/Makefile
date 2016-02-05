OBJS=*.o
ODIR=obj

$(ROOT_DIR)/$(BIN_DIR)/$(BIN):$(ODIR)/$(OBJS)
	@echo $(BIN_DIR)
	$(CC) -o $@ $^ $(foreach n,$(HEADERS),-I$(ROOT_DIR)/$(n))
