# Modelo 3D disponible

- [`carcasa_semaforo.scad`](carcasa_semaforo.scad): fuente paramétrica OpenSCAD con base, tapa, abertura de pantalla, LED, botón, USB y ventilación.
- [`base.stl`](base.stl) y [`tapa.stl`](tapa.stl): piezas exportadas para impresión de prueba. Son mallas correspondientes al diseño de esta carpeta.

**Medidas exteriores nominales:** 100 × 80 × 33,5 mm, con base de 31 mm, tapa de 2,5 mm y paredes/fondo de 2,5 mm. Radio de esquinas 6 mm. La ventana OLED está modelada en la tapa como recorte de 26 × 16 mm; los tres agujeros LED tienen radio 2,7 mm y el del botón radio 3,5 mm. La base tiene abertura USB y cinco ranuras de ventilación. Se prevén cuatro postes y tornillos desmontables; antes de fabricar se deben medir en el hardware real la altura de la pantalla, del pulsador, de los headers y del cable USB.

La impresora del DEC admite una base de **100 × 100 mm** según lo informado por el equipo: una pieza de 100 mm de ancho queda justo en el límite, sin margen de brim ni error de máquina. Conviene verificar con DEC si acepta ese límite y, si no, reducir `ancho` a 98 mm y confirmar que la protoboard realmente cabe. No reducir a ciegas: el interior de 95 mm (100 menos dos paredes de 2,5 mm) deja aproximadamente 11 mm de margen respecto de una protoboard de referencia de 84 mm, antes de considerar conectores y fijaciones.

**Estado respecto a EV3:** el archivo `.scad` es editable y los `.stl` son imprimibles, pero esto **no es un ensamble Fusion 360** ni representa todavía todos los componentes internos. No existe aquí un `.f3d`/`.f3z`, plano acotado en PDF ni render exterior/interior validado de ese ensamble. Esos entregables se deben construir y subir tras medir las piezas reales. No renombrar un STL a `.f3d`.

Para modificar la geometría, editar las variables al inicio del `.scad` y exportar cada pieza en OpenSCAD con `pieza="base"` o `pieza="tapa"`. Guardar posteriormente los planos, el archivo nativo Fusion y renders en esta misma carpeta, con nombres que indiquen versión y fecha.
