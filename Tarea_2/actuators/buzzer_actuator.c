#include <stdio.h>
#include <stdlib.h>
#include "actuator.h"

/* Estructura privada para los parámetros del buzzer */
typedef struct {
    int state;  // 0=apagado, 1=sonando
    const char *name;
} BuzzerParams;

static void buzzer_activate(void *p) {
    BuzzerParams *bz = (BuzzerParams *)p;
    bz->state = 1;
    printf("[Buzzer] %s sonando 🔊\n", bz->name);
}

static void buzzer_deactivate(void *p) {
    BuzzerParams *bz = (BuzzerParams *)p;
    bz->state = 0;
    printf("[Buzzer] %s apagado 🔇\n", bz->name);
}

static int buzzer_status(void *p) {
    BuzzerParams *bz = (BuzzerParams *)p;
    return bz->state;
}

/* Función fábrica: crea un Actuator configurado como Buzzer */
Actuator *create_buzzer(const char *name) {
    Actuator *a = malloc(sizeof(Actuator));
    BuzzerParams *params = malloc(sizeof(BuzzerParams));
    params->state = 0;
    params->name = name;

    a->params = params;
    a->activate = buzzer_activate;
    a->deactivate = buzzer_deactivate;
    a->status = buzzer_status;

    return a;
}

/* Liberar memoria */
void destroy_buzzer(Actuator *a) {
    if (!a) return;
    free(a->params);
    free(a);
}
