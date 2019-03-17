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

/**
 * Class for finding a contiguous region of pixels
 * of similar color in an image.
 *
 * Typical usage is to initialize ContiguousRegionFinder
 * object with an opencv image and call
 * find() function
 */

class ContiguousRegionFinder {
public:


    /**
     * Constructs a ContiguousRegionFinder finder object
     *
     * @param image is an opencv image to be moved and owned by the finder object
     */
    ContiguousRegionFinder(cv::Mat&& image);

    /**
     * Reset the opencv image owned by the finder object
     *
     * @param newImage - new image object to be moved and owned (or empty image)
     * @return previously stored image
     */
    cv::Mat reset(cv::Mat&& newImage = cv::Mat{}) {
        cv::Mat ret(std::move(_image));
        _image = std::move(newImage);
        _queued.clear();
        _queued.resize(_image.cols * _image.rows, false);
        return ret;
    }

    /**
     * Run finding algorithm. Accepts pixel to start from and RGB component deltas
     *
     * @param pixelCol - pixel column coordinate
     * @param pixelRow - pixel row coordinate
     * @param deltaBlue - delta for Blue component
     * @param deltaGreen - delta for Green component
     * @param deltaRed - delta for Red component
     * @return vector of points of the contiguous region
     */
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
