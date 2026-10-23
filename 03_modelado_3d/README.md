# Modelado 3D — carcasa del montaje con una protoboard

Archivos paramétricos de **base y tapa**: [carcasa_semaforo.scad](carcasa_semaforo.scad), [base.stl](base.stl), [tapa.stl](tapa.stl). El modelo se diseñó para **una protoboard de referencia de 84 × 55 mm**, NodeMCU ESP8266, CCS811, OLED 0,96″, tres LED y pulsador. Las dimensiones exteriores nominales son **100 × 80 × 33,5 mm**; pared y piso 2,5 mm. No es un archivo nativo de Fusion 360: los STL pueden importarse allí como malla, mientras que el `.scad` conserva los parámetros editables en OpenSCAD.

**Distribución:** ventana de OLED en la tapa, orificios separados para los tres LED y el botón, entrada USB lateral y ranuras de ventilación. La protoboard se fija al piso con cinta de montaje; los cuatro postes de esquina son para sujetar la tapa con tornillos M3 adecuados al material de impresión. La ubicación precisa de la OLED, USB y ventilación debe ajustarse con medidas del montaje real antes de imprimir.

**Para cambiar medidas:** editar las variables al principio de `carcasa_semaforo.scad`. Para regenerar los STL con OpenSCAD: `openscad -o base.stl -D 'pieza="base"' carcasa_semaforo.scad` y `openscad -o tapa.stl -D 'pieza="tapa"' carcasa_semaforo.scad`.

**Para el Entregable 2:** mostrar la vista de base y tapa, describir accesibilidad del USB, legibilidad de OLED y LED, circulación de aire alrededor del sensor y mantenimiento. El encapsulado físico se valida cuando se miden y prueban las piezas impresas.
