# -*- coding: utf-8 -*-
"""
Carcasa - Semaforo inteligente de calidad de aire (COV / TVOC) con CCS811
=========================================================================
Script para Autodesk Fusion 360 (Utilidades > Complementos > Scripts y
complementos > "+" > seleccionar la carpeta que contiene este archivo > Ejecutar).

Crea un documento nuevo con tres componentes (rev. G):
  - "Base"  : caja con frente (3 LEDs), camara aislada del sensor, cuna de pilas
              2xAA, marco para protoboard, 4 torres para el ESP8266, boton trasero y 4 torres con insertos.
  - "Tapa"  : tapa plana atornillada con 4 tornillos M3 a insertos de laton.
  - "TapaPilas": tapa deslizante inferior del compartimento 2xAA (sin herramientas).

Todas las medidas estan en mm en el bloque PARAMETROS.
Sistema de ejes: X = ancho (izq->der), Y = profundidad (frente -> atras),
Z = altura. El FRENTE es la cara Y negativa.

El mismo archivo, ejecutado con Python + CadQuery fuera de Fusion, exporta
STEP/STL identicos (para verificacion o impresion directa).
"""

# =============================== PARAMETROS (mm) ===============================
T        = 2.0     # espesor de paredes
T_FLOOR  = 4.0     # espesor del piso (aloja la guia de la tapa de pilas)
T_LID    = 3.0     # espesor de la tapa
W        = 94.0    # ancho interior (X)  -> exterior 98 mm (cama de 100 x 100)
D        = 94.0    # profundidad interior (Y) -> exterior 98 mm
H        = 50.0    # altura interior (Z) = altura del conjunto protoboard+ESP (5 cm)

# Torres de union (insertos roscados de laton M3, metal-metal con tornillo M3)
BOSS_R        = 3.6   # radio torre (diam 7.2: pared 1.6 alrededor del inserto, minimo Ruthex)
BOSS_C        = 2.6   # centro de la torre a 2.6 mm de las paredes (queda fundida con ellas)
INSERT_D      = 4.0   # agujero para inserto M3 x 5.7 (Ruthex / CNC Kitchen) -> 4.0 mm
INSERT_DEPTH  = 6.5   # profundidad (inserto 5.7 mm + margen)
PILOT_D       = 2.5   # alivio bajo el inserto para la punta del tornillo
PILOT_DEPTH   = 12.0
LID_HOLE_D    = 3.4   # paso libre M3 en la tapa

# LEDs (frente). 5 mm -> 5.2 ; para LEDs de 3 mm usar 3.2
LED_D   = 5.2
LED_X   = 83.0        # sobre la camara del sensor (el ESP8266 ocupa el lado izquierdo)
LED_Z   = (45.0, 36.5, 28.0)   # rojo (arriba), amarillo, verde (abajo), sobre el techo de la camara

# Pantalla OLED H210328 (frente, a la izquierda de los LEDs): orificio 19 ancho x 21 alto
OLED_W, OLED_H = 19.0, 21.0
OLED_X  = LED_X - 2.9 - 2.0 - OLED_W / 2   # centro: 2 mm libres hasta la pestana del LED
OLED_Z  = 37.5        # centro en altura (por encima del ESP8266)

# Boton (pared trasera) 12 x 12 mm (pulsador tactil 12x12)
BTN_W   = 12.0
BTN_X   = W / 2
BTN_Z   = 40.0        # sobre la protoboard elevada

# Portapilas 2xAA (tipico 58 x 32 x 15 mm). Va pegado BOCA ABAJO al techo de su compartimento,
# que queda DEBAJO de la protoboard (plataforma elevada). Las pilas se cambian por abajo,
# sacando la tapa deslizante (sale por la pared izquierda).
BAT_L, BAT_W, BAT_CLR = 58.0, 32.0, 0.6
BAT_H   = 15.0        # alto del portapilas
BAT_X0  = 10.0        # inicio en X
BAT_Y0  = 47.0        # inicio en Y (bajo la protoboard)
ROOF_T  = 2.0         # techo del compartimento
DOOR_T  = 1.6         # espesor tapa de pilas
DOOR_RAIL = 2.0       # cuanto monta la tapa sobre el labio a cada lado
DOOR_CLR  = 0.3       # holgura de la guia
DOOR_PULL = 35.0      # tapa dibujada medio afuera (0 = cerrada)
WALL2   = 2.0         # espesor de paredes internas

