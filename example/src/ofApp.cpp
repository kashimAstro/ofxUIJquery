#include "ofApp.h"

void ofApp::setup(){
    //ofSetLogLevel(OF_LOG_SILENT);

    ofSetFrameRate(50);
    w=ofGetWidth();
    h=ofGetHeight();

    gui.setup();
    gui.add(dark.set("dark",0.,0.,255.));
    gui.add(dis.set("dis",ofVec2f(0),ofVec2f(0),ofVec2f(10)));
    gui.add(camerap.set("camerapos",ofVec3f(0),ofVec3f(0),ofVec3f(500)));
    gui.add(fullscreen.set("fullscreen",true));
    gui.add(hidedebug.set("stoprotation", true));
    gui.add(disablelight.set("disablelight", true));
    gui.add(info.set("info", ""));
    gui.setPosition(ofVec3f(10,80,0));

    STYLE c = DARK_HIVE;
    host.setup(PORT,ofPoint(15,25,255),c);
    host.setParameterBool(fullscreen,  ofPoint(200,145),ofPoint(10));
    host.setParameterBool(hidedebug,   ofPoint(200,33), ofPoint(10));
    host.setParameterBool(disablelight,ofPoint(200,56), ofPoint(10));
    host.setParameterFloat(dark,       ofPoint(200,1),  ofPoint(10));
    host.setParameterVec2(dis,         ofPoint(200,23), ofPoint(10));
    host.setParameterVec3(camerap,     ofPoint(200,65), ofPoint(10));
    host.setParameterString(info,      ofPoint(200,90), ofPoint(10));
    host.init();

    rp.resize(40);
    rc.resize(rp.size() );
    for (int i=0; i<rp.size(); i++) {
           rp[i].set( ofRandom(-150, 150), ofRandom(-150, 150), ofRandom(-150, 150) );
           rc[i].set( ofRandom(0., 1.), ofRandom(0., 1.), ofRandom(0., 1.));
    }
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
    pointLight.setPointLight();
    spotLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
    spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    spotLight.setSpotlight();
    spotLight.setSpotlightCutOff( 50 );
    spotLight.setSpotConcentration( 45 );
    directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
    directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    directionalLight.setDirectional();
    directionalLight.setOrientation( ofVec3f(0, 90, 0) );
    material.setShininess( 120 );
    material.setSpecularColor(ofColor(255, 255, 255, 255));

}

void ofApp::exit(){

}

void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    info.set(info.get());
    host.update();
}

void ofApp::draw(){
    ofBackgroundGradient(255,ofColor(dark));
    camera.begin();
    ofEnableDepthTest();
    ofEnableLighting();
    material.begin();
    if(disablelight) {
        pointLight.enable();
        spotLight.enable();
        directionalLight.enable();
    }
    ofRotate(ofGetFrameNum()%1500);
    for (int i=0; i<rp.size(); i++) {
        ofSetColor(rc[i]);
        ofRotateY(ofGetFrameNum()/100);
        ofDrawSphere( rp[i].x+camerap.get().x+50*dis.get().x, rp[i].y+camerap.get().y+50, rp[i].z+camerap.get().z, 40);
        ofRotateZ(ofGetFrameNum()/100);
        ofDrawCone( rp[i].x+camerap.get().x-50*dis.get().x, rp[i].y+camerap.get().y-50, rp[i].z+camerap.get().z, 10, 100*dis.get().y );
        ofDrawBox( rp[i].x+camerap.get().x*dis.get().x, rp[i].y+camerap.get().y*dis.get().y, rp[i].z+camerap.get().z, 40+dis.get().y );
    }
    if(disablelight){
        pointLight.disable();
        spotLight.disable();
        directionalLight.disable();
    }
    material.end();
    ofDisableLighting();
    ofDisableDepthTest();
    camera.end();
    ofDrawBitmapStringHighlight(host.getResult(),20,40,ofColor(255,0,0),ofColor(45));
    gui.draw();
}

void ofApp::keyPressed(int key){
    if(key == 'u')
        host.upsocketUI(PORT);
}
