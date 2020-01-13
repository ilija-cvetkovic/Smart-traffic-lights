var automobil = document.getElementById('automobil');
var kamion = document.getElementById('kamion');
var autobus = document.getElementById('autobus');
var temp = document.getElementById('temp');
var vreme = document.getElementById('vreme');
var ukupno = document.getElementById('ukupno');

var ref = firebase.database().ref();
ref.on('value', snap => {
    console.log(snap.val());
    //automobil
    var auto = snap.child('automobil').val();
    automobil.innerHTML = auto;
    //kamion
    var k = snap.child('kamion').val();
    kamion.innerHTML = k;
    //autobus
    var bus = snap.child('autobus').val();
    autobus.innerHTML = bus;
    //temperatura
    var t = snap.child('temp').val();
    temp.innerHTML = t + "°C";
    //ukupna prolaznost vozila
    var u = snap.child('ukupno').val();
    ukupno.innerHTML = u;
});

function startTime() {
      var today = new Date();
      var h = today.getHours();
      var m = today.getMinutes();
      var s = today.getSeconds();
      m = checkTime(m);
      s = checkTime(s);
      document.getElementById('vreme').innerHTML =
      h + ":" + m + ":" + s;
      var t = setTimeout(startTime, 500);
      ref.update({
            '/vreme': h + ":" + m + ":" + s
      });
}
function checkTime(i) {
  if (i < 10) {i = "0" + i};  
  return i;
}

