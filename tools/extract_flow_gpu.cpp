#include "dense_flow.h"
#include "utils.h"

INITIALIZE_EASYLOGGINGPP

using namespace cv::gpu;

int main(int argc, char** argv){
	// IO operation
	const char* keys =
		{
			"{ f  | vidFile      | ex2.avi | filename of video }"
			"{ x  | xFlowFile    | flow_x | filename of flow x component }"
			"{ y  | yFlowFile    | flow_y | filename of flow x component }"
			"{ i  | imgFile      | flow_i | filename of flow image}"
			"{ b  | bound | 15 | specify the maximum of optical flow}"
			"{ t  | type | 0 | specify the optical flow algorithm }"
			"{ d  | device_id    | 0  | set gpu id}"
			"{ s  | step  | 1 | specify the step for frame sampling}"
                        "{ ss | stride| 1 | specify the stride for flow calculation}"
			"{ o  | out | zip | output style}"
			"{ w  | newWidth | 0 | output style}"
			"{ h  | newHeight | 0 | output style}"
			"{ r  | reverse   | 0 | if calculate flow reversely }"
		};

	CommandLineParser cmd(argc, argv, keys);
	string vidFile = cmd.get<string>("vidFile");
	string xFlowFile = cmd.get<string>("xFlowFile");
	string yFlowFile = cmd.get<string>("yFlowFile");
	string imgFile = cmd.get<string>("imgFile");
	string output_style = cmd.get<string>("out");
	int bound = cmd.get<int>("bound");
    int type  = cmd.get<int>("type");
    int device_id = cmd.get<int>("device_id");
    int step = cmd.get<int>("step");
    int stride = cmd.get<int>("stride");
    int new_height = cmd.get<int>("newHeight");
    int new_width = cmd.get<int>("newWidth");
    int if_reverse = cmd.get<int>("reverse");

	vector<vector<uchar> > out_vec_x, out_vec_y, out_vec_img;

	//calcDenseFlowGPU(vidFile, bound, type, step, device_id,
	//				 out_vec_x, out_vec_y, out_vec_img, new_width, new_height, if_reverse);
	calcDenseFlowGPU(vidFile, bound, type, step, device_id,
					 out_vec_x, out_vec_y, out_vec_img, new_width, new_height, stride, if_reverse);

	if (output_style == "dir") {
		writeImages(out_vec_x, xFlowFile);
		writeImages(out_vec_y, yFlowFile);
		writeImages(out_vec_img, imgFile);
	}else{
//		LOG(INFO)<<"Writing results to Zip archives";
                if (!if_reverse) {
                  if (stride>1) {
                    char name_x_leading[256];
                    char name_x_tmpl[256];
                    char name_y_leading[256];
                    char name_y_tmpl[256];
                    sprintf(name_x_leading, "x_s%05d", stride);
                    strcpy(name_x_tmpl, name_x_leading);
                    strcat(name_x_tmpl, "_%05d.jpg");
                    sprintf(name_y_leading, "y_s%05d", stride);
                    strcpy(name_y_tmpl, name_y_leading);
                    strcat(name_y_tmpl, "_%05d.jpg");
		    writeZipFile(out_vec_x, name_x_tmpl, xFlowFile+".zip");
		    writeZipFile(out_vec_y, name_y_tmpl, yFlowFile+".zip");
                  } else {
		    writeZipFile(out_vec_x, "x_%05d.jpg", xFlowFile+".zip");
		    writeZipFile(out_vec_y, "y_%05d.jpg", yFlowFile+".zip");
                  }
                } else {
                  if (stride>1) {
                    char name_x_leading[256];
                    char name_x_tmpl[256];
                    char name_y_leading[256];
                    char name_y_tmpl[256];
                    sprintf(name_x_leading, "xr_s%05d", stride);
                    strcpy(name_x_tmpl, name_x_leading);
                    strcat(name_x_tmpl, "_%05d.jpg");
                    sprintf(name_y_leading, "yr_s%05d", stride);
                    strcpy(name_y_tmpl, name_y_leading);
                    strcat(name_y_tmpl, "_%05d.jpg");
		    writeZipFile(out_vec_x, name_x_tmpl, xFlowFile+".zip");
		    writeZipFile(out_vec_y, name_y_tmpl, yFlowFile+".zip");
                  } else {
		    writeZipFile(out_vec_x, "xr_%05d.jpg", xFlowFile+".zip");
		    writeZipFile(out_vec_y, "yr_%05d.jpg", yFlowFile+".zip");
                  }
                }
		writeZipFile(out_vec_img, "img_%05d.jpg", imgFile+".zip");
	}

	return 0;
}
