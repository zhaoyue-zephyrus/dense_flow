//
// Created by yjxiong on 11/18/15.
//

#include "common.h"

void convertFlowToImage(const Mat &flow_x, const Mat &flow_y, Mat &img_x, Mat &img_y,
                               double lowerBound, double higherBound) {
#define CAST(v, L, H) ((v) > (H) ? 255 : (v) < (L) ? 0 : cvRound(255*((v) - (L))/((H)-(L))))
    for (int i = 0; i < flow_x.rows; ++i) {
        for (int j = 0; j < flow_y.cols; ++j) {
            float x = flow_x.at<float>(i,j);
            float y = flow_y.at<float>(i,j);
            img_x.at<uchar>(i,j) = CAST(x, lowerBound, higherBound);
            img_y.at<uchar>(i,j) = CAST(y, lowerBound, higherBound);
        }
    }
#undef CAST
}

void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,double, const Scalar& color){
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
}

void encodeFlowMap(const Mat& flow_map_x, const Mat& flow_map_y,
                   vector<uchar>& encoded_x, vector<uchar>& encoded_y,
                   int bound, bool to_jpg){
    Mat flow_img_x(flow_map_x.size(), CV_8UC1);
    Mat flow_img_y(flow_map_y.size(), CV_8UC1);

    convertFlowToImage(flow_map_x, flow_map_y, flow_img_x, flow_img_y,
                       -bound, bound);

    if (to_jpg) {
        imencode(".jpg", flow_img_x, encoded_x);
        imencode(".jpg", flow_img_y, encoded_y);
    }else {
        encoded_x.resize(flow_img_x.total());
        encoded_y.resize(flow_img_y.total());
        memcpy(encoded_x.data(), flow_img_x.data, flow_img_x.total());
        memcpy(encoded_y.data(), flow_img_y.data, flow_img_y.total());
    }
}

void convertFlowToImage16bit(const Mat &flow_x, const Mat &flow_y, Mat &img_x, Mat &img_y,
                               double lowerBound, double higherBound) {
#define CAST(v, L, H) ((v) > (H) ? 65535 : (v) < (L) ? 0 : cvRound(65535*((v) - (L))/((H)-(L))))
    for (int i = 0; i < flow_x.rows; ++i) {
        for (int j = 0; j < flow_y.cols; ++j) {
            float x = flow_x.at<float>(i,j);
            float y = flow_y.at<float>(i,j);
            unsigned int tmp_x = CAST(x, lowerBound, higherBound);
            img_x.at<uint16_t>(i,j) = tmp_x; 
            unsigned int tmp_y = CAST(y, lowerBound, higherBound);
            img_y.at<uint16_t>(i,j) = tmp_y; 
        }
    }
#undef CAST
}

void encodeFlowMap16bit(const Mat& flow_map_x, const Mat& flow_map_y,
                   vector<uchar>& encoded_x, vector<uchar>& encoded_y,
                   int bound, bool to_jp2){
    Mat flow_img_x(flow_map_x.size(), CV_16UC1);
    Mat flow_img_y(flow_map_y.size(), CV_16UC1);

    convertFlowToImage16bit(flow_map_x, flow_map_y, flow_img_x, flow_img_y,
                       -bound, bound);

    if (to_jp2) {
        imencode(".jp2", flow_img_x, encoded_x);
        imencode(".jp2", flow_img_y, encoded_y);
    }else {
        encoded_x.resize(flow_img_x.total());
        encoded_y.resize(flow_img_y.total());
        memcpy(encoded_x.data(), flow_img_x.data, flow_img_x.total());
        memcpy(encoded_y.data(), flow_img_y.data, flow_img_y.total());
    }
}

void convertFlowToImages(const Mat &flow_x, const Mat &flow_y, Mat &img_xh, Mat &img_xl, Mat &img_yh, Mat &img_yl,
                               double lowerBound, double higherBound) {
#define CAST(v, L, H) ((v) > (H) ? 65535 : (v) < (L) ? 0 : cvRound(65535*((v) - (L))/((H)-(L))))
    for (int i = 0; i < flow_x.rows; ++i) {
        for (int j = 0; j < flow_y.cols; ++j) {
            float x = flow_x.at<float>(i,j);
            float y = flow_y.at<float>(i,j);
            unsigned int tmp_x = CAST(x, lowerBound, higherBound);
            img_xh.at<uchar>(i,j) = (tmp_x >> 8) & 0xFF;
            img_xl.at<uchar>(i,j) = tmp_x & 0xFF;
            unsigned int tmp_y = CAST(y, lowerBound, higherBound);
            img_yh.at<uchar>(i,j) = (tmp_y >> 8) & 0xFF;
            img_yl.at<uchar>(i,j) = tmp_y & 0xFF;
        }
    }
#undef CAST
}

void encodeFlowMaps(const Mat& flow_map_x, const Mat& flow_map_y,
                   std::vector<uchar>& encoded_xh, std::vector<uchar>& encoded_xl,
                   std::vector<uchar>& encoded_yh, std::vector<uchar>& encoded_yl,
                   int bound, bool to_jpg){
    Mat flow_img_xh(flow_map_x.size(), CV_8UC1);
    Mat flow_img_xl(flow_map_x.size(), CV_8UC1);
    Mat flow_img_yh(flow_map_y.size(), CV_8UC1);
    Mat flow_img_yl(flow_map_y.size(), CV_8UC1);

    convertFlowToImages(flow_map_x, flow_map_y, flow_img_xh, flow_img_xl,
                       flow_img_yh, flow_img_yl, -bound, bound);

    if (to_jpg) {
        imencode(".jpg", flow_img_xh, encoded_xh);
        imencode(".jpg", flow_img_xl, encoded_xl);
        imencode(".jpg", flow_img_yh, encoded_yh);
        imencode(".jpg", flow_img_yl, encoded_yl);
    }else {
        encoded_xh.resize(flow_img_xh.total());
        encoded_xl.resize(flow_img_xl.total());
        encoded_yh.resize(flow_img_yh.total());
        encoded_yl.resize(flow_img_yl.total());
        memcpy(encoded_xh.data(), flow_img_xh.data, flow_img_xh.total());
        memcpy(encoded_xl.data(), flow_img_xl.data, flow_img_xl.total());
        memcpy(encoded_yh.data(), flow_img_yh.data, flow_img_yh.total());
        memcpy(encoded_yl.data(), flow_img_yl.data, flow_img_yl.total());
    }
}

void writeImages(vector<vector<uchar>> images, string name_temp){
    for (int i = 0; i < images.size(); ++i){
        char tmp[256];
        sprintf(tmp, "_%05d.jpg", i+1);
        FILE* fp;
        fp = fopen((name_temp + tmp).c_str(), "wb");
        fwrite( images[i].data(), 1, images[i].size(), fp);
        fclose(fp);
    }
}

