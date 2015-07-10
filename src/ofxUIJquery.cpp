#include "ofxUIJquery.h"

ofxUIJquery::ofxUIJquery(){

}

ofxUIJquery::~ofxUIJquery(){

}

void ofxUIJquery::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

void ofxUIJquery::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    sock_value = args.conn.getClientIP() + " - " + args.conn.getClientName();
}

void ofxUIJquery::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

void ofxUIJquery::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
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
    cout<<"got broadcast "<<args.message<<endl;
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

void ofxUIJquery::setup(int port, ofPoint bg, STYLE TYPE) {
    LOCAL_IP_ADDRESS="127.0.0.1";
    PORT_CLIENT=9092;

    TCP.setup(port);
    TCP.setMessageDelimiter("</html>");

    /* recevede websocket */
    //TCPclient.setup(PORT_CLIENT);
    //TCPclient.setMessageDelimiter("\n");
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = getPort();
    options.bUseSSL = false;
    server.addListener(this);
    bSetup = server.setup( options );


    header+="HTTP/1.0 200 OK\n";
    header+="Server: Apache/1.3.29 (Unix) PHP/4.3.4\n";
    header+="Vary: Accept-Encoding,Cookie\n";
    header+="Cache-Control: private, s-maxage=0, max-age=0, must-revalidate\n";
    header+="Content-Language: en\n";
    header+="Content-Type: text/html; charset=utf-8\n";
    header+="X-Cache: HIT from ofxGUIHost.x\n";
    header+="Connection: close\n";

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

    buffer+=header+"\n<html><head><title>ofxUIJquery</title>";
    buffer+="<style>";
    buffer+=CSSbuffer.str();
    buffer+="* { font-family: \"Arial Verdana\", Arial, Verdana; }";
    buffer+=".d_div { padding:25px; border-radius: 25px; }";
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
    buffer+="</html>";

}

string ofxUIJquery::response(string value) {
    vector<string> sp = ofSplitString(value,":");
    string token,tvalue;
    if(sp[0]!="")
        token = sp[0];
    if(sp[0]!="")
        tvalue = sp[1];
    if(token!=""&&tvalue!=""){
        /* bool */
        if(token=="UIBool"){
            ofLog()<<"UIBool detect!";
            boolParam->set( !boolParam->get() );
        }
        /**/

        /* int */
        if(token=="UIinteger"){
            ofLog()<<"UIinteger detect!";
        }

        /* float */
        if(token=="UIFloat"){
            ofLog()<<"UIFloat detect!";
            floatParam->set(ofToInt(tvalue));
        }

        /* vec2 */
        if(token=="XUIVec2"){
            ofLog()<<"XUIVec2 detect!";
            vec2Param->set(ofVec2f(ofToInt(tvalue),vec2Param->get().y));
        }
        if(token=="YUIVec2"){
            ofLog()<<"YUIVec2 detect!";
            vec2Param->set(ofVec2f(vec2Param->get().x,ofToInt(tvalue)));
        }

        /* vec3 */
        if(token=="XUIVec3"){
            ofLog()<<"XUIVec3 detect!";
            vec3Param->set(ofVec3f(ofToInt(tvalue),vec3Param->get().y,vec3Param->get().z));
        }
        if(token=="YUIVec3"){
            ofLog()<<"YUIVec3 detect!";
            vec3Param->set(ofVec3f(vec3Param->get().x,ofToInt(tvalue),vec3Param->get().z));
        }
        if(token=="ZUIVec3"){
            ofLog()<<"ZUIVec3 detect!";
            vec3Param->set(ofVec3f(vec3Param->get().x,vec3Param->get().y,ofToInt(tvalue)));
        }

        /* vec4 */
        if(token=="XUIVec4"){
            ofLog()<<"XUIVec4 detect!";
        }
        if(token=="YUIVec4"){
            ofLog()<<"YUIVec4 detect!";
        }
        if(token=="ZUIVec4"){
            ofLog()<<"ZUIVec4 detect!";
        }
        if(token=="WUIVec4"){
            ofLog()<<"WUIVec4 detect!";
        }
    }
    return Response;
}

void ofxUIJquery::threadedFunction() {
    while(isThreadRunning()) {
        /*for(int i = 0; i < TCPclient.getLastID(); i++) {
            if( TCPclient.isClientConnected(i)) {
                string str = TCPclient.receive(i);
                if(str != ""){
                    Response=str;
                    ofLog()<<Response;
                }
            }
        }*/
    }
}

void ofxUIJquery::start(){
    startThread(true, false);
}

void ofxUIJquery::stop(){
    stopThread();
}

void ofxUIJquery::update(){
    for(int i = 0; i < TCP.getLastID(); i++) {
        if( TCP.isClientConnected(i) && request == true) {
            TCP.send(i, buffer);
            cout<<buffer;
            ofLog()<<"CLOSE SOCKET BUFFER!\n\n\n";
            request=false;
            TCP.close();
        }
    }
}

void ofxUIJquery::upsocketUI(int port){
    TCP.setup(port);
    TCP.setMessageDelimiter("</html>");
}

