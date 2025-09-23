// Incluye el archivo de cabecera correspondiente para las declaraciones de CPU
#include "cpu.h"
// Incluye librería estándar para funciones de entrada/salida (fopen, fgets, etc.)
#include <stdio.h>
// Incluye librería estándar para funciones de memoria dinámica (malloc, free, realloc)
#include <stdlib.h>
// Incluye librería estándar para funciones de manipulación de cadenas (strncmp, strchr, strncpy)
#include <string.h>

// Función para leer el modelo de CPU desde /proc/cpuinfo
int read_cpu_model(char *model, size_t len) {
    // Abre el archivo /proc/cpuinfo en modo lectura
    FILE *fp = fopen("/proc/cpuinfo", "r");
    // Verifica si el archivo se abrió correctamente, retorna -1 si hay error
    if (!fp) return -1;
    // Buffer para leer líneas del archivo
    char line[512];
    // Inicializa el primer carácter del modelo como terminador nulo
    model[0] = '\0';
    // Lee línea por línea del archivo hasta el final
    while (fgets(line, sizeof(line), fp)) {
        // Busca la línea que contiene "model name" y verifica que el modelo esté vacío
        if (strncmp(line, "model name", 10) == 0 && model[0] == '\0') {
            // Encuentra la posición de los dos puntos en la línea
            char *p = strchr(line, ':');
            // Si encuentra los dos puntos, procesa la línea
            if (p) {
                // Avanza el puntero después de los dos puntos
                p++;
                // Salta espacios y tabulaciones después de los dos puntos
                while (*p == ' ' || *p == '\t') p++;
                // Copia el modelo al buffer proporcionado, limitando la longitud
                strncpy(model, p, len - 1);
                // Asegura terminación nula del string
                model[len - 1] = '\0';
                // Encuentra el salto de línea en el modelo copiado
                char *nl = strchr(model, '\n');
                // Si encuentra salto de línea, lo elimina
                if (nl) *nl = '\0';
                // Sale del bucle una vez encontrado el modelo
                break;
            }
        }
    }
    // Cierra el archivo
    fclose(fp);
    // Retorna 0 indicando éxito
    return 0;
}

// Función para leer estadísticas de CPU desde /proc/stat
int read_proc_stat(CPUCore **cores, int *count) {
    // Abre el archivo /proc/stat en modo lectura
    FILE *fp = fopen("/proc/stat", "r");
    // Verifica si el archivo se abrió correctamente, retorna -1 si hay error
    if (!fp) return -1;
    // Buffer para leer líneas del archivo
    char line[512];
    // Puntero al array dinámico de núcleos de CPU
    CPUCore *arr = NULL;
    // Contador de núcleos encontrados
    int n = 0;

    // Lee línea por línea del archivo hasta el final
    while (fgets(line, sizeof(line), fp)) {
        // Buffer para el nombre del núcleo
        char name[16];
        // Variables para almacenar las estadísticas de tiempo de CPU
        unsigned long long user=0, nice=0, system=0, idle=0, iowait=0, irq=0, softirq=0, steal=0, guest=0, guest_nice=0;
        // Parsea la línea para extraer las estadísticas del núcleo
        int fields = sscanf(line, "%15s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                            name,
                            &user,&nice,&system,&idle,&iowait,&irq,&softirq,&steal,&guest,&guest_nice);
        // Si no se parsearon al menos 5 campos, continúa con la siguiente línea
        if (fields < 5) continue; // no es línea CPU válida
        // Verifica que el nombre comience con "cpu"
        if (strncmp(name, "cpu", 3) != 0) continue;
        // Omite la línea agregada "cpu" (solo queremos cpu0, cpu1,...)
        if (name[3] == '\0') continue; // omitir la línea agregada "cpu" (solo queremos cpu0, cpu1,...)

        // Crea una estructura temporal para el núcleo
        CPUCore tmp;
        // Copia el nombre del núcleo a la estructura temporal
        strncpy(tmp.name, name, sizeof(tmp.name)-1);
        // Asegura terminación nula del nombre
        tmp.name[sizeof(tmp.name)-1] = '\0';
        // Asigna las estadísticas de tiempo a la estructura temporal
        tmp.user = user;
        tmp.nice = nice;
        tmp.system = system;
        tmp.idle = idle;
        tmp.iowait = iowait;
        tmp.irq = irq;
        tmp.softirq = softirq;
        tmp.steal = steal;
        tmp.guest = guest;
        tmp.guest_nice = guest_nice;
        // Calcula el tiempo total de inactividad (idle + iowait)
        tmp.idle_all = idle + iowait;
        // Calcula el tiempo total sumando todos los componentes
        tmp.total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;

        // Redimensiona el array dinámico para agregar un nuevo núcleo
        CPUCore *tmp_arr = realloc(arr, sizeof(CPUCore) * (n + 1));
        // Si la reasignación falla, libera memoria, cierra archivo y retorna error
        if (!tmp_arr) { free(arr); fclose(fp); return -1; }
        // Actualiza el puntero al array redimensionado
        arr = tmp_arr;
        // Agrega el núcleo temporal al array
        arr[n] = tmp;
        // Incrementa el contador de núcleos
        n++;
    }

    // Cierra el archivo
    fclose(fp);
    // Asigna el array de núcleos al puntero de salida
    *cores = arr;
    // Asigna el número de núcleos al contador de salida
    *count = n;
    // Retorna 0 indicando éxito
    return 0;
}

// Función para liberar memoria asignada por read_proc_stat
void free_cpu_cores(CPUCore *cores) {
    // Libera la memoria asignada dinámicamente para el array de núcleos
    free(cores);
}