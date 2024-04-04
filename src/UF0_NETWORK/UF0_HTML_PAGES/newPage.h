#ifndef NEWPAGE_h
#define NEWPAGE_h

const char* credentialsHTML = R"=====(

  <!DOCTYPE html>
  <html>
  <head>
    <title>Jamminnnn'</title>
    <style>
      table, th, td{
        border:1px solid black;
      }
      .collapsible {
        background-color: #777;
        color: white;
        cursor: pointer;
        padding: 18px;
        width: 100%;
        border: none;
        text-align: left;
        outline: none;
        font-size: 15px;
      }
      .active, .collapsible:hover {
        background-color: #555;
      }
      .collapsible:after {
        content: '\002B';
        color: white;
        font-weight: bold;
        float: right;
        margin-left: 5px;
      }
      .active:after {
        content: "\2212";
      }
      .content {
        padding: 0 18px;
        display: none;
        overflow: hidden;
        background-color: #f1f1f1;
      }
      /* Style the tabs*/
      .tab{
        overflow: hidden;
        border: 1px solid #ccc;
        background-color: #f1f1f1;
      }
      /* Style the buttons inside the tab */
      .tab button{
        background-color: inherit;
        float: left;
        border: none;
        outline: none;
        cursor: pointer;
        padding: 14px 16px;
        transition: 0.3s;
        font-size: 17px;
      }
      /* Change background color of buttons on hover */
      .tab button:hover{
        background-color: #ddd;
      }
      /* Create an active/current tablink class */
      .tab button.active{
        background-color: #ccc;
      }
      h1 {text-align: center;}
      p {text-align: center;}
      form {text-align: center;}
      {text-align: center;}
    </style>
  </head>
  <body>
    <h1>~~~~~ Jammm the object ~~~~~</h1>
    <div class="tab">
      <button class="tablinks" onclick="openPage(event, 'page1')"id="defaultOpen">page1</button>
      <button class="tablinks" onclick="openPage(event, 'page2')">page2</button>
      <button class="tablinks" onclick="openPage(event, 'page3')">page3</button>
      <button class="tablinks" onclick="openPage(event, 'page4')">page4</button>
      <button class="tablinks" onclick="openPage(event, 'page5')">page5</button>
      <button class="tablinks" onclick="openPage(event, 'page6')">page6</button>
      <button class="tablinks" onclick="openPage(event, 'page7')">page7</button>
      <button class="tablinks" onclick="openPage(event, 'page8')">page8</button>
    </div>
    
    <!-- Page 1 -->
    <div id="page1" class="tabcontent">
      <h3>Preset: Aruna</h3>
      <button type="button" class="collapsible">Instrument</button>
      <div class="content">
        <p>
          <table style="width:100%">
            <tr>
              <th>Instrument Name</th>
            </tr>
            <tr>
              <th>Note On</th>
              <td>Trigger/Hit</td>
              <td>Hold/Toggle/Bang*</td>
              <td>*(Duration)</td>
              <td>MIDI Note</td>
              <td>MIDI CHANNEL</td>
            </tr>
            <td colspan="5">
              <button type="button" class="collapsible">Scale</button>
              <div class="content">
                  <table style="width:100%">
                      <tr>
                        <th>Controller</th>
                        <th>Motion</th>
                        <th colspan="3">Motion Limits</th>
                        <th>Root Note</th>
                      </tr>
                      <tr>
                        <td>(L,R,Both)</td>
                        <td>YAW</td>
                        <td>Grid Length?</td>
                        <td>X Mmin</td>
                        <td>X Mmax</td>
                        <td>
                          MIDI Note
                          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
                        </td>
                        <td>VG X?</td>
                      </tr>
                      <tr>
                        <td>(L,R,Both)</td>
                        <td>PITCH</td>
                        <td>Grid Height?</td>
                        <td>Y Mmin</td>
                        <td>Y Mmax</td>
                        <td>
                          Custom Note
                          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
                        </td>
                        <td>VG Y?</td>
                      </tr>
                  </table>
                  <table style="width:100%">
                    <div class="content">
                      <tr>
                        <th>
                          Note/St
                          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
                        </th>
                        <td>G-1/-5</td>
                        <td>Gm-1/-4</td>
                        <td>A-1/-3</td>
                        <td>Am-1/-2</td>
                        <td>B-1/-1</td>
                        <td>C0/0</td>
                        <td>Cm0/+1</td>
                        <td>D0/+2</td>
                        <td>Dm0/+3</td>
                        <td>E0/+4</td>
                        <td>F0/+5</td>
                        <td>Fm0/+6</td>
                      </tr>
                    </div>
                  </table>
                  </th>
              </div>
            </td>
              <td>
                On / Off
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
            <tr>
            <td colspan="5">
              <button type="button" class="collapsible">Octave</button>
            <div class="content">
                  <table style="width:100%">
                      <tr>
                        <th>Controller</th>
                        <th>Motion</th>
                        <th colspan="3">Motion Limits</th>
                      </tr>
                      <tr>
                        <td>(L,R,Both)</td>
                        <td>YAW</td>
                        <td>Grid Length?</td>
                        <td>X Mmin</td>
                        <td>X Mmax</td>
                        <td>VG X?</td>
                      </tr>
                      <tr>
                        <td>(L,R,Both)</td>
                        <td>PITCH</td>
                        <td>Grid Height?</td>
                        <td>Y Mmin</td>
                        <td>Y Mmax</td>
                        <td>VG Y?</td>
                      </tr>
                  </table>
                  </th>
                </div>
              </td>
              <td>
                On / Off
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
            </tr>
            <tr>
            <td colspan="5">
              <button type="button" class="collapsible">Chord</button>
            <div class="content">
                  <table style="width:100%">
                      <tr>
                        <th>Controller</th>
                        <th>Motion</th>
                        <th colspan="3">Motion Limits</th>
                      </tr>
                      <tr>
                        <td>(L,R,Both)</td>
                        <td>YAW</td>
                        <td>Grid Length?</td>
                        <td>X Mmin</td>
                        <td>X Mmax</td>
                        <td>VG X?</td>
                      </tr>
                      <tr>
                        <td>(L,R,Both)</td>
                        <td>PITCH</td>
                        <td>Grid Height?</td>
                        <td>Y Mmin</td>
                        <td>Y Mmax</td>
                        <td>VG Y?</td>
                      </tr>
                  </table>
                  </th>
                </div>
              </td>
              <td>
                On / Off
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
            </tr>
          </table> 
        </p>
      </div>
      <button type="button" class="collapsible">Controls</button>
      <div class="content">
        <p>
          <table style="width:100%">
            <tr>
              <th>Control Name</th>
              <th>Controller</th>
              <th>Motion</th>
              <th colspan="3">Motion Limits</th>
              <th colspan="2">MIDI Output</th>
              <th colspan="4">Edit</th>
            </tr>
            <tr>
              <td>Aruja</td>
              <td>(L,R, Both)</td>
              <td>PITCH</td>
              <td>Mmin</td>
              <td>Mmax</td>
              <td>
                Flip
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
              <td>CHANNEL</td>
              <td>CC</td>
              <td><button>Copy</button></td>
              <td><button>Paste</button></td>
              <td><button>Rename</button></td>
              <td><button>Remove</button></td>
            </tr>
            <tr>
              <td>Aruja</td>
              <td>(L,R, Both)</td>
              <td>ROLL</td>
              <td>Mmin</td>
              <td>Mmax</td>
              <td>
                Flip
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
              <td>CHANNEL</td>
              <td>CC</td>
              <td><button>Copy</button></td>
              <td><button>Paste</button></td>
              <td><button>Rename</button></td>
              <td><button>Remove</button></td>
            </tr>
            <tr>
              <td>Aruja</td>
              <td>(L,R, Both)</td>
              <td>YAW</td>
              <td>Mmin</td>
              <td>Mmax</td>
              <td>
                Flip
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
              <td>CHANNEL</td>
              <td>CC</td>
              <td><button>Copy</button></td>
              <td><button>Paste</button></td>
              <td><button>Rename</button></td>
              <td><button>Remove</button></td>
            </tr>
            <tr>
              <td>Aruja</td>
              <td>(L,R, Both)</td>
              <td>JOYSTICK X</td>
              <td>Mmin</td>
              <td>Mmax</td>
              <td>
                Flip
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
              <td>CHANNEL</td>
              <td>CC</td>
              <td><button>Copy</button></td>
              <td><button>Paste</button></td>
              <td><button>Rename</button></td>
              <td><button>Remove</button></td>
            </tr>
            <tr>
              <td>Aruja</td>
              <td>(L,R, Both)</td>
              <td>JOYSTICK Y</td>
              <td>Mmin</td>
              <td>Mmax</td>
              <td>
                Flip
          <input type="checkbox" id="flip" name="vehicle1" value="Bike">
              </td>
              <td>CHANNEL</td>
              <td>CC</td>
              <td><button>Copy</button></td>
              <td><button>Paste</button></td>
              <td><button>Rename</button></td>
              <td><button>Remove</button></td>
            </tr>
          </table>
          <button onclick="editControl(event, 'addControl')"id="addControl">+</button>
        </p>
      </div>
    </div>

    <script>
      function openPage(evt, pageNumber) {
        var i, tabcontent, tablinks;
        tabcontent = document.getElementsByClassName("tabcontent");
        for (i = 0; i < tabcontent.length; i++) {
          tabcontent[i].style.display = "none";
        }
        tablinks = document.getElementsByClassName("tablinks");
        for (i = 0; i < tablinks.length; i++) {
          tablinks[i].className = tablinks[i].className.replace(" active", "");
        }
        document.getElementById(pageNumber).style.display = "block";
        evt.currentTarget.className += " active";
      }
      
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
      
      var coll = document.getElementsByClassName("collapsible");
      var i;
      for (i = 0; i < coll.length; i++) {
        coll[i].addEventListener("click", function() {
          this.classList.toggle("active");
          var content = this.nextElementSibling;
          if (content.style.display === "block") {
            content.style.display = "none";
          } else {
            content.style.display = "block";
          }
        });
      }
      document.getElementById("defaultOpen").click();
    </script>
  </body>
  </html>

)=====";

#endif