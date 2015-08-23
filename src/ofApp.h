#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

#define IR_WIDTH 512
#define IR_HEIGHT 424

#define READY 0
#define CROSS 1
#define VECTOR 2

struct Position_str{
	ofVec2f	pixelPos;
	ofVec3f worldPos;
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
		bool isFullscreen;
		int	 select;

		vector<Position_str> position_vec;

		ofVec3f		getWorldFromColor(ofPoint position);
		ofVec2f		getIntersectionPoint(ofPoint lineA1, ofPoint lineA2,ofPoint lineB1, ofPoint lineB2);
		ofVec3f		getNormalVector(ofVec3f P, ofVec3f Q, ofVec3f R);

		ofVec2f		intersectionPoint;
		ofVec3f		intersectionWorld;
		ofVec2f		rightVectorPoint;
		ofVec3f		rightVectorWorld;

		int mode;
};
