#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

/* Definición única de la variable extern declarada en sensor.h */
int sensor_has_csv = 0;

/* Datos internos (definidos solo en este archivo) */
static double *values = NULL;
static size_t values_count = 0;
static size_t values_idx = 0;

void sensor_init(void) {
    const char *csv_path = "sensor_feed.csv";
    FILE *f = fopen(csv_path, "r");
    if (!f) {
        /* No hay CSV: modo aleatorio */
        sensor_has_csv = 0;
        srand((unsigned)time(NULL));
        return;
    }

    char line[256];
    size_t cap = 0;
    values_count = 0;

    while (fgets(line, sizeof(line), f)) {
        /* trim inicial */
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0' || *p == '\n' || *p == '\r' || *p == '#') continue;

        char *endptr = NULL;
        double v = strtod(p, &endptr);
        if (p == endptr) continue; /* no number parsed */

        if (values_count >= cap) {
            size_t newcap = cap ? cap * 2 : 16;
            double *tmp = realloc(values, newcap * sizeof(double));
            if (!tmp) {
                perror("realloc");
                break;
            }
            values = tmp;
            cap = newcap;
        }
        values[values_count++] = v;
    }

    fclose(f);

    if (values_count == 0) {
        /* Falla la carga: modo aleatorio */
        free(values);
        values = NULL;
        sensor_has_csv = 0;
        srand((unsigned)time(NULL));
    } else {
        /* Si se cambia el csv a 0, 
        se le obliga a que entre siempre al modoa laeatorio*/
        sensor_has_csv = 1;
        values_idx = 0;
    }
}

double sensor_read(void) {
    if (sensor_has_csv && values_count > 0) {
        double v = values[values_idx];
        values_idx = (values_idx + 1) % values_count;
        return v;
    } else {
        /* Valor aleatorio en [0,100) */
        return (double)rand() / (double)RAND_MAX * 100.0;
    }
}

void sensor_close(void) {
    free(values);
    values = NULL;
    values_count = 0;
    values_idx = 0;
    sensor_has_csv = 0;
}