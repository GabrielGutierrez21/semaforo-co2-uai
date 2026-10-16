# Qué hacer en el DEC

La guía de Avance 02 indica que el modelo 3D conceptual es obligatorio y que **no es necesario tener la carcasa impresa** para ese avance. Consultar disponibilidad y horarios directamente. El [catálogo DEC](https://dec.uai.cl/componentes/) no mostró sensores CO₂ al buscar el término el 16-10-2026; stock limitado, confirmar presencialmente, llenar formulario al retirar y devolver según condiciones del curso.

## Antes de ir

- Llevar este repositorio y, si ya se consiguió, el breakout SCD40/SCD41 real. Pedir ESP32 #787, protoboard #802, pulsador #726 y USB MicroUSB #844/#845 si el conector coincide. Preguntar por tres LED individuales, tres resistencias 330 Ω y 10–15 cables Dupont.
- Llevar `cad/carcasa.scad`, STL, fotos y medidas con regla o pie de metro.
- Llevar el diagrama de `conexiones.md` y explicar que la alimentación es solo USB de baja tensión.
- Consultar si tienen un sensor CO₂ real, incluso fuera de catálogo; idealmente SCD40/SCD41. MH-Z19C/E o STCC4 + SHT4x necesitarían adaptación del código. MQ-7 #659 mide **CO** y MQ-135 #662 no da CO₂ selectivo, así que no sustituyen al sensor requerido.
- Preguntar por OLED #778 (mejora pendiente de firmware y ventana) y buzzer activo. El #708 KY-006 es pasivo y no funciona sin cambiar la lógica de buzzer. Consultar multímetro, impresora y filamento.

## En la mesa de electrónica

1. Identificar el modelo exacto de ESP32 y breakout. Confirmar pines, tensión de entrada y conectores.
2. Probar SCD40 solo, luego LEDs y pulsador. Solicitar revisión de cableado antes de energizar.
3. Mostrar al encargado cinco minutos de lecturas continuas y la página local, o registrar el fallo que hay que corregir.
4. Si todo funciona, fijar en placa perforada o asegurar los cables de protoboard para la demo. Evitar soldar antes de comprobar cada función.

## En la mesa de fabricación

1. Medir largo, ancho, alto y ubicación USB del ESP32; largo, ancho, alto y abertura sensible del breakout.
2. Editar medidas y posiciones de `cad/carcasa.scad`. La planta conceptual es **96 × 76 mm**, base de 28 mm y tapa de 2,4 mm; no certifica el encaje físico. El [DEC recomienda que cada pieza no supere 100 × 100 × 100 mm](https://dec.uai.cl/impresion-3d-corte-laser/), STL en milímetros y no ajusta la escala por el equipo. No hay ventana OLED hasta medir el módulo.
3. Abrir `parte="base"` y `parte="tapa"` por separado. Revisar en el laminador paredes, huecos, orientación, soportes y tiempo de impresión con el personal del DEC.
4. Hacer un primer montaje en seco. Ajustar ranuras de ventilación, salida USB, orificios de LED, botón y postes. Imprimir la versión corregida solo si cabe en el tiempo disponible.
5. Tomar fotos del modelo, dimensiones, primer montaje, problema encontrado y corrección. Eso es evidencia de iteración. Para solicitar impresión, seguir la página vigente del DEC y enviar STL a **DEC@UAI.CL**, asunto `Impresión Grupo X - Sección X - CursoX - Peñalolén`; confirmar sede, material y plazo antes de enviar.

Si no imprimen, presenten el archivo OpenSCAD manipulable, sus vistas, cotas y distribución interna; el requisito de esa pauta es **conceptualizar** el encapsulado.

## Correo sugerido para consultar CO₂

**Asunto:** Consulta de sensor de CO₂ y materiales para prototipo ESP32

Hola, equipo DEC:

Somos estudiantes de la UAI y estamos construyendo un semáforo autónomo que mide CO₂ en una sala con ESP32. ¿Tienen para préstamo algún sensor de CO₂ real, idealmente SCD40 o SCD41 en módulo I²C? Si no, ¿cuentan con MH-Z19C/E u otro sensor de CO₂ directo? También quisiéramos consultar por tres LED individuales, resistencias de 330 Ω y cables Dupont. Vimos en el catálogo ESP32 #787, protoboard #802, pulsador #726 y pantalla OLED #778.

Iremos presencialmente para confirmar stock y procedimiento de préstamo. Muchas gracias.

Saludos,  
[Nombre, sección y curso]

Fuentes: [catálogo](https://dec.uai.cl/componentes/), [impresión 3D](https://dec.uai.cl/impresion-3d-corte-laser/) y guía Avance 02 entregada por el curso.
