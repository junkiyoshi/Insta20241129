#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofSetLineWidth(1.5);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->noise_param = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 50 < 30) {

		this->noise_param += ofMap(ofGetFrameNum() % 50, 0, 30, 0.1, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	auto radius = 30;
	auto x_span = radius * sqrt(3);
	auto flg = true;
	ofColor color;

	for (float y = 0; y <= 720 + radius; y += radius * 1.5) {

		for (float x = 0; x <= 720 + radius; x += x_span) {

			glm::vec3 location;
			if (flg) {

				location = glm::vec3(x, y, 0);
			}
			else {

				location = glm::vec3(x + (x_span / 2), y, 0);
			}

			ofPushMatrix();
			ofTranslate(location);
			ofRotate(90);

			int hue = ofMap(x, 0, 720, 120, 230);
			for (int draw_radius = radius - 5; draw_radius > 0; draw_radius -= 200) {

				auto x_seed = ofRandom(1000);
				auto y_seed = ofRandom(1000);

				auto deg_start = ofMap(ofNoise(location.x * 0.05, location.y * 0.05, this->noise_param, x_seed), 0, 1, 0, 360);
				auto deg_len = ofMap(ofNoise(location.x * 0.05, location.y * 0.05, this->noise_param), 0, 1, -100, 720);
				auto deg_end = deg_start + deg_len;

				if (deg_len < 0) { continue; }

				vector<glm::vec2> vertices;
				for (int deg = deg_start; deg <= deg_end && deg <= deg_start + 360; deg += 1) {

					int param = deg % 60;

					glm::vec2 location;
					if (param == 0) {

						location = glm::vec2(draw_radius * cos(deg * DEG_TO_RAD), draw_radius * sin(deg * DEG_TO_RAD));
					}
					else {

						int p1 = deg / 60;
						int p2 = p1 + 1;

						auto p1_deg = p1 * 60;
						auto p2_deg = p2 * 60;

						auto source = glm::vec2(draw_radius * cos(p1_deg * DEG_TO_RAD), draw_radius * sin(p1_deg * DEG_TO_RAD));
						auto target = glm::vec2(draw_radius * cos(p2_deg * DEG_TO_RAD), draw_radius * sin(p2_deg * DEG_TO_RAD));

						location = source + ((target - source) / 60 * param);
					}

					vertices.push_back(location);
				}

				hue = (hue + 32) % 255;
				color.setHsb(hue, 255, 255);

				ofFill();
				ofSetColor(ofColor(color, 92));

				ofBeginShape();
				ofVertices(vertices);
				ofEndShape(true);

				ofNoFill();
				ofSetColor(color);

				ofBeginShape();
				ofVertices(vertices);
				ofEndShape(true);
			}
			
			ofPopMatrix();
		}
		flg = !flg;
	}

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}