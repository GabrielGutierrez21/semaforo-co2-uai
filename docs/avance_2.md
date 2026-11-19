# Registro de la iteración anterior

El avance anterior pasó de una propuesta de sensor NDIR/ESP32 a un prototipo de señales locales basado en **ESP8266 y CCS811**. Se conservó el firmware inicial de tres LED y pulsador en [`firmware/historico/SemaforoLED/`](../firmware/historico/SemaforoLED/) como evidencia del desarrollo. Después se añadió una OLED en el bus I²C compartido; el programa vigente está en [`firmware/SemaforoAmbiental/`](../firmware/SemaforoAmbiental/).

El CCS811 entrega TVOC y eCO₂ estimado, no medición directa de CO₂. En noviembre de 2026 se retiraron las caritas de la interfaz OLED para mantener la demostración centrada en cifras y estado. También se cambió el calentamiento de la versión de pantalla de 1 a 20 minutos de acondicionamiento previo a la referencia de 30 lecturas.

Las fotografías disponibles en [`hardware/fotos/`](../hardware/fotos/) muestran el montaje de LED y componentes antes del ensamble final. No hay en este repositorio series de medición de aquel avance ni prueba documentada de funcionamiento de la OLED; no se atribuyen valores ni resultados a esa etapa.
