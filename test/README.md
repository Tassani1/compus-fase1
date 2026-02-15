# Tests

## Compilar (des de /Users/toni.tassani/code/marc/compus-fase1):

gcc -Wall -Wextra -Itest -I. \
  TAD_TIMER.c TAD_INTENSITY.c \
  test/mock_hw.c test/test_main.c \
  -o test/test_runner

## Executar:

./test/test_runner