# Simple System Monitor (C)

Lee periódicamente `/proc/stat`, `/proc/cpuinfo` y `/proc/meminfo` y muestra:
- Memoria instalada y usada (física y swap).
- Tipo de procesador.
- Número de núcleos lógicos.
- Carga del procesador por núcleo (cada 2 segundos).

## Compilar
```bash
make
