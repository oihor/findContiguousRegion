/*
 * findcontiguousregion.cpp
 *
 *  Created on: Mar 16, 2019
 *      Author: igor
 */

#include <iostream>
#include <queue>

#include "findcontiguousregion.h"

namespace findcontiguousregion {

ContiguousRegionFinder::ContiguousRegionFinder(cv::Mat&& image)
    : _image(image)
{
    _queued.resize(_image.cols * _image.rows, false);
}

std::vector<cv::Point> ContiguousRegionFinder::find(
    uint16_t pixelCol,
    uint16_t pixelRow,
    uint8_t deltaBlue,
    uint8_t deltaGreen,
    uint8_t deltaRed
) const
{
    // Below array details 8 possible movements
    // Probably could also be 4 - East, West, South, North
    const cv::Point Shifts[] = {
        {-1, -1},
        {-1,  0},
        {-1,  1},
        { 0, -1},
        { 0,  1},
        { 1, -1},
        { 1,  0},
        { 1,  1}
    };
    const cv::Point originCoord{pixelCol, pixelRow};
    const cv::Vec3b originColor = _image.at<cv::Vec3b>(originCoord);

    std::vector<cv::Point> ret;
    std::queue<cv::Point> queue;

    _queued.clear();
    _queued.resize(_image.cols * _image.rows, false);

    queue.push(originCoord);
    setQueued(originCoord);
    ret.push_back(originCoord);

    // run till queue is not empty
    while(!queue.empty()) {
        // pop front node from queue and process it
        cv::Point coord = queue.front();
        queue.pop();

        // process adjacent pixels of current pixel and
        // enqueue each valid pixel
        for(const cv::Point& shift: Shifts) {
            cv::Point adjacent{coord.x + shift.x, coord.y + shift.y};
            if(!isQueued(adjacent)
                && isContiguous(adjacent, originColor, deltaBlue, deltaGreen, deltaRed)
            ) {
                // enqueue adjacent pixel
                queue.push(adjacent);
                setQueued(adjacent);
                ret.push_back(adjacent);
            }
        }

    }

    return ret;
}

} /* namespace findcontiguousregion */
