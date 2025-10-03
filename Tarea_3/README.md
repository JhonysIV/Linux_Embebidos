# Assignment Sensor - Servicio de Sensor Simulado

Un servicio de sistema Linux que simula un sensor de temperatura y registra datos de manera continua en archivos de log.

## 📋 Descripción

Este proyecto implementa un servicio systemd que simula un sensor de temperatura generando valores aleatorios entre 20.0°C y 30.0°C. Los datos se registran con timestamps en formato ISO-8601 UTC en archivos de log del sistema.

## ✨ Características

- **Simulación de sensor**: Genera valores de temperatura aleatorios entre 20.0°C y 30.0°C
- **Logging robusto**: Registra datos con timestamps precisos en formato ISO-8601 UTC
- **Configuración flexible**: Intervalo de muestreo configurable via línea de comandos
- **Manejo de señales**: Cierre ordenado con SIGTERM
- **Fallback de logging**: Intenta escribir en `/tmp/` y fallback a `/var/tmp/`
- **Servicio systemd**: Listo para ejecutarse como servicio del sistema

## 🏗️ Estructura del Proyecto

```
Tarea_3/
├── src/                    # Código fuente
│   ├── main.c             # Programa principal
│   ├── logger.c/h         # Módulo de logging
│   └── sensor_mock.c/h    # Simulador de sensor
├── systemd/               # Configuración de servicio
│   └── assignment-sensor.service
├── tests/                 # Scripts de prueba
│   ├── test.sh           # Suite completa de pruebas
│   └── demo.realtime.sh  # Demo en tiempo real
├── build/                # Archivos compilados (generado)
├── Makefile              # Configuración de compilación
└── README.md             # Este archivo
```

## 🚀 Instalación y Compilación

### Requisitos previos
- Sistema Linux con systemd
- Compilador GCC
- Make

### Compilación

```bash
# Compilar el proyecto
make

# Limpiar archivos compilados
make clean
```

El ejecutable se generará en `build/assignment-sensor`.

## 🏃‍♂️ Ejecución Rápida

### Comandos Esenciales

```bash
# 1. Compilar el proyecto
make clean && make

# 2. Ejecutar con intervalo por defecto (5 segundos)
./build/assignment-sensor

# 3. Ejecutar con intervalo personalizado (3 segundos)
./build/assignment-sensor --interval 3

# 4. Ver los logs generados
tail -f /tmp/sensor_log.txt

# 5. Detener el programa (si está ejecutándose)
# Presiona Ctrl+C o envía SIGTERM
```

### Ejemplo de Ejecución Completa

```bash
# Paso 1: Compilar
cd /home/jhonys/Linux/Linux_Embebidos/Tarea_3
make clean && make

# Paso 2: Ejecutar por 10 segundos y ver logs
timeout 10s ./build/assignment-sensor --interval 2 &
sleep 12 && tail -5 /tmp/sensor_log.txt

# Paso 3: Limpiar logs (opcional)
rm -f /tmp/sensor_log.txt
```

### Comandos de Verificación

```bash
# Verificar que compila sin errores
make clean && make

# Probar argumentos inválidos (debe mostrar error)
./build/assignment-sensor --interval -1

# Ejecutar por 15 segundos y verificar logs
timeout 15s ./build/assignment-sensor --interval 3
tail -10 /tmp/sensor_log.txt

# Verificar que el programa se cierra ordenadamente
echo "Programa ejecutándose... (Ctrl+C para detener)"
./build/assignment-sensor
```

## 🎯 Uso

### Ejecución manual

```bash
# Con intervalo por defecto (5 segundos)
./build/assignment-sensor

# Con intervalo personalizado
./build/assignment-sensor --interval 3

# Ejemplo de salida
Iniciando servicio de sensor. Log en /tmp/sensor_log.txt o /var/tmp/sensor_log.txt. Presione Ctrl+C para salir (si se ejecuta manualmente).
```

### Como servicio systemd

```bash
# Instalar el servicio
sudo cp build/assignment-sensor /usr/local/bin/
sudo cp systemd/assignment-sensor.service /etc/systemd/system/

# Habilitar y iniciar el servicio
sudo systemctl daemon-reload
sudo systemctl enable assignment-sensor
sudo systemctl start assignment-sensor

# Verificar estado
sudo systemctl status assignment-sensor

# Ver logs del servicio
sudo journalctl -u assignment-sensor -f

# Detener el servicio
sudo systemctl stop assignment-sensor
```

## 📊 Formato de Log

Los datos se registran en formato:
```
[2025-10-03T00:42:54Z] Sensor Value: 21.06
[2025-10-03T00:42:57Z] Sensor Value: 25.13
[2025-10-03T00:43:00Z] Sensor Value: 22.96
```

- **Timestamp**: Formato ISO-8601 UTC con 'Z' al final
- **Valores**: Números flotantes con 2 decimales
- **Ubicación**: `/tmp/sensor_log.txt` (fallback a `/var/tmp/sensor_log.txt`)

## 🧪 Pruebas

### Script de pruebas completo

```bash
# Ejecutar suite completa de pruebas
./tests/test.sh
```

Este script incluye:
- Monitoreo en tiempo real
- Pruebas de instalación/desinstalación
- Verificación de comportamiento de fallback
- Pruebas de cierre ordenado
- Validación de configuración

### Demo en tiempo real

```bash
# Ver logs en tiempo real por 10 segundos
./tests/demo.realtime.sh
```

## ⚙️ Configuración

### Parámetros de línea de comandos

| Parámetro | Descripción | Valor por defecto |
|-----------|-------------|-------------------|
| `--interval N` | Intervalo entre lecturas en segundos | 5 |

### Archivos de configuración

- **Servicio systemd**: `systemd/assignment-sensor.service`
- **Makefile**: Configuración de compilación

## 🔧 Desarrollo

### Estructura del código

- **`main.c`**: Punto de entrada, manejo de argumentos y señales
- **`logger.c/h`**: Módulo de logging con fallback automático
- **`sensor_mock.c/h`**: Simulador de sensor con generación de valores aleatorios

### Compilación con debug

```bash
# El Makefile ya incluye flags de debug (-g)
make clean && make
```

## 📝 Logs del Sistema

### Archivos de log del servicio

```bash
# Logs del servicio (systemd)
sudo journalctl -u assignment-sensor

# Logs de datos del sensor
tail -f /tmp/sensor_log.txt
```

### Rotación de logs

Para implementar rotación de logs, considera usar `logrotate`:

```bash
# Crear configuración de logrotate
sudo nano /etc/logrotate.d/assignment-sensor
```

## 🐛 Solución de problemas

### Problemas comunes

1. **No se puede escribir en /tmp**
   - Verificar permisos del directorio
   - El programa automáticamente intenta `/var/tmp/` como fallback

2. **Servicio no inicia**
   - Verificar que el ejecutable esté en `/usr/local/bin/`
   - Ejecutar `sudo systemctl daemon-reload`

3. **Compilación falla**
   - Verificar que GCC esté instalado
   - Ejecutar `make clean && make`

### Debug

```bash
# Ejecutar con debug
gdb ./build/assignment-sensor

# Ver logs detallados
strace -f ./build/assignment-sensor --interval 1
```

## 📄 Licencia

Este proyecto es parte de una tarea académica para el curso de Linux Embebidos.

## 👥 Contribuciones

Este es un proyecto educativo. Para mejoras o correcciones, por favor crear issues o pull requests.

---

**Nota**: Este servicio está diseñado para propósitos educativos y de demostración. Para uso en producción, considere implementar autenticación, cifrado de logs y monitoreo adicional.
