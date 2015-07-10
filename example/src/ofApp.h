#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGuiHost.h"
#include "ofxLibwebsockets.h"

class ofApp : public ofBaseApp {
	public:
	        ofEasyCam camera;
            vector <ofVec3f> rp;
	        vector <ofFloatColor> rc;
            ofxPanel gui;
            ofParameter<float> dark;
            ofParameter<ofVec2f> dis;
            ofParameter<ofVec3f> camerap;
            ofParameter<bool> fullscreen;
            ofParameter<bool> hidedebug;
            ofParameter<bool> disablelight;
            ofParameter<string> fps;
            ofxGuiHost host;

            int w,h;
            int PORT = 80;
            bool bSetup;
            ofxLibwebsockets::Server server;
            string sock_value;

    		void setup();
            void update();
            void draw();
            void exit();
            void keyPressed(int key);

            void onConnect( ofxLibwebsockets::Event& args );
            void onOpen( ofxLibwebsockets::Event& args );
            void onClose( ofxLibwebsockets::Event& args );
            void onIdle( ofxLibwebsockets::Event& args );
            void onMessage( ofxLibwebsockets::Event& args );
            void onBroadcast( ofxLibwebsockets::Event& args );

};
