#include "ofxUIJquery.h"

ofxUIJquery::ofxUIJquery(){

}

ofxUIJquery::~ofxUIJquery(){

}

string ofxUIJquery::place(ofxPanel &p){
    string b;
    ofLog()<<p.getNumControls();
    vector<string> fn = p.getControlNames();
    for(int  i = 0; i < fn.size(); i++) {
        ofLog()<<fn[i];
    }
    return b;
}

void ofxUIJquery::log(string _file,string _buffer){
    ofFile file;
    file.open(_file, ofFile::WriteOnly);
    file << _buffer;
    file.close();
}

void ofxUIJquery::onConnect( ofxLibwebsockets::Event& args ){
    ofLog()<<"on connected";
}

void ofxUIJquery::onOpen( ofxLibwebsockets::Event& args ){
    ofLog()<<"new connection open";
    sock_value = args.conn.getClientIP() + " - " + args.conn.getClientName();
}

void ofxUIJquery::onClose( ofxLibwebsockets::Event& args ){
    ofLog()<<"on close";
}

void ofxUIJquery::onIdle( ofxLibwebsockets::Event& args ){
    ofLog()<<"on idle";
}

void ofxUIJquery::onMessage( ofxLibwebsockets::Event& args ){
    if ( !args.json.isNull() ){
        sock_value = args.json.toStyledString();
    } else {
        sock_value = args.message;
    }
    response(sock_value);
}

void ofxUIJquery::onBroadcast( ofxLibwebsockets::Event& args ){
    ofLog()<<"got broadcast "<<args.message;
}

string ofxUIJquery::getResult(){
    return sock_value;
}

string ofxUIJquery::setStyle(STYLE TYPE) {
    string path;
    string css="jquery-ui-1.10.3.custom.min.css";
    switch(TYPE){
    case BLITZER:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/blitzer/"+css);;
        break;
    case CUPERTINO:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/cupertino/"+css);;
        break;
    case DARK_HIVE:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/dark_hive/"+css);;
        break;
    case FLICK:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/flick/"+css);;
        break;
    case LEFROG:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/lefrog/"+css);;
        break;
    case OVERCATS:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/overcats/"+css);;
        break;
    case SMOOTHESS:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/smoothess/"+css);;
        break;
    case SUNNY:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/sunny/"+css);;
        break;
    case VADER:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/vader/"+css);;
        break;
    case REDMOND:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/themes/redmond/"+css);;
        break;
    default:
        path=ofToDataPath("../../../../ofxUIJquery/src/jquery/jquery-ui.min.css");
        break;
    }
    return path;
}

string ofxUIJquery::prepareHeader(int _size){
    string buf;
    string time = ofGetTimestampString("%w, %e %b %Y %H:%M:%S GMT");

    buf+="HTTP/1.1 200 OK\n";
    buf+="Date: "+time+"\n";
    buf+="Server: Apache/2.2\n";
    buf+="Last-Modified: Fri, 20 Feb 2015 21:34:27 GMT\n";
    buf+="ETag: \"42aa183-151b-50f8bd1209f9d\"\n";
    buf+="Accept-Ranges: bytes\n";
    buf+="Content-Length: "+ofToString(_size)+"\n";
    buf+="Connection: close\n";
    buf+="Content-Type: text/html\n\n";
    return buf;
}

