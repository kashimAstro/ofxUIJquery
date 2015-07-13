#include "ofApp.h"

void ofApp::setup(){
    //ofSetLogLevel(OF_LOG_SILENT);

    ofSetFrameRate(60);
    w=ofGetWidth();
    h=ofGetHeight();
    font.loadFont("verdana.ttf",58);

    gui.setup();
    gui.add(dark.set("dark",0.,0.,255.));
    gui.add(dis.set("dis",ofVec2f(0),ofVec2f(0),ofVec2f(10)));
    gui.add(pos.set("position",ofVec3f(0),ofVec3f(0),ofVec3f(500)));
    gui.add(color.set("color",ofVec3f(0),ofVec3f(0),ofVec3f(255)));
    gui.add(disablelight.set("disablelight", true));
    gui.add(fullscreen.set("fullscreen", false));
    gui.add(info.set("info", ""));
    gui.setPosition(ofVec3f(10,65,0));

    /*ofxUIjquery */
    PORT=80;
    PORTWS=9092;
    STYLE c = VADER;
    host.setup("172.16.200.230",       PORT,    PORTWS, ofPoint(186,65,25),    c);
    host.setParameterBool(disablelight,ofPoint(242,47,136), ofPoint(50,205,50));
    host.setParameterBool(fullscreen,  ofPoint(242,47,136), ofPoint(204,0,56));
    host.setParameterFloat(dark,       ofPoint(242,47,136), ofPoint(50,205,50));
    host.setParameterVec2(dis,         ofPoint(242,47,136), ofPoint(204,0,56));
    host.setParameterVec3(pos,         ofPoint(242,47,136), ofPoint(50,205,50));
    host.setParameterVec3(color,       ofPoint(242,47,136), ofPoint(204,0,56));
    host.setParameterString(info,      ofPoint(242,47,136), ofPoint(50,205,50));
    host.init();
    /**/

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
    directionalLight.setOrientation( ofVec3f(0, 190, 0) );
    material.setShininess( 255 );
    material.setSpecularColor(ofColor(255, 255, 255, 255));
}

void ofApp::exit(){
    host.exit();
}

void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
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

    ofRotate(ofGetFrameNum()/3.6);
    for (int i=0; i<rp.size(); i++) {

        ofSetColor(rc[i]);
        ofRotateY(ofGetFrameNum()/2);
        ofDrawSphere( rp[i].x+pos.get().x+50*dis.get().x, rp[i].y+pos.get().y+50, rp[i].z+pos.get().z, 25);
        ofRotateZ(ofGetFrameNum()/2);
        ofDrawCone( rp[i].x+pos.get().x-50*dis.get().x, rp[i].y+pos.get().y-50, rp[i].z+pos.get().z, 10, 10+100*dis.get().y );

        ofSetColor(color.get().x,color.get().y,color.get().z);
        ofDrawBox( rp[i].x+pos.get().x*dis.get().x, rp[i].y+pos.get().y*dis.get().y, rp[i].z+pos.get().z, 40+dis.get().y );
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

    ofPushStyle();
    ofSetColor(ofColor::red);
    font.drawString(info.get(),10,h/2);
    ofSetColor(ofColor::white);
    font.setGlobalDpi(64);
    font.drawString(info.get(),20,h/2+60);
    ofPopStyle();
}

void ofApp::keyPressed(int key){
    if(key == 'u')
        host.upsocketUI(PORT);
}
