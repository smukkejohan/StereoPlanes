//
//  Voro3D.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "GhostLights.h"

void GhostLights::setup() {
    
    wallCube = new VoroGhost();
    wallCube->setup(0.2,0.2,0.2,100);
    
    mat.diffuseColor = ofVec4f(1.0, 1.0, 1.0, 0.3);
    mat.specularColor = ofVec4f(1.0, 1.0, 1.0, 0.5);
    mat.specularShininess = 0.8;
    
    mainTimeline->addPage(name);
    tlenabled = mainTimeline->addCurves("Enabled");
    tlexplode = mainTimeline->addCurves("Explode");
    
    tlscale = mainTimeline->addCurves("Scale");
    tlscale->setValueRangeMax(3);
    
    tlx = mainTimeline->addCurves("X");
    tlx->setValueRangeMax(3);
    tlx->setValueRangeMin(-3);
    
    tly = mainTimeline->addCurves("Y");
    tly->setValueRangeMax(3);
    tly->setValueRangeMin(-3);
    
    tlz = mainTimeline->addCurves("Z");
    tlz->setValueRangeMax(6);
    tlz->setValueRangeMin(-2);
    
    
}

void GhostLights::draw(int _surfaceId) {
    
    if(_surfaceId == primarySurface) {
    
        ofxOlaShaderLight::setMaterial(mat);

        ofPushMatrix(); {
            ofTranslate(tlx->getValue(), tly->getValue(), tlz->getValue());
            
            ofRotateX(ofGetElapsedTimef()*6);
            ofRotateY(ofGetElapsedTimef()*7);
            ofRotateZ(ofGetElapsedTimef()*8);
            
            for(int i = 0; i < wallCube->cellMeshes.size(); i++){
                ofPushMatrix(); {
                    
                    //wallCube->cellMeshes[i].setColor(, const ofFloatColor &c)
                    
                    ofScale(tlscale->getValue(),tlscale->getValue(),tlscale->getValue());
                    float explode = tlexplode->getValue(); //ofMap(ofSignedNoise(ofGetElapsedTimef()), 0, 1, 0.8, 1);
                    
                    ofTranslate(wallCube->cellMeshes[i].getCentroid().x, wallCube->cellMeshes[i].getCentroid().y, wallCube->cellMeshes[i].getCentroid().z);
                    ofScale(explode,explode,explode);
                    ofTranslate(-wallCube->cellMeshes[i].getCentroid().x, -wallCube->cellMeshes[i].getCentroid().y, -wallCube->cellMeshes[i].getCentroid().z);
                    
                    wallCube->cellMeshes[i].drawFaces();
                } ofPopMatrix();
                
            }
        } ofPopMatrix();
    }
}

void GhostLights::update() {
    enabled = (tlenabled->getValue());
    wallCube->update();
    
}

void GhostLights::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
}

void GhostLights::receiveOsc(ofxOscMessage * m, string rest) {
}

void GhostLights::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}



