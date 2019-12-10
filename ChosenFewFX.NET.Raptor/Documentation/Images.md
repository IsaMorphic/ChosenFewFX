### Accessing Image Data in Raptor Scripts

Chosen Few FX's **Raptor** API allows user code to access image data on the fly through a special `Image` object.  This object wraps a native image object in a way that is safe and easy for Raptor scripts to use and access through a few methods and properties.  

#### Accessing the Image object

The Raptor API object contains an `Image` object that wraps the native source image.  For the time being, Raptor scripts are not able to access destination image data.  If a script's main function is defined as follows:

`(api, src, x, y) => { ... }`

then the image object may be obtained via the `api.Image` property.  

#### Basic properties and methods

Raptor's `Image` object includes three basic members that allow user code to access and interpret image data.  The `Width` and `Height` properties allow scripts to access the width and height of the source image.  Simple enough.  The `Image` object also contains a `Read` method that user code can use to read pixel data directly from the source image.  The `Read` method takes in a couple of `int`s representing the x and y coordinates on the image to read the pixel from, and outputs a `Pixel` object accordingly.  

#### Extra properties and methods

As of the first iteration of the Raptor API, the `Image` object exposes a special `AverageColor` property that allows Raptor scripts to know the average overall color of the source image.  For efficiency, this average is calculated upon construction of the API object and before the main processing loop, and then stored for later access.  

#### Pitfalls and shortcomings

Here are some things to watch out for while writing and troubleshooting `Image` related issues in Raptor scripts:

* Because the Raptor script engine makes use of multi-threaded processing, it is possible for a script to attempt to access the same pixel at the same time from different threads, causing an error. 
* When using the `Image` object's `Read` method, Raptor scripts should not attempt to read pixels outside of the boundaries of the source image.  Doing so will cause an exception to be thrown, and all processing of the image is immediately halted.

Happy coding!