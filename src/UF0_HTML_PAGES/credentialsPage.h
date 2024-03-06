#ifndef CREDENTIALSPAGE_h
#define CREDENTIALSPAGE_h

const char* credentialsHTML = R"=====(

  <!DOCTYPE html>
  <html>
  <head>
    <title>UF0 Setup</title>
    <style>
      h1 {text-align: center;}
      p {text-align: center;}
      form {text-align: center;}
      {text-align: center;}
      button {
        padding: 15px;
        font-size: 20px;
      }
      input {
        padding: 8px;
        margin-bottom: 10px;
        font-size: 16px;
      }
    </style>
  </head>
  <body>
    <h1>~~~ Enter WiFi credentials ~~~</h1>
    <form id="wifiForm">
      <label for="ssid">Wi-Fi SSID:</label><br>
      <input type="text" id="ssid" name="ssid"><br>
      <label for="password">Wi-Fi Password:</label><br>
      <input type="password" id="password" name="password"><br><br>
      <button id="connectBtn">Log In</button>
      <button id="jammBtn">Jamm!</button>
      <a href=/redirect\">Jam!</a><br>
      <span id="isConnected">%isConnected%</span>
    </form>
    <script>
      document.getElementById("connectBtn").onclick = function() {
        var ssid = document.getElementById("ssid").value;
        var password = document.getElementById("password").value;
        fetch('/connect?ssid=' + ssid + '&password=' + password);
        return false;
      };
      document.getElementById("jammBtn").onclick = function() {
        fetch('/redirect');
        return false;
      };
      function updatePage(){
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