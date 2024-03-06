#include <Arduino.h>
#include <StreamString.h>

String html = "";
String head = "";
String title = "";
String style = "";
String body = "";
String header = "";
String division = "";
String button = "";
String script = "";
String function = "";

void appendHTML(){
	html += "<!DOCTYPE html>\n<html>";
	html += head;
	html += body;
	html += "</html>";
}

void appendHead(){
	head += "<head>";
	head += title;
	head += style;
	head += "</head>";
}

void appendTitle(String titleInput){
	title += "<title>";
	title += titleInput;
	title += "</title>";
}

void appendStyle(String styleInput){
	style += "<style>";
	style += styleInput;
	style += "</style>";
}

void appendBody(String body){
	body += "<body>";
	body += header;
	body += division;
	body += "</body>";
}

void appendHeader1(String headerInput){
	header += "<h1>";
	header += headerInput;
	header += "</h1>";
}

void appendHeader2(String headerInput){
	header += "<h2>";
	header += headerInput;
	header += "</h2>";
}

void appendHeader3(String headerInput){
	header += "<h3>";
	header += headerInput;
	header += "</h3>";
}

void appendDiv(String divID, String divClass, String divInput){
	division += "<div id=\"";
	division += divID;
	division += "\" class=\"";
	division += divClass;
	division += "\">";
	division += divInput;
	division += "</div>";
}

void appendParagraph(){

}
void appendButton(String btnType, String btnClass, String btnOnClick, String btnInput){
	button += "<button id=\"";
	button += btnType;
	button += "\" class=\"";
	button += btnClass;
	button += "\" onclick=\"";
	button += btnOnClick;
	button += "\">";
	button += btnInput;
	button += "</button>";
}

void appendScript(String scriptInput){
	script += "<script>";
	script += scriptInput;
	script += "</script>";
}

void appendFunction(String functionInput){
	function += "<function>";
	function += functionInput;
	function += "</function>";
}


void setupWebSite(){
	// **** HTML Structure ****
	// 	<html>
	// 	<head>
		// 	<title>Jamminnnn'</title>
		// 	<style>
		// 	</style>
	// 	</head>
	// 	<body>
		//  <div>
			//	<button>
			//	</button>
		//  </div>
		//	<script>
			//	<function>
			//	</function>
		//	</script>
	//	</body>

	// H T M L
	appendHTML(){

		// H E A D
		appendHead(){
			appendTitle();
			appendStyle();
		}

		// B O D Y
		appendBody(){
			appendheader1("~~~~~ Jammm the object ~~~~~");
			appendDiv("tab"){
				appendButton("page1", "tablinks", "openPage(event, 'page1')", "Page 1");
				appendButton("page2", "tablinks", "openPage(event, 'page2')", "Page 2");
				appendButton("page3", "tablinks", "openPage(event, 'page3')", "Page 3");
				appendButton("page4", "tablinks", "openPage(event, 'page4')", "Page 4");
				appendButton("page5", "tablinks", "openPage(event, 'page5')", "Page 5");
				appendButton("page6", "tablinks", "openPage(event, 'page6')", "Page 6");
				appendButton("page7", "tablinks", "openPage(event, 'page7')", "Page 7");
				appendButton("page8", "tablinks", "openPage(event, 'page8')", "Page 8");
			}

			// For 8 Pages (Presets)
			for (int i=0; i<8; i++){
				appendDiv("page_i", "tabcontent"){
					appendheader3("Preset #%d", i);
					appendButton("button", "collapsible", "Instrument")
					appendDiv("content"){
						appendParagraph(){
							appendTable(){
								
							}
							appendButton("addControl", "editControl(event, 'addControl')", "+");
						}
					}
					appendButton("button", "collapsible", "Control")
					appendDiv("content"){
						appendpar

					}
				}
			}

			appendScript(){
				appendFunction("openPage"){

				}
				appendFunction("updatePage"){

				}
			}
		}
	}
}