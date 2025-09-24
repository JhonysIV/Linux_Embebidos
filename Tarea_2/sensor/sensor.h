#ifndef SENSOR_H
#define SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Indica si el sensor está reproduciendo desde sensor_feed.csv.
 * Declarado aquí como extern (una sola definición en sensor.c).
 */
extern int sensor_has_csv;

/* Inicializa la librería de sensor.
 * - Si encuentra "sensor_feed.csv" en el directorio de ejecución, carga
 *   sus valores (uno por línea) y sensor_read() irá devolviéndolos cyclicamente.
 * - Si no hay CSV válido, pasará a modo aleatorio (0..100).
 */
void sensor_init(void);

/* Devuelve la siguiente lectura del sensor (double). */
double sensor_read(void);

/* Libera recursos (opcional, pero útil para pruebas). */
void sensor_close(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_H */