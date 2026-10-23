// Semaforo ESP8266 + CCS811 + OLED + tres LED + pulsador.
// Carcasa conceptual parametrica. Unidades: mm.
pieza = "conjunto"; // "base", "tapa", "conjunto"
ancho = 100;
largo = 80;
alto_base = 31;
pared = 2.5;
alto_tapa = 2.5;
radio = 6;
separacion_tapa = 8;
$fn = 48;

module contorno(w, h, r) {
  hull() for (x = [-w/2+r, w/2-r], y = [-h/2+r, h/2-r])
    translate([x,y]) circle(r=r);
}

module poste(x,y) {
  translate([x,y,pared]) cylinder(h=alto_base-pared-0.8, r=4.5);
}

module base() {
  difference() {
    union() {
      difference() {
        linear_extrude(alto_base) contorno(ancho,largo,radio);
        translate([0,0,pared])
          linear_extrude(alto_base+0.2)
            contorno(ancho-2*pared,largo-2*pared,radio-pared);
      }
      for (x=[-ancho/2+8,ancho/2-8], y=[-largo/2+8,largo/2-8])
        poste(x,y);
    }
    // Pilotos para tornillos M3; ajustar al material y tornillos.
    for (x=[-ancho/2+8,ancho/2-8], y=[-largo/2+8,largo/2-8])
      translate([x,y,alto_base-12]) cylinder(h=12.2,r=1.25);
    // Entrada USB del ESP8266 en pared izquierda.
    translate([-ancho/2-1,-10,7]) cube([pared+3,20,12]);
    // Ventilacion en pared derecha, cerca del CCS811.
    for (y=[-21,-12,-3,6,15])
      translate([ancho/2-pared-1,y,10]) cube([pared+3,6,3]);
  }
}

module tapa() {
  difference() {
    linear_extrude(alto_tapa) contorno(ancho,largo,radio);
    // Ventana OLED: ajustar a zona visible y posicion real.
    translate([18,10,-1]) linear_extrude(alto_tapa+2)
      contorno(26,16,2);
    // Verde, amarillo y rojo.
    for (y=[21,0,-21])
      translate([-28,y,-1]) cylinder(h=alto_tapa+2,r=2.7);
    // Pulsador.
    translate([19,-20,-1]) cylinder(h=alto_tapa+2,r=3.5);
    // Paso de tornillos de tapa.
    for (x=[-ancho/2+8,ancho/2-8], y=[-largo/2+8,largo/2-8])
      translate([x,y,-1]) cylinder(h=alto_tapa+2,r=1.65);
  }
}

if (pieza=="base") base();
else if (pieza=="tapa") tapa();
else {
  color("white") base();
  color("lightgray") translate([0,0,alto_base+separacion_tapa]) tapa();
}