void ofxUIJquery::setup(string ADRESS, int port, int port2, ofPoint bg, STYLE TYPE) {
    LOCAL_IP_ADDRESS=ADRESS;
    PORT_CLIENT=port2;

    TCP.setup(port);
    TCP.setMessageDelimiter("</html>\r\n");

    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = getPort();
    options.bUseSSL = false;
    server.addListener(this);
    bSetup = server.setup( options );

    stringstream CSSbuffer;
    ifstream CSSfile(setStyle(TYPE).c_str());
    if(CSSfile.is_open()) {
        CSSbuffer << CSSfile.rdbuf();
    }
    stringstream jQuerybuffer;
    ifstream jQfile(ofToDataPath("../../../../ofxUIJquery/src/jquery/external/jquery/jquery.js").c_str());
    if(jQfile.is_open()) {
        jQuerybuffer << jQfile.rdbuf();
    }
    stringstream JUIbuffer;
    ifstream UIfile(ofToDataPath("../../../../ofxUIJquery/src/jquery/jquery-ui.min.js").c_str());
    if(UIfile.is_open()) {
        JUIbuffer << UIfile.rdbuf();
    }
    stringstream JSONbuffer;
    ifstream JSONfile(ofToDataPath("../../../../ofxUIJquery/src/jquery/jquery.json-2.2.min.js").c_str());
    if(JSONfile.is_open()) {
        JSONbuffer << JSONfile.rdbuf();
    }
    stringstream WEBSbuffer;
    ifstream WEBSfile(ofToDataPath("../../../../ofxUIJquery/src/jquery/simplewebscoket.js").c_str());
    if(WEBSfile.is_open()) {
        WEBSbuffer << WEBSfile.rdbuf();
    }

    buffer+="<html><head><title>ofxUIJquery</title>";
    buffer+="<style>";
    buffer+=CSSbuffer.str();
    buffer+="* { font-family: \"Arial Verdana\", Arial, Verdana; }";
    buffer+=".d_div { padding:25px; border-radius: 5px; box-shadow: 5px 5px 5px #888888; }";
    buffer+="input { height:40px;border-bottom-color: #b3b3b3;border-bottom-left-radius: 3px; border-bottom-right-radius: 3px;border-bottom-style: solid;";
    buffer+="border-bottom-width: 1px;border-left-color: #b3b3b3;border-left-style: solid;border-left-width: 1px;border-right-color: #b3b3b3;border-right-style: solid;";
    buffer+="border-right-width: 1px;border-top-color: #b3b3b3;border-top-left-radius: 3px;border-top-right-radius: 3px;border-top-style: solid;border-top-width: 1px; }";
    buffer+="div{margin-bottom:10px;}";
    buffer+="</style>";
    buffer+="<script>";
    buffer+=jQuerybuffer.str();
    buffer+="</script>";
    buffer+="<script>";
    buffer+=JUIbuffer.str();
    buffer+="</script>";
    buffer+="<script>";
    buffer+=JSONbuffer.str();
    buffer+="</script>";

    buffer+="<script>";
    buffer+=WEBSbuffer.str();

    /* prepare websocket */
    buffer+="var Server;";
    buffer+="function send( text ) { Server.send( 'message', text ); }";
    buffer+="$(document).ready(function() {";
    buffer+="console.log('Connecting...');";
    buffer+="Server = new FancyWebSocket('ws://"+LOCAL_IP_ADDRESS+":"+ofToString(PORT_CLIENT)+"');";
    buffer+="console.log(Server);";
    buffer+="Server.bind('open', function() { console.log( 'Connected.' ); });";
    buffer+="Server.bind('close', function( data ) { console.log( 'Disconnected.' ); });";
    buffer+="Server.bind('message', function( payload ) { console.log( payload ); });";
    buffer+="Server.connect();";
    buffer+="});";
    /**/

    buffer+="</script>";

    buffer+="</head>";
    string rgb = ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z);
    buffer+="<body style=\"background:rgb("+rgb+");\"><div style=\"width:50%;height:90%;padding:10px;\">";
    request = true;
}

int ofxUIJquery::getPort(){
    return PORT_CLIENT;
}

void ofxUIJquery::init(){
    buffer+="</div></body>";
    buffer+="</html>\r\n";
}

