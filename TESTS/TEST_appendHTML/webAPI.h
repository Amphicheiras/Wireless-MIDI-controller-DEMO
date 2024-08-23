#ifndef UF0_WEBSITE_API_h
#define UF0_WEBSITE_API_h

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

class WebsiteAPI{
	private:

	public:

    WebsiteAPI(){};
    WebsiteAPI(bool xx){
      setup();
    };
    void setup(){
        appendTitle("Jaamminngua");
        appendStyle("styleZ");
        appendHead();
        // appendHeader("~~~~~ Jammm the object ~~~~~");
        // appendDiv("page_i", "tabcontent");
        // appendButton("button", "collapsible", "Control");
        appendScript("");
        appendFunction("");
      appendBody();
      appendHTML();

      Serial.print(html);
    }
		void appendHTML(){
			html += "<!DOCTYPE html>\n<html>";
			html += head;
			html += body;
			html += "\n</html>";
		}

		void appendHead(){
			head += "\n<head>";
			head += title;
			head += style;
			head += "\n</head>";
		}

		void appendBody(){
			body += "\n<body>";
			body += header;
			body += division;
			body += "\n</body>";
		}

		void appendTitle(char* titleInput){
			title += "\n  <title>";
			title += titleInput;
			title += "</title>";
		}

		void appendStyle(char* styleInput){
			style += "\n  <style>\n    ";
			style += styleInput;
			style += "\n  </style>";
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

			// // H T M L
			// appendHTML(){

			// 	// H E A D
			// 	appendHead(){
			// 		appendTitle();
			// 		appendStyle();
			// 	}

			// 	// B O D Y
			// 	appendBody(){
			// 		appendheader1("~~~~~ Jammm the object ~~~~~");
			// 		appendDiv("tab"){
			// 			// For 8 page tabs
			// 			for (int i=0; i<8; i++){
			// 				appendButton("page_i", "tablinks", "openPage(event, 'page_i')", "Page i");
			// 			}
			// 		}

			// 		// For 8 pages (Presets)
			// 		for (int i=0; i<8; i++){
			// 			appendDiv("page_i", "tabcontent"){
			// 				appendHeader3("Preset #%d", i);
			// 				appendButton("button", "collapsible", "Instrument")
			// 				appendDiv("content"){
			// 					appendParagraph(){
			// 						appendTable(){
										
			// 						}
			// 						appendButton("addControl", "editControl(event, 'addControl')", "+");
			// 					}
			// 				}
			// 				appendButton("button", "collapsible", "Control")
			// 				appendDiv("content"){
			// 					appendpar

			// 				}
			// 			}
			// 		}

			// 		appendScript(){
			// 			appendFunction("openPage"){

			// 			}
			// 			appendFunction("updatePage"){

			// 			}
			// 		}
			// 	}
			// }
		}
};

#endif