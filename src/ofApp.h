#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

#define IR_WIDTH 512
#define IR_HEIGHT 424

struct Position_str{
	ofVec2f	pixelPos;
	ofVec3f worldPos;
	//CameraSpacePoint csp;
};

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
		ofFloatPixels	colorPixelsToWorld;
		ofFloatImage depthImg;  

		bool isDepthDraw;
		bool isClicked;
		bool isCspUpdate;
		int	 select;

		vector<Position_str> position_vec;
		ofPoint			firstClick;
		//float getDistanceAt(int x, int y);

		//CameraSpacePoint getDepthToCamera(ofPoint position);
		ofVec3f		getWorldFromColor(ofPoint position);

	//	c++로 그냥 사용해보려고 해봤음
		/*
		IDepthFrameSource *depthSourse;
		IDepthFrameReader *depthReader;
		*/
};
