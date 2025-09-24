#ifndef ACTUATOR_H
#define ACTUATOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Estado genérico de un actuador */
typedef struct {
    void *params;                                // parámetros específicos del actuador
    void (*activate)(void *params);              // función para activar
    void (*deactivate)(void *params);            // función para desactivar
    int  (*status)(void *params);                // obtener estado (0=off, 1=on)
} Actuator;

/* Fábricas y destructores (útiles para pruebas y controlador) */
Actuator *create_led(const char *name);
void destroy_led(Actuator *a);
Actuator *create_buzzer(const char *name);
void destroy_buzzer(Actuator *a);

#ifdef __cplusplus
}
#endif

#endif /* ACTUATOR_H */
