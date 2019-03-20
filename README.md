# findContiguousRegion

## Lib API

Class IsColorDistanceClose represents a functor
that checks color distance as a square root
of a sum of squared component distances.

```
IsColorDistanceClose(uint16_t distance);
```
Constructs IsColorDistanceClose functor.


Class AreColorComponentsClose represents a functor
that checks color distance for each component separately.
If any component distance is bigger, colors are not considered as close.

```
AreColorComponentsClose(
	uint16_t comp0distance,
	uint16_t comp1distance,
	uint16_t comp2distance
);
AreColorComponentsClose(
	uint16_t compDistance
);
```
Constructs AreColorComponentsClose functor.


Class ContiguousRegionFinder represents a searching algorithm 
for a contiguous region of pixels of similar color in an image.

```
ContiguousRegionFinder(cv::Mat&& image);
```
Constructs a ContiguousRegionFinder finder object

```
template<class IsSimilar = IsColorDistanceClose>
std::vector<cv::Point> find(
	const cv::Point& originCoord,
	const IsSimilar& isSimilar
) const;
```
Runs the search algorithm.
Accepts a point to start from and a comparison functor.


## CLI API
Options:  
  -h [ --help ]           Help  
  -i [ --image ] arg      image file  
  -r [ --row ] arg        pixel row coordinate  
  -c [ --col ] arg        pixel column coordinate  
  -d [ --dist ] arg (=64) total color distance (sqrt(sum(pow(componentDistance,2)))  
  --db arg (=32)          delta for Blue component  
  --dg arg (=32)          delta for Green component  
  --dr arg (=32)          delta for Red component
  
Uses color distance if dist specified or none of components is specified.
Uses component distance if any of components is specified.
Marks the contiguous region with black color and ouputs image to out.inputFilename


## References

* https://en.wikipedia.org/wiki/Flood_fill
* https://www.techiedelight.com/flood-fill-algorithm
* https://www.hackerearth.com/practice/algorithms/graphs/flood-fill-algorithm/tutorial
* https://techdifferences.com/difference-between-bfs-and-dfs.html
* https://stackoverflow.com/questions/935992/find-images-of-similar-color
