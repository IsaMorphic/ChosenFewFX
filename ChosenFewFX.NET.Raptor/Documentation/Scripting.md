

### Scripting with Raptor and Chosen Few FX

This document gives a brief overview of how Chosen Few FX's **Raptor** scripting system is designed and a basic explanation of how it functions.  For more information regarding the Raptor API itself, see the relevant documents in [Documentation](../Documentation).  

#### Introduction

First of all, it is important to note that the Raptor plugins make use of Microsoft's C# Scripting API in order to compile and run user code during runtime.  For a general reference on C# and more specifically the modifications made to the language in the scripting context, see https://docs.microsoft.com/en-us/dotnet/csharp/ and https://github.com/dotnet/roslyn/wiki/Interactive-Window#directives (relevant sections: `#r` and `#load`), respectively.  

#### Environment

By default, Raptor scripts **only** import the `System` and `ChosenFewFX.NET.Raptor` namespaces.  This is usually all your scripts will ever need.  If it is desired, scripts may also reference other assemblies and load code from other scripts as they please.  (Note: all relative paths specified in the `#r` and `#load` directives are relative to the location of the script file, not the host executable.)

#### Requirements

All Raptor scripts are expected to return an expression of the type `Func<Api, Pixel, int, int, Pixel>`, a function object that receives the Raptor API object, a source pixel, and the pixel's X and Y coordinates on the source image.  After presumably doing some processing, the function is expected to return a Pixel object containing the desired color information.  

#### Execution

Under the hood, Raptor first builds the API object.  During this phase of execution, the plugin wraps the source image in a safe wrapper object, as well as the values of the three parameters (see Parameters.md).  After constructing the API object, Raptor begins to loop through all of the pixels in the source image.  For each pixel in the source image, Raptor executes the function that the user has provided, passing to it the required inputs.  Once the user's function has finished processing, Raptor writes the output pixel to the destination image (at the same coordinates as the source pixel).  After Raptor has repeated this process for all pixels in the source image, the resulting destination image is sent off to the host software.  

#### Examples

For example scripts see [Examples](../Examples)

#### Suggestions?

It is important to note that all elements of the Raptor API are subject to change, and more features will come with time.  If you have any suggestions on how Chosen Few Software should expand Raptor's functionality, feel free to open an issue on GitHub.  