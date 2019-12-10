### Accessing Parameters in Raptor Scripts

Chosen Few FX's **Raptor** API makes it very easy to customize your scripts using the power of parameters.  

All Raptor scripts have access to three mandated parameters:

* Primary range parameter
* Secondary range parameter
* Color parameter

The primary and secondary range parameters can be accessed as `double` precision floating point values.  Both parameters have a minimum value of -10.0 and a maximum value of 10.0, but can be scaled up or down to the desired functional ranges during processing.  

The color parameter can be accessed as a `Pixel` object, and can be adjusted by the user via the plugin-host's color picking dialog.  

If a script's main function is defined as follows:

`(api, src, x, y) => { ... }`

then the parameters that Raptor provides may be accessed through the API object via the following expressions:

* `api.Param.Primary` for the primary range parameter

* `api.Param.Secondary` for the secondary range parameter

* `api.Param.Color` for the color parameter

Happy coding!