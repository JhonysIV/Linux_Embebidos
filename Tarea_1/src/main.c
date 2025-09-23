// Incluye librería estándar para funciones de entrada/salida (printf, perror, etc.)
#include <stdio.h>
// Incluye librería estándar para funciones de memoria dinámica (free, etc.)
#include <stdlib.h>
// Incluye librería estándar para funciones de manipulación de cadenas (strcpy, etc.)
#include <string.h>
// Incluye librería estándar para funciones del sistema (sleep, etc.)
#include <unistd.h>
// Incluye el archivo de cabecera para funciones y estructuras de CPU
#include "cpu.h"
// Incluye el archivo de cabecera para funciones y estructuras de memoria
#include "memory.h"

// Función principal del programa
int main(void) {
    // Buffer para almacenar el modelo de CPU
    char cpu_model[256];
    // Intenta leer el modelo de CPU, si falla asigna "Unknown"
    if (read_cpu_model(cpu_model, sizeof(cpu_model)) != 0) {
        // Copia "Unknown" al buffer si no se pudo leer el modelo
        strcpy(cpu_model, "Unknown");
    }

    // Puntero para almacenar la muestra anterior de núcleos de CPU
    CPUCore *prev = NULL;
    // Contador de núcleos de la muestra anterior
    int prev_count = 0;

    // Bucle principal del monitor (se ejecuta indefinidamente)
    while (1) {
        // Puntero para almacenar la muestra actual de núcleos de CPU
        CPUCore *curr = NULL;
        // Contador de núcleos de la muestra actual
        int curr_count = 0;
        // Intenta leer las estadísticas actuales de CPU
        if (read_proc_stat(&curr, &curr_count) != 0) {
            // Si hay error, imprime mensaje y espera 2 segundos antes de continuar
            perror("read_proc_stat");
            sleep(2);
            continue;
        }

        // Variables para almacenar información de memoria
        unsigned long long mem_total_kb=0, mem_available_kb=0, swap_total_kb=0, swap_free_kb=0;
        // Intenta leer la información de memoria
        if (read_meminfo(&mem_total_kb, &mem_available_kb, &swap_total_kb, &swap_free_kb) != 0) {
            // Si hay error, imprime mensaje de error
            perror("read_meminfo");
        }

        /* Limpia la pantalla (interfaz de usuario simple) */
        // Secuencia de escape ANSI para limpiar la pantalla y mover cursor al inicio
        printf("\033[H\033[J");
        // Imprime encabezado del monitor
        printf("=== Simple system monitor ===\n\n");
        // Imprime el modelo de CPU
        printf("CPU Model: %s\n", cpu_model);
        // Imprime el número de núcleos lógicos detectados
        printf("Logical cores detected: %d\n\n", curr_count);

        // Convierte memoria total de KB a MB
        double mem_total_mb = mem_total_kb / 1024.0;
        // Calcula memoria usada en MB (total - disponible)
        double mem_used_mb = (double)(mem_total_kb - mem_available_kb) / 1024.0;
        // Convierte swap total de KB a MB
        double swap_total_mb = swap_total_kb / 1024.0;
        // Calcula swap usada en MB (total - libre)
        double swap_used_mb = (double)(swap_total_kb - swap_free_kb) / 1024.0;

        // Imprime información de memoria en MB
        printf("Memory: total: %.2f MB, used: %.2f MB\n", mem_total_mb, mem_used_mb);
        // Imprime información de swap en MB
        printf("Swap:   total: %.2f MB, used: %.2f MB\n\n", swap_total_mb, swap_used_mb);

        // Verifica si hay muestra anterior y el número de núcleos coincide
        if (prev != NULL && prev_count == curr_count) {
            // Imprime encabezado para el uso de CPU por núcleo
            printf("CPU usage per core:\n");
            // Itera sobre cada núcleo para calcular el uso
            for (int i = 0; i < curr_count; i++) {
                // Obtiene el tiempo total de la muestra anterior
                unsigned long long prev_total = prev[i].total;
                // Obtiene el tiempo inactivo de la muestra anterior
                unsigned long long prev_idle = prev[i].idle_all;
                // Obtiene el tiempo total de la muestra actual
                unsigned long long curr_total = curr[i].total;
                // Obtiene el tiempo inactivo de la muestra actual
                unsigned long long curr_idle = curr[i].idle_all;

                // Calcula la diferencia de tiempo total entre muestras
                unsigned long long total_delta = curr_total - prev_total;
                // Calcula la diferencia de tiempo inactivo entre muestras
                unsigned long long idle_delta = curr_idle - prev_idle;

                // Inicializa el porcentaje de uso
                double usage = 0.0;
                // Calcula el porcentaje de uso si hay diferencia de tiempo
                if (total_delta != 0) {
                    // Fórmula: (tiempo activo / tiempo total) * 100
                    usage = (double)(total_delta - idle_delta) / (double)total_delta * 100.0;
                }
                // Imprime el nombre del núcleo y su porcentaje de uso
                printf("%-6s : %6.2f %%\n", curr[i].name, usage);
            }
        } else {
            // Si no hay muestra anterior, muestra mensaje de cálculo
            printf("Calculating CPU usage (need two samples)...\n");
            // Itera sobre cada núcleo mostrando "--" como placeholder
            for (int i = 0; i < curr_count; i++) {
                // Imprime el nombre del núcleo con "--" como uso
                printf("%-6s : --\n", curr[i].name);
            }
        }

        /* Establece la muestra actual como anterior (liberamos la anterior) */
        // Si hay muestra anterior, libera su memoria
        if (prev) free_cpu_cores(prev);
        // Asigna la muestra actual como anterior
        prev = curr;
        // Asigna el contador actual como anterior
        prev_count = curr_count;

        // Fuerza la escritura de la salida al terminal
        fflush(stdout);
        // Espera 2 segundos antes de la siguiente iteración
        sleep(2);
    }

    // Libera la memoria de la muestra anterior (código inalcanzable en este bucle infinito)
    if (prev) free_cpu_cores(prev);
    // Retorna 0 indicando éxito (código inalcanzable en este bucle infinito)
    return 0;
}