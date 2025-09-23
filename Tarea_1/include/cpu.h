// Guardia de inclusión para evitar múltiples definiciones
#ifndef CPU_H
#define CPU_H

// Incluye la librería estándar para tipos de tamaño específico
#include <stddef.h>

// Estructura que representa un núcleo de CPU con sus estadísticas
typedef struct {
    char name[16];                    // Nombre del núcleo (ej: "cpu0", "cpu1", etc.)
    unsigned long long user;          // Tiempo gastado en procesos de usuario
    unsigned long long nice;          // Tiempo gastado en procesos de usuario con prioridad nice
    unsigned long long system;        // Tiempo gastado en procesos del sistema
    unsigned long long idle;          // Tiempo de inactividad
    unsigned long long iowait;        // Tiempo esperando operaciones de E/S
    unsigned long long irq;           // Tiempo manejando interrupciones hardware
    unsigned long long softirq;       // Tiempo manejando interrupciones software
    unsigned long long steal;         // Tiempo "robado" por hipervisor (virtualización)
    unsigned long long guest;         // Tiempo ejecutando guest OS
    unsigned long long guest_nice;    // Tiempo ejecutando guest OS con prioridad nice
    unsigned long long total;         // Suma total de todos los tiempos
    unsigned long long idle_all;      // Suma de idle + iowait (tiempo no productivo)
} CPUCore;

// Función para leer el modelo de CPU desde /proc/cpuinfo
// Parámetros: model - buffer donde guardar el nombre, len - tamaño del buffer
// Retorna: 0 si es exitoso, -1 si hay error
int read_cpu_model(char *model, size_t len);

// Función para leer estadísticas de CPU desde /proc/stat
// Parámetros: cores - puntero a array de CPUCore (se asigna dinámicamente)
//            count - puntero donde se guarda la cantidad de núcleos
// Retorna: 0 si es exitoso, -1 si hay error
// Nota: El array devuelto debe liberarse con free_cpu_cores()
int read_proc_stat(CPUCore **cores, int *count);

// Función para liberar memoria asignada por read_proc_stat
// Parámetros: cores - puntero al array de CPUCore a liberar
void free_cpu_cores(CPUCore *cores);

// Fin de la guardia de inclusión
#endif // CPU_H