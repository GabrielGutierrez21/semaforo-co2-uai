// Carcasa conceptual paramétrica para ESP32 DevKit y breakout Adafruit SCD40.
// Todas las medidas en mm. Medir componentes reales antes de imprimir.
// Exportación: openscad -o base.stl -D 'parte="base"' carcasa.scad
//              openscad -o tapa.stl -D 'parte="tapa"' carcasa.scad

parte = "conjunto"; // "base", "tapa" o "conjunto"
ancho = 120;
alto = 76;
profundidad_base = 28;
pared = 2.4;
espesor_tapa = 2.4;
radio = 7;
$fn = 48;

module redondeado(w, h, r) {
  hull() for (x=[-w/2+r,w/2-r], y=[-h/2+r,h/2-r])
    translate([x,y]) circle(r);
}

module poste(x,y) {
  translate([x,y,pared]) cylinder(h=profundidad_base-pared-0.2,r=5.2);
}

module base() {
  difference() {
    union() {
      difference() {
        linear_extrude(profundidad_base) redondeado(ancho,alto,radio);
        translate([0,0,pared])
          linear_extrude(profundidad_base+0.2)
            redondeado(ancho-2*pared,alto-2*pared,radio-pared);
      }
      for(x=[-ancho/2+9,ancho/2-9],y=[-alto/2+9,alto/2-9]) poste(x,y);
    }
    // Pilotos de 2.7 mm para tornillos M3 autorroscantes; verificar en el DEC.
    for(x=[-ancho/2+9,ancho/2-9],y=[-alto/2+9,alto/2-9])
      translate([x,y,profundidad_base-11]) cylinder(h=12,r=1.35);
    // Ventanas laterales cerca del SCD40 (ubicado en el extremo derecho).
    for(y=[-18,-9,0,9,18])
      translate([ancho/2-pared-1,y-3,11]) cube([pared+3,6,3]);
    // Abertura de USB en el lado izquierdo: ajustar posición según placa.
    translate([-ancho/2-1,-10,8]) cube([pared+3,20,12]);
    // Ranuras en piso para bridas de montaje ajustables del ESP32.
    for(x=[-38,-8],y=[-15,15])
      translate([x,y,0]) cube([3,8,pared*3],center=true);
    // Ranuras para fijar el breakout SCD40 junto a ventilación.
    for(x=[22,46],y=[-12,12])
      translate([x,y,0]) cube([3,6,pared*3],center=true);
  }
}

module tapa() {
  difference() {
    linear_extrude(espesor_tapa) redondeado(ancho,alto,radio);
    // LED verde arriba, amarillo al centro y rojo abajo.
    for(y=[22,0,-22]) translate([-21,y,-1]) cylinder(h=espesor_tapa+2,r=2.7);
    // Pulsador de modo prueba (opcional; puede taparse si no se instala).
    translate([29,-22,-1]) cylinder(h=espesor_tapa+2,r=3.3);
    // Orificios para cuatro tornillos.
    for(x=[-ancho/2+9,ancho/2-9],y=[-alto/2+9,alto/2-9])
      translate([x,y,-1]) cylinder(h=espesor_tapa+2,r=1.65);
  }
}

if (parte=="base") base();
else if (parte=="tapa") tapa();
else {
  color("white") base();
  color("gainsboro") translate([0,0,profundidad_base+2]) tapa();
}
