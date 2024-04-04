#ifndef BATTERYGAUGEHTML_H
#define BATTERYGAUGEHTML_H
//
const char* batteryGaugeHTML = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Battery</title>
</head>
<body>
    <h1>ESP32 Battery</h1>
    <p>Battery: <span style="color: red;"><span id="battery">Loading...</span> &#8451;</span></p>
    <script>
        function fetchBattery(){
            fetch("/battery")
                .then(response => response.text())
                .then(data => {
                    document.getElementById("battery").textContent = data;
                });
        }
        setInterval(fetchBattery, 100);
    </script>
</body>
</html>
)=====";

#endif