#ifndef UF0_WEBSITE_API_h
#define UF0_WEBSITE_API_h

#define CLEAR_STRING true

const String function_openPage PROGMEM = R"rawliteral(function openPage(evt, pageNumber){
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
})rawliteral";

const String function_updatePage PROGMEM = R"rawliteral(function updatePage(){
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
setInterval(updatePage, 10);)rawliteral";

const String script_input PROGMEM = R"rawliteral(var coll = document.getElementsByClassName("collapsible");
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
document.getElementById("defaultOpen").click();)rawliteral";

const String style_input PROGMEM = R"rawliteral(  table, th, td{
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
  {text-align: center;})rawliteral";

const String button_tabs PROGMEM = R"rawliteral(  <button class="tablinks" onclick="openPage(event, 'page1')"id="defaultOpen">page1</button>
  <button class="tablinks" onclick="openPage(event, 'page2')">page2</button>
  <button class="tablinks" onclick="openPage(event, 'page3')">page3</button>
  <button class="tablinks" onclick="openPage(event, 'page4')">page4</button>
  <button class="tablinks" onclick="openPage(event, 'page5')">page5</button>
  <button class="tablinks" onclick="openPage(event, 'page6')">page6</button>
  <button class="tablinks" onclick="openPage(event, 'page7')">page7</button>
  <button class="tablinks" onclick="openPage(event, 'page8')">page8</button>
  )rawliteral";

class WebsiteAPI{
	private:
		String html = "";
		String head = "";
		String title = "";
		String style = "";
		String body = "";
		String header1 = "";
		String header2 = "";
		String header3 = "";
		String division = "";
		String paragraph = "";
		String page = "";
		String instrument = "";
		String root = "";
		String scale = "";
		String chord = "";
		String control = "";
		String tabs = "";
		String button = "";
		String script = "";
		String function = "";
		String line = "";
		String column = "";
		String table = "";

	public:
		String readHTML(){
			return html;
		}
		
		void appendHTML(){
			html += "<!DOCTYPE html>\n<html>\n";
			html += head;
			html += body;
			html += "\n</html>";
		}

		void appendHead(){
			head += "<head>\n";
			head += title;
			head += style;
			head += "\n</head>";
		}

		void appendTitle(String titleInput){
			title += "<title>";
			title += titleInput;
			title += "</title>\n";
		}

		void appendStyle(String styleInput){
			style += "<style>\n";
			style += styleInput;
			style += "\n</style>";
		}

		void appendBody(){
			body += "\n<body>\n";
			body += header1;
			body += tabs;
			body += page;
			body += script;
			body += "</body>";
		}

		void appendHeader1(String headerInput){
			header1 = "<h1>";
			header1+= headerInput;
			header1 += "</h1>\n";
		}

		void appendHeader2(String headerInput){
			header2 = "<h2>";
			header2 += headerInput;
			header2 += "</h2>\n";
		}

		void appendHeader3(String headerInput){
			header3 = "<h3>";
			header3 += headerInput;
			header3 += "</h3>\n";
		}

		void appendDiv(String divID, String divClass, String divInput){
			division = "<div id=\"";
			division += divID;
			division += "\" class=\"";
			division += divClass;
			division += "\">\n";
			division += divInput;
			division += "</div>\n";
		}

		void appendTabs(){
			appendDiv("button_tabs_div", "tab", button_tabs);
			tabs += division;
		}
		
		void appendTabContent(int i){
			// appendHeader3("Preset #" + i);
			// appendButton("button", "collapsible", "Instrument", "btnipnyt");
			// // appendDiv("content"){
			// // 	appendParagraph(){
			// // 		appendTable(){
						
			// // 		}
			// // 		appendButton("addControl", "editControl(event, 'addControl')", "+");
			// // 	}
			// // }
			// appendButton("button", "collapsible", "Control", "btninpututt");
			// appendDiv("content", "divclassss", "sivvinpyyytt");
		}

		void appendInstrument(){
						appendHeader3("Instrument Name");
					appendColumn(header3, CLEAR_STRING);
				appendLine(column, CLEAR_STRING);
				instrument += line;
					appendColumn("Note On", CLEAR_STRING);
					appendColumn("Trigger/Hit");
					appendColumn("Hold/Toggle/Bang");
					appendColumn("*(Duration)");
					appendColumn("MIDI Note");
				appendLine(column, CLEAR_STRING);
				instrument += line;
					appendRoot();
				appendLine(root, CLEAR_STRING);
				instrument += line;
				// 	appendScale();
				// appendLine(scale, CLEAR_STRING);
				// instrument += line;
			// appendChord();
			// instrument += chord;
			// appendTable(instrument);
			// instrument += table;
		}

		void appendRoot(){
			appendButton("button", "collapsible", "", "Root");
			root += button;
							appendHeader3("Controler Root");
						appendColumn(header3, CLEAR_STRING);
							appendHeader3("Motion");
						appendColumn(header3);
							appendHeader3("Motion Limits");
						appendColumn(header3, 0, 3);
					appendLine(column, CLEAR_STRING);
						appendColumn("(L, R, Both)", CLEAR_STRING);
						appendColumn("YAW");
						appendColumn("Grid Length?");
						appendColumn("X Mmin");
						appendColumn("X Max");
						appendColumn("VG X?");
					appendLine(column);
						appendColumn("(L, R, Both)", CLEAR_STRING);
						appendColumn("PITCH");
						appendColumn("Grid Height?");
						appendColumn("Y Mmin");
						appendColumn("Y Mmax");
						appendColumn("VG Y?");
					appendLine(column);
				appendTable(line);
			appendDiv("root_div", "content", table);
			root += division;
		}

