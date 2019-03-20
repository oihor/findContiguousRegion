/*
 * findcontiguousregion.cpp
 *
 *  Created on: Mar 16, 2019
 *      Author: igor
 */

#include "findcontiguousregion.h"

namespace findcontiguousregion {

ContiguousRegionFinder::ContiguousRegionFinder(cv::Mat&& image)
    : _image(image)
{
    _queued.resize(_image.cols * _image.rows, false);
}

cv::Mat ContiguousRegionFinder::reset(cv::Mat&& newImage) {
    cv::Mat ret(std::move(_image));
    _image = std::move(newImage);
    _queued.clear();
    _queued.resize(_image.cols * _image.rows, false);
    return ret;
}

} /* namespace findcontiguousregion */
