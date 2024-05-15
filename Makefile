# CFLAGS =-Wall -Wextra
OUT_TEST=build/test/gcc-lin-x64
OUT_TEST2=build/test2/gcc-lin-x64
OUT_LIB=build/lib/gcc-lin-x64
SRC=src
CFLAGS=-Wall -Wextra
LIB_EXPORT=lib
H=src

# .PHONY: test1 test2 all build_module_1 build_module_2

# do all the things by default
build:	init bumpalloc bumpalloc_wrapper test1 test2 export_lib

# make the folders for the build files
init:
	-mkdir -p $(LIB_EXPORT)
	-mkdir -p $(OUT_LIB)
	-mkdir -p $(OUT_TEST)
	-mkdir -p $(OUT_TEST2)

# clear the build files
clean:
	rm -rf build
	rm -rf $(LIB_EXPORT)

# build the actual allocator
bumpalloc:
	gcc -c $(SRC)/bumpalloc.c -o $(OUT_LIB)/bumpalloc.a

# build the malloc wrapper for the allocator
bumpalloc_wrapper: bumpalloc
	gcc -c $(SRC)/bumpalloc_wrapper.c -L$(H)/bumpalloc_wrapper.h -o $(OUT_LIB)/bumpalloc_wrapper.a


# check that bump allocator works
test1: bumpalloc
	gcc $(SRC)/test.c $(OUT_LIB)/bumpalloc.a -o $(OUT_TEST)/test
	$(OUT_TEST)/test

# check that the wrapper works
test2: bumpalloc_wrapper bumpalloc
	gcc $(CFLAGS) $(SRC)/test2.c -o $(OUT_TEST2)/test2 $(OUT_LIB)/bumpalloc_wrapper.a $(OUT_LIB)/bumpalloc.a $(H)/atexit.h $(H)/bumpalloc.h $(H)/bumpalloc_wrapper.h  -L$(H)/atexit.a
	$(OUT_TEST2)/test2

# todo: export lib
export_lib:
	cp -r $(OUT_LIB)/* $(LIB_EXPORT)/
	cp -r $(H)/*.h $(LIB_EXPORT)/

# todo: test for mis-alignment tolerance