string ofxUIJquery::response(string value) {
    vector<string> sp = ofSplitString(value,":");
    string token,tvalue,name;

    if(sp[0]!="") token = sp[0]; //inutile
    if(sp[1]!="") tvalue = sp[1];//inutile

    if(token!=""&&tvalue!="") {
        /* string */
        if(token=="UIString"){
            name=sp[2];
            string c;
            ofLog()<<"UIString detect:"<<tvalue;
            for(int i = 0; i < stringParam.size(); i++){
                    if(stringParam[i]->getName()==name){
                        c+=tvalue;
                        stringParam[i]->set(name, c);
                    }
            }
        }
        /**/

        /* bool */
        if(token=="UIBool"){
            ofLog()<<"UIBool detect:"<<tvalue;
            for(int i = 0; i < boolParam.size(); i++){
                    if(boolParam[i]->getName()==tvalue){
                        boolParam[i]->set(tvalue, !boolParam[i]->get());
                    }
            }
        }
        /**/

        /* int */
        if(token=="UIinteger"){
            name=sp[2];
            ofLog()<<"UIinteger detect:"<<tvalue<<":"<<name;
            for(int i = 0; i < intParam.size(); i++){
                if(intParam[i]->getName()==name){
                    intParam[i]->set(name,ofToInt(tvalue));
                }
            }
        }
        /**/

        /* float */
        if(token=="UIFloat"){
            name=sp[2];
            ofLog()<<"UIFloat detect:"<<tvalue;
            for(int i = 0; i < floatParam.size(); i++){
                if(floatParam[i]->getName()==name){
                    floatParam[i]->set(name,ofToFloat(tvalue));
                }
            }
        }
        /**/

        /* vec2 */
        if(token=="XUIVec2"){
            name=sp[2];
            ofLog()<<"XUIVec2 detect!";
            for(int i = 0; i < vec2Param.size(); i++){
                if(vec2Param[i]->getName()==name){
                    vec2Param[i]->set(name,ofVec2f(ofToFloat(tvalue),vec2Param[i]->get().y));
                }
            }
        }
        if(token=="YUIVec2"){
            name=sp[2];
            ofLog()<<"YUIVec2 detect!";
            for(int i = 0; i < vec2Param.size(); i++){
                if(vec2Param[i]->getName()==name){
                    vec2Param[i]->set(name,ofVec2f(vec2Param[i]->get().x,ofToFloat(tvalue)));
                }
            }
        }
        /**/

        /* vec3 */
        if(token=="XUIVec3"){
            name=sp[2];
            ofLog()<<"XUIVec3 detect!";
            for(int i = 0; i < vec3Param.size(); i++){
                if(vec3Param[i]->getName()==name){
                    vec3Param[i]->set(name,ofVec3f(ofToFloat(tvalue),vec3Param[i]->get().y,vec3Param[i]->get().z));
                }
            }
        }
        if(token=="YUIVec3"){
            name=sp[2];
            ofLog()<<"YUIVec3 detect!";
            for(int i = 0; i < vec3Param.size(); i++){
                if(vec3Param[i]->getName()==name){
                    vec3Param[i]->set(name,ofVec3f(vec3Param[i]->get().x,ofToFloat(tvalue),vec3Param[i]->get().z));
                }
            }
        }
        if(token=="ZUIVec3"){
            name=sp[2];
            ofLog()<<"ZUIVec3 detect!";
            for(int i = 0; i < vec3Param.size(); i++){
                if(vec3Param[i]->getName()==name){
                    vec3Param[i]->set(ofVec3f(vec3Param[i]->get().x,vec3Param[i]->get().y,ofToFloat(tvalue)));
                }
            }
        }
        /**/

        /* vec4 */
        if(token=="XUIVec4"){
            name=sp[2];
            ofLog()<<"XUIVec4 detect!";
            for(int i = 0; i < vec4Param.size(); i++){
                if(vec4Param[i]->getName()==name){
                    vec4Param[i]->set(ofVec4f(ofToFloat(tvalue),vec4Param[i]->get().y,vec4Param[i]->get().z,vec4Param[i]->get().w));
                }
            }
        }
        if(token=="YUIVec4"){
            name=sp[2];
            ofLog()<<"YUIVec4 detect!";
            for(int i = 0; i < vec4Param.size(); i++){
                if(vec4Param[i]->getName()==name){
                    vec4Param[i]->set(ofVec4f(vec4Param[i]->get().x,ofToFloat(tvalue),vec4Param[i]->get().z,vec4Param[i]->get().w));
                }
            }
        }
        if(token=="ZUIVec4"){
            name=sp[2];
            ofLog()<<"ZUIVec4 detect!";
            for(int i = 0; i < vec4Param.size(); i++){
                if(vec4Param[i]->getName()==name){
                    vec4Param[i]->set(ofVec4f(vec4Param[i]->get().x,vec4Param[i]->get().y,ofToFloat(tvalue),vec4Param[i]->get().w));
                }
            }
        }
        if(token=="WUIVec4"){
            name=sp[2];
            ofLog()<<"WUIVec4 detect!";
            for(int i = 0; i < vec4Param.size(); i++){
                if(vec4Param[i]->getName()==name){
                    vec4Param[i]->set(ofVec4f(vec4Param[i]->get().x,vec4Param[i]->get().y,vec4Param[i]->get().z,ofToFloat(tvalue)));
                }
            }
        }
        /**/
    }
    return Response;
}

void ofxUIJquery::threadedFunction() {
    while(isThreadRunning()) {

    }
}

