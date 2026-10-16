#include <cassert>
#include "../firmware/SemaforoCO2/logic.h"

int main() {
  assert(clasificar(Estado::INICIANDO, 699) == Estado::VERDE);
  assert(clasificar(Estado::INICIANDO, 700) == Estado::AMARILLO);
  assert(clasificar(Estado::INICIANDO, 1300) == Estado::AMARILLO);
  assert(clasificar(Estado::INICIANDO, 1301) == Estado::ROJO);
  assert(clasificar(Estado::VERDE, 701) == Estado::VERDE);
  assert(clasificar(Estado::VERDE, 750) == Estado::AMARILLO);
  assert(clasificar(Estado::AMARILLO, 649) == Estado::VERDE);
  assert(clasificar(Estado::AMARILLO, 1351) == Estado::ROJO);
  assert(clasificar(Estado::ROJO, 1251) == Estado::ROJO);
  assert(clasificar(Estado::ROJO, 1250) == Estado::AMARILLO);
  assert(clasificar(Estado::ERROR, 900) == Estado::AMARILLO);
}
