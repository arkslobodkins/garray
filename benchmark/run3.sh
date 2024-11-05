echo -e "BENCHMARK DYNAMIC ARRAYS\n\n"
./array_gcc.x
echo -e ""
./array_icpx.x
echo -e ""
./array_clang.x

echo -e "\n\nBENCHMARK FIXED-SIZE ARRAYS\n\n"
./fixedarray_gcc.x
echo -e ""
./fixedarray_icpx.x
echo -e ""
./fixedarray_clang.x

make clean
