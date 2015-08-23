#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFullscreen(true);
	ofSetVerticalSync(true);
	kinect.open();
	kinect.initDepthSource();
	kinect.initInfraredSource();
	kinect.initColorSource();
	isDepthDraw = true;
	isFullscreen = true;

	mode = 0;
	select = 0;
	
	colorPixelsToWorld.allocate(1920*3,1080*3,ofPixelFormat::OF_PIXELS_RGB);
	depthImg.allocate(1920,1080,OF_IMAGE_COLOR);  
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetFullscreen(isFullscreen);
	this->kinect.update();
	//getColorToWorldMap을 통해 Color에 맞춰 만든 Depth값을 받아온다.
	colorPixelsToWorld = kinect.getDepthSource()->getColorToWorldMap();

	switch (mode)
	{
		case READY :
			if(position_vec.size() >= 4){
				mode = CROSS;
			}
			break;

		case CROSS :
			if(position_vec.size() < 4){
				mode = READY;
				break;
			}

			intersectionPoint = getIntersectionPoint(position_vec[2].pixelPos,position_vec[0].pixelPos,position_vec[3].pixelPos,position_vec[1].pixelPos);
			intersectionWorld = getWorldFromColor(intersectionPoint);

			if(position_vec.size() >= 5){
				mode = VECTOR;
				break;
			}
			break;

		case VECTOR :
			if(position_vec.size() < 5){
				mode = CROSS;
				break;
			}

			rightVectorPoint = position_vec[4].pixelPos;
			rightVectorWorld =  position_vec[4].worldPos;

			break;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(150,255);
	this->kinect.getColorSource()->draw(0,0);

	if(isDepthDraw)
		ofSetColor(10,10,10,100);
	else
		ofSetColor(255,255,255,200);
		depthImg.setFromPixels(colorPixelsToWorld.getPixels(),1920,1080,OF_IMAGE_COLOR);
		depthImg.draw(0,0,1920,1080);

	ofSetColor(255,255);
	ofDrawBitmapStringHighlight("frame : " + ofToString(ofGetFrameRate()),0,10);

	switch (mode)
	{
		case READY :
			if(position_vec.size() > 0){
				for(int i=0; i<position_vec.size(); i++){
					if(i == select){
						ofSetColor(0,255,0);
						ofCircle(position_vec[i].pixelPos,5);
					}else{
						ofSetColor(255,255,255);
						ofCircle(position_vec[i].pixelPos,2);
					}
				}
			}
								
			break;

		case CROSS :
			for(int i=0; i<position_vec.size(); i++){
				if(i == select){
					ofSetColor(0,255,0);
					ofCircle(position_vec[i].pixelPos,5);
				}else{
					ofSetColor(255,255,255);
					ofCircle(position_vec[i].pixelPos,2);
				}
			}

			ofSetColor(0,255,0,150);
			ofSetLineWidth(0.5);
			ofLine(position_vec[0].pixelPos,position_vec[2].pixelPos);
			ofLine(position_vec[1].pixelPos,position_vec[3].pixelPos);
			ofCircle(intersectionPoint,3);
			
			ofDrawBitmapStringHighlight("display center : " + ofToString(intersectionWorld),0,30);

			break;

		case VECTOR :
			for(int i=0; i<position_vec.size(); i++){
				if(i == select){
					ofSetColor(0,255,0);
					ofCircle(position_vec[i].pixelPos,5);
				}else{
					ofSetColor(255,255,255);
					ofCircle(position_vec[i].pixelPos,2);
				}
			}

			ofSetColor(255,255,255,150);
			ofSetLineWidth(0.5);
			ofLine(position_vec[0].pixelPos,position_vec[2].pixelPos);
			ofLine(position_vec[1].pixelPos,position_vec[3].pixelPos);
			ofCircle(intersectionPoint,3);

			ofLine(intersectionPoint,position_vec[4].pixelPos);
			
			ofDrawBitmapStringHighlight("display center : " + ofToString(intersectionWorld),0,30);
			ofDrawBitmapStringHighlight("right vector : " + ofToString(rightVectorWorld),0,50);
			break;
	}
}

