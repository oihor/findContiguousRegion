/*
 * findcontiguousregion.h
 *
 *  Created on: Mar 16, 2019
 *      Author: igor
 */

#ifndef CLI_FINDCONTIGUOUSREGION_H_
#define CLI_FINDCONTIGUOUSREGION_H_

#include <opencv2/core/core.hpp>

namespace findcontiguousregion {

class ContiguousRegionFinder {
public:
    ContiguousRegionFinder(cv::Mat&& image);

    cv::Mat reset(cv::Mat&& newImage = cv::Mat{}) {
        cv::Mat ret(std::move(_image));
        _image = std::move(newImage);
        _queued.clear();
        _queued.resize(_image.cols * _image.rows, false);
        return ret;
    }

    std::vector<cv::Point> find(
        uint16_t pixelCol,
        uint16_t pixelRow,
        uint8_t deltaBlue = 16,
        uint8_t deltaGreen = 16,
        uint8_t deltaRed = 16
    ) const;

private:

    static inline uchar absDiff(uchar a, uchar b) {
        return (a > b) ? a - b : b - a;
    }

    // following functions can be optimized for less boundaries checks
    // check for queued inside isContiguous

    inline bool isContiguous(
        const cv::Point& coord,
        const cv::Vec3b& originColor,
        uint8_t deltaBlue,
        uint8_t deltaGreen,
        uint8_t deltaRed
    ) const {
        // check for boundaries
        if(coord.x < 0 || coord.x >= _image.cols || coord.y < 0 || coord.y >= _image.rows) {
            return false;
        }

        // check for color components delta
        const cv::Vec3b& pixelColor = _image.at<cv::Vec3b>(coord);
        if(    absDiff(pixelColor[0], originColor[0]) > deltaBlue
            || absDiff(pixelColor[1], originColor[1]) > deltaGreen
            || absDiff(pixelColor[2], originColor[2]) > deltaRed
        ) {
            return false;
        }

        return true;
    }

    inline bool isQueued(const cv::Point& coord) const {
        // check for boundaries
        if(coord.x < 0 || coord.x >= _image.cols || coord.y < 0 || coord.y >= _image.rows) {
            return true;
        }

        return _queued[coord.x + coord.y * _image.cols];
    }

    inline void setQueued(const cv::Point& coord) const {
        // check for boundaries
        if(coord.x < 0 || coord.x >= _image.cols || coord.y < 0 || coord.y >= _image.rows) {
            return;
        }

        _queued[coord.x + coord.y * _image.cols] = true;
    }

    cv::Mat _image;
    mutable std::vector<uint8_t> _queued;
};

} /* namespace findcontiguousregion */

#endif /* CLI_FINDCONTIGUOUSREGION_H_ */