void ofxUIJquery::start(){
    startThread(true, false);
}

void ofxUIJquery::stop(){
    stopThread();
}

void ofxUIJquery::exit(){
    TCP.close();
    server.close();
}

void ofxUIJquery::update(){
    for(int i = 0; i < TCP.getLastID(); i++) {
        if( TCP.isClientConnected(i) && request == true) {
            string h = prepareHeader(strlen(buffer.c_str()));
            string p = ofToString(h)+""+ofToString(buffer);

            TCP.send(i, p);
            log("ofxUIJquery.log",p);
            ofLog()<<p;
            ofLog()<<"\nCLOSE SOCKET BUFFER!\n";
            request=false;
            TCP.close();
        }
    }
}

void ofxUIJquery::upsocketUI(int port){
    TCP.setup(port);
    TCP.setMessageDelimiter("</html>\r\n");
}

void ofxUIJquery::setParameterBool(ofParameter<bool> &p, ofPoint bg, ofPoint color) {
    boolParam.push_back(&p);
    buffer+="\n<div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><a id=\"bool"+p.getName()+"\">\n";
    buffer+=p.getName();
    buffer+="\n</a></div>\n";
    buffer+="\n<script>\n";
    buffer+="\n$(function() { $( \"#bool"+ofToString(p.getName())+"\" ).button().click(function( event ) { send('UIBool:"+p.getName()+"'); event.preventDefault(); }); });\n";
    buffer+="\n</script>\n";
}

void ofxUIJquery::setParameterString(ofParameter<string> &p, ofPoint bg, ofPoint color) {
    stringParam.push_back(&p);
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\">";
    buffer+="<p style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">"+ofToString(p.getName())+":<br>";
    buffer+="<input id='UIString"+ofToString(p.getName())+"' value='"+ofToString(p.get())+"'/></p></div>";
    buffer+="<script>$(function() { ";
    buffer+="$('#UIString"+ofToString(p.getName())+"').keypress(function() { var dInput = this.value; send( 'UIString:'+dInput+':"+p.getName()+"' ); });";
    buffer+=" });</script>";
}

void ofxUIJquery::setParameterChar(ofParameter<char> &p, ofPoint bg, ofPoint color) { //serve?
    charParam.push_back(&p);
    buffer+="<p style=\"color:red\">Char</p>";
}

void ofxUIJquery::setParameterInt(ofParameter<int> &p, ofPoint bg, ofPoint color) { //bug
    intParam.push_back(&p);
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleInt\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+="<div style='float:left;'>&nbsp;&nbsp;"+p.getName()+":</div><div id='"+p.getName()+"_values'>0</div>";
    buffer+="</div><br>";
    buffer+="<div id='sliderI"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#sliderI"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin())+", max: "+ofToString(p.getMax())+", change: function(event, ui) { console.log('UIinteger:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_values').html(ui.value); send( 'UIinteger:'+ui.value ); }  }); });</script>";
}

void ofxUIJquery::setParameterFloat(ofParameter<float> &p, ofPoint bg, ofPoint color) {
    floatParam.push_back(&p);
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleFloat\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+="<div style='float:left;'>&nbsp;&nbsp;"+p.getName()+":</div><div style='float:left;' id='"+p.getName()+"_values'>0</div>";
    buffer+="</div><br>";
    buffer+="<div id='sliderF"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#sliderF"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin())+", max: "+ofToString(p.getMax())+", change: function(event, ui) { console.log('UIFloat:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_values').html(ui.value); send( 'UIFloat:'+ui.value+':"+p.getName()+"' ) }  }); });</script>";
}

void ofxUIJquery::setParameterVec2(ofParameter<ofVec2f> &p, ofPoint bg, ofPoint color) {
    vec2Param.push_back(&p);
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleVec2\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+="<div style='float:left;'>&nbsp;&nbsp;"+p.getName()+":</div><div style='float:left;' id='"+p.getName()+"_valuesX'>ofVec2f(0,</div><div style='float:left;' id='"+p.getName()+"_valuesY'>0)</div>";
    buffer+="</div><br>";
    buffer+="<div id='XsliderVec2"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='YsliderVec2"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#XsliderVec2"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('XUIVec2:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesX').html('ofVec2f('+ui.value+','); send( 'XUIVec2:'+ui.value+':"+p.getName()+"' ) }  } ); });</script>";
    buffer+="<script> $(function() { $( '#YsliderVec2"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().y)+", max: "+ofToString(p.getMax().y)+", change: function(event, ui) { console.log('YUIVec2:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesY').html(ui.value+')'); send( 'YUIVec2:'+ui.value+':"+p.getName()+"' ) }  } ); });</script>";
}