		void appendScale(){
			appendButton("button", "collapsible", "", "Scale");
			scale += button;
						appendColumn("**Controler Scale**", CLEAR_STRING);
						appendColumn("**Motion**");
						appendColumn("**Motion Limits**", 0, 3);
					appendLine(column, CLEAR_STRING);
						appendColumn("(L, R, Both)", CLEAR_STRING);
						appendColumn("YAW");
						appendColumn("Grid Length?");
						appendColumn("X Mmin");
						appendColumn("X Max");
						appendColumn("VG X?");
					appendLine(column);
						appendColumn("(L, R, Both)", CLEAR_STRING);
						appendColumn("PITCH");
						appendColumn("Grid Height?");
						appendColumn("Y Mmin");
						appendColumn("Y Mmax");
						appendColumn("VG Y?");
					appendLine(column);
				appendTable(line);	
			appendDiv("root_div", "content", table);
			scale += division;
		}

		void appendChord(){
			appendButton("button", "collapsible", "", "Chord");
			chord += button;
				appendColumn("**Controler Chord**", CLEAR_STRING);
				appendColumn("**Motion**");
				appendColumn("**Motion Limits**", 0, 3);
			appendLine(column, CLEAR_STRING);
				appendColumn("(L, R, Both)", CLEAR_STRING);
				appendColumn("YAW");
				appendColumn("Grid Length?");
				appendColumn("X Mmin");
				appendColumn("X Max");
				appendColumn("VG X?");
			appendLine(column);
				appendColumn("(L, R, Both)", CLEAR_STRING);
				appendColumn("PITCH");
				appendColumn("Grid Height?");
				appendColumn("Y Mmin");
				appendColumn("Y Mmax");
				appendColumn("VG Y?");
			appendLine(column);
			appendTable(line);
			chord += table;
		}

		void appendColumn(String column_input, bool clear = false, int numOfColumns = 1){
			if (clear){
				column = "<td colspan=\"";
				column += numOfColumns;
				column += "\">";
			}
			else{
				column += "<td colspan=\"";
				column += numOfColumns;
				column += "\">";
			}
			column += column_input;
			column += "</td>\n";
		}

		void appendLine(String line_input, bool clear = false){
			if (clear){
				line = "<tr>\n";
			}
			else{
				line += "<tr>\n";
			}
			line += line_input;
			line += "</tr>\n";
		}

		void appendTable(String table_input){
			table = "<table style=\"width:100%\">\n";
			table += table_input;
			table += "</table>\n";
		}

		void appendControl(){
		}

		void appendControlContent(){
			// appendHeader3("Instrument Name");
			// control_content += header3;
			// appendDiv("", "", control_content);
		}

		void appendPage(){
				appendHeader3("Preset: Aruma");
				page += header3;
		// Instrument section
				appendButton("button", "collapsible", "", "Instrument");
				page += button;
				appendInstrument();
			// DBG("FFFFFFFFFFFFFFFFFFF INSTRUUUUUUU:", instrument);
			appendTable(instrument);
			// DBG("GGGGGGGGGGGGGGGGGG INSTRUUUUUUU:", table);
		// // Control section
		// 	appendButton("button", "collapsible", "", "Control");
		// 	page += button;
		// 	appendControl();
		// // Append: Instrument + Section -> Page
		// 	instrument += control;
			appendParagraph(table);
			appendDiv("intrument_content", "content", paragraph);
			page += division;
			// For 8 pages (Presets)
			// for (int i=0; i<8; i++){
			// 	appendTabContent(i);
			// }
		}
		
		void appendButton(String btnType, String btnClass, String btnOnClick, String btnInput){
			button = "<button id=\"";
			button += btnType;
			button += "\" class=\"";
			button += btnClass;
			button += "\" onclick=\"";
			button += btnOnClick;
			button += "\">";
			button += btnInput;
			button += "</button>\n";
		}

		void appendParagraph(String paragraph_input){
			paragraph = "<p>\n";
			paragraph += paragraph_input;
			paragraph += "\n</p>\n";
		}

		void appendScript(){
			script += "<script>\n";
			script += function;
			script += script_input;
			script += "\n</script>\n";
		}

		void appendFunction(String functionInput){
			function += functionInput;
			function += "\n\n";
		}

		void setupWebSite(){
				// H E A D
					appendTitle("Jamminnnn'");
					appendStyle(style_input);
				appendHead();

				// B O D Y
					appendHeader1("~~~~~ Jammm the object ~~~~~");
					appendTabs();
					appendPage();
						// appendFunction(function_openPage);
						// appendFunction(function_updatePage);
					appendScript();
				appendBody();

			// H T M L
			appendHTML();

			/**** HTML Structure ****
				<html>
				<head>
					<title>Jamminnnn'</title>
					<style>
					</style>
				</head>
				<body>
					<div>
						<button>
						</button>
					</div>
					<script>
						<function>
						</function>
					</script>
				</body>
			*/
		}
};

#endif