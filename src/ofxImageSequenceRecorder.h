/*

Based on code by Memo from this thread:
http://forum.openframeworks.cc/index.php?topic=1687.0

*/

#include "ofMain.h"    

template<typename PixelType>
struct QueuedImage {
	string fileName;
	ofPixels_<PixelType> pix;
};

template<typename PixelType>
class ofxImageSequenceRecorder_ : public ofThread {
public:    
      
    ofxImageSequenceRecorder_(){  
		resetCounter();
    }

	int getFrameCount() {
		return counter;
	}

	void resetCounter() {
		counter = 0;
	}
    
    void setPrefix(string pre){
        prefix = pre;
    }
    
    void setFormat(string fmt){
        format = fmt;
    }
       
    void threadedFunction() {    
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage<PixelType> i = q.front();

				if (format == "raw") {
					ofBuffer buffer = ofBuffer((char*) i.pix.getData(), i.pix.size() * i.pix.getBytesPerPixel());
					ofBufferToFile(i.fileName, buffer, true);
				} else {
					ofSaveImage(i.pix, i.fileName);
				}                

                q.pop();
            }
        }
    }   

	template<typename PixelType>
	void addFrame(ofBaseHasPixels_<PixelType>& img) {
		addFrame(img.getPixels());
	}

	template<typename PixelType>
    void addFrame(ofPixels_<PixelType>& pix) {  
        char fileName[255]; 
        sprintf(fileName, "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());     
        counter++;
        
        QueuedImage<PixelType> qImage;
        
        qImage.fileName = fileName;    
        qImage.pix = pix;
        
        q.push(qImage);
    }

protected:
	int counter;
	string prefix;
	string format;
	queue<QueuedImage<PixelType>> q;

};

typedef ofxImageSequenceRecorder_<unsigned char> ofxImageSequenceRecorder;
typedef ofxImageSequenceRecorder_<unsigned short> ofxShortImageSequenceRecorder;
typedef ofxImageSequenceRecorder_<float> ofxFloatImageSequenceRecorder;

template class ofxImageSequenceRecorder_<unsigned char>;
template class ofxImageSequenceRecorder_<unsigned short>;
template class ofxImageSequenceRecorder_<float>;