# Camara aislada del sensor CCS811 (esquina frontal derecha)
CH_X0   = 74.0        # cara interior de la pared divisoria (X)
CH_Y1   = 36.0        # cara interior de la pared divisoria (Y)
CH_H    = 20.0        # alto interior de la camara; la cierra un techo de 2 mm (encima van los LEDs)
SENS_W  = 20.0        # placa CCS811 medida: 20 x 15 mm
SENS_T  = 1.6         # espesor PCB
SLOT    = 2.0         # ranura guia
SLOT_X0 = W - 6.0     # cara de la ranura (placa queda a 4 mm de la pared con rejilla)
SENS_Y0 = 12.5        # inicio de la placa en Y
GUIDE_H = 12.0
VENT_D  = 2.5         # agujeros de ventilacion hacia el exterior
VENT_Y  = (17.5, 23.0, 28.5)
VENT_Z  = (5.0, 10.0)

# ESP8266 NodeMCU V3 LoLin (58 x 31 x 13) montado sobre 4 torres que salen del piso,
# detras de la pared frontal (debajo de la pantalla OLED), conectado por cables.
# Agujeros de la placa: d = 3 mm, centros a 52 x 25 mm  -> MEDIR Y CONFIRMAR
ESP_HOLE_DX, ESP_HOLE_DY = 52.0, 25.0   # largo de la placa a lo ancho de la caja (X)
ESP_CX, ESP_CY = 37.0, 20.0     # centro de la placa (zona delantera izquierda)
ESP_L, ESP_W   = 58.0, 31.5     # solo referencia (largo en X, ancho en Y)
ESP_POST_H     = 22.0           # alto de torre desde el piso: lugar para pines + cables dupont hembra
ESP_POST_D     = 8.0            # torre Ø8 con inserto M2.5x5.7 (agujero 4.0, igual que M3)

# Protoboard 30 lineas (largo en X, atras) sobre una plataforma de DECK_Z mm que aloja las pilas
PB_L, PB_W, PB_CLR = 81.0, 54.0, 0.6   # protoboard medida: 81 x 54 mm (holgura 0.3 por lado)
PB_X0, PB_Y0 = (W - 81.6) / 2, 39.0
PB_RIB_H, PB_RIB_T = 3.0, 1.5
DECK_Z = BAT_H + ROOF_T   # altura de la plataforma (techo del compartimento de pilas)

# Labio de centrado en la tapa
LIP_H, LIP_T, LIP_GAP = 3.0, 1.5, 0.3

EXPLODE = 20.0        # separacion de la tapa en el modelo (0 = cerrada)
# ===============================================================================


