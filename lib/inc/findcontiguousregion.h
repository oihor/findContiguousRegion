/*
 * findcontiguousregion.h
 *
 *  Created on: Mar 16, 2019
 *      Author: igor
 */

#ifndef CLI_FINDCONTIGUOUSREGION_H_
#define CLI_FINDCONTIGUOUSREGION_H_

#include <queue>

#include <opencv2/core/core.hpp>

namespace findcontiguousregion {

/**
 * Class IsColorDistanceClose represents a functor
 * that checks color distance as a square root
 * of a sum of squared component distances.
 */
class IsColorDistanceClose {
public:

    /**
     * Constructs IsColorDistanceClose functor.
     *
     * @param distance - total color distance
     */
    IsColorDistanceClose(uint16_t distance)
        : _distanceSquared(distance * distance)
    {}

    bool operator()(const cv::Vec3b& pixelA, const cv::Vec3b& pixelB) const {
        // although total color distance could be more appropriate
        uint32_t total = 0;
        for(size_t i = 0; i < cv::Vec3b::channels; ++i) {
            int diff = int(pixelA[0]) - int(pixelB[0]);
            total += diff * diff;
        }
        return total <= _distanceSquared;
    }

private:

    int _distanceSquared;
};

/**
 * Class AreColorComponentsClose represents a functor
 * that checks color distance for each component separately.
 * If any component distance is bigger, colors are not considered as close.
 */
class AreColorComponentsClose {
public:

    /**
     * Constructs AreColorComponentsClose functor.
     *
     * @param comp0distance - distance for the 1st color component
     * @param comp1distance - distance for the 2nd color component
     * @param comp2distance - distance for the 3rd color component
     */
    AreColorComponentsClose(
        uint16_t comp0distance,
        uint16_t comp1distance,
        uint16_t comp2distance
    )
        : _comp0distance(comp0distance)
        , _comp1distance(comp1distance)
        , _comp2distance(comp2distance)
    {}

    /**
     * Constructs AreColorComponentsClose functor.
     *
     * @param compDistance - distance for a color component,
     *                       same for all components
     */
    AreColorComponentsClose(
        uint16_t compDistance
    )
        : AreColorComponentsClose(
            compDistance,
            compDistance,
            compDistance
          )
    {}

    bool operator()(const cv::Vec3b& pixelA, const cv::Vec3b& pixelB) const {
        return absDiff(pixelA[0], pixelB[0]) <= _comp0distance
            && absDiff(pixelA[1], pixelB[1]) <= _comp1distance
            && absDiff(pixelA[2], pixelB[2]) <= _comp2distance;
    }

private:

    static inline uchar absDiff(uchar a, uchar b) {
        return (a > b) ? a - b : b - a;
    }

    uint16_t _comp0distance;
    uint16_t _comp1distance;
    uint16_t _comp2distance;
};

/**
 * Class ContiguousRegionFinder represents a searching algorithm
 * for a contiguous region of pixels of similar color in an image.
 *
 * Typical usage is to initialize ContiguousRegionFinder
 * object with an opencv image and call
 * find() function.
 */

class ContiguousRegionFinder {
public:


    /**
     * Constructs a ContiguousRegionFinder finder object.
     *
     * @param image is an opencv image to be moved and owned by the finder object
     */
    ContiguousRegionFinder(cv::Mat&& image);

    /**
     * Reset the opencv image owned by the finder object.
     *
     * @param newImage - new image object to be moved and owned (or empty image)
     * @return previously stored image
     */
    cv::Mat reset(cv::Mat&& newImage = cv::Mat{});

    /**
     * Runs the search algorithm.
     * Accepts a point to start from and a comparison functor.
     *
     * @param originCoord - pixel origin coordinate
     * @param isSimilar - binary functor, expects two cv::Vec3b objects to compare,
     *                    returns true, if similar
     * @return vector of points of the contiguous region
     *         returned vector contains at least origin coordinate
     *         empty vector means error
     */
    template<class IsSimilar = IsColorDistanceClose>
    std::vector<cv::Point> find(
        const cv::Point& originCoord,
        const IsSimilar& isSimilar
    ) const
    {
        // Below array details 8(4) possible movements
        // 4 - East, West, South, North
        // valgrind shows 4 movements take less cycles
        const cv::Point Shifts[] = {
//          {-1, -1},
            {-1,  0},
//          {-1,  1},
            { 0, -1},
            { 0,  1},
//          { 1, -1},
            { 1,  0},
//          { 1,  1}
        };

        std::vector<cv::Point> ret;
        std::queue<cv::Point> queue; // deque in my gcc

        // check for boundaries
        if(!isValidPoint(originCoord)) {
            return ret;
        }

        const cv::Vec3b originColor = _image.at<cv::Vec3b>(originCoord);

        ret.reserve(_image.cols * _image.rows / 4);
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
                if(isValidPoint(adjacent)
                    && !isQueued(adjacent)
                    && isSimilar(_image.at<cv::Vec3b>(adjacent), originColor)
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

private:

    inline bool isValidPoint(const cv::Point& coord) const {
        return coord.x >= 0 && coord.x < _image.cols
            && coord.y >= 0 && coord.y < _image.rows;
    }

    inline bool isQueued(const cv::Point& coord) const {
        return _queued[coord.x + coord.y * _image.cols];
    }

    inline void setQueued(const cv::Point& coord) const {
        _queued[coord.x + coord.y * _image.cols] = true;
    }

    cv::Mat _image;
    mutable std::vector<uint8_t> _queued;
};

} /* namespace findcontiguousregion */

#endif /* CLI_FINDCONTIGUOUSREGION_H_ */
