// Incluye el archivo de cabecera correspondiente para las declaraciones de memoria
#include "memory.h"
// Incluye librería estándar para funciones de entrada/salida (fopen, fgets, sscanf, etc.)
#include <stdio.h>
// Incluye librería estándar para funciones de manipulación de cadenas (strcmp, etc.)
#include <string.h>

// Función para leer información de memoria desde /proc/meminfo
int read_meminfo(unsigned long long *mem_total_kb, unsigned long long *mem_available_kb,
                 unsigned long long *swap_total_kb, unsigned long long *swap_free_kb) {
    // Abre el archivo /proc/meminfo en modo lectura
    FILE *fp = fopen("/proc/meminfo", "r");
    // Verifica si el archivo se abrió correctamente, retorna -1 si hay error
    if (!fp) return -1;
    // Buffer para leer líneas del archivo
    char line[256];
    // Variables para almacenar los valores de memoria leídos del archivo
    unsigned long long memTotal=0, memAvailable=0, memFree=0, swapTotal=0, swapFree=0;

    // Lee línea por línea del archivo hasta el final
    while (fgets(line, sizeof(line), fp)) {
        // Buffer para almacenar la clave (nombre del campo)
        char key[64];
        // Variable para almacenar el valor numérico
        unsigned long long val;
        // Parsea la línea para extraer clave y valor
        if (sscanf(line, "%63s %llu", key, &val) < 2) continue;
        // Compara la clave con "MemTotal:" y asigna el valor si coincide
        if (strcmp(key, "MemTotal:") == 0) memTotal = val;
        // Compara la clave con "MemAvailable:" y asigna el valor si coincide
        else if (strcmp(key, "MemAvailable:") == 0) memAvailable = val;
        // Compara la clave con "MemFree:" y asigna el valor si coincide
        else if (strcmp(key, "MemFree:") == 0) memFree = val;
        // Compara la clave con "SwapTotal:" y asigna el valor si coincide
        else if (strcmp(key, "SwapTotal:") == 0) swapTotal = val;
        // Compara la clave con "SwapFree:" y asigna el valor si coincide
        else if (strcmp(key, "SwapFree:") == 0) swapFree = val;
    }

    // Cierra el archivo
    fclose(fp);
    // Si no hay MemAvailable, usa MemFree como fallback
    if (memAvailable == 0) memAvailable = memFree; // fallback si no hay MemAvailable

    // Asigna los valores a los punteros de salida si no son NULL
    if (mem_total_kb) *mem_total_kb = memTotal;
    if (mem_available_kb) *mem_available_kb = memAvailable;
    if (swap_total_kb) *swap_total_kb = swapTotal;
    if (swap_free_kb) *swap_free_kb = swapFree;
    // Retorna 0 indicando éxito
    return 0;
}