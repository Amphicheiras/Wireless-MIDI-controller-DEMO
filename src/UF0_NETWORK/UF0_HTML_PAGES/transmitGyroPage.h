#ifndef PRESETSPAGE_h
#define PRESETSPAGE_h

const char* transmitGyroHTML = R"=====(

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
      <a href=/startX\">Start</a>/<a href=/stopX\">Stop</a>/<a href=/soloX\">Solo</a> X transmition<br>
      <a href=/startY\">Start</a>/<a href=/stopY\">Stop</a>/<a href=/soloY\">Solo</a> Y transmition<br>
      <a href=/startZ\">Start</a>/<a href=/stopZ\">Stop</a>/<a href=/soloZ\">Solo</a> Z transmition<br>
      <a href=/enableAll\">Enable</a> all transmitions<br>
      <a href=/disableAll\">Disable</a> all transmitions<br>
      <a href=/save\">Save</a> preset<br>
      <a href=/load\">Load</a> preset<br>
    </p>
  </body>
  </html>

)=====";

#endif