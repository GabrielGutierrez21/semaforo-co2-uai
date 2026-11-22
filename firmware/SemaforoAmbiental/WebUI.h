#pragma once
#include <Arduino.h>

// Interfaz local. No usa CDN, Internet ni datos simulados.
const char WEB_UI[] PROGMEM = R"HTML(
<!doctype html>
<html lang="es">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#e9f0e9">
<title>Aire UAI · monitor ambiental</title>
<style>
:root{font-family:ui-sans-serif,system-ui,-apple-system,"Segoe UI",sans-serif;color:#183129;background:#edf2ed}
*{box-sizing:border-box}body{margin:0;min-height:100vh;display:grid;place-items:center;padding:20px}
.app{width:min(100%,420px);min-height:740px;background:#f8faf6;border:1px solid #dce5dc;border-radius:34px;box-shadow:0 22px 70px #244b3422;overflow:hidden}
header{display:flex;align-items:center;justify-content:space-between;padding:26px 27px 8px}
.brand{font-size:12px;font-weight:800;letter-spacing:.15em;color:#557569}.live{font-size:11px;font-weight:750;padding:8px 10px;border-radius:20px;background:#e2eee3;color:#2b6c4b}
.live.off{background:#f6e7e2;color:#a75543}
main{padding:4px 24px 28px}.eyebrow{font-size:12px;color:#658076;margin:8px 0 0}.hero{text-align:center;padding:18px 0 24px}
.hero h1{font-size:30px;letter-spacing:-.045em;margin:8px 0 2px}.subtitle{font-size:13px;color:#688076;margin:0}
.face-wrap{height:192px;display:grid;place-items:center;position:relative}
.face-wrap:before{content:"";position:absolute;width:178px;height:178px;border-radius:50%;background:#c9e8d0;filter:blur(21px);opacity:.7}
.face{position:relative;width:146px;height:146px;border-radius:50%;display:grid;place-items:center;background:#a7dcad;border:5px solid #f8faf6;box-shadow:0 13px 35px #3f705038, inset 0 -9px 0 #588b6922;transition:background .25s}
.face svg{width:105px;height:105px;fill:none;stroke:#224a32;stroke-width:5;stroke-linecap:round}
.face.medium{background:#f3d483}.face.medium svg{stroke:#624d28}
.face.high{background:#efa89b}.face.high svg{stroke:#703e39}
.face.unknown{background:#dce2de}.face.unknown svg{stroke:#69756e}
.state{font-size:24px;font-weight:800;letter-spacing:-.035em;margin:0}.hint{font-size:13px;line-height:1.45;color:#61776b;min-height:38px;margin:8px auto 0;max-width:310px}
.metrics{display:grid;grid-template-columns:1fr 1fr;gap:12px}
.metric{border:1px solid #e4eae4;background:#fff;text-align:left;border-radius:20px;padding:17px 15px;min-height:126px;box-shadow:0 4px 15px #2c503508;cursor:pointer;color:inherit}
.metric:focus-visible,.action:focus-visible{outline:3px solid #22784a;outline-offset:2px}
.metric .k{display:flex;justify-content:space-between;font-size:12px;color:#64776a;font-weight:700}
.metric .v{display:block;font-size:29px;font-weight:800;letter-spacing:-.04em;margin:16px 0 0}.metric .u{font-size:12px;font-weight:600;color:#728078}
.metric .more{font-size:14px;color:#708576}.section-title{display:flex;align-items:center;justify-content:space-between;margin:26px 1px 11px}
.section-title h2{font-size:16px;margin:0;letter-spacing:-.02em}.section-title small{color:#73877a;font-size:11px}
.chart{background:#fff;border:1px solid #e4eae4;border-radius:20px;padding:12px 13px}
canvas{width:100%;height:124px;display:block}.chart-note{color:#75887c;font-size:11px;line-height:1.35;margin:8px 1px 0}
.footer{border-top:1px solid #e3eae2;margin-top:22px;padding-top:16px;display:flex;justify-content:space-between;align-items:center;gap:10px}
.footer span{font-size:11px;color:#748578}.action{font-size:12px;font-weight:750;color:#276849;text-decoration:none;border:1px solid #c9ddce;border-radius:10px;padding:9px 11px}
dialog{border:0;border-radius:22px;padding:25px;width:min(90vw,380px);box-shadow:0 20px 75px #102b2270;color:#183129}
dialog::backdrop{background:#18312988}dialog h2{margin:0 0 12px;font-size:21px}dialog p{font-size:14px;line-height:1.55;color:#4d675a}
dialog button{border:0;border-radius:12px;background:#214f37;color:#fff;font-weight:700;padding:11px 17px}
@media(max-width:500px){body{display:block;padding:0;background:#f8faf6}.app{width:100%;min-height:100dvh;border:0;border-radius:0;box-shadow:none}}
</style>
</head>
<body>
<div class="app">
<header><div class="brand">AIRE UAI <span aria-hidden="true">✳</span></div><div class="live off" id="live">CONECTANDO</div></header>
<main>
<p class="eyebrow">MONITOR AMBIENTAL · CCS811</p>
<section class="hero" aria-live="polite">
<div class="face-wrap"><div class="face unknown" id="face">
<svg viewBox="0 0 100 100" role="img" aria-label="Estado del aire"><path d="M32 42v5M68 42v5" /><path id="mouth" d="M34 69H66"/></svg>
</div></div>
<h1>Así está el aire</h1><p class="state" id="state">Conectando…</p>
<p class="hint" id="hint">Buscando el dispositivo. Mantén este celular conectado a Aire-UAI.</p>
</section>
<div class="metrics">
<button class="metric" type="button" data-info="tvoc"><span class="k">TVOC <span class="more">ⓘ</span></span><span class="v" id="tvoc">-- <span class="u">ppb</span></span></button>
<button class="metric" type="button" data-info="eco2"><span class="k">eCO₂ estimado <span class="more">ⓘ</span></span><span class="v" id="eco2">-- <span class="u">ppm</span></span></button>
</div>
<div class="section-title"><h2>Evolución del TVOC</h2><small>Registro cada 60 s</small></div>
<div class="chart"><canvas id="graph" aria-label="Gráfico de TVOC histórico"></canvas><p class="chart-note" id="chart-note">Esperando registros reales del sensor.</p></div>
<div class="footer"><span id="update">Sin lecturas todavía</span><a class="action" href="/historico.csv" download>Descargar CSV</a></div>
</main></div>
<dialog id="detail"><h2 id="detail-title"></h2><p id="detail-body"></p><button id="close" type="button">Entendido</button></dialog>
<script>
(function(){
'use strict';
var face=document.getElementById('face'),mouth=document.getElementById('mouth');
var live=document.getElementById('live'),state=document.getElementById('state'),hint=document.getElementById('hint');
var chart=document.getElementById('graph'),ctx=chart.getContext('2d');
var info={
 tvoc:{title:'¿Qué es TVOC?',body:'Son compuestos orgánicos volátiles totales estimados por el CCS811, en ppb. Productos de limpieza, pinturas y perfumes pueden liberar algunos de ellos. Según la EPA, los efectos sobre la salud dependen del compuesto, la cantidad y la exposición; algunos pueden irritar ojos o vías respiratorias. Esta lectura no identifica sustancias ni diagnostica un riesgo médico.'},
 eco2:{title:'¿Qué significa eCO₂?',body:'Es CO₂ equivalente estimado a partir de gases detectados por el CCS811, en ppm. No es una medición directa de CO₂ y no demuestra si la ventilación de la sala es adecuada. Se muestra como tendencia complementaria; para medir CO₂ real se necesita un sensor apropiado, como uno NDIR.'}
};
document.querySelectorAll('[data-info]').forEach(function(button){button.addEventListener('click',function(){var x=info[button.dataset.info];document.getElementById('detail-title').textContent=x.title;document.getElementById('detail-body').textContent=x.body;document.getElementById('detail').showModal()})});
document.getElementById('close').addEventListener('click',function(){document.getElementById('detail').close()});
function mood(kind,label,desc){
 face.className='face '+kind;state.textContent=label;hint.textContent=desc;
 mouth.setAttribute('d',kind==='low'?'M28 63Q50 83 72 63':kind==='high'?'M28 76Q50 54 72 76':'M34 69H66');
}
function number(id,value,unit){document.getElementById(id).innerHTML=(typeof value==='number'?String(value):'--')+' <span class="u">'+unit+'</span>'}
function render(data){
 live.textContent='EN VIVO';live.className='live';number('tvoc',data.tvoc,'ppb');number('eco2',data.eco2,'ppm');
 var e=data.estado;
 if(e==='VERDE')mood('low','Cambio bajo · 😊','TVOC cerca de la referencia inicial del dispositivo.');
 else if(e==='AMARILLO')mood('medium','Cambio medio · 😐','Aumentó el TVOC respecto de la referencia; observa la tendencia.');
 else if(e==='ROJO')mood('high','Cambio alto · ☹️','El TVOC aumentó más. Revisa posibles fuentes y ventila si corresponde.');
 else if(e==='CALENTANDO')mood('unknown','Acondicionando · ◌','Lectura disponible; faltan '+data.restante_s+' s para estabilizar la referencia.');
 else if(e==='REFERENCIA')mood('unknown','Tomando referencia · ◌',data.muestras_base+'/30 muestras de TVOC para comparar cambios.');
 else mood('unknown',e==='SIN_SENSOR'?'Sensor no detectado':'Revisar sensor','Comprueba alimentación, WAKE a GND y conexiones I²C.');
 document.getElementById('update').textContent='Sesión '+data.sesion+' · '+(data.uptime_s||0)+' s desde encendido'+(data.registrando?'':' · sin almacenamiento');
}
function offline(){live.textContent='SIN CONEXIÓN';live.className='live off';number('tvoc',null,'ppb');number('eco2',null,'ppm');mood('unknown','Sin conexión','Conecta el celular a la red Aire-UAI y abre 192.168.4.1.');document.getElementById('update').textContent='No se reciben datos';}
async function update(){
 try{var r=await fetch('/api/estado',{cache:'no-store'});if(!r.ok)throw Error('HTTP');render(await r.json())}catch(e){offline()}
}
function draw(rows){
 var ratio=window.devicePixelRatio||1,w=chart.clientWidth,h=chart.clientHeight;chart.width=Math.round(w*ratio);chart.height=Math.round(h*ratio);ctx.setTransform(ratio,0,0,ratio,0,0);ctx.clearRect(0,0,w,h);
 ctx.strokeStyle='#e7eee8';ctx.lineWidth=1;for(var j=0;j<3;j++){ctx.beginPath();ctx.moveTo(8,16+j*(h-32)/2);ctx.lineTo(w-8,16+j*(h-32)/2);ctx.stroke()}
 if(!rows.length){ctx.fillStyle='#75887c';ctx.font='12px system-ui';ctx.fillText('Sin datos guardados',14,h/2);return}
 var vals=rows.map(function(x){return x.tvoc}),min=Math.min.apply(null,vals),max=Math.max.apply(null,vals);if(max===min){min=Math.max(0,min-10);max+=10}
 ctx.strokeStyle='#428e64';ctx.lineWidth=3;ctx.lineJoin='round';ctx.beginPath();rows.forEach(function(x,i){var px=12+i*(w-24)/Math.max(1,rows.length-1),py=h-18-(x.tvoc-min)*(h-36)/(max-min);if(i)ctx.lineTo(px,py);else ctx.moveTo(px,py)});ctx.stroke();
 document.getElementById('chart-note').textContent=rows.length+' registros recientes · TVOC '+min+'–'+max+' ppb. La escala es relativa.';
}
async function history(){try{var r=await fetch('/historico.csv',{cache:'no-store'});if(!r.ok)throw Error('HTTP');var lines=(await r.text()).trim().split(/\r?\n/).slice(1);var rows=lines.map(function(line){var c=line.split(',');return {tvoc:Number(c[2])}}).filter(function(x){return Number.isFinite(x.tvoc)&&x.tvoc>=0}).slice(-60);draw(rows)}catch(e){draw([])}}
update();history();setInterval(update,2000);setInterval(history,60000);window.addEventListener('resize',history);
})();
</script>
</body></html>
)HTML";
