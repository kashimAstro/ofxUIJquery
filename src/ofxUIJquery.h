#ifndef OFXUIJQUERY_H
#define OFXUIJQUERY_H

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxTCPServer.h"
#include "ofxLibwebsockets.h"

enum STYLE {
    BLITZER,
    CUPERTINO,
    DARK_HIVE,
    FLICK,
    LEFROG,
    OVERCATS,
    SMOOTHESS,
    SUNNY,
    VADER,
    REDMOND
};

class ofxUIJquery : public ofThread {
    public:
        ofxTCPServer TCP;
        ofxLibwebsockets::Server server;

        vector<ofParameter<string>  *> stringParam;
        vector<ofParameter<char>    *> charParam;
        vector<ofParameter<int>     *> intParam;
        vector<ofParameter<bool>    *> boolParam;
        vector<ofParameter<float>   *> floatParam;
        vector<ofParameter<ofVec2f> *> vec2Param;
        vector<ofParameter<ofVec3f> *> vec3Param;
        vector<ofParameter<ofVec4f> *> vec4Param;

        string buffer;
        string sock_value;
        string Response;
        string LOCAL_IP_ADDRESS;
        bool request;
        bool bSetup;
        int PORT_CLIENT;

        ofxUIJquery();
        virtual ~ofxUIJquery();

        int getPort();
        string getResult();
        string prepareHeader(int _size);
        string setStyle(STYLE TYPE);
        string response(string value);
        string place(ofxPanel &p);

        void init();
        void setup(string ADRESS, int port, int port2, ofPoint bg, STYLE TYPE = REDMOND);
        void threadedFunction();
        void start();//not use
        void stop();//not use
        void update();
        void upsocketUI(int port);
        void exit();
        void log(string _file,string _buffer);

        void setParameterBool(ofParameter<bool> &p, ofPoint bg, ofPoint color);
        void setParameterString(ofParameter<string> &p, ofPoint bg, ofPoint color);
        void setParameterChar(ofParameter<char> &p, ofPoint bg, ofPoint color);
        void setParameterInt(ofParameter<int> &p, ofPoint bg, ofPoint color);
        void setParameterFloat(ofParameter<float> &p, ofPoint bg, ofPoint color);
        void setParameterVec2(ofParameter<ofVec2f> &p, ofPoint bg, ofPoint color);
        void setParameterVec3(ofParameter<ofVec3f> &p, ofPoint bg, ofPoint color);
        void setParameterVec4(ofParameter<ofVec4f> &p, ofPoint bg, ofPoint color);

        /* websocket callback */
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );
};

#endif // OFXUIJQUERY_H
