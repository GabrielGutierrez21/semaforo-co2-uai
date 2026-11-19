# EV3: estado y trabajo pendiente

Pauta oficial aportada por el equipo: **Avance #3, Prototipo IoT Completo**, TEI201 DEC UAI. Fecha indicada: **23 de noviembre de 2026**. Presentación presencial de 8–10 min y 3 min de preguntas. El docente aclaró que **Presentación 3 y Revisión Final de GitHub valen 50 % cada una**; el repositorio y planos se pueden omitir durante la exposición porque se revisarán aparte. Esto no elimina la exigencia de entregarlos.

| Criterio | Evidencia disponible en este repo | Falta para EV3 |
|---|---|---|
| Captura y señal | [`firmware/SemaforoAmbiental/`](../firmware/SemaforoAmbiental/), tabla de conexiones | Compilar y probar el código actualizado en la placa; registrar lecturas y una demo continua. |
| Persistencia y dashboard | Ningún servicio ni dashboard implementado | Web local en el ESP8266 con lectura real, histórico persistente y visualización usable; luego prueba en vivo. |
| Hardware | [`BOM.csv`](../hardware/BOM.csv), [diagrama](../hardware/esquematico.pdf) y dos fotos de etapas previas | Comprobar resistencia y módulos reales, completar costos documentados y fotografiar ensamblado final desde al menos tres ángulos. |
| Reparación y 3D | [OpenSCAD + dos STL](../diseno-3d/) | Medir montaje, ajustar encaje, modelar componentes y ensamble en Fusion 360, entregar `.f3d`/`.f3z`, planos PDF y renders. |
| Pruebas | [Protocolo con criterios](../testing/protocolo.md) | Logs y CSV reales, gráficas, análisis cuantitativo, fallo y solución, conexión explícita con el problema. |
| Integridad | [`FUENTES.md`](../FUENTES.md) | Añadir fuentes de librerías/código de la web cuando se implemente. |

## Arquitectura de la siguiente etapa

Una web **alojada en el ESP8266** evita contratar servidor, acceso a Internet y app móvil: el microcontrolador crea un punto de acceso Wi-Fi local, sirve el HTML/CSS/JS y expone valores TVOC/eCO₂ por una API local. La persistencia puede implementarse con LittleFS (CSV estructurado) y descarga del histórico. El navegador muestra valores en vivo, umbrales relativos y una gráfica. Esta arquitectura es una **propuesta**, no funcionalidad ya entregada. Primero hay que medir espacio libre de flash y ensayar el guardado para evitar desgaste y pérdida por reinicios.

La web debe nombrar **eCO₂ estimado**, nunca `CO₂ real`, y mostrar que las luces representan **cambios relativos de TVOC**. Un resultado sobre ventilación requeriría un sensor NDIR de CO₂ o una validación externa adecuada.

## Orden de trabajo

1. Confirmar cableado, cargar el firmware numérico sin caritas y ejecutar el protocolo con fotos/logs reales.
2. Implementar la web local y persistencia en la misma placa ESP8266; validar navegador sin internet, reinicio e histórico.
3. Medir físicamente los módulos, actualizar BOM/costos, montar de forma segura y tomar tres fotos del ensamble.
4. Completar modelo Fusion 360 con volúmenes internos, plano PDF y renders; subir archivos nativos que abran correctamente.
5. Agregar resultados reales y preparar la presentación con problema, demo, dashboard y validación, sin gastar tiempo mostrando el repositorio.

**Riesgo principal:** con el CCS811 no se puede demostrar medición directa de CO₂ de una sala. La defensa debe describir honestamente la observación de VOC/eCO₂ como primera aproximación de calidad ambiental.
