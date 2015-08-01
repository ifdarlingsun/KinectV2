#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetFullscreen(true);
	kinect.open();
	kinect.initDepthSource();
	kinect.initInfraredSource();
	kinect.initColorSource();
	isDepthDraw = true;
	isClicked = true;

	select = 0;
	//depthPixelsRaw.allocate(IR_WIDTH, IR_HEIGHT, 1);

	colorPixelsToWorld.allocate(1920*3,1080*3,ofPixelFormat::OF_PIXELS_RGB);
	depthImg.allocate(1920,1080,OF_IMAGE_COLOR);  
}

//--------------------------------------------------------------
void ofApp::update(){
	this->kinect.update();
	//getColorToWorldMap�� ���� Color�� ���� ���� Depth���� �޾ƿ´�.
	colorPixelsToWorld = kinect.getDepthSource()->getColorToWorldMap();

}

//--------------------------------------------------------------
void ofApp::draw(){
	//Depth �ҽ��� �޾Ƽ� ���� ���� �������ش�.(/8000) �װ����� Depth �̹����� �׸�
	/*
	ofSetColor(255,255);
	depthPixelsRaw = this->kinect.getDepthSource()->getPixelsRef();
	for (int i = 0; i < depthPixelsRaw.size(); i++){
		depthPixelsRaw[i] = (depthPixelsRaw[i] * 65536 / 8000);
	}
	depthImage.setFromPixels(depthPixelsRaw);
	ofSetColor(255);
	depthImage.draw(0,0);
	*/

	ofSetColor(255,255);
	this->kinect.getColorSource()->draw(0,0);

	if(isDepthDraw){
		ofSetColor(255,255,255,100);
		depthImg.setFromPixels(colorPixelsToWorld.getPixels(),1920,1080,OF_IMAGE_COLOR);
		depthImg.draw(0,0,1920,1080);
	}

	//���콺�� ������ ���� ���� �����
	if(position_vec.size() > 0){
		for(int i=0; i<position_vec.size(); i++){
			if(i == select)
				ofSetColor(0,255,0);
			else
				ofSetColor(255,255,255);
			ofCircle(position_vec[i].pixelPos,2);
			ofLine(position_vec[i].pixelPos,ofPoint(550,20*i+20));
			ofDrawBitmapStringHighlight(ofToString(position_vec[i].pixelPos),550,20*i+10);
			ofDrawBitmapStringHighlight("from Camera : " + ofToString(position_vec[i].worldPos.x) + "/ "
													     + ofToString(position_vec[i].worldPos.y) + "/ "
														 + ofToString(position_vec[i].worldPos.z),630,20*i+10);
		}
	}
}

/*
//--depth �ҽ��� x,y ��ǥ�� �����ϸ� �ش� ���� ���� �������ִ� �Լ�
float ofApp::getDistanceAt(int x, int y) {
	return depthPixelsRaw[y * IR_WIDTH + x]; 
}
*/

/*
//--���콺�� ������ ���� MapDepthPointToCameraSpace ���� �������ִ� �Լ�
CameraSpacePoint ofApp::getDepthToCamera(ofPoint position){
	CameraSpacePoint csp;
	DepthSpacePoint dsp;
	dsp.X = position.x;
	dsp.Y =	position.y;
	kinect.getDepthSource()->coordinateMapper->MapDepthPointToCameraSpace(dsp,getDistanceAt(position.x,position.y),&csp);
	
	return csp;
}
*/

//depthImg���� ������ R,G,B�� ���� ���� ��ǥ�� X,Y,Z�̴�. �� ���� ��������.
ofVec3f ofApp::getWorldFromColor(ofPoint position){
	ofVec3f colorToWorld;
	colorToWorld = ofVec3f(depthImg.getColor(position.x,position.y).r,depthImg.getColor(position.x,position.y).g,depthImg.getColor(position.x,position.y).b);
	return colorToWorld;
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
			}
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
	//TODO : �巡�� �־� ���� ��
	//mousePosition[select] += ofVec2f(x-firstClick.x,y-firstClick.y);
	//cout <<x-firstClick.x << endl;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	isClicked = true;	
	if(button == 0){
		Position_str tmp;

		firstClick = ofPoint(x,y);
		tmp.pixelPos = ofVec2f(mouseX,mouseY);
		tmp.worldPos = getWorldFromColor(tmp.pixelPos);

 		position_vec.push_back(tmp);
		if(position_vec.size() == 1)
			select = 0;
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
	isClicked = false;
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
