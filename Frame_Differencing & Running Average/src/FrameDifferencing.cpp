#include "FrameDifferencing.h"
#include <algorithm>
#include <cstdint>

void FrameDifferencing::bgrToGrayManual(const cv::Mat& bgr, cv::Mat& gray) {
    // Validaciones mínimas
    if (bgr.empty() || bgr.type() != CV_8UC3) {
        gray.release();
        return;
    }

    gray.create(bgr.rows, bgr.cols, CV_8UC1);

    for (int y = 0; y < bgr.rows; ++y) {
        const cv::Vec3b* row = bgr.ptr<cv::Vec3b>(y);
        unsigned char* out = gray.ptr<unsigned char>(y);

        for (int x = 0; x < bgr.cols; ++x) {
            const unsigned char B = row[x][0];
            const unsigned char G = row[x][1];
            const unsigned char R = row[x][2];

            // Luma aproximada (BT.601) en enteros:
            // gray = 0.299R + 0.587G + 0.114B
            // = (77*R + 150*G + 29*B) / 256
            const int g = (77 * R + 150 * G + 29 * B) >> 8;
            out[x] = static_cast<unsigned char>(g);
        }
    }
}

void FrameDifferencing::absDiffManual(const cv::Mat& a, const cv::Mat& b, cv::Mat& diff) {
    if (a.empty() || b.empty() || a.type() != CV_8UC1 || b.type() != CV_8UC1 ||
        a.size() != b.size()) {
        diff.release();
        return;
    }

    diff.create(a.rows, a.cols, CV_8UC1);

    for (int y = 0; y < a.rows; ++y) {
        const unsigned char* pa = a.ptr<unsigned char>(y);
        const unsigned char* pb = b.ptr<unsigned char>(y);
        unsigned char* pd = diff.ptr<unsigned char>(y);

        for (int x = 0; x < a.cols; ++x) {
            const int da = static_cast<int>(pa[x]);
            const int db = static_cast<int>(pb[x]);
            const int v = da - db;
            pd[x] = static_cast<unsigned char>(v < 0 ? -v : v);
        }
    }
}

void FrameDifferencing::thresholdManual(const cv::Mat& src, cv::Mat& dst, unsigned char thresh) {
    if (src.empty() || src.type() != CV_8UC1) {
        dst.release();
        return;
    }

    dst.create(src.rows, src.cols, CV_8UC1);

    for (int y = 0; y < src.rows; ++y) {
        const unsigned char* ps = src.ptr<unsigned char>(y);
        unsigned char* pd = dst.ptr<unsigned char>(y);

        for (int x = 0; x < src.cols; ++x) {
            pd[x] = (ps[x] > thresh) ? 255 : 0;
        }
    }
}
