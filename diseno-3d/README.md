# Diseño físico y piezas 3D

La carcasa vigente es **Semáforo Ambiental, revisión G**. Sus archivos de fabricación están organizados por formato:

| Archivo | Uso |
|---|---|
| [Plano acotado](Plano.pdf) | Vistas, cotas y disposición de los componentes. |
| [Script para Fusion 360](fusion-script/SemaforoCCS811/SemaforoCCS811.py) y [manifest](fusion-script/SemaforoCCS811/SemaforoCCS811.manifest) | Generan Base, Tapa y TapaPilas como componentes editables; modificar las medidas en `PARAMETROS` del script. |
| [Piezas STEP](piezas/) | Intercambio CAD de las tres piezas. El STEP de la base se generó con CadQuery desde el script de esta revisión; los STEP de las dos tapas fueron aportados por el equipo. |
| [STL de impresión](impresion/) | Base, tapa superior y tapa deslizante de las pilas, aportadas por el equipo. |
| [Paquete STL](Semaforo_STL_impresion.zip) y [paquete del script Fusion](SemaforoCCS811_Fusion.zip) | Archivos originales recibidos del equipo para esta revisión. |

## Dimensiones y montaje

La **base mide 98 × 98 × 54 mm** (ancho × fondo × alto); el conjunto con tapa alcanza **98 × 98 × 57 mm**. La tapa superior tiene una caja envolvente de **98 × 98 × 6 mm**, incluido el labio; la tapa de pilas mide **70,6 × 36,6 × 2,6 mm**. El [plano](Plano.pdf) da las cotas de montaje, y estas dimensiones exteriores se verificaron con los STL de la revisión G. En el script, `W=94` y `D=94` son las medidas interiores; cada pared agrega `2` mm. La altura de la base resulta de `H=50` más un piso de `4` mm.

La base contiene la cuna del portapilas **2 × AA** bajo la plataforma de la protoboard, torres para el ESP8266, una cámara ventilada para el CCS811, tres orificios frontales para LED, alojamiento para el pulsador posterior y **abertura frontal de 19 × 21 mm para la OLED**. Las [conexiones eléctricas](../hardware/conexiones.md) se documentan por separado.

Para generar el ensamble en Fusion 360: abrir **Utilidades → Scripts y complementos**, agregar la carpeta `fusion-script/SemaforoCCS811` y ejecutar el script. Guardar el documento de Fusion después de generarlo. Los STL están dispuestos como ensamble; orientar cada pieza sobre su cara de apoyo antes de laminarla. Los [archivos de la carcasa inicial](../iteraciones/diseno-3d-inicial/) se conservan como iteración anterior.
