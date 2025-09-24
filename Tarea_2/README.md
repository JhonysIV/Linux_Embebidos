Embedded Systems Programming – Controlador de Lazo Cerrado

Descripción del Proyecto

Este proyecto es una asignación de programación de sistemas embebidos que explora conceptos fundamentales de C, como la modularidad, el proceso de compilación y enlace, y el diseño de interfaces polimórficas. El resultado es un controlador de lazo cerrado que interactúa con un sensor simulado y dos actuadores (un LED y un zumbador) para tomar decisiones en tiempo real basadas en un umbral.

El proyecto se desarrolló en tres etapas principales:
Biblioteca del Sensor: Una biblioteca modular que simula la lectura de un sensor, ya sea a partir de valores aleatorios o de un archivo CSV.
Interfaz de Actuadores Polimórfica: Una interfaz genérica (Actuator) que permite controlar diferentes dispositivos (LED y Buzzer) con el mismo conjunto de funciones.

Controlador de Lazo Cerrado: Un programa principal que integra el sensor y los actuadores para implementar una lógica de control basada en umbrales y temporización.

Estructura del Repositorio

Tarea_2 /
├── sensor_feed.csv
├── Makefile
├── README.md
├── ai_log.md
├── sensor/
│   ├── sensor.h
│   └── sensor.c
├── actuators/
│   ├── actuator.h
│   ├── led_actuator.c
│   └── buzzer_actuator.c
└── controller/
    └── ctl.c


Instrucciones de Compilación y Ejecución

Para compilar y ejecutar el proyecto, se requiere gcc y un sistema compatible con make. Es posible que necesite instalar las herramientas de compilación cruzada de 32 bits (gcc-multilib) para el objetivo ctl32.
Compilación

El Makefile incluye varios objetivos:

make ctl64: Compila el controlador principal (ctl.c) como un binario de 64 bits. Este es el objetivo por defecto.

make ctl32: Compila el controlador principal como un binario de 32 bits.

make clean: Limpia todos los archivos binarios y objeto generados.

# Compilar el binario de 64 bits
make ctl64

# Compilar el binario de 32 bits (requiere herramientas adicionales)
make ctl32

# Limpiar los archivos de compilación
make clean


Ejecución

El programa principal del controlador se encuentra en controller/ctl64 y controller/ctl32. Por defecto, el umbral es 50.0. Se puede modificar el umbral mediante la variable de entorno CTL_THRESHOLD.

# Ejecutar con el umbral por defecto (50.0)
./controller/ctl64

# Ejecutar con un umbral personalizado (e.g., 80.0)
CTL_THRESHOLD=80.0 ./controller/ctl64


Log de Ejecución

A continuación, se muestra un log de ejecución del controlador (usando el umbral por defecto de 50.0) cuando lee los datos del archivo sensor_feed.csv.
Valores iniciales (50): El controlador enciende el LED y el zumbador inmediatamente.

Valores bajos (\<50): Los actuadores se apagan, pero con un retraso: el zumbador después de 1 segundo y el LED después de 5 segundos.

Reactivación (50): Al volver a superar el umbral, los actuadores se encienden de nuevo, cancelando los temporizadores de apagado pendientes.

$ ./controller/ctl64
[ 0.000000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.100000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.200000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.300000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.400000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.500000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.600000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.700000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.800000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.900000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 1.000000000] value=20.000 threshold=50.000 LED=1 Buzzer=1
[Buzzer] Buzzer1 apagado 🔇
[ 1.100000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
[ 1.200000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
[ 1.300000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
[ 1.400000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
[ 1.500000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
[ 1.600000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
...
[ 5.900000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
[ 6.000000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
[ 6.000000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 6.100000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
...


Inspección de Binarios

El Makefile permite la compilación de 64 bits y 32 bits, lo que es útil para la portabilidad. Podemos verificar las diferencias en la arquitectura con las herramientas de file y readelf.
file
La salida del comando file confirma la arquitectura del binario.
$ file controller/ctl64
controller/ctl64: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, ...

$ file controller/ctl32
controller/ctl32: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked, ...


readelf

El comando readelf -h muestra los encabezados del archivo ELF. Las principales diferencias se ven en los campos Class (clase de arquitectura) y Machine (tipo de procesador).

$ readelf -h controller/ctl64
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  ...
  Machine:                           Advanced Micro Devices X86-64

$ readelf -h controller/ctl32
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF32
  Data:                              2's complement, little endian
  ...
  Machine:                           Intel 80386


Reflexión sobre los Conceptos

Compilación vs. Enlace

El proceso de construcción de este proyecto ilustra la diferencia entre la compilación y el enlace.

Compilación: El compilador (gcc) toma un archivo fuente (.c) y lo traduce a un archivo objeto (.o). En esta etapa, el compilador verifica la sintaxis y la semántica del código, basándose en las declaraciones de los encabezados (.h). Un error de compilación (como una función no declarada) se detecta aquí.

Enlace (Linking): El enlazador toma todos los archivos objeto (.o) y las librerías, resolviendo las referencias a las definiciones de las funciones y variables globales. Es aquí donde se encuentra la implementación real. Un error de enlace (como una función declarada pero no definida) ocurre si el enlazador no puede encontrar la implementación.

El Makefile en este proyecto maneja ambos pasos de forma automática, compilando cada archivo .c por separado y luego enlazándolos para crear el ejecutable final.

Interfaces y Wrappers

El diseño de la interfaz polimórfica para los actuadores (actuator.h) es un excelente ejemplo de un wrapper o envoltorio. En lugar de interactuar directamente con funciones específicas para el LED (led_activate, led_deactivate) o el zumbador (buzzer_activate, buzzer_deactivate), el controlador solo necesita interactuar con la estructura genérica Actuator y sus punteros a funciones. Esto logra un alto grado de desacoplamiento entre el controlador y la implementación de los actuadores, lo que facilita el mantenimiento, la prueba y la adición de nuevos tipos de actuadores en el futuro.