void ofxUIJquery::setParameterVec3(ofParameter<ofVec3f> &p, ofPoint bg, ofPoint color) {
    vec3Param.push_back(&p);
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleVec3\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+="<div style='float:left;'>&nbsp;&nbsp;"+p.getName()+":</div><div style='float:left;' id='"+p.getName()+"_valuesX'>ofVec3f(0,</div><div style='float:left;' id='"+p.getName()+"_valuesY'>0,</div><div style='float:left;' id='"+p.getName()+"_valuesZ'>0)</div>";
    buffer+="</div><br>";
    buffer+="<div id='XsliderVec3"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='YsliderVec3"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='ZsliderVec3"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#XsliderVec3"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('XUIVec3:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesX').html('ofVec2f('+ui.value+','); send('XUIVec3:'+ui.value+':"+p.getName()+"'); } }); });</script>";
    buffer+="<script> $(function() { $( '#YsliderVec3"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().y)+", max: "+ofToString(p.getMax().y)+", change: function(event, ui) { console.log('YUIVec3:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesY').html(ui.value+','); send('YUIVec3:'+ui.value+':"+p.getName()+"'); } }); });</script>";
    buffer+="<script> $(function() { $( '#ZsliderVec3"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().z)+", max: "+ofToString(p.getMax().z)+", change: function(event, ui) { console.log('ZUIVec3:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesZ').html(ui.value+')'); send('ZUIVec3:'+ui.value+':"+p.getName()+"'); } }); });</script>";
}

void ofxUIJquery::setParameterVec4(ofParameter<ofVec4f> &p, ofPoint bg, ofPoint color) {
    vec4Param.push_back(&p);
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleVec4\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+="<div style='float:left;'>&nbsp;&nbsp;"+p.getName()+":</div><div style='float:left;' id='"+p.getName()+"_valuesX'>ofVec4f(0,</div><div style='float:left;' id='"+p.getName()+"_valuesY'>0,</div><div style='float:left;' id='"+p.getName()+"_valuesZ'>0,</div><div style='float:left;' id='"+p.getName()+"_valuesW'>0)</div>";
    buffer+="</div><br>";
    buffer+="<div id='XsliderVec4"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='YsliderVec4"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='ZsliderVec4"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='WsliderVec4"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#XsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('XUIVec4:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesX').html('ofVec4f('+ui.value+','); send('XUIVec4:'+ui.value+':"+p.getName()+"'); } }); });</script>";
    buffer+="<script> $(function() { $( '#YsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('YUIVec4:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesY').html(ui.value+','); send('YUIVec4:'+ui.value+':"+p.getName()+"'); } }); });</script>";
    buffer+="<script> $(function() { $( '#ZsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('ZUIVec4:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesZ').html(ui.value+','); send('ZUIVec4:'+ui.value+':"+p.getName()+"'); } }); });</script>";
    buffer+="<script> $(function() { $( '#WsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('WUIVec4:'+ui.value+':"+p.getName()+"'); $('#"+p.getName()+"_valuesW').html(ui.value+')'); send('WUIVec4:'+ui.value+':"+p.getName()+"'); } }); });</script>";
}

/*void ofxUIJquery::setParameterColor(ofParameter<ofColor> &p) { //poi..
    buffer+="<script> $(function() { $( \"#XsliderColor\" ).slider(); });</script>";
    buffer+="<script> $(function() { $( \"#YsliderColor\" ).slider(); });</script>";
    buffer+="<script> $(function() { $( \"#ZsliderColor\" ).slider(); });</script>";
    //buffer+="<script> $(function() { $( \"#WsliderColor\" ).slider(); });</script>";

    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:#A4A4A4;padding:10px;\"><div id=\"titleColor\" style=\"color:white;\">";
    buffer+=p.getName();
    buffer+="</div><br>";
    //buffer+="<div id=\"XsliderColor\"></div><br>";
    buffer+="<div id=\"YsliderColor\"></div><br>";
    buffer+="<div id=\"ZsliderColor\"></div><br>";
    buffer+="<div id=\"WsliderColor\"></div></div><br>";
}*/

