#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

#define IR_WIDTH 512
#define IR_HEIGHT 424


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxKFW2::Device kinect;

		ofImage			depthImage;
		ofImage			depthImage2;

		ofShortPixels	depthPixelsRaw;
		ofFloatPixels	depthPixelsFloats;

		bool isIrDraw;
		bool isClicked;
		int		select;

		vector<ofVec2f> mousePosition;
		ofPoint			firstClick;
		float getDistanceAt(int x, int y);

		//test
		ICoordinateMapper * coordinateMapperSB;
		CameraSpacePoint csp;
		CameraSpacePoint getDepthToCamera(ofPoint position);
	//	c++로 그냥 사용해보려고 해봤음
		/*
		IDepthFrameSource *depthSourse;
		IDepthFrameReader *depthReader;
		*/
};
