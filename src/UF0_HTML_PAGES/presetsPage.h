#ifndef PRESETSPAGE_h
#define PRESETSPAGE_h

const char* presetsHTML = R"=====(

  <!DOCTYPE html>
  <html>
  <head>
    <title>Jamminnnn'</title>
    <style>
      h1 {text-align: center;}
      p {text-align: center;}
      form {text-align: center;}
      {text-align: center;}
    </style>
  </head>
  <body>
    <h1>~~~~~ Jam the object ~~~~~</h1>
    <p>
      <a href=/startX\">Start</a>/<a href=/stopX\">Stop</a>/<a href=/soloX\">Solo</a> X transmition (<span id="pitchTransmit">%pitchTransmit%</span>)<br>
      <a href=/startY\">Start</a>/<a href=/stopY\">Stop</a>/<a href=/soloY\">Solo</a> Y transmition (<span id="rollTransmit">%rollTransmit%</span>)<br>
      <a href=/startZ\">Start</a>/<a href=/stopZ\">Stop</a>/<a href=/soloZ\">Solo</a> Z transmition (<span id="yawTransmit">%yawTransmit%</span>)<br>
      <a href=/enableAll\">Enable</a> all transmitions<br>
      <a href=/disableAll\">Disable</a> all transmitions<br>
      <a href=/save\">Save</a> preset<br>
      <a href=/load\">Load</a> preset<br>
      <span id="isConnected">%isConnected%</span>
    </p>
    <script>
      function updatePage(){
        fetch("/updateX")
          .then(response => response.text())
          .then(data => {
            document.getElementById("pitchTransmit").textContent = data;
          });
        fetch("/updateY")
          .then(response => response.text())
          .then(data => {
            document.getElementById("rollTransmit").textContent = data;
          });
        fetch("/updateZ")
          .then(response => response.text())
          .then(data => {
            document.getElementById("yawTransmit").textContent = data;
          });
        fetch("/updateWiFi")
          .then(response => response.text())
          .then(data => {
            document.getElementById("isConnected").textContent = data;
          });
      }
      setInterval(updatePage, 10);
    </script>
  </body>
  </html>

)=====";

#endif