def base_ops():
    """Lista de operaciones: (tipo, '+'/'-', datos). Coordenadas en mm."""
    ops = []
    box = lambda m, a, b: ops.append(('box', m, a, b))
    cyl = lambda m, p1, p2, r: ops.append(('cyl', m, p1, p2, r))

    # Cascaron
    box('+', (-T, -T, -T_FLOOR), (W + T, D + T, H))
    box('-', (0, 0, 0), (W, D, H + 1))

    # Torres de esquina + relleno contra las paredes
    corners = [(BOSS_C, BOSS_C), (W - BOSS_C, BOSS_C),
               (BOSS_C, D - BOSS_C), (W - BOSS_C, D - BOSS_C)]
    for (cx, cy) in corners:
        cyl('+', (cx, cy, 0), (cx, cy, H), BOSS_R)
        if BOSS_C > BOSS_R:          # solo si la torre no alcanza a tocar las paredes
            sx = 0 if cx < W / 2 else W
            sy = 0 if cy < D / 2 else D
            box('+', (min(sx, cx), min(sy, cy - BOSS_R), 0), (max(sx, cx), max(sy, cy + BOSS_R), H))
            box('+', (min(sx, cx - BOSS_R), min(sy, cy), 0), (max(sx, cx + BOSS_R), max(sy, cy), H))

    # Plataforma de la protoboard: dentro va el compartimento de pilas, abierto hacia abajo
    bx0, by0 = BAT_X0, BAT_Y0
    bx1, by1 = BAT_X0 + BAT_L + BAT_CLR, BAT_Y0 + BAT_W + BAT_CLR
    zr = BAT_H                      # cara inferior del techo (portapilas pegado a el)
    pbx1 = PB_X0 + PB_L + PB_CLR
    box('+', (PB_X0 - PB_RIB_T, PB_Y0 - PB_RIB_T, 0), (pbx1 + PB_RIB_T, D, DECK_Z))
    box('-', (bx0, by0, -0.01), (bx1, by1, zr))
    box('-', (bx0 + 20, PB_Y0 - PB_RIB_T - 1, 8), (bx0 + 28, by0 + 0.01, 13))   # tunel para los cables de las pilas
    # Guia en T dentro del piso (capas desde abajo): labio | ranura de la tapa | techo de 0.8
    z_lip, z_slot = -T_FLOOR + 1.4, -0.8
    box('-', (bx0, by0, z_slot - 0.01), (bx1, by1, 0.01))                       # abertura al compartimento
    box('-', (-T - 1, by0 - DOOR_RAIL - DOOR_CLR, z_lip),
             (bx1 + DOOR_CLR, by1 + DOOR_RAIL + DOOR_CLR, z_slot))              # ranura
    box('-', (-T - 1, by0, -T_FLOOR - 1), (bx1 - 1.0, by1, z_lip + 0.01))       # ventana inferior
    yc = (by0 + by1) / 2
    box('-', (0.2, yc - 3.5, z_slot - 0.01), (2.8, yc + 3.5, 0.01))             # agujero del trinquete

    # Camara del sensor: paredes divisorias + techo (aislan del aire interno)
    box('+', (CH_X0 - WALL2, 0, 0), (CH_X0, CH_Y1 + WALL2, CH_H + ROOF_T))
    box('+', (CH_X0 - WALL2, CH_Y1, 0), (W, CH_Y1 + WALL2, CH_H + ROOF_T))
    box('+', (CH_X0 - WALL2, 0, CH_H), (W, CH_Y1 + WALL2, CH_H + ROOF_T))
    # Guias en U para la placa CCS811 (vertical, sensor mirando a la rejilla)
    gx0, gx1 = SLOT_X0 - 1.5, SLOT_X0 + SLOT + 1.5
    ya, yb = SENS_Y0 - 0.4, SENS_Y0 + SENS_W + 0.4     # extremos del canal (holgura 0.4)
    # guia delantera: tope + 3 mm de canal donde entra el borde de la placa
    box('+', (gx0, ya - 1.5, 0), (gx1, ya, GUIDE_H))                    # tope
    box('+', (gx0, ya, 0), (SLOT_X0, ya + 3.4, GUIDE_H))                 # labio interior
    box('+', (SLOT_X0 + SLOT, ya, 0), (gx1, ya + 3.4, GUIDE_H))          # labio exterior
    # guia trasera
    box('+', (gx0, yb, 0), (gx1, yb + 1.5, GUIDE_H))
    box('+', (gx0, yb - 3.4, 0), (SLOT_X0, yb, GUIDE_H))
    box('+', (SLOT_X0 + SLOT, yb - 3.4, 0), (gx1, yb, GUIDE_H))
    # Paso de cables (sellar con silicona / hot-glue tras cablear)
    box('-', (CH_X0 - WALL2 - 1, 26, CH_H - 6), (CH_X0 + 1, 33, CH_H))

    # Rejilla de ventilacion del sensor (pared derecha) + 2 de flujo cruzado (frente)
    for y in VENT_Y:
        for z in VENT_Z:
            cyl('-', (W - 1, y, z), (W + T + 1, y, z), VENT_D / 2)
    for x in (W - 16.0, W - 10.0):
        cyl('-', (x, -T - 1, 6.0), (x, 1, 6.0), VENT_D / 2)

    # Marco de la protoboard (atras)
    px1, py1 = PB_X0 + PB_L + PB_CLR, PB_Y0 + PB_W + PB_CLR
    box('+', (PB_X0 - PB_RIB_T, PB_Y0 - PB_RIB_T, DECK_Z - 0.01), (px1 + PB_RIB_T, py1 + PB_RIB_T, DECK_Z + PB_RIB_H))
    box('-', (PB_X0, PB_Y0, DECK_Z), (px1, py1, DECK_Z + PB_RIB_H + 1))

    # Torres del ESP8266 con inserto M2.5
    for sx in (-1, 1):
        for sy in (-1, 1):
            ex, ey = ESP_CX + sx * ESP_HOLE_DX / 2, ESP_CY + sy * ESP_HOLE_DY / 2
            z0p = 0.5                            # torres nacen del piso
            z1p = z0p + ESP_POST_H
            cyl('+', (ex, ey, z0p - 0.5), (ex, ey, z1p), ESP_POST_D / 2)
            cyl('-', (ex, ey, z1p - INSERT_DEPTH), (ex, ey, z1p + 1), INSERT_D / 2)
            cyl('-', (ex, ey, z1p - PILOT_DEPTH), (ex, ey, z1p), PILOT_D / 2)

    # Insertos roscados (agujeros en la cima de las torres)
    for (cx, cy) in corners:
        cyl('-', (cx, cy, H - INSERT_DEPTH), (cx, cy, H + 1), INSERT_D / 2)
        cyl('-', (cx, cy, H - PILOT_DEPTH), (cx, cy, H), PILOT_D / 2)

    # 3 LEDs en el frente
    for z in LED_Z:
        cyl('-', (LED_X, -T - 1, z), (LED_X, 1, z), LED_D / 2)

    # Pantalla OLED (frente)
    box('-', (OLED_X - OLED_W / 2, -T - 1, OLED_Z - OLED_H / 2), (OLED_X + OLED_W / 2, 1, OLED_Z + OLED_H / 2))

    # Boton 12x12 atras
    box('-', (BTN_X - BTN_W / 2, D - 1, BTN_Z - BTN_W / 2),
             (BTN_X + BTN_W / 2, D + T + 1, BTN_Z + BTN_W / 2))
    return ops


