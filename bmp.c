#include "bmp.h"
#include <GL/glut.h>

BMPImage* readBMP(const char *filename) {
  BMPImage *image = malloc(sizeof(BMPImage));
  FILE *file = fopen(filename, "rb");

  if (!file) {
    fprintf(stderr, "No se puede abrir el archivo %s\n", filename);
    free(image);
    return NULL;
  }

  fread(image->header, sizeof(unsigned char), 54, file);
  image->width = *(int*)&image->header[18];
  image->height = *(int*)&image->header[22];

  image->data = malloc(image->width * image->height * 3); // 3 bytes por píxel
  fread(image->data, sizeof(unsigned char), image->width * image->height * 3, file);
  fclose(file);

  for (int i = 0; i < image->width * image->height * 3; i += 3) {
        unsigned char r = image->data[i + 2]; // Canal Rojo (R)
        unsigned char g = image->data[i + 1]; // Canal Verde (G)
        unsigned char b = image->data[i];     // Canal Azul (B)

        // Convertir a escala de grises usando la fórmula de luminancia
        unsigned char gray = (unsigned char)(0.3 * r + 0.59 * g + 0.11 * b);

        // Asignar el mismo valor a los tres canales para hacer el píxel gris
        image->data[i] = gray;     // Canal Azul
        image->data[i + 1] = gray; // Canal Verde
        image->data[i + 2] = gray; // Canal Rojo
    }

    return image;
}

void freeBMP(BMPImage *image) {
  free(image->data);
  free(image);
}

void drawBMP(BMPImage *image) {
  glDrawPixels(image->width, image->height, GL_BGR, GL_UNSIGNED_BYTE, image->data);
}
