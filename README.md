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

You can add comments to a meta file using two slashes at the beginning of the <b>line</b> i.e ```// I am a comment```. </br>
A example Meta file can be found in game/res/example.meta

## Creating Model Files
Model files are stored as .amod files in Aquarite3D. These files can contain multiple models, but it is not necessary to store all models in 1 file, a model file also uses the 'radius' & 'ignoreFrustum' variables for frustum culling purposes.

To create model files you need to have a mesh and a material, these should be specified in the .meta file
A model can hold multiple meshes and materials, a model should be difined in this order:
```
#MODEL
name=UNIQUE_MODEL_NAME
mesh=MESH_01
material=MATERIAL_01
mesh=MESH_02
material=MATERIAL_02
radius=20.0f
ignoreFrustum=0
```
As you can see you can define multiple meshes and materials, though it should be noted that the material of the mesh should be defined <b>below</b> the mesh itself. So if you want to use the same material on multiple meshes:
```
#MODEL
name=UNIQUE_MODEL_NAME
mesh=MESH_01
material=MATERIAL_01
mesh=MESH_02
material=MATERIAL_01
radius=20.0f
ignoreFrustum=0
```
As you can see Mesh_02 now uses Material_01
A example Model file can be found in game/res/example/models/model.amod

## Creating Material Files
Material files are stored as .amat files in Aquarite3D. Thes files can contain multiple materials, but it is not necessary to store all materials in 1 file.

A material in a .amat file is defined by the <b>#MAT</b> keyword, a material should have a unique name.
A material has multiple properties such as color, diffuseMap, diffuseColor, ambientColor, specular and more.

an material definition could look like:
```
#MAT
name=MATERIAL_NAME
diffuseMap=TEXTURE_NAME
shader=_aquariteDefaultShader
shininess=32.0f
color=1.0f,1.0f,1.0f
diffuseColor=1.0f,1.0f,1.0f
ambientColor=1.0f,1.0f,1.0f
specular=0.5f,0.5f,0.5f
```
If you dont want to use a diffuseMap or any other property you can just remove the entire property, also Texture Name should be the variable name you defined in the .meta file

<b> Temporary Notice </b></br>
Currently shaders are not supported in meta files, they will be in the near future and shaders can then be loaded by the ResourceManager, for now you can use the ```_aquariteDefaultShader``` shader that is loaded by default. 
Also more properties like normal maps are to be added.

## License

Copyright (C) 2019  Jens Heukers

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
