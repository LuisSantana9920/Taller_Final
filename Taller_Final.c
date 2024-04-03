#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_DATOS 100

// Definición de la estructura Sensor
typedef struct {
    time_t timestamp;
    float temperatura;
    float humedad;
} Sensor;

// Prototipos de funciones
void calcular_estadisticas(Sensor datos[], int contador);
void escribir_analisis(Sensor datos[], int contador);
float calcular_punto_rocio(float temperatura, float humedad);

int main() {
    FILE *archivo;
    char linea[100];
    Sensor datos[MAX_DATOS];
    int contador = 0;

    archivo = fopen("D:\\Codigos\\C\\Datos_de_Sensor.txt", "r");

    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Ignorar la primera línea (encabezado)
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL && contador < MAX_DATOS) {
        sscanf(linea, "%ld,%f,%f", &datos[contador].timestamp, &datos[contador].temperatura, &datos[contador].humedad);
        contador++;
    }

    fclose(archivo);

    // Calcular estadísticas
    calcular_estadisticas(datos, contador);
    // Escribir análisis en un archivo CSV
    escribir_analisis(datos, contador);

    return 0;
}

// Función para calcular y mostrar las estadísticas de temperatura y humedad
void calcular_estadisticas(Sensor datos[], int contador) {
    float temp_max = datos[0].temperatura;
    float temp_min = datos[0].temperatura;
    float humd_max = datos[0].humedad;
    float humd_min = datos[0].humedad;
    time_t hora_temp_max = datos[0].timestamp;
    time_t hora_temp_min = datos[0].timestamp;
    time_t hora_humd_max = datos[0].timestamp;
    time_t hora_humd_min = datos[0].timestamp;
    float temp_suma = 0;
    float humd_suma = 0;

    for (int i = 0; i < contador; i++) {
        // Temperatura máxima
        if (datos[i].temperatura > temp_max) {
            temp_max = datos[i].temperatura;
            hora_temp_max = datos[i].timestamp;
        }
        // Temperatura mínima
        if (datos[i].temperatura < temp_min) {
            temp_min = datos[i].temperatura;
            hora_temp_min = datos[i].timestamp;
        }
        // Humedad máxima
        if (datos[i].humedad > humd_max) {
            humd_max = datos[i].humedad;
            hora_humd_max = datos[i].timestamp;
        }
        // Humedad mínima
        if (datos[i].humedad < humd_min) {
            humd_min = datos[i].humedad;
            hora_humd_min = datos[i].timestamp;
        }
        temp_suma += datos[i].temperatura;
        humd_suma += datos[i].humedad;
    }

    // Calcula el promedio de temperatura y humedad
    float temp_promedio = temp_suma / contador;
    float humd_promedio = humd_suma / contador;

    // Imprime las estadísticas
    printf("Temperatura máxima: %.2f, Hora: %s", temp_max, asctime(localtime(&hora_temp_max)));
    printf("Temperatura mínima: %.2f, Hora: %s", temp_min, asctime(localtime(&hora_temp_min)));
    printf("Humedad máxima: %.2f, Hora: %s", humd_max, asctime(localtime(&hora_humd_max)));
    printf("Humedad mínima: %.2f, Hora: %s", humd_min, asctime(localtime(&hora_humd_min)));
    printf("Promedio de temperatura: %.2f\n", temp_promedio);
    printf("Promedio de humedad: %.2f\n", humd_promedio);
}

// Función para escribir los resultados del análisis en un archivo CSV
void escribir_analisis(Sensor datos[], int contador) {
    FILE *archivo;
    archivo = fopen("analytics.csv", "w");

    if (archivo == NULL) {
        perror("Error al abrir el archivo analytics.csv");
        exit(EXIT_FAILURE);
    }

    fprintf(archivo, "Variable,Promedio,Máximo,Mínimo\n");
    
    // Temperatura
    float temp_max = datos[0].temperatura;
    float temp_min = datos[0].temperatura;
    float temp_suma = datos[0].temperatura;

    for (int i = 1; i < contador; i++) {
        temp_suma += datos[i].temperatura;
        if (datos[i].temperatura > temp_max)
            temp_max = datos[i].temperatura;
        if (datos[i].temperatura < temp_min)
            temp_min = datos[i].temperatura;
    }
    float temp_promedio = temp_suma / contador;
    fprintf(archivo, "Temperatura,%.2f,%.2f,%.2f\n", temp_promedio, temp_max, temp_min);

    // Humedad
    float humd_max = datos[0].humedad;
    float humd_min = datos[0].humedad;
    float humd_suma = datos[0].humedad;

    for (int i = 1; i < contador; i++) {
        humd_suma += datos[i].humedad;
        if (datos[i].humedad > humd_max)
            humd_max = datos[i].humedad;
        if (datos[i].humedad < humd_min)
            humd_min = datos[i].humedad;
    }
    float humd_promedio = humd_suma / contador;
    fprintf(archivo, "Humedad,%.2f,%.2f,%.2f\n", humd_promedio, humd_max, humd_min);

    fclose(archivo);
}

// Función para calcular el punto de rocío
float calcular_punto_rocio(float temperatura, float humedad) {
    const float b = 17.67;
    const float c = 243.5;
    float punto_rocio = c * (log(humedad / 100.0) + ((b * temperatura) / (c + temperatura))) / (b - (log(humedad / 100.0) + ((b * temperatura) / (c + temperatura))));
    return punto_rocio;
}
