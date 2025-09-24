# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I. -Iactuators

# Archivos
SENSOR_SRC = sensor/sensor.c
TEST_SENSOR_SRC = tests/test_sensor.c

## Solo controlador

.PHONY: all clean

all: ctl64

ACTUATORS_SRC = actuators/led_actuator.c actuators/buzzer_actuator.c
ACTUATORS_OBJ = actuators/led_actuator.o actuators/buzzer_actuator.o

# Limpiar
clean:
	rm -f controller/ctl64 controller/ctl32 $(ACTUATORS_OBJ)

# Controller targets (64-bit y 32-bit)
.PHONY: ctl64 ctl32

CTL_SRC = controller/ctl.c $(SENSOR_SRC) $(ACTUATORS_OBJ)

ctl64: $(CTL_SRC)
	$(CC) $(CFLAGS) -m64 -I. -Iactuators $(CTL_SRC) -o controller/ctl64

# Para 32 bits, compilar y enlazar directamente desde los .c para asegurar -m32
ctl32:
	$(CC) $(CFLAGS) -m32 -I. -Iactuators controller/ctl.c sensor/sensor.c $(ACTUATORS_SRC) -o controller/ctl32

actuators/%.o: actuators/%.c actuators/actuator.h
	$(CC) $(CFLAGS) -I. -c $< -o $@