//depthImg에서 선택한 R,G,B는 각각 월드 좌표의 X,Y,Z이다. 그 값을 리턴해줌.
ofVec3f ofApp::getWorldFromColor(ofPoint position){
	ofVec3f _colorToWorld;
	_colorToWorld = ofVec3f(depthImg.getColor(position.x,position.y).r,depthImg.getColor(position.x,position.y).g,depthImg.getColor(position.x,position.y).b);
	return _colorToWorld;
}

//법선벡터 구하는 것
ofVec3f ofApp::getNormalVector(ofVec3f P, ofVec3f Q, ofVec3f R){
	ofVec3f PQ,PR;
	ofVec3f _normalVector;
	PQ = Q-P;
	PR = R-P;

	_normalVector = PQ.cross(PR);
	return _normalVector;
}

//교차점 구하는 것 
ofVec2f ofApp::getIntersectionPoint(ofPoint lineA1, ofPoint lineA2,ofPoint lineB1, ofPoint lineB2){
	//line1 : y = Ax + B
	//line2 : y = Cx + D
	ofVec2f _intersectionPoint;
	double A,B,C,D;

	A = (lineA2.y - lineA1.y)/(lineA2.x - lineA1.x);
	B = lineA1.y - ((lineA2.y - lineA1.y)/(lineA2.x - lineA1.x))*lineA1.x;

	C = (lineB2.y - lineB1.y)/(lineB2.x - lineB1.x);
	D = lineB1.y - ((lineB2.y - lineB1.y)/(lineB2.x - lineB1.x))*lineB1.x;

	_intersectionPoint.x = (D-B)/(A-C);
	_intersectionPoint.y = A*_intersectionPoint.x + B;

	return _intersectionPoint;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case OF_KEY_TAB:
			if(position_vec.size() > 0)
				select = ++select%position_vec.size();
			break;
		
		case ' ' :
			isDepthDraw = !isDepthDraw;
			break;

		case 'u' :
			if(position_vec.size() > 0){
				position_vec[select].worldPos = getWorldFromColor(position_vec[select].pixelPos);
				if(mode == CROSS){
					intersectionPoint = getIntersectionPoint(position_vec[2].pixelPos,position_vec[0].pixelPos,position_vec[3].pixelPos,position_vec[1].pixelPos);
					intersectionWorld = getWorldFromColor(intersectionPoint);
				}else if(mode == VECTOR){
					intersectionPoint = getIntersectionPoint(position_vec[2].pixelPos,position_vec[0].pixelPos,position_vec[3].pixelPos,position_vec[1].pixelPos);
					intersectionWorld = getWorldFromColor(intersectionPoint);

					rightVectorPoint = position_vec[4].pixelPos;
					rightVectorWorld =  position_vec[4].worldPos;
				}
			}
			break;


		case 'f':
			isFullscreen = !isFullscreen;
			break;


		case OF_KEY_UP :
			if(position_vec.size() > 0){
				position_vec[select].pixelPos.y--;
				position_vec[select].worldPos = getWorldFromColor(position_vec[select].pixelPos);
			}
			break;

		case OF_KEY_DOWN :
			if(position_vec.size() > 0){
				position_vec[select].pixelPos.y++;
				position_vec[select].worldPos = getWorldFromColor(position_vec[select].pixelPos);
			}
			break;

		case OF_KEY_LEFT :
			if(position_vec.size() > 0){
				position_vec[select].pixelPos.x--;
				position_vec[select].worldPos = getWorldFromColor(position_vec[select].pixelPos);
			}
			break;

		case OF_KEY_RIGHT :
			if(position_vec.size() > 0){
				position_vec[select].pixelPos.x++;
				position_vec[select].worldPos = getWorldFromColor(position_vec[select].pixelPos);
			}
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(button == 0){
		Position_str tmp;
		tmp.pixelPos = ofVec2f(mouseX,mouseY);
		tmp.worldPos = getWorldFromColor(tmp.pixelPos);

 		position_vec.push_back(tmp);
		if(position_vec.size() > 1)
			select = position_vec.size()-1;
	}else{ 
		if(position_vec.size() > 0){
			position_vec.erase(position_vec.begin()+select);
			select = position_vec.size()-1;
		}else if(position_vec.size() == 0){
			select = 0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
