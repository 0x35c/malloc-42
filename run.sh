make re > /dev/null && gcc main.c -g -L. -lft_malloc
valgrind --track-origins=yes --leak-check=full ./a.out
