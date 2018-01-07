# **RTR Textures Project**
Repository for hosting RTR2017 Final Project

Idea can be found [here](https://docs.google.com/document/d/1upf7tDbqfl3QZoaCxFFnFiGu49uX4hOHmkiuX2KfyOI/edit?usp=sharing).

### Project Structure

```
Root
|-- build                    // contains files required for building the solution. e.g. configuration files etc.
|-- common                   // contains files shared throughout the project. This can further include the headers and sources.
    |-- include
    |-- src
|-- doc                      // contains project documentation including design, flow of events etc.
|-- prototypes               // folder containing prototypes created for various approaches grouped by contributions per user
    |-- (username)
|-- src						 // folder containing main project source code
    |-- features			 // contains set of features plugged in the framework for drawing
		|-- Test			 // contains sources for test feature
		|-- (feature)
	|-- framework			 // contains framework headers and sources                         
|-- tools					 // folder containing tools required to build the project
|-- x64						 // 64-bit executable location
|-- RTRTextures.sln			 // main solution file for this project
|-- RTRTextures.vcxproj		 // main project file created in VS2015
|-- RTRTextures.vcxproj.filters // contains internal project structure for .vcxproj file
|-- README.md				// contains information about this repository
|-- .gitignore				// contains git file filters
.
.
```
