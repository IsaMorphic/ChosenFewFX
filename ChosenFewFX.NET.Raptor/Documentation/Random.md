### Generating Random Numbers in Raptor Scripts

Chosen Few FX's **Raptor** API provides a specific facility for the generation of pseudo-random numbers in user code.  If a script's main function is defined as follows:

`(api, src, x, y) => { ... }`

then a safe-to-use `System.Random` object may be obtained via the `api.Random` property.  

**Note:** the Raptor API specifically provides this facility due to the difficulties of using the `System.Random` object in multi-threaded code.  **User code should not create its own instances of `System.Random`, use the `Random` property instead.**  

More about the `System.Random` class and how it can be used can be found here at https://docs.microsoft.com/en-us/dotnet/api/system.random

Happy coding!