void ofxUIJquery::setParameterBool(ofParameter<bool> &p, ofPoint bg, ofPoint color) {
    boolParam=&p;
    buffer+="\n<div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><a id=\"bool"+p.getName()+"\">\n";
    buffer+=p.getName();
    buffer+="\n</a></div>\n";
    buffer+="\n<script>\n";
    buffer+="\n$(function() { $( \"#bool"+ofToString(p.getName())+"\" ).button().click(function( event ) { send('UIBool:"+p.getName()+"'); event.preventDefault(); }); });\n";
    buffer+="\n</script>\n";
}

void ofxUIJquery::setParameterString(ofParameter<string> &p, ofPoint bg, ofPoint color) {
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\">";
    buffer+="<p style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">"+ofToString(p.getName())+": "+ofToString(p.get())+"</p></div>";
}

void ofxUIJquery::setParameterChar(ofParameter<char> &p, ofPoint bg, ofPoint color) {
    buffer+="<p style=\"color:red\">Char</p>";
}

void ofxUIJquery::setParameterInt(ofParameter<int> &p, ofPoint bg, ofPoint color) {
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleInt\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+=p.getName();
    buffer+="</div><br>";
    buffer+="<div id='sliderI"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#sliderI"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin())+", max: "+ofToString(p.getMax())+", change: function(event, ui) { console.log('UIinteger:'+ui.value); send( 'UIinteger:'+ui.value ) }  }); });</script>";
}

void ofxUIJquery::setParameterFloat(ofParameter<float> &p, ofPoint bg, ofPoint color) {
    floatParam=&p;
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleFloat\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+=p.getName();
    buffer+="</div><br>";
    buffer+="<div id='sliderF"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#sliderF"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin())+", max: "+ofToString(p.getMax())+", change: function(event, ui) { console.log('UIFloat:'+ui.value); send( 'UIFloat:'+ui.value ) }  }); });</script>";
}

void ofxUIJquery::setParameterVec2(ofParameter<ofVec2f> &p, ofPoint bg, ofPoint color) {
    vec2Param=&p;
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleVec2\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+=p.getName();
    buffer+="</div><br>";
    buffer+="<div id='XsliderVec2"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='YsliderVec2"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#XsliderVec2"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('XUIVec2:'+ui.value); send( 'XUIVec2:'+ui.value ) }  } ); });</script>";
    buffer+="<script> $(function() { $( '#YsliderVec2"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().y)+", max: "+ofToString(p.getMax().y)+", change: function(event, ui) { console.log('YUIVec2:'+ui.value); send( 'YUIVec2:'+ui.value ) }  } ); });</script>";
}

void ofxUIJquery::setParameterVec3(ofParameter<ofVec3f> &p, ofPoint bg, ofPoint color) {
    vec3Param=&p;
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleVec3\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+=p.getName();
    buffer+="</div><br>";
    buffer+="<div id='XsliderVec3"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='YsliderVec3"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='ZsliderVec3"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#XsliderVec3"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('XUIVec3:'+ui.value); send('XUIVec3:'+ui.value); } }); });</script>";
    buffer+="<script> $(function() { $( '#YsliderVec3"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().y)+", max: "+ofToString(p.getMax().y)+", change: function(event, ui) { console.log('YUIVec3:'+ui.value); send('YUIVec3:'+ui.value); } }); });</script>";
    buffer+="<script> $(function() { $( '#ZsliderVec3"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().z)+", max: "+ofToString(p.getMax().z)+", change: function(event, ui) { console.log('ZUIVec3:'+ui.value); send('ZUIVec3:'+ui.value); } }); });</script>";
}

void ofxUIJquery::setParameterVec4(ofParameter<ofVec4f> &p, ofPoint bg, ofPoint color) {
    vec4Param=&p;
    buffer+="<br><div class=\"d_div\" style=\"margin-left:10px;margin-top:10px;background:rgb("+ofToString(bg.x)+","+ofToString(bg.y)+","+ofToString(bg.z)+");padding:10px;\"><div id=\"titleVec4\" style=\"color:rgb("+ofToString(color.x)+","+ofToString(color.y)+","+ofToString(color.z)+");\">";
    buffer+=p.getName();
    buffer+="</div><br>";
    buffer+="<div id='XsliderVec4"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='YsliderVec4"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='ZsliderVec4"+ofToString(p.getName())+"'></div><br>";
    buffer+="<div id='WsliderVec4"+ofToString(p.getName())+"'></div></div><br>";
    buffer+="<script> $(function() { $( '#XsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('XUIVec4:'+ui.value); send('XUIVec4:'+ui.value); } }); });</script>";
    buffer+="<script> $(function() { $( '#YsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('YUIVec4:'+ui.value); send('YUIVec4:'+ui.value); } }); });</script>";
    buffer+="<script> $(function() { $( '#ZsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('ZUIVec4:'+ui.value); send('ZUIVec4:'+ui.value); } }); });</script>";
    buffer+="<script> $(function() { $( '#WsliderVec4"+ofToString(p.getName())+"' ).slider({ min: "+ofToString(p.getMin().x)+", max: "+ofToString(p.getMax().x)+", change: function(event, ui) { console.log('WUIVec4:'+ui.value); send('WUIVec4:'+ui.value); } }); });</script>";
}

/*void ofxUIJquery::setParameterColor(ofParameter<ofColor> &p) {
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

