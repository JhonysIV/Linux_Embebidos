#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "sensor/sensor.h"
#include "actuators/actuator.h"

/* Umbral: puede ajustarse por variable de entorno CTL_THRESHOLD */
static double get_threshold(void) {
    const char *env = getenv("CTL_THRESHOLD");
    if (env && *env) {
        char *endp = NULL;
        double v = strtod(env, &endp);
        if (endp != env) return v;
    }
    return 50.0; /* por defecto */
}

static volatile sig_atomic_t g_stop = 0;
static void handle_sigint(int sig) {
    (void)sig;
    g_stop = 1;
}

static inline void now_mono(struct timespec *ts) {
    clock_gettime(CLOCK_MONOTONIC, ts);
}

static inline long long ts_to_ns(const struct timespec *ts) {
    return (long long)ts->tv_sec * 1000000000LL + ts->tv_nsec;
}

static inline void ns_to_ts(long long ns, struct timespec *ts) {
    ts->tv_sec = ns / 1000000000LL;
    ts->tv_nsec = ns % 1000000000LL;
}

int main(void) {
    signal(SIGINT, handle_sigint);

    /* Inicializar sensores y actuadores */
    sensor_init();
    Actuator *led = create_led("LED");
    Actuator *buzzer = create_buzzer("Buzzer");

    const double threshold = get_threshold();

    /* Tiempos de apagado programado (0 = no programado) en nanosegundos monocónicos */
    long long buzzer_off_deadline_ns = 0; /* 1 s después de bajar del umbral */
    long long led_off_deadline_ns = 0;    /* 5 s después de bajar del umbral */

    /* Periodo de muestreo: 100 ms */
    const long long sample_period_ns = 100LL * 1000LL * 1000LL;

    while (!g_stop) {
        struct timespec tnow_ts;
        now_mono(&tnow_ts);
        long long tnow_ns = ts_to_ns(&tnow_ts);

        /* Leer sensor */
        double value = sensor_read();

        /* Lógica de control */
        if (value >= threshold) {
            /* Activar inmediatamente y cancelar apagados pendientes */
            if (!led->status(led->params)) {
                led->activate(led->params);
            }
            if (!buzzer->status(buzzer->params)) {
                buzzer->activate(buzzer->params);
            }
            buzzer_off_deadline_ns = 0;
            led_off_deadline_ns = 0;
        } else {
            /* Programar apagados si no están ya programados */
            if (buzzer->status(buzzer->params) && buzzer_off_deadline_ns == 0) {
                buzzer_off_deadline_ns = tnow_ns + 1000000000LL; /* 1 s */
            }
            if (led->status(led->params) && led_off_deadline_ns == 0) {
                led_off_deadline_ns = tnow_ns + 5000000000LL; /* 5 s */
            }
        }

        /* Ejecutar apagados programados si corresponde */
        if (buzzer_off_deadline_ns && tnow_ns >= buzzer_off_deadline_ns) {
            if (buzzer->status(buzzer->params)) {
                buzzer->deactivate(buzzer->params);
            }
            buzzer_off_deadline_ns = 0;
        }
        if (led_off_deadline_ns && tnow_ns >= led_off_deadline_ns) {
            if (led->status(led->params)) {
                led->deactivate(led->params);
            }
            led_off_deadline_ns = 0;
        }

        /* Log: timestamp, valor, estados */
        int led_state = led->status(led->params);
        int buz_state = buzzer->status(buzzer->params);
        printf("[%10ld.%09ld] value=%6.3f threshold=%6.3f LED=%d Buzzer=%d\n",
               (long)tnow_ts.tv_sec, tnow_ts.tv_nsec, value, threshold, led_state, buz_state);
        fflush(stdout);

        /* Dormir hasta próximo ciclo (100 ms) */
        struct timespec req;
        ns_to_ts(sample_period_ns, &req);
        nanosleep(&req, NULL);
    }

    /* Limpieza */
    destroy_led(led);
    destroy_buzzer(buzzer);
    sensor_close();
    return 0;
}
