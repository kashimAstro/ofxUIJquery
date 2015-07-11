#ifndef OFXUIJQUERY _H
#define OFXUIJQUERY _H

#include "ofMain.h"
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

        bool bSetup;
        ofxLibwebsockets::Server server;
        string sock_value;

        ofParameter<string>  *stringParam;
        ofParameter<char>    *charParam;
        ofParameter<int>     *intParam;
        ofParameter<bool>    *boolParam;
        ofParameter<float>   *floatParam;
        ofParameter<ofVec2f> *vec2Param;
        ofParameter<ofVec3f> *vec3Param;
        ofParameter<ofVec4f> *vec4Param;
        string buffer;
        string header;
        string Response;
        string LOCAL_IP_ADDRESS;
        bool request;
        int PORT_CLIENT;

        ofxUIJquery ();
        virtual ~ofxUIJquery ();

        int getPort();
        string getResult();

        void init();
        void setup(string ADRESS, int port, ofPoint bg, STYLE TYPE = REDMOND);
        void threadedFunction();
        void start();
        void stop();
        void update();
        void upsocketUI(int port);

        void setParameterBool(ofParameter<bool> &p, ofPoint bg, ofPoint color);
        void setParameterString(ofParameter<string> &p, ofPoint bg, ofPoint color);
        void setParameterChar(ofParameter<char> &p, ofPoint bg, ofPoint color);
        void setParameterInt(ofParameter<int> &p, ofPoint bg, ofPoint color);
        void setParameterFloat(ofParameter<float> &p, ofPoint bg, ofPoint color);
        void setParameterVec2(ofParameter<ofVec2f> &p, ofPoint bg, ofPoint color);
        void setParameterVec3(ofParameter<ofVec3f> &p, ofPoint bg, ofPoint color);
        void setParameterVec4(ofParameter<ofVec4f> &p, ofPoint bg, ofPoint color);

        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );

        string setStyle(STYLE TYPE);
        string response(string value);
};

#endif // OFXUIJQUERY _H
