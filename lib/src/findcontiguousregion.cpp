/*
 * findcontiguousregion.cpp
 *
 *  Created on: Mar 16, 2019
 *      Author: igor
 */

#include <queue>

#include "findcontiguousregion.h"

namespace findcontiguousregion {

ContiguousRegionFinder::ContiguousRegionFinder(cv::Mat&& image)
    : _image(image)
{
}

std::vector<ColRow> ContiguousRegionFinder::find(
    uint16_t pixelCol,
    uint16_t pixelRow,
    uint8_t deltaBlue,
    uint8_t deltaGreen,
    uint8_t deltaRed
) const
{
    // Below array details 8 possible movements
    // Probably could also be 4 - East, West, South, North
    const ColRow Shifts[] = {
        {-1, -1},
        {-1,  0},
        {-1,  1},
        { 0, -1},
        { 0,  1},
        { 1, -1},
        { 1,  0},
        { 1,  1}
    };
    const ColRow originCoord{pixelCol, pixelRow};
    const cv::Vec3b originColor = _image.at<cv::Vec3b>(originCoord.col, originCoord.row);

    std::vector<ColRow> ret;
    std::queue<ColRow> queue;

    queue.push(originCoord);
    ret.push_back(originCoord);

    // run till queue is not empty
    while (!queue.empty()) {
        // pop front node from queue and process it
        ColRow coord = queue.front();
        queue.pop();

        // process adjacent pixels of current pixel and
        // enqueue each valid pixel
        for(const ColRow& shift: Shifts) {
            ColRow adjacent{coord.col + shift.col, coord.row + shift.row};
            if(isContiguous(adjacent, originColor, deltaBlue, deltaGreen, deltaRed)) {
                // enqueue adjacent pixel
                queue.push(adjacent);
                ret.push_back(adjacent);
            }
        }
    }

    return ret;
}

} /* namespace findcontiguousregion */