def lid_ops():
    ops = []
    box = lambda m, a, b: ops.append(('box', m, a, b))
    cyl = lambda m, p1, p2, r: ops.append(('cyl', m, p1, p2, r))
    z0 = H + EXPLODE
    box('+', (-T, -T, z0), (W + T, D + T, z0 + T_LID))
    # Labio de centrado por tramos (evita torres y paredes de la camara)
    g, t = LIP_GAP, LIP_T
    zl = (z0 - LIP_H, z0)
    segs = [((12, g), (OLED_X - OLED_W / 2 - 3, g + t)),    # frente (termina antes de la OLED)
            ((12, D - g - t), (W - 12, D - g)),              # atras
            ((g, 12), (g + t, D - 12)),                      # izquierda
            ((W - g - t, CH_Y1 + WALL2 + 2), (W - g, D - 12))]  # derecha
    for (a, b) in segs:
        box('+', (a[0], a[1], zl[0]), (b[0], b[1], zl[1]))
    for (cx, cy) in [(BOSS_C, BOSS_C), (W - BOSS_C, BOSS_C),
                     (BOSS_C, D - BOSS_C), (W - BOSS_C, D - BOSS_C)]:
        cyl('-', (cx, cy, z0 - 1), (cx, cy, z0 + T_LID + 1), LID_HOLE_D / 2)
    return ops


def door_ops():
    """Tapa de pilas deslizante con lengueta-trinquete."""
    ops = []
    box = lambda m, a, b: ops.append(('box', m, a, b))
    cyl = lambda m, p1, p2, r: ops.append(('cyl', m, p1, p2, r))
    dx = -DOOR_PULL
    by0, by1 = BAT_Y0, BAT_Y0 + BAT_W + BAT_CLR
    bx1 = BAT_X0 + BAT_L + BAT_CLR
    yc = (by0 + by1) / 2
    z0 = -T_FLOOR + 1.4 + 0.1
    z1 = z0 + DOOR_T
    box('+', (-T + dx, by0 - DOOR_RAIL, z0), (bx1 + dx, by1 + DOOR_RAIL, z1))
    # lengueta flexible (2 ranuras laterales + 1 frontal) con resalte cilindrico
    box('-', (-T + 1 + dx, yc - 5, z0 - 1), (9 + dx, yc - 4, z1 + 1))
    box('-', (-T + 1 + dx, yc + 4, z0 - 1), (9 + dx, yc + 5, z1 + 1))
    box('-', (-T + 1 + dx, yc - 5, z0 - 1), (-T + 2 + dx, yc + 5, z1 + 1))
    cyl('+', (1.5 + dx, yc - 3, z1), (1.5 + dx, yc + 3, z1), 1.0)
    # estrias de agarre (cara inferior)
    for x in (20, 23, 26, 29):
        box('-', (x + dx, by0 + 2, z0 - 1), (x + 1 + dx, by1 - 2, z0 + 0.5))
    return ops


