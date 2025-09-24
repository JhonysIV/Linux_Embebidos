#include <stdio.h>
#include <stdlib.h>
#include "actuator.h"

/* Estructura privada para los parámetros del LED */
typedef struct {
    int state;  // 0=apagado, 1=encendido
    const char *name;
} LedParams;

static void led_activate(void *p) {
    LedParams *led = (LedParams *)p;
    led->state = 1;
    printf("[LED] %s encendido ✅\n", led->name);
}

static void led_deactivate(void *p) {
    LedParams *led = (LedParams *)p;
    led->state = 0;
    printf("[LED] %s apagado ❌\n", led->name);
}

static int led_status(void *p) {
    LedParams *led = (LedParams *)p;
    return led->state;
}

/* Función fábrica: crea un Actuator configurado como LED */
Actuator *create_led(const char *name) {
    Actuator *a = malloc(sizeof(Actuator));
    LedParams *params = malloc(sizeof(LedParams));
    params->state = 0;
    params->name = name;

    a->params = params;
    a->activate = led_activate;
    a->deactivate = led_deactivate;
    a->status = led_status;

    return a;
}

/* Liberar memoria */
void destroy_led(Actuator *a) {
    if (!a) return;
    free(a->params);
    free(a);
}
