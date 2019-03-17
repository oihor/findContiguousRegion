# findContiguousRegion

## Lib API
Class ContiguousRegionFinder represents a finding algorithm 
for a contiguous region of pixels of similar color in an image.

```
ContiguousRegionFinder(cv::Mat&& image);
```
Constructs a ContiguousRegionFinder finder object

```
std::vector<cv::Point> find(
    uint16_t pixelCol,
    uint16_t pixelRow,
    uint8_t deltaBlue = 16,
    uint8_t deltaGreen = 16,
    uint8_t deltaRed = 16
) const;
```
Runs the finding algorithm. Accepts a point to start from and RGB component deltas


## CLI API
Options:
  -h [ --help ]         Help  
  -i [ --image ] arg    image file  
  -r [ --row ] arg      pixel row coordinate   
  -c [ --col ] arg      pixel column coordinate  
  --db arg (=32)        delta for Blue component  
  --dg arg (=32)        delta for Green component  
  --dr arg (=32)        delta for Red component
  
Marks the contiguous region with black color and ouputs image to out.inputFilename


## References

* https://en.wikipedia.org/wiki/Flood_fill
* https://www.techiedelight.com/flood-fill-algorithm
* https://www.hackerearth.com/practice/algorithms/graphs/flood-fill-algorithm/tutorial
* https://techdifferences.com/difference-between-bfs-and-dfs.html
