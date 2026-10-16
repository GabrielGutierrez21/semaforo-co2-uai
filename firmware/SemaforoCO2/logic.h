#pragma once

#include <stdint.h>

// Umbrales del prototipo: son decisiones de diseño, no límites sanitarios.
enum class Estado : uint8_t { INICIANDO, VERDE, AMARILLO, ROJO, ERROR };

inline Estado clasificar(Estado anterior, uint16_t ppm) {
  // La primera lectura usa los límites publicados por el equipo.
  if (anterior == Estado::INICIANDO || anterior == Estado::ERROR) {
    if (ppm < 700) return Estado::VERDE;
    if (ppm <= 1300) return Estado::AMARILLO;
    return Estado::ROJO;
  }

  // Banda de 50 ppm para evitar parpadeos cerca de cada límite.
  if (anterior == Estado::VERDE) {
    if (ppm > 1350) return Estado::ROJO;
    return ppm >= 750 ? Estado::AMARILLO : Estado::VERDE;
  }
  if (anterior == Estado::ROJO) {
    if (ppm < 650) return Estado::VERDE;
    return ppm <= 1250 ? Estado::AMARILLO : Estado::ROJO;
  }
  if (ppm < 650) return Estado::VERDE;
  if (ppm > 1350) return Estado::ROJO;
  return Estado::AMARILLO;
}

inline const char* nombreEstado(Estado estado) {
  switch (estado) {
    case Estado::VERDE: return "VERDE";
    case Estado::AMARILLO: return "AMARILLO";
    case Estado::ROJO: return "ROJO";
    case Estado::ERROR: return "ERROR SENSOR";
    default: return "INICIANDO";
  }
}
