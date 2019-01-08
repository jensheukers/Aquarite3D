# Aquarite3D
Aquarite3D is a 3D game framework based on Modern OpenGL that currently only runs on Windows, 
It uses GLEW, GLFW3 and GLM as external libraries. The framework also has its own resource management. 
More on this later in the readme.

## How to get source for Windows
    1. Make sure to have Visual Studio 2015 or higher installed on your system with Visual C++
    2. Make sure you have Cmake installed.
    3. Clone the repository
    4. Run cmake from the root directory
   
## Meta Files and loading
Meta files are used in Aquarite3D to handle resource management, This allows for efficient and structured asset creation.
In the meta files you can specify Textures, Meshes, Materials and Models.
A meta file can be loaded into the framework using the method:
``` ResourceManager::LoadMeta(PATH_TO_METAFILE)```

## Creating Meta Files
To create a meta files a few things should be kept in mind, most importantly is the order of calling, Aquarite3D reads
Meta files from top to bottom. Since a material might require a texture and a model might require a material it is 
important to specify these files in a structured order. This order should always be:
```
0. Meta (Information about asset)
1. Textures
2. Meshes
3. Materials
4. Models
```

Aquarite3D requires some information about a meta file to access its contents, these are specified as the following:
```
#META
resourceName=YOUR_RESOURCE_NAME
offset=OFFSET_FROM_BUILD_DIRECTORY
author=YOUR_NAME
version=VERSION_HERE
```

a example Meta file can be found in game/res/example.meta

## License

Copyright (C) 2019  Jens Heukers

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
