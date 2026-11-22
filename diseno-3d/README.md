# Diseño físico y piezas 3D

La fuente paramétrica [`carcasa_semaforo.scad`](carcasa_semaforo.scad) permite editar una base con ventilación, salida USB y postes y una tapa con recortes de OLED, tres LED y pulsador. Las piezas [`base.stl`](base.stl) y [`tapa.stl`](tapa.stl) son exportaciones para impresión de prueba.

Dimensiones exteriores nominales: **100 × 80 × 33,5 mm**, con base de 31 mm y tapa de 2,5 mm; pared y fondo de 2,5 mm, radio de esquinas de 6 mm. Ventana de OLED: 26 × 16 mm; tres agujeros LED de radio 2,7 mm y pulsador de radio 3,5 mm. Antes de fabricar, contrastar el espacio con el ESP8266, sensor, protoboard, headers y cable USB. Para una cama útil de 100 × 100 mm, verificar tolerancia con DEC; el ancho nominal está justo en el límite.

Para adaptar el modelo, editar las variables al inicio del SCAD y exportar cada pieza con `pieza="base"` y `pieza="tapa"`. Convención de entrega: conservar en **esta misma carpeta** los archivos nativos de Fusion 360, renders y planos acotados, con nombres descriptivos y enlaces en este índice.
