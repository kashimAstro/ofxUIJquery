#include "ofMain.h"
#include "ofxGui.h"
#include "ofxUIJquery.h"

class ofApp : public ofBaseApp {
	public:
	        ofEasyCam camera;
            vector <ofVec3f> rp;
	        vector <ofFloatColor> rc;
            ofxPanel gui;
            ofParameter<float> dark;
            ofParameter<ofVec2f> dis;
            ofParameter<ofVec3f> camerap;
            ofParameter<bool> disablelight;
            ofParameter<bool> fullscreen;
            ofParameter<string> info;
            ofxUIJquery host;

            ofLight pointLight;
            ofLight spotLight;
            ofLight directionalLight;
            ofMaterial material;
            ofTrueTypeFont font;

            int w,h;
            int PORT = 80;

    		void setup();
            void update();
            void draw();
            void exit();
            void keyPressed(int key);
};