# ================================ FUSION 360 ==================================
try:
    import adsk.core, adsk.fusion, traceback
    _IN_FUSION = True
except ImportError:
    _IN_FUSION = False


def _fusion_tool(tbm, op):
    c = 0.1  # mm -> cm (la API de Fusion trabaja en cm)
    P = adsk.core.Point3D.create
    V = adsk.core.Vector3D.create
    if op[0] == 'box':
        (x0, y0, z0), (x1, y1, z1) = op[2], op[3]
        obb = adsk.core.OrientedBoundingBox3D.create(
            P((x0 + x1) / 2 * c, (y0 + y1) / 2 * c, (z0 + z1) / 2 * c),
            V(1, 0, 0), V(0, 1, 0),
            abs(x1 - x0) * c, abs(y1 - y0) * c, abs(z1 - z0) * c)
        return tbm.createBox(obb)
    p1, p2, r = op[2], op[3], op[4]
    return tbm.createCylinderOrCone(P(*[v * c for v in p1]), r * c,
                                    P(*[v * c for v in p2]), r * c)


def run(context):
    ui = None
    try:
        app = adsk.core.Application.get()
        ui = app.userInterface
        app.documents.add(adsk.core.DocumentTypes.FusionDesignDocumentType)
        design = adsk.fusion.Design.cast(app.activeProduct)
        design.designType = adsk.fusion.DesignTypes.DirectDesignType
        design.fusionUnitsManager.distanceDisplayUnits = adsk.fusion.DistanceUnits.MillimeterDistanceUnits
        root = design.rootComponent
        tbm = adsk.fusion.TemporaryBRepManager.get()
        U = adsk.fusion.BooleanTypes.UnionBooleanType
        S = adsk.fusion.BooleanTypes.DifferenceBooleanType
        for name, ops in (('Base', base_ops()), ('Tapa', lid_ops()), ('TapaPilas', door_ops())):
            body = None
            for op in ops:
                tool = _fusion_tool(tbm, op)
                if body is None:
                    body = tool
                else:
                    tbm.booleanOperation(body, tool, U if op[1] == '+' else S)
            occ = root.occurrences.addNewComponent(adsk.core.Matrix3D.create())
            comp = occ.component
            comp.name = name
            b = comp.bRepBodies.add(body)
            b.name = name
        app.activeViewport.fit()
        ui.messageBox('Carcasa creada: componentes "Base", "Tapa" y "TapaPilas".\n'
                      'Para editar medidas cambia el bloque PARAMETROS del script.')
    except Exception:
        if ui:
            ui.messageBox('Error:\n{}'.format(traceback.format_exc()))


# ============================ CADQUERY (fuera de Fusion) ========================
def _cq_build(ops):
    import cadquery as cq
    solid = None
    for op in ops:
        if op[0] == 'box':
            (x0, y0, z0), (x1, y1, z1) = op[2], op[3]
            s = cq.Workplane().box(abs(x1-x0), abs(y1-y0), abs(z1-z0)).translate(
                ((x0+x1)/2, (y0+y1)/2, (z0+z1)/2))
        else:
            p1, p2, r = op[2], op[3], op[4]
            v = cq.Vector(*p2) - cq.Vector(*p1)
            s = cq.Workplane().add(cq.Solid.makeCylinder(r, v.Length, cq.Vector(*p1), v))
        solid = s if solid is None else (solid.union(s) if op[1] == '+' else solid.cut(s))
    return solid


if __name__ == '__main__' and not _IN_FUSION:
    import cadquery as cq
    for name, ops in (('Base', base_ops()), ('Tapa', lid_ops()), ('TapaPilas', door_ops())):
        s = _cq_build(ops)
        cq.exporters.export(s, f'Semaforo_{name}.step')
        cq.exporters.export(s, f'Semaforo_{name}.stl')
        bb = s.val().BoundingBox()
        print(name, 'valid', s.val().isValid(), 'bbox', round(bb.xlen, 2), round(bb.ylen, 2), round(bb.zlen, 2))
