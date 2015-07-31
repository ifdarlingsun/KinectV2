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

	//그냥 c++로 사용해보려고 했던 것. of의 어떤 부분과 충돌하는 듯 하다.
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
	1) 적외선으로만 Depth를 받아오는 것은 어떤가 싶네. 왜냐하면 아무리해도 Color와 IR이 보는 것이 다르기 때문
	IR의 값을 가지고도 영상처리를 할 수 있을 것 같다. 그러면 마우스 지점의 depth를 받아오도록 해보자.

	2) 하지만 적외선이 볼 수 있는 영역이 많지 않은 것 같다.  color 이미지가 필요하겠음. 하지만 어느 정도 먼 곳만 유효하다.
	*/
	
	//Depth 소스를 받아서 깊이 값을 보정해준다.(/8000) 그것으로 Depth 이미지도 그림
	depthPixelsRaw = this->kinect.getDepthSource()->getPixelsRef();
	for (int i = 0; i < depthPixelsRaw.size(); i++){
		depthPixelsRaw[i] = (depthPixelsRaw[i] * 65536 / 8000);
	}
	depthImage.setFromPixels(depthPixelsRaw);
	ofSetColor(255);
	depthImage.draw(0,0);

	//IR 보여줌
	if(isIrDraw){
		this->kinect.getInfraredSource()->draw(0,0);
	}

	//마우스로 선택한 곳의 값을 출력함
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

//--depth 소스의 x,y 좌표를 선택하면 해당 깊이 값을 리턴해주는 함수
float ofApp::getDistanceAt(int x, int y) {
	return depthPixelsRaw[y * IR_WIDTH + x]; 
}

//--마우스를 선택한 곳의 MapDepthPointToCameraSpace 값을 리턴해주는 함수
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
	//TODO : 드래그 넣어 보는 중
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
