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

struct ColRow {
    int col = 0;
    int row = 0;
};

class ContiguousRegionFinder {
public:
    ContiguousRegionFinder(cv::Mat&& image);

    cv::Mat reset(cv::Mat&& newImage = cv::Mat{}) {
        cv::Mat ret(std::move(_image));
        _image = std::move(newImage);
        _visited.clear();
        _visited.resize(_image.cols * _image.rows, false);
        return ret;
    }

    std::vector<ColRow> find(
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

    // following three functions can be optimized for less boundaries checks
    // check for visited inside isContiguous

    inline bool isContiguous(
        ColRow coord,
        cv::Vec3b originColor,
        uint8_t deltaBlue,
        uint8_t deltaGreen,
        uint8_t deltaRed
    ) const {
        // check for boundaries
        if(coord.col < 0 || coord.col >= _image.cols || coord.row < 0 || coord.row >= _image.rows) {
            return false;
        }

        // check for color components delta
        cv::Vec3b pixelColor = _image.at<cv::Vec3b>(coord.row, coord.col);
        if(    absDiff(pixelColor[0], originColor[0]) > deltaBlue
            || absDiff(pixelColor[1], originColor[1]) > deltaGreen
            || absDiff(pixelColor[2], originColor[2]) > deltaRed
        ) {
            return false;
        }

        return true;
    }

    inline bool isVisited(ColRow coord) const {
        // check for boundaries
        if(coord.col < 0 || coord.col >= _image.cols || coord.row < 0 || coord.row >= _image.rows) {
            return true;
        }

        return _visited[coord.col * coord.row];
    }

    inline void setVisited(ColRow coord) const {
        // check for boundaries
        if(coord.col < 0 || coord.col >= _image.cols || coord.row < 0 || coord.row >= _image.rows) {
            return;
        }

        _visited[coord.col * coord.row] = true;
    }

    cv::Mat _image;
    mutable std::vector<bool> _visited;
};

} /* namespace findcontiguousregion */

#endif /* CLI_FINDCONTIGUOUSREGION_H_ */
