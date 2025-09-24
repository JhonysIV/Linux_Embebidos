#🖥️ Embedded Systems Programming – Controlador de Lazo Cerrado
#📌 Descripción del Proyecto

Este proyecto es una asignación de programación de sistemas embebidos que explora conceptos fundamentales de C, como:

Modularidad del código

Proceso de compilación y enlace

Diseño de interfaces polimórficas

El resultado es un controlador de lazo cerrado que interactúa con un sensor simulado y dos actuadores (LED y Buzzer), tomando decisiones en tiempo real basadas en un umbral configurable.

Etapas de desarrollo

Biblioteca del Sensor → simula lecturas con valores aleatorios o desde un CSV.

Interfaz de Actuadores Polimórfica → interfaz genérica Actuator con backends LED y Buzzer.

Controlador de Lazo Cerrado → integra sensor y actuadores con lógica basada en umbrales y temporización.

#📂 Estructura del Repositorio

```text
Tarea_2/
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
```




#⚙️ Instrucciones de Compilación y Ejecución
#🔨 Compilación

El proyecto requiere gcc y make. Para el binario de 32 bits se necesitan las herramientas de compilación cruzada (gcc-multilib).

# Compilar binario de 64 bits (por defecto)
make ctl64

# Compilar binario de 32 bits
make ctl32

# Limpiar compilación
make clean

#▶️ Ejecución

Por defecto el umbral es 50.0. Puede configurarse con la variable de entorno CTL_THRESHOLD.

# Ejecutar con umbral por defecto (50.0)
./controller/ctl64

# Ejecutar con umbral personalizado (ej: 80.0)
CTL_THRESHOLD=80.0 ./controller/ctl64

#📊 Log de Ejecución (ejemplo)

Cuando el valor del sensor supera el umbral → LED y buzzer se activan inmediatamente.
Cuando baja del umbral → buzzer se apaga tras 1 s y LED tras 5 s.

$ ./controller/ctl64
```text
[ 0.000000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
[ 0.100000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
...
[ 1.000000000] value=20.000 threshold=50.000 LED=1 Buzzer=1
[Buzzer] Buzzer1 apagado 🔇
[ 1.100000000] value=20.000 threshold=50.000 LED=1 Buzzer=0
...
[ 6.000000000] value=60.000 threshold=50.000 LED=1 Buzzer=1
```

#🔍 Inspección de Binarios

El Makefile genera binarios en 64 bits y 32 bits.

file
$ file controller/ctl64
controller/ctl64: ELF 64-bit LSB executable, x86-64, dynamically linked

$ file controller/ctl32
controller/ctl32: ELF 32-bit LSB executable, Intel 80386, dynamically linked

readelf -h
$ readelf -h controller/ctl64
Class:     ELF64
Machine:   Advanced Micro Devices X86-64

$ readelf -h controller/ctl32
Class:     ELF32
Machine:   Intel 80386

#💡 Reflexión sobre los Conceptos
#🔹 Compilación vs. Enlace

Compilación: cada archivo .c se convierte en .o (verificación sintáctica/semántica).

Enlace: el linker combina todos los .o y librerías para formar el ejecutable.

Errores típicos:

Compilación → función no declarada.

Enlace → función declarada pero no definida.

El Makefile gestiona ambos pasos automáticamente.

🔹 Interfaces y Wrappers

El diseño de la interfaz polimórfica (actuator.h) demuestra cómo un wrapper desacopla el controlador de implementaciones concretas:

El controlador no necesita llamar a led_activate() o buzzer_activate().

Solo invoca funciones genéricas (activate, deactivate, status) definidas en Actuator.

Esto facilita pruebas, mantenimiento y la integración de nuevos actuadores en el futuro.