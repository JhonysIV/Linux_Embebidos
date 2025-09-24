1. Estructura inicial del proyecto

Prompt:
"Hola, me podrías ayudar a crear un repositorio en GitHub y organizar las carpetas para el proyecto de sistemas embebidos."

Answer (resumen):
La IA sugirió una estructura inicial con directorios sensor/, actuators/, controller/, tests/ y un Makefile.
También explicó cómo usar tree en Linux para visualizar el proyecto.

Reflexión:
Esto me ayudó a partir con un esqueleto claro y organizado, asegurando modularidad desde el inicio.

2. Creación del sensor (Ejercicio 1)

Prompt:
"Empecemos con el ejercicio 1 paso a paso."

Answer (resumen):
Se creó sensor.h con include guards y extern, y sensor.c con funciones sensor_init() y sensor_read().
Se discutió la diferencia entre declaración en headers y definición en fuentes, para evitar redefinitions.

Reflexión:
Aprendí a separar declaración/definición correctamente, evitando errores típicos de compilación.

3. Interfaz de actuadores (Ejercicio 2)

Prompt:
"Listo tenemos todo esto funcionando, vamos a hacer el ejercicio 2."

Answer (resumen):
Se diseñó actuator.h con void *params y punteros a funciones (activate, deactivate, status).
Se implementaron led_actuator.c y buzzer_actuator.c, demostrando polimorfismo.

Reflexión:
Comprendí cómo usar punteros a funciones en C para lograr interfaces polimórficas.

4. Controlador en lazo cerrado (Ejercicio 3)

Prompt:
"Listo tenemos el ejercicio 2, le modifiqué algunas cosas, ahora sigamos con el 3. ¿Qué me hace falta para completar el ejercicio 3?"

Answer (resumen):
La IA explicó que faltaba integrar:

Bucle de muestreo cada 100 ms

Activación inmediata al superar el umbral

Apagado diferido (1 s buzzer, 5 s LED)

Logs con tiempo monotónico

También aclaró que ctl.c podía ser el main.

Reflexión:
Esto me dio una hoja de ruta clara. Luego implementé el controlador completo con clock_gettime, timers, señales, y lo revisamos juntos.

5. Makefile y compilación 64/32 bits

Prompt:
"¿No es necesario un archivo Makefile para que pueda funcionar todo?"

Answer (resumen):
La IA confirmó que sí, y sugirió un Makefile con targets ctl64, ctl32 y clean.
Se explicó cómo usar -m32 y qué dependencias instalar.
