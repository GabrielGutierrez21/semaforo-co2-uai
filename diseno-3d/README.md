# Carcasa del prototipo actual

El diseño **Semáforo Ambiental, revisión E** está disponible en archivos separados para revisar, editar e imprimir. El [paquete original del equipo](SEMAFORO_AMBIENTAL.zip) se conserva como respaldo; los archivos de abajo se extrajeron de ese mismo paquete, sin modificar la geometría.

| Archivo | Uso |
|---|---|
| [Plano acotado](Plano.pdf) | Vistas, cotas y comprobaciones de encaje. |
| [Script para Fusion 360](fusion-script/SemaforoCCS811/SemaforoCCS811.py) y [manifest](fusion-script/SemaforoCCS811/SemaforoCCS811.manifest) | Generan los componentes Base, Tapa y TapaPilas como un diseño directo editable mediante parámetros del script. Es un script, no un archivo `.f3d`. |
| [Piezas STEP](piezas/) | Intercambio CAD de las tres piezas. |
| [STL de impresión](impresion/) | Base, tapa superior y tapa deslizante del compartimento de pilas. |

## Dimensiones y montaje

La base tiene dimensiones exteriores **107 × 131 × 54 mm** (ancho × fondo × alto). La tapa superior mide **107 × 131 × 6 mm** de caja envolvente, incluyendo su labio; la tapa de pilas, **71,1 × 36,6 × 2,6 mm**. Las medidas se comprobaron en los STL incluidos. El script dispone una cuna para portapilas **2 × AA**, una zona para protoboard de 81 × 54 mm, torres para ESP8266 y una cámara ventilada para CCS811. El [plano](Plano.pdf) indica posiciones y holguras previstas.

El cuerpo y la tapa **exceden la cama de 100 × 100 mm informada para el DEC**. Para imprimir esta revisión se necesita una impresora con área útil de al menos 107 × 131 mm, dejando además el margen que exija la máquina. Girar las piezas dentro de una cama de 100 × 100 mm no resuelve el límite. Si se debe imprimir en el DEC, se requiere rediseñar o dividir las piezas y actualizar los STEP, STL y plano de manera consistente.

El STL de la tapa está separado verticalmente en el ensamble para mostrar el interior: antes de laminar cada STL se debe colocar la pieza sobre la cama en el programa de impresión. El modelo representa una tapa superior plana y **no incluye abertura ni soporte para la OLED**; hay que definir físicamente su ubicación para que la pantalla numérica sea visible. La geometría del compartimento no resuelve por sí sola la alimentación eléctrica: véase [conexiones](../hardware/conexiones.md).

Para generar el ensamble en Fusion 360: abrir **Utilidades → Scripts y complementos**, agregar la carpeta `fusion-script/SemaforoCCS811` y ejecutar el script. Para editar dimensiones, ajustar su bloque `PARAMETROS`; guardar el documento de Fusion después de generarlo. Los [archivos de la carcasa inicial](../iteraciones/diseno-3d-inicial/) se conservaron aparte para mostrar la evolución.
