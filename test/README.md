# Tests

## Compilar (des de /Users/toni.tassani/code/marc/compus-fase1):

gcc -Wall -Wextra -Itest -Itest/lib -I. \
  TAD_TIMER.c TAD_INTENSITY.c \
  test/lib/mock_hw.c test/test_main.c \
  -o test/bin/test_runner

## Executar:

./test/bin/test_runner

## Simular main.c (des de /Users/toni.tassani/code/marc/compus-fase1):

gcc -Wall -Wextra -Itest -Itest/lib -I. \
  main.c TAD_TIMER.c TAD_INTENSITY.c TAD_SERIAL.c TAD_TECLAT.c TAD_CONTROLLER.c TAD_LEDS.c TAD_HALL.c TAD_SPEAKER.c \
  test/lib/mock_hw.c test/sim_main.c \
  -o test/bin/sim_main

## Executar simulació:

./test/bin/sim_main