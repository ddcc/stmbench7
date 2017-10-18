include Makefile.in

.PHONY: clean docs

OBJFILES = $(OBJ_DIR)/sb7_tt.o \
	$(OBJ_DIR)/tid.o $(OBJ_DIR)/memory.o \
	$(OBJ_DIR)/tm_ptr.o \
	$(OBJ_DIR)/mersenne.o $(OBJ_DIR)/random.o $(OBJ_DIR)/helpers.o $(OBJ_DIR)/parameters.o \
	$(OBJ_DIR)/data_holder.o $(OBJ_DIR)/benchmark.o \
	$(OBJ_DIR)/tm_spec.o $(OBJ_DIR)/tm_tx.o $(OBJ_DIR)/thread.o $(OBJ_DIR)/thread_fun.o $(OBJ_DIR)/id_pool.o \
	$(OBJ_DIR)/manual.o $(OBJ_DIR)/design_obj.o $(OBJ_DIR)/assembly.o \
	$(OBJ_DIR)/composite_part.o $(OBJ_DIR)/document.o $(OBJ_DIR)/atomic_part.o \
	$(OBJ_DIR)/operations.o $(OBJ_DIR)/traversal_ops.o $(OBJ_DIR)/query_ops.o \
	$(OBJ_DIR)/short_traversal_ops.o $(OBJ_DIR)/operation_ops.o \
	$(OBJ_DIR)/structural_modification_ops.o

all: $(OBJ_DIR)/sb7_tt #$(OBJ_DIR)/test/test_obj_log

# main
$(OBJ_DIR)/sb7_tt: $(OBJFILES)
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)
	cp $(OBJ_DIR)/sb7_tt .

$(OBJ_DIR)/sb7_tt.o: $(SRC_DIR)/sb7_tt.cc $(SRC_DIR)/tm/tm_ptr.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/sb7_tt.cc -c -o $@

$(OBJ_DIR)/tid.o: $(SRC_DIR)/common/tid.cc $(SRC_DIR)/common/tid.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/common/tid.cc -c -o $@

$(OBJ_DIR)/memory.o: $(SRC_DIR)/common/memory.cc \
		$(SRC_DIR)/common/memory.h $(SRC_DIR)/common/tid.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/common/memory.cc -c -o $@

$(OBJ_DIR)/tm_ptr.o: $(SRC_DIR)/tm/tm_ptr.cc  \
		$(SRC_DIR)/tm/tm_ptr.h $(SRC_DIR)/common/memory.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/tm/tm_ptr.cc -c -o $@

$(OBJ_DIR)/id_pool.o: $(SRC_DIR)/id_pool.cc $(SRC_DIR)/id_pool.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/id_pool.cc -c -o $@

# build struct
$(OBJ_DIR)/design_obj.o: $(SRC_DIR)/struct/design_obj.cc $(SRC_DIR)/struct/design_obj.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/struct/design_obj.cc -c -o $@

$(OBJ_DIR)/manual.o: $(SRC_DIR)/struct/manual.cc $(SRC_DIR)/struct/manual.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/struct/manual.cc -c -o $@

$(OBJ_DIR)/document.o: $(SRC_DIR)/struct/document.cc $(SRC_DIR)/struct/document.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/struct/document.cc -c -o $@

$(OBJ_DIR)/assembly.o: $(SRC_DIR)/struct/assembly.cc $(SRC_DIR)/struct/assembly.h \
		$(OBJ_DIR)/parameters.o
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/struct/assembly.cc -c -o $@

$(OBJ_DIR)/composite_part.o: $(SRC_DIR)/struct/composite_part.cc \
		$(SRC_DIR)/struct/composite_part.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/struct/composite_part.cc -c -o $@

$(OBJ_DIR)/atomic_part.o: $(SRC_DIR)/struct/atomic_part.cc $(SRC_DIR)/struct/atomic_part.h \
		$(SRC_DIR)/struct/connection.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/struct/atomic_part.cc -c -o $@

$(OBJ_DIR)/benchmark.o: $(SRC_DIR)/benchmark.cc $(SRC_DIR)/benchmark.h \
		$(OBJ_DIR)/thread_fun.o
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/benchmark.cc -c -o $@

# build random impl
$(OBJ_DIR)/mersenne.o: $(SRC_DIR)/randomc/mersenne.cpp
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/randomc/mersenne.cpp -c -o $@

$(OBJ_DIR)/random.o: $(SRC_DIR)/random.cc $(SRC_DIR)/random.h \
		$(SRC_DIR)/common/time.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/random.cc -c -o $@

# settings
$(OBJ_DIR)/parameters.o: $(SRC_DIR)/parameters.cc $(SRC_DIR)/parameters.h $(SRC_DIR)/helpers.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/parameters.cc -c -o $@

#
$(OBJ_DIR)/data_holder.o: $(SRC_DIR)/data_holder.cc $(SRC_DIR)/data_holder.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/data_holder.cc -c -o $@

$(OBJ_DIR)/tm_spec.o: $(SRC_DIR)/tm/tm_spec.cc $(SRC_DIR)/tm/tm_spec.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/tm/tm_spec.cc -c -o $@

$(OBJ_DIR)/tm_tx.o: $(SRC_DIR)/tm/tm_tx.cc $(SRC_DIR)/tm/tm_tx.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/tm/tm_tx.cc -c -o $@

$(OBJ_DIR)/thread.o: $(SRC_DIR)/thread/thread.cc $(SRC_DIR)/thread/thread.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/thread/thread.cc -c -o $@

$(OBJ_DIR)/thread_fun.o: $(SRC_DIR)/thread/thread_fun.cc $(SRC_DIR)/thread/thread_fun.h \
		$(SRC_DIR)/thread/thread.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/thread/thread_fun.cc -c -o $@

# helpers
$(OBJ_DIR)/helpers.o: $(SRC_DIR)/helpers.cc $(SRC_DIR)/helpers.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/helpers.cc -c -o $@

#operations
$(OBJ_DIR)/operations.o: $(SRC_DIR)/operations/operations.cc $(SRC_DIR)/operations/operations.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/operations/operations.cc -c -o $@

$(OBJ_DIR)/traversal_ops.o: $(SRC_DIR)/operations/traversal_ops.cc \
		$(SRC_DIR)/operations/traversal_ops.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/operations/traversal_ops.cc -c -o $@

$(OBJ_DIR)/query_ops.o: $(SRC_DIR)/operations/query_ops.cc \
		$(SRC_DIR)/operations/query_ops.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/operations/query_ops.cc -c -o $@

$(OBJ_DIR)/short_traversal_ops.o: $(SRC_DIR)/operations/short_traversal_ops.cc \
		$(SRC_DIR)/operations/short_traversal_ops.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/operations/short_traversal_ops.cc -c -o $@

$(OBJ_DIR)/operation_ops.o: $(SRC_DIR)/operations/operation_ops.cc \
		$(SRC_DIR)/operations/operation_ops.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/operations/operation_ops.cc -c -o $@

$(OBJ_DIR)/structural_modification_ops.o: $(SRC_DIR)/operations/structural_modification_ops.cc \
		$(SRC_DIR)/operations/structural_modification_ops.h
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/operations/structural_modification_ops.cc -c -o $@

# general
clean:
	rm -rf $(TARGET_DIR)
	rm -f sb7_tt test_obj_log

docs:
	mkdir -p $(TARGET_DOCS_DIR)
	doxygen $(SRC_DOCS_DIR)/Doxyfile

# tests
$(OBJ_DIR)/test/test_obj_log: $(OBJ)/test/test_obj_log.o $(OBJ_DIR)/tm_ptr.o $(OBJ_DIR)/tid.o
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)
	cp $(OBJ_DIR)/test/test_obj_log .

$(OBJ_DIR)/test/test_obj_log.o: $(SRC_DIR)/test/test_obj_log.cc
	mkdir -p $(OBJ_DIR)/test
	$(CPP) $(CPPFLAGS) $(SRC_DIR)/test/test_obj_log.cc -c -o $@




