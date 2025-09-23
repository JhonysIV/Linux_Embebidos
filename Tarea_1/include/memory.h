// Guardia de inclusión para evitar múltiples definiciones del archivo
#ifndef MEMORY_H
// Define el identificador de la guardia de inclusión
#define MEMORY_H

/* Función para leer información de memoria desde /proc/meminfo
   Lee /proc/meminfo y devuelve (en kB) : mem total, mem available (o fallback a MemFree),
   swap total y swap free. Devuelve 0 si ok */
// Parámetros de entrada (punteros donde se guardarán los valores):
// mem_total_kb - puntero donde se guardará la memoria total en KB
// mem_available_kb - puntero donde se guardará la memoria disponible en KB
// swap_total_kb - puntero donde se guardará el swap total en KB  
// swap_free_kb - puntero donde se guardará el swap libre en KB
// Retorna: 0 si la operación fue exitosa, -1 si hubo error
int read_meminfo(unsigned long long *mem_total_kb, unsigned long long *mem_available_kb,
                 unsigned long long *swap_total_kb, unsigned long long *swap_free_kb);

// Fin de la guardia de inclusión
#endif // MEMORY_H
