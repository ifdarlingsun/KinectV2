#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initInfraredSource();
	kinect.initColorSource();
	isIrDraw = true;
	isClicked = true;

	select = 0;
	depthPixelsRaw.allocate(IR_WIDTH, IR_HEIGHT, 1);
	
	//depthPixelsFloats.allocate(IR_WIDTH, IR_HEIGHT, ofPixelFormat::OF_PIXELS_RGB);

	//�׳� c++�� ����غ����� �ߴ� ��. of�� � �κа� �浹�ϴ� �� �ϴ�.
	/*
	HRESULT	hResult = depthSourse->OpenReader(&depthReader);
	if(FAILED(hResult)){
		cerr << "error : iframesource->iframereader" <<endl;
	}
	*/
}

//--------------------------------------------------------------
void ofApp::update(){
	this->kinect.update();
	
	//IDepthFrame *depthFrame = nullptr;
}

//--------------------------------------------------------------
void ofApp::draw(){
	/*
	1) ���ܼ����θ� Depth�� �޾ƿ��� ���� ��� �ͳ�. �ֳ��ϸ� �ƹ����ص� Color�� IR�� ���� ���� �ٸ��� ����
	IR�� ���� ������ ����ó���� �� �� ���� �� ����. �׷��� ���콺 ������ depth�� �޾ƿ����� �غ���.

	2) ������ ���ܼ��� �� �� �ִ� ������ ���� ���� �� ����.  color �̹����� �ʿ��ϰ���. ������ ��� ���� �� ���� ��ȿ�ϴ�.
	*/
	
	//Depth �ҽ��� �޾Ƽ� ���� ���� �������ش�.(/8000) �װ����� Depth �̹����� �׸�
	depthPixelsRaw = this->kinect.getDepthSource()->getPixelsRef();
	for (int i = 0; i < depthPixelsRaw.size(); i++){
		depthPixelsRaw[i] = (depthPixelsRaw[i] * 65536 / 8000);
	}
	depthImage.setFromPixels(depthPixelsRaw);
	ofSetColor(255);
	depthImage.draw(0,0);

	//IR ������
	if(isIrDraw){
		this->kinect.getInfraredSource()->draw(0,0);
	}

	//���콺�� ������ ���� ���� �����
	if(mousePosition.size() > 0){
		for(int i=0; i<mousePosition.size(); i++){
			if(i == select)
				ofSetColor(0,255,0);
			else
				ofSetColor(255,255,255);
			ofCircle(mousePosition[i],2);
			ofLine(mousePosition[i],ofPoint(550,20*i+20));
			ofDrawBitmapStringHighlight(ofToString(mousePosition[i]),550,20*i+10);
			//ofDrawBitmapStringHighlight("from Camera : " + ofToString(getDistanceAt((int)mousePosition[i].x,(int)mousePosition[i].y))+"mm",630,20*i+10);
			ofDrawBitmapStringHighlight("from Camera : " + ofToString(getDepthToCamera(mousePosition[i]).X*100) + ","
													     + ofToString(getDepthToCamera(mousePosition[i]).Y*100) + ","
														 + ofToString(getDepthToCamera(mousePosition[i]).Z*100),630,20*i+10);
		}
	}
}

//--depth �ҽ��� x,y ��ǥ�� �����ϸ� �ش� ���� ���� �������ִ� �Լ�
float ofApp::getDistanceAt(int x, int y) {
	return depthPixelsRaw[y * IR_WIDTH + x]; 
}

//--���콺�� ������ ���� MapDepthPointToCameraSpace ���� �������ִ� �Լ�
CameraSpacePoint ofApp::getDepthToCamera(ofPoint position){
	CameraSpacePoint csp;
	DepthSpacePoint dsp;
	dsp.X = position.x;
	dsp.Y =	position.y;
	kinect.getDepthSource()->coordinateMapper->MapDepthPointToCameraSpace(dsp,getDistanceAt(position.x,position.y),&csp);
	
	return csp;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case OF_KEY_TAB:
			if(mousePosition.size() > 0)
				select = ++select%mousePosition.size();
			break;
		
		case ' ' :
			isIrDraw = !isIrDraw;
			break;

		case OF_KEY_UP :
			mousePosition[select].y--;
			break;

		case OF_KEY_DOWN :
			mousePosition[select].y++;
			break;

		case OF_KEY_LEFT :
			mousePosition[select].x--;
			break;

		case OF_KEY_RIGHT :
			mousePosition[select].x++;
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
		firstClick = ofPoint(x,y);
		mousePosition.push_back(ofVec2f(mouseX,mouseY));
		if(mousePosition.size() == 1)
			select = 0;
	}else{
		if(mousePosition.size() > 0){
			mousePosition.erase(mousePosition.begin()+select);
			select = mousePosition.size()-1;
		}else if(mousePosition.size() == 0){
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
