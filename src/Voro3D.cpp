//
//  Voro3D.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "Voro3D.h"



void Voro3D::setup() {
    
        makeTissue(40, 0.2, 0.2, 0.02);
    
}

void Voro3D::draw() {
    
    ofPushMatrix();
    ofTranslate(0,-1.1,-0.5);
    ofRotateY(0);
    ofRotateX(90);
    
    /*for (int i = 0; i < cellCentroids.size(); i++){
        ofSetColor(255,0,0);
		//ofDrawSphere(cellCentroids[i], cellRadius[i]*0.15);
    }*/
    
    for(int i = 0; i < cellMeshes.size(); i++){
        ofSetColor(255,255,255,220);
        cellMeshes[i].drawFaces();
        
        ofPushStyle();
        ofSetLineWidth(3);
        ofSetColor(0,0,0);
        //cellMeshes[i].drawWireframe();
        ofPopStyle();
    }
    
    ofPopMatrix();
    
    
    
    
    ofPushMatrix();
    ofRotateX(90);
        ofRect(-1,-1,1,1);
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofTranslate(0,1,0);
    
    
    ofSetColor(255);

    
    
    ofSetColor(0,0,0,200);
    ofDrawBox(1,0.2,1);
    
    
    ofPopMatrix();
    
    
    
    
}

void Voro3D::update() {
    
}


void Voro3D::makeTissue(int _nCells, float _width, float _height, float _deep){
    
    //  Fresh begining
    //
    cellMeshes.clear();
    cellCentroids.clear();
    cellRadius.clear();
    
    //  Define a container
    //
    voro::container con(-_width,_width,
                        -_height,_height,
                        -_deep,_deep,
                        1,1,1,
                        false,false,false, // set true to flow beyond box
                        8);
    
    //  Add walls (un comment one pair if you like to shape the container)
    //
   // voro::wall_cylinder cyl(0,0,0,0,0,20, min(_width, _height));
   // con.add_wall(cyl);
    
      //  voro::wall_sphere sph(0, 0, 0, min(_width, _height) );
      //  con.add_wall(sph);
    
       //voro::wall_cone cone(0,0,min(_width*0.5, _height*0.5),0,0,-1,atan(0.5));
        //con.add_wall(cone);
    
    //  Add the cell seed to the container
    //
    
    for(int i = 0; i < _nCells;i++){
        ofPoint newCell = ofPoint(ofRandom(-_width,_width),
                                  ofRandom(-_height,_height),
                                  ofRandom(-_deep,_deep));
        
        addCellSeed(con, newCell, i, true);
    }
    
    cellMeshes = getCellsFromContainer(con);
    cellRadius = getCellsRadius(con);
    cellCentroids = getCellsCentroids(